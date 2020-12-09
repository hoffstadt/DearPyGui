#include "mvThreadPoolManager.h"
#include "mvThreadPool.h"
#include "mvAppLog.h"
#include "mvProfiler.h"
#include "mvCallbackRegistry.h"

namespace Marvel {

	mvThreadPoolManager* mvThreadPoolManager::s_instance = nullptr;
	thread_local mvWorkStealingQueue* mvThreadPool::m_local_work_queue;
	thread_local unsigned mvThreadPool::m_index;

	mvThreadPoolManager* mvThreadPoolManager::GetThreadPoolManager()
	{
		if (s_instance)
			return s_instance;

		s_instance = new mvThreadPoolManager();
		return s_instance;
	}

	mvThreadPoolManager::mvThreadPoolManager()
	{
		mvEventBus::Subscribe(this, mvEVT_PRE_RENDER);
		mvEventBus::Subscribe(this, mvEVT_END_FRAME);
	}

	bool mvThreadPoolManager::onEvent(mvEvent& event)
	{
		mvEventDispatcher dispatcher(event);
		dispatcher.dispatch(BIND_EVENT_METH(mvThreadPoolManager::onPreRender), mvEVT_PRE_RENDER);
		dispatcher.dispatch(BIND_EVENT_METH(mvThreadPoolManager::onPostRender), mvEVT_END_FRAME);

		return false;
	}

	bool mvThreadPoolManager::onPreRender(mvEvent& event)
	{

		// check if threadpool is ready to be cleaned up
		if (m_threadTime > m_threadPoolTimeout)
		{
			if (m_tpool != nullptr)
			{

				// set pool to delete when finishing last task
				m_tpool->setDone();

				// check if last task is complete
				if (m_tpool->isReadyToDelete())
				{
					delete m_tpool;
					m_tpool = nullptr;
					m_threadTime = 0.0;
					m_threadPool = false;
					mvAppLog::Log("Threadpool destroyed");
				}
				else
					m_poolStart = clock_::now();
			}
			else
				m_poolStart = clock_::now();

		}

		return false;
	}

	bool mvThreadPoolManager::onPostRender(mvEvent& event)
	{
		Py_BEGIN_ALLOW_THREADS
		MV_PROFILE_FUNCTION()

			// async callbacks
			if (mvCallbackRegistry::GetCallbackRegistry()->hasAsyncCallbacks())
			{
				// check if threadpool is valid, if not, create it
				if (m_tpool == nullptr)
				{
					m_tpool = new mvThreadPool(m_threadPoolHighPerformance ? 0 : m_threads);
					m_poolStart = clock_::now();
					m_threadPool = true;
					mvAppLog::Log("Threadpool created");
				}

				mvCallbackRegistry::GetCallbackRegistry()->runAsyncCallbacks(m_tpool);
			}

		// update timer if thread pool exists
		if (m_tpool != nullptr)
			m_threadTime = std::chrono::duration_cast<second_>(clock_::now() - m_poolStart).count();

		Py_END_ALLOW_THREADS

		return false;
	}

}