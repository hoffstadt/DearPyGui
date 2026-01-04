#pragma once

#include <mutex>
#include <vector>
#include <unordered_map>
#include "mvContext.h"

//-----------------------------------------------------------------------------
// mvFunctionWrapper
//-----------------------------------------------------------------------------

class mvFunctionWrapper
{
    struct impl_base {
        virtual void call() = 0;
        virtual ~impl_base() = default;
    };

    template<typename F>
    struct impl_type : impl_base
    {
        F f;
        explicit impl_type(F&& f) : f(std::move(f)) {}
        void call() override { f(); }
    };

public:

    mvFunctionWrapper() = default;

    template<typename F>
    mvFunctionWrapper(F&& f) : m_impl(new impl_type<F>(std::move(f))) {}

    mvFunctionWrapper(mvFunctionWrapper&& other) noexcept
            : m_impl(std::move(other.m_impl))
    {

    }

    mvFunctionWrapper& operator=(mvFunctionWrapper&& other)
    {
        m_impl = std::move(other.m_impl);
        return *this;
    }

    // delete copy constructor and assignment operator
    mvFunctionWrapper(const mvFunctionWrapper&) = delete;
    mvFunctionWrapper(mvFunctionWrapper&) = delete;
    mvFunctionWrapper& operator=(const mvFunctionWrapper&) = delete;

    void operator()()
    {
        m_impl->call();
    }

private:

    std::unique_ptr<impl_base> m_impl;

};

//-----------------------------------------------------------------------------
// mvQueue
//-----------------------------------------------------------------------------
template<typename T>
class mvQueue
{

public:

    struct node
    {
        std::shared_ptr<T>    data;
        std::unique_ptr<node> next;
    };

public:

    mvQueue() : m_head(new node), m_tail(m_head.get()) {}

    // copy assignment and constructor deleted
    mvQueue(const mvQueue& other) = delete;
    mvQueue& operator=(const mvQueue& other) = delete;

    std::shared_ptr<T> wait_and_pop()
    {
        std::unique_ptr<node> const old_head = wait_pop_head();
        return old_head->data;
    }

    std::shared_ptr<T> try_pop()
    {
        std::unique_ptr<node> const old_head = try_pop_head();
        return old_head ? old_head->data : std::shared_ptr<T>();
    }

    void wait_and_pop(T& value)
    {
        std::unique_ptr<node> const old_head = wait_pop_head(value);
    }

    bool try_pop(T& value)
    {
        std::unique_ptr<node> const old_head = try_pop_head(value);
        if (old_head)
            return true;
        return false;
    }

    void push(T value)
    {
        std::shared_ptr<T> new_data = std::make_shared<T>(std::move(value));
        auto p = std::make_unique<node>();

        // scoped in order to unlock tail mutex before notifying other threads
        {
            std::lock_guard<std::mutex> tail_lock(m_tail_mutex);
            m_tail->data = std::move(new_data);
            node* const new_tail = p.get();
            m_tail->next = std::move(p);
            m_tail = new_tail;
        }

        m_data_cond.notify_one();
    }

    bool empty()
    {
        std::lock_guard<std::mutex> head_lock(m_head_mutex);
        return (m_head.get() == get_tail());
    }

private:

    node* get_tail()
    {
        std::lock_guard<std::mutex> tail_lock(m_tail_mutex);
        return m_tail;
    }

    std::unique_ptr<node> pop_head()
    {
        std::unique_ptr<node> old_head = std::move(m_head);
        m_head = std::move(old_head->next);
        return old_head;
    }

    std::unique_ptr<node> try_pop_head()
    {
        std::lock_guard<std::mutex> head_lock(m_head_mutex);
        if (m_head.get() == get_tail())
            return std::unique_ptr<node>();
        return pop_head();
    }

    std::unique_ptr<node> try_pop_head(T& value)
    {
        std::lock_guard<std::mutex> head_lock(m_head_mutex);
        if (m_head.get() == get_tail())
            return std::unique_ptr<node>();

        value = std::move(*m_head->data);
        return pop_head();
    }

    std::unique_ptr<node> wait_pop_head()
    {
        std::unique_lock<std::mutex> head_lock(wait_for_data());
        return pop_head();
    }

    std::unique_ptr<node> wait_pop_head(T& value)
    {
        std::unique_lock<std::mutex> head_lock(wait_for_data());
        value = std::move(*m_head->data);
        return pop_head();
    }

    std::unique_lock<std::mutex> wait_for_data()
    {
        std::unique_lock<std::mutex> head_lock(m_head_mutex);
        m_data_cond.wait(head_lock, [&] {return m_head.get() != get_tail(); });
        return head_lock;
    }

private:

    std::mutex              m_head_mutex;
    std::mutex              m_tail_mutex;
    std::unique_ptr<node>   m_head;
    node* m_tail;
    std::condition_variable m_data_cond;

};

struct mvCallbackJob
{
    std::weak_ptr<void> owner;
    // Only valid if `owner` is alive; one must lock() the owner before accessing
    // the callback.
    PyObject* callback = nullptr;
    std::shared_ptr<mvPyObject> user_data;
    mvUUID sender = 0;
    std::string alias;
    std::function<PyObject*()> app_data_func;
    // Either `callback` (and `owner`) or `ownerless_callback` must be set,
    // but not both - otherwise one of them will be ignored.
    std::shared_ptr<mvPyObject> ownerless_callback = nullptr;
};

struct mvCallbackRegistry
{
    // TODO: ideally, it should be configurable (e.g. via configure_app)
	const i32 maxNumberOfCalls = 500;

