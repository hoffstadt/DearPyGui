#pragma once

#include <future>
#include <memory>
#include <functional>
#include <iostream>
#include <vector>
#include <atomic>
#include <queue>
#include <deque>
#include "Core/mvCore.h"
#include "mvQueue.h"
#include "Core/mvMemory.h"

namespace Marvel {

    //-----------------------------------------------------------------------------
    // mvThreadJoiner
    //-----------------------------------------------------------------------------
    class mvThreadJoiner
    {

    public:

        explicit mvThreadJoiner(std::vector<std::thread>& threads);

        ~mvThreadJoiner();

    private:

        std::vector<std::thread>& m_threads;

    };

    //-----------------------------------------------------------------------------
    // mvFunctionWrapper
    //     - Reguired because packaged_task reguires movable function objects
    //-----------------------------------------------------------------------------
    class mvFunctionWrapper
    {
        struct impl_base {
            virtual void call() = 0;
            virtual ~impl_base() {}
        };

        template<typename F>
        struct impl_type : impl_base
        {
            F f;
            impl_type(F&& f) : f(std::move(f)) {}
            void call() { f(); }
        };

    public:

        mvFunctionWrapper() = default;

        template<typename F>
        mvFunctionWrapper(F&& f) : m_impl(new impl_type<F>(std::move(f))){}

        mvFunctionWrapper(mvFunctionWrapper&& other);

        mvFunctionWrapper& operator=(mvFunctionWrapper&& other);

        // delete copy constructor and assignment operator
        mvFunctionWrapper(const mvFunctionWrapper&)            = delete;
        mvFunctionWrapper(mvFunctionWrapper&)                  = delete;
        mvFunctionWrapper& operator=(const mvFunctionWrapper&) = delete;

        void operator()();

    private:

        OwnedPtr<impl_base> m_impl;

    };

    //-----------------------------------------------------------------------------
    // mvWorkStealingQueue
    //-----------------------------------------------------------------------------
    class mvWorkStealingQueue
    {

    public:

        mvWorkStealingQueue(){}
        ~mvWorkStealingQueue() = default;

        // deleted copy constructor/assignment operator.
        mvWorkStealingQueue(const mvWorkStealingQueue& other)            = delete;
        mvWorkStealingQueue& operator=(const mvWorkStealingQueue& other) = delete;

        void push     (mvFunctionWrapper data);
        bool empty    () const;
        bool try_pop  (mvFunctionWrapper& res);
        bool try_steal(mvFunctionWrapper& res);

    private:

        std::deque<mvFunctionWrapper> m_queue;
        mutable std::mutex            m_mutex;

    };

    //-----------------------------------------------------------------------------
    // mvThreadPool
    //-----------------------------------------------------------------------------
    class mvThreadPool
    {

        typedef mvFunctionWrapper task_type;

    public:

        mvThreadPool(unsigned threadcount = 0);
        ~mvThreadPool(){ m_done = true;}

        template<typename F>
        std::future<typename std::result_of<F()>::type> submit(F f)
        {
            typedef typename std::result_of<F()>::type result_type;
            std::packaged_task<result_type()> task(std::move(f));
            std::future<result_type> res(task.get_future());
            if (m_local_work_queue)
                m_local_work_queue->push(std::move(task));
            else
                m_pool_work_queue.push(std::move(task));
            
            return res;
        }

    private:

        void worker_thread                   (unsigned index);
        void run_pending_task                ();
        bool pop_task_from_local_queue       (task_type& task);
        bool pop_task_from_pool_queue        (task_type& task);
        bool pop_task_from_other_thread_queue(task_type& type);

    private:

        std::atomic_bool                            m_done;
        mvQueue<task_type>                          m_pool_work_queue;
        std::vector<OwnedPtr<mvWorkStealingQueue> > m_queues;
        std::vector<std::thread>                    m_threads;
        mvThreadJoiner                              m_joiner;
        static thread_local mvWorkStealingQueue*    m_local_work_queue;
        static thread_local unsigned                m_index;
        //static mvThreadPool*                        s_instance;

    };

}