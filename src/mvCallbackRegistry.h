#pragma once

#include <memory>
#include <mutex>
#include <vector>
#include <unordered_map>
#include <utility>
#include "mvContext.h"
#include "mvPyUtils.h"

using mvCallbackRefcountFlags = unsigned int;
constexpr mvCallbackRefcountFlags MV_CALLBACK_BORROW_ALL = 0;
constexpr mvCallbackRefcountFlags MV_CALLBACK_STEAL_APP_DATA = 1;

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
// mvCallbackWithData
//-----------------------------------------------------------------------------

class mvCallbackWithData
{
public:
    mvPyObjectStrict callback;
    mvPyObjectStrict appData;
    mvPyObjectStrict userData;

    mvCallbackWithData() = default;

    mvCallbackWithData(PyObject* callback, PyObject* appData, PyObject* userData, mvCallbackRefcountFlags flags)
        : callback(callback),
          appData(appData, !(flags & MV_CALLBACK_STEAL_APP_DATA)),
          userData(userData)
    {
    }

    mvCallbackWithData(mvCallbackWithData&& f) noexcept
        : callback(std::move(f.callback)),
          appData(std::move(f.appData)),
          userData(std::move(f.userData))
    {
    }

    mvCallbackWithData& operator=(mvCallbackWithData&& other) noexcept
    {
        callback = std::move(other.callback);
        appData = std::move(other.appData);
        userData = std::move(other.userData);
        return *this;
    }

    void nullToNone() noexcept
    {
        callback.nullToNone();
        appData.nullToNone();
        userData.nullToNone();
    }

    mvCallbackWithData copy()
    {
        // increases the python reference counts
        return mvCallbackWithData(*callback, *appData, *userData, MV_CALLBACK_BORROW_ALL);
    }
};

//-----------------------------------------------------------------------------
// mvCallbackPoint
//-----------------------------------------------------------------------------

class mvCallbackPoint
{
    const char* pythonName;
    mvCallbackWithData cwd;

public:
    mvCallbackPoint(const char* pythonName)
        : pythonName(pythonName)
    {
    }

    PyObject* set_from_python(PyObject* self, PyObject* args, PyObject* kwargs);
    
    void run(mvUUID sender = 0, PyObject *appData = nullptr);
    void run_blocking(mvUUID sender = 0, PyObject *appData = nullptr);
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

static PyObject* SanitizeCallback(PyObject* callback)
{
	if (callback == Py_None)
		return nullptr;

	return callback;
}

// forward declaration: mvCallbackRegistry.h is included in mvAppItem.h
class mvAppItem;

class mvCallbackJob
{
public:
    mvCallbackWithData cwd;
    mvUUID sender = 0;
    std::string sender_str;
    bool valid = true;

    mvCallbackJob(mvCallbackWithData&& wrapper, mvUUID sender, std::string sender_str, bool valid = true);
    inline mvCallbackJob(mvCallbackWithData&& wrapper, mvUUID sender, bool valid = true) :
        mvCallbackJob(std::move(wrapper), sender, std::string(), valid)
    {
    }
    inline mvCallbackJob(mvCallbackWithData&& wrapper, std::string sender, bool valid = true) :
        mvCallbackJob(std::move(wrapper), 0, sender, valid)
    {
    }

    mvCallbackJob(PyObject* callback, mvUUID sender, std::string sender_str, PyObject* app_data, PyObject* user_data, mvCallbackRefcountFlags flags = MV_CALLBACK_STEAL_APP_DATA);
    inline mvCallbackJob(PyObject* callback, mvUUID sender, PyObject* app_data, PyObject* user_data, mvCallbackRefcountFlags flags = MV_CALLBACK_STEAL_APP_DATA) :
        mvCallbackJob(callback, sender, std::string(), app_data, user_data, flags)
    {
    }
    inline mvCallbackJob(PyObject* callback, std::string sender, PyObject* app_data, PyObject* user_data, mvCallbackRefcountFlags flags = MV_CALLBACK_STEAL_APP_DATA):
        mvCallbackJob(callback, 0, sender, app_data, user_data, flags)
    {
    }

    // for convenience: a lot of callbacks are generated from mvAppItem
    mvCallbackJob(mvAppItem& item, PyObject* app_data, mvCallbackRefcountFlags flags = MV_CALLBACK_STEAL_APP_DATA);
    mvCallbackJob(PyObject* callback, mvAppItem& item, PyObject* app_data, mvCallbackRefcountFlags flags = MV_CALLBACK_STEAL_APP_DATA);

    mvCallbackJob(mvCallbackJob&& other) noexcept
    {
        cwd = std::move(other.cwd);
        sender = other.sender;
        sender_str = other.sender_str;
        valid = other.valid;
        other.valid = false;
    }

    mvCallbackJob& operator=(mvCallbackJob&& other) noexcept
    {
        cwd = std::move(other.cwd);
        sender = other.sender;
        sender_str = other.sender_str;
        valid = other.valid;
        other.valid = false;
        return *this;
    }

    mvCallbackJob copy()
    {
        // increases the python reference counts
        return mvCallbackJob(cwd.copy(), sender, sender_str, valid);
    }

    bool is_valid() const { return valid; }

    static PyObject* to_python_tuple(mvCallbackJob&& job);
};

struct mvCallbackRegistry
{
	const i32 maxNumberOfCalls = 50;

	std::vector<mvCallbackJob> jobs;
	mvQueue<mvFunctionWrapper> tasks;
	mvQueue<mvFunctionWrapper> calls;
	std::atomic<b8>            running = false;
	std::atomic<i32>           callCount = 0;

	// callbacks
    mvCallbackPoint viewportResizeCallbackPoint { "set_viewport_resize_callback" };
    mvCallbackPoint exitCallbackPoint { "set_exit_callback" };

    void dropCallback(const std::vector<std::string>& filenames);

    // frame callbacks
	i32 highestFrame = 0;
	std::unordered_map<i32, PyObject*> frameCallbacks;
	std::unordered_map<i32, PyObject*> frameCallbacksUserData;
};

void mvRunTasks();
void mvFrameCallback(i32 frame);
bool mvRunCallbacks();

void mvAddCallbackJob(mvCallbackJob&& job, bool allowManualManagement = true);
void mvRunCallbackJob(mvCallbackJob&& job);

template<typename F, typename ...Args>
std::future<typename std::invoke_result<F, Args...>::type> mvSubmitTask(F f)
{

	typedef typename std::invoke_result<F, Args...>::type result_type;
	std::packaged_task<result_type()> task(std::move(f));
	std::future<result_type> res(task.get_future());

	if (GContext->started)
		GContext->callbackRegistry->tasks.push(std::move(task));
	else
		task();

	return res;
}

template<typename F, typename ...Args>
std::future<typename std::invoke_result<F, Args...>::type> mvSubmitCallback(F f)
{

	if (GContext->callbackRegistry->callCount > GContext->callbackRegistry->maxNumberOfCalls)
	{
		return {};
	}

	GContext->callbackRegistry->callCount++;

	typedef typename std::invoke_result<F, Args...>::type result_type;
	std::packaged_task<result_type()> task(std::move(f));
	std::future<result_type> res(task.get_future());

	GContext->callbackRegistry->calls.push(std::move(task));

	return res;
}

inline auto mvSubmitAddCallbackJob(mvCallbackJob&& job)
{
	// This gets wrapped in an std::function so it can't be move-only, sadly.
	auto jobp = std::make_shared<mvCallbackJob>(std::move(job));

    return mvSubmitCallback([jobp = std::move(jobp)]() {
        mvAddCallbackJob(std::move(*jobp));
        });
}

inline auto mvSubmitRunCallbackJob(mvCallbackJob&& job)
{
	// This gets wrapped in an std::function so it can't be move-only, sadly.
	auto jobp = std::make_shared<mvCallbackJob>(std::move(job));

	return mvSubmitCallback([jobp = std::move(jobp)]() {
		mvRunCallbackJob(std::move(*jobp));
	    });
}
