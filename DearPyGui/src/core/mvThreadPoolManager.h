#pragma once
#include <chrono>
#include <mutex>
#include "mvEvents.h"

//-----------------------------------------------------------------------------
// Typedefs for chrono's ridiculously long names
//-----------------------------------------------------------------------------
typedef std::chrono::high_resolution_clock clock_;
typedef std::chrono::duration<double, std::ratio<1> > second_;
#if defined (_WIN32)
typedef std::chrono::steady_clock::time_point time_point_;
#elif defined(__APPLE__)
typedef std::chrono::steady_clock::time_point time_point_;
#else
typedef std::chrono::system_clock::time_point time_point_;
#endif

namespace Marvel {

	class mvThreadPool;

	class mvThreadPoolManager : public mvEventHandler
	{

	public:

		static mvThreadPoolManager* GetThreadPoolManager();

		bool onEvent    (mvEvent& event) override;
		bool onPreRender(mvEvent& event);
		bool onPostRender(mvEvent& event);

		void                     setThreadPoolTimeout(double time) { m_threadPoolTimeout = time; }
		void                     setThreadCount(unsigned count) { m_threads = count; }
		void                     activateThreadPool() { m_threadPool = true; }
		void                     setThreadPoolHighPerformance() { m_threadPoolHighPerformance = true; }

		double                   getThreadPoolTimeout() const { return m_threadPoolTimeout; }
		unsigned                 getThreadCount() const { return m_threads; }
		bool                     usingThreadPool() const { return m_threadPool; }
		bool                     usingThreadPoolHighPerformance() const { return m_threadPoolHighPerformance; }

	private:

		mvThreadPoolManager();

		static mvThreadPoolManager* s_instance;

		mvThreadPool*                    m_tpool = nullptr;
		mutable std::mutex               m_mutex;
		bool                             m_threadPool = false;                // is threadpool activated
		double                           m_threadPoolTimeout = 30.0;          // how long til trying to delete pool
		unsigned                         m_threads = 2;                       // how many threads to use
		bool                             m_threadPoolHighPerformance = false; // when true, use max number of threads
		double                           m_threadTime = 0.0;                  // how long threadpool has been active
		time_point_                      m_poolStart;                         // threadpool start time

	};

}