	std::vector<mvCallbackJob> jobs;
	mvQueue<mvFunctionWrapper> tasks;
	mvQueue<mvFunctionWrapper> calls;
	std::atomic<b8>            running = false;
	std::atomic<i32>           callCount = 0;

	// callbacks
	std::shared_ptr<mvPyObject> resizeCallback          = std::make_shared<mvPyObject>(nullptr);
    std::shared_ptr<mvPyObject> resizeCallbackUserData  = std::make_shared<mvPyObject>(nullptr);
    std::shared_ptr<mvPyObject> onCloseCallback         = std::make_shared<mvPyObject>(nullptr);
    std::shared_ptr<mvPyObject> onCloseCallbackUserData = std::make_shared<mvPyObject>(nullptr);

	i32 highestFrame = 0;
	std::unordered_map<i32, mvPyObject> frameCallbacks;
	std::unordered_map<i32, mvPyObject> frameCallbacksUserData;
};

void mvFrameCallback(i32 frame);
void mvRunTasks();
// All PyObject references here are borrowed references - caller must release them after this call
void mvRunCallback(PyObject* callback, PyObject* user_data, mvUUID sender = 0, const std::string& sender_alias = "", PyObject* app_data = nullptr);

// Note: We pass the `callback` and its `user_data` as two separate arguments (rather
// than a single object) because, even though they only make sense together, `mvAppItem` may
// combine the same `user_data` with different callbacks.  We don't want to spread `user_data`
// instances all across `mvAppItem`.
// The `callback` must be valid all the time while `owner` is alive.  This works for
// the fields of `owner`; if `callback` is not a field of `owner`, the caller must make
// sure that `callback`'s lifetime is at least as long as `owner`'s.
// When the callback is about to be executed on the handlers thread, the callback queue
// acquires a shared_ptr to `owner` and holds it while the callback is being executed.
// If the `owner` is already lost by the moment the callback is fetched from the
// queue, the callback will be silently ignored.  This effectively cleans the queue
// from irrelevant callbacks - lingering there after `mvAppItem` deletion and such.
template<typename AppDataFunc>
void mvAddCallback(const std::weak_ptr<void>& owner,
                   PyObject* callback,
                   const std::shared_ptr<mvPyObject>& user_data,
                   mvUUID sender,
                   const std::string& alias,
                   AppDataFunc&& app_data_func)
{
	if (GContext->IO.manualCallbacks)
	{
		GContext->callbackRegistry->jobs.push_back({owner, callback, user_data, sender, alias, std::forward<AppDataFunc>(app_data_func)});
		return;
	}
	mvSubmitCallback([=, app_data_func = std::forward<AppDataFunc>(app_data_func)] () {
        auto liveOwner = owner.lock();  // we need it to live through the mvRunCallback, hence constructing it separately rather than within "if"
        if (liveOwner)
    		mvRunCallback(callback, *user_data, sender, alias, mvPyObject(app_data_func()));
    });
}

// This overload exists purely to provide default argument values - we can't do this
// directly on the template version above because the compiler won't be able to deduce
// `app_data_func` type if `app_data_func` is omitted (that is, we can't really use
// the default on `app_data_func`).
void mvAddCallback(const std::weak_ptr<void>& owner,
                   PyObject* callback,
                   const std::shared_ptr<mvPyObject>& user_data,
                   mvUUID sender = 0,
                   const std::string& alias = "");

template<typename AppDataFunc>
void mvAddOwnerlessCallback(const std::shared_ptr<mvPyObject>& callback,
                            const std::shared_ptr<mvPyObject>& user_data,
                            mvUUID sender,
                            const std::string& alias,
                            AppDataFunc&& app_data_func)
{
	if (GContext->IO.manualCallbacks)
	{
		GContext->callbackRegistry->jobs.push_back({{}, nullptr, user_data, sender, alias, std::forward<AppDataFunc>(app_data_func), callback});
		return;
	}
	mvSubmitCallback([=, app_data_func = std::forward<AppDataFunc>(app_data_func)]() {
		mvRunCallback(*callback, *user_data, sender, alias, mvPyObject(app_data_func()));
    });
}

// This overload exists purely to provide default argument values - we can't do this
// directly on the template version above because the compiler won't be able to deduce
// `app_data_func` type if `app_data_func` is omitted (that is, we can't really use
// the default on `app_data_func`).
void mvAddOwnerlessCallback(const std::shared_ptr<mvPyObject>& callback,
                            const std::shared_ptr<mvPyObject>& user_data,
                            mvUUID sender = 0,
                            const std::string& alias = "");

bool mvRunCallbacks();

template<typename F, typename ...Args>
std::future<typename std::invoke_result<F, Args...>::type> mvSubmitTask(F f)
{

	typedef typename std::invoke_result<F, Args...>::type result_type;
	std::packaged_task<result_type()> task(std::move(f));
	std::future<result_type> res(task.get_future());

	if (GContext->running)
		GContext->callbackRegistry->tasks.push(std::move(task));
	else
		task();

	return res;
}

template<typename F, typename ...Args>
std::future<typename std::invoke_result<F, Args...>::type> mvSubmitCallback(F f, bool ignore_limit = false)
{

	if (GContext->callbackRegistry->callCount > GContext->callbackRegistry->maxNumberOfCalls && !ignore_limit)
	{
        assert(false);
		return {};
	}

	GContext->callbackRegistry->callCount++;

	typedef typename std::invoke_result<F, Args...>::type result_type;
	std::packaged_task<result_type()> task(std::move(f));
	std::future<result_type> res(task.get_future());

	GContext->callbackRegistry->calls.push(std::move(task));

	return res;
}