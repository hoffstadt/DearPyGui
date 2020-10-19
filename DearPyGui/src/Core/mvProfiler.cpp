#include "mvProfiler.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvInstrumentor
	//-----------------------------------------------------------------------------
	void mvInstrumentor::BeginSession(const std::string& name)
	{
		if (m_CurrentSession) 
		{
			// If there is already a current session, then close it before beginning new one.
			// Subsequent profiling output meant for the original session will end up in the
			// newly opened session instead.  That's better than having badly formatted
			// profiling output.

			InternalEndSession();
		}

	}

	void mvInstrumentor::EndSession()
	{
		InternalEndSession();
	}

	void mvInstrumentor::WriteProfile(const mvProfileResult& result)
	{

		m_results[result.Name] = result.ElapsedTime;

	}

	mvInstrumentor& mvInstrumentor::Get()
	{
		static mvInstrumentor instance;
		return instance;
	}

	void mvInstrumentor::InternalEndSession() {
		if (m_CurrentSession) 
		{
			delete m_CurrentSession;
			m_CurrentSession = nullptr;
		}
	}

	//-----------------------------------------------------------------------------
	// mvInstrumentationTimer
	//-----------------------------------------------------------------------------
	mvInstrumentationTimer::mvInstrumentationTimer(const char* name)
		: m_Name(name), m_Stopped(false)
	{
		m_StartTimepoint = std::chrono::steady_clock::now();
	}

	mvInstrumentationTimer::~mvInstrumentationTimer()
	{
		if (!m_Stopped)
			Stop();
	}

	void mvInstrumentationTimer::Stop()
	{
		auto endTimepoint = std::chrono::steady_clock::now();
		auto highResStart = FloatingPointMicroseconds{ m_StartTimepoint.time_since_epoch() };
		auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch();

		mvInstrumentor::Get().WriteProfile({ m_Name, highResStart, elapsedTime });

		m_Stopped = true;
	}

}