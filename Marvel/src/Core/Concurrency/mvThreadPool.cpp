#include "mvThreadPool.h"
#include "Core/mvApp.h"
#include "Core/StandardWindows/mvAppLog.h"
//#include "mvLogging.h"

namespace Marvel {

    thread_local mvWorkStealingQueue* mvThreadPool::m_local_work_queue;
    thread_local unsigned mvThreadPool::m_index;
    mvThreadPool* mvThreadPool::s_instance = nullptr;

    //-----------------------------------------------------------------------------
    // mvThreadJoiner
    //-----------------------------------------------------------------------------
    mvThreadJoiner::mvThreadJoiner(std::vector<std::thread>& threads) :
        m_threads(threads) 
    {
    }

    mvThreadJoiner::~mvThreadJoiner()
    {
        for (int i = 0; i < m_threads.size(); i++)
        {
            if (m_threads[i].joinable())
                m_threads[i].join();
        }
    }

    //-----------------------------------------------------------------------------
    // mvFunctionWrapper
    //-----------------------------------------------------------------------------
    mvFunctionWrapper::mvFunctionWrapper(mvFunctionWrapper&& other) : 
        m_impl(std::move(other.m_impl))
    {
    }

    mvFunctionWrapper& mvFunctionWrapper::operator=(mvFunctionWrapper&& other)
    {
        m_impl = std::move(other.m_impl);
        return *this;
    }

    void mvFunctionWrapper::operator()() 
    { 
        m_impl->call();
    }

    //-----------------------------------------------------------------------------
   // mvWorkStealingQueue
   //-----------------------------------------------------------------------------
    void mvWorkStealingQueue::push(mvFunctionWrapper data)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push_front(std::move(data));
    }

    bool mvWorkStealingQueue::empty() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_queue.empty();
    }

    bool mvWorkStealingQueue::try_pop(mvFunctionWrapper& res)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_queue.empty())
            return false;

        res = std::move(m_queue.front());
        m_queue.pop_front();
        return true;
    }

    bool mvWorkStealingQueue::try_steal(mvFunctionWrapper& res)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_queue.empty())
            return false;

        res = std::move(m_queue.back());
        m_queue.pop_back();
        return true;
    }

    //-----------------------------------------------------------------------------
    // mvThreadPool
    //-----------------------------------------------------------------------------
    mvThreadPool::mvThreadPool() :
        m_done(false), m_joiner(m_threads)
    {
        unsigned thread_count;
        
        if (mvApp::GetApp()->usingThreadPoolHighPerformance())
            thread_count = std::thread::hardware_concurrency();
        else
            thread_count = mvApp::GetApp()->getThreadCount();

        mvAppLog::getLogger()->LogInfo("Thread pool activated with " + std::to_string(thread_count) + " threads.");

        try
        {

            for (unsigned i = 0; i < thread_count; ++i)
                m_queues.push_back(OwnedPtr<mvWorkStealingQueue>(new mvWorkStealingQueue));

            for (unsigned i = 0; i < thread_count; ++i)
                m_threads.push_back(
                    std::thread(&mvThreadPool::worker_thread, this, i));

        }
        catch (...)
        {
            m_done = true;
            throw;
        }
    }

    mvThreadPool* mvThreadPool::GetThreadPool()
    {
        if (s_instance)
            return s_instance;
        
        s_instance = new mvThreadPool();
        return s_instance;
    }

    void mvThreadPool::worker_thread(unsigned index)
    {
        m_index = index;

        m_local_work_queue = m_queues[m_index].get();

        while (!m_done)
            run_pending_task();

    }

    void mvThreadPool::run_pending_task()
    {
        task_type task;
        if (pop_task_from_local_queue(task) ||
            pop_task_from_pool_queue(task) ||
            pop_task_from_other_thread_queue(task))
        {
            task();
        }

        else
            std::this_thread::yield();

    }

    bool mvThreadPool::pop_task_from_local_queue(task_type& task)
    {
        return m_local_work_queue && m_local_work_queue->try_pop(task);
    }

    bool mvThreadPool::pop_task_from_pool_queue(task_type& task)
    {
        return m_pool_work_queue.try_pop(task);
    }

    bool mvThreadPool::pop_task_from_other_thread_queue(task_type& task)
    {
        for (unsigned i = 0; i < m_queues.size(); i++)
        {
            const unsigned index = (m_index + i + 1) % m_queues.size();
            if (m_queues[index]->try_steal(task))
                return true;
        }

        return false;
    }

}

