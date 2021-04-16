#pragma once

#include <chrono>
#include <string>
#include <map>

namespace Marvel {

	using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

	//-----------------------------------------------------------------------------
	// Useful structs
	//-----------------------------------------------------------------------------
	struct mvProfileResult
	{
		std::string Name;
		FloatingPointMicroseconds Start;
		std::chrono::microseconds ElapsedTime;
	};

	struct mvInstrumentationSession
	{
		std::string Name;
	};

	//-----------------------------------------------------------------------------
	// mvInstrumentor
	//-----------------------------------------------------------------------------
	class mvInstrumentor
	{

	public:

		mvInstrumentor() : m_CurrentSession(nullptr) {}

		// Call to begin recording profile data (must call end session at a later time)
		void BeginSession(const std::string& name);

		// Call to end recording profile data
		void EndSession();

		// Used to write data to the profiling file
		void WriteProfile(const mvProfileResult& result);

		const std::map<std::string, std::chrono::microseconds>& getResults() const { return m_results; }

		static mvInstrumentor& Get();

	private:

		// Note: you must already own lock on m_Mutex before
		// calling InternalEndSession()
		void InternalEndSession();

	private:

		mvInstrumentationSession* m_CurrentSession;
		std::map<std::string, std::chrono::microseconds> m_results;

	};

	//-----------------------------------------------------------------------------
	// mvInstrumentationTimer
	//-----------------------------------------------------------------------------
	class mvInstrumentationTimer
	{

	public:

		mvInstrumentationTimer(const char* name);

		~mvInstrumentationTimer();

		void Stop();

	private:

		const char*                                        m_Name;
		bool                                               m_Stopped;
		std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;

	};

}

#define MV_PROFILE_BEGIN_SESSION(name) ::Marvel::mvInstrumentor::Get().BeginSession(name)
#define MV_PROFILE_END_SESSION() ::Marvel::mvInstrumentor::Get().EndSession()
#define MV_PROFILE_SCOPE(name) ::Marvel::mvInstrumentationTimer timer##__LINE__(name);
#define MV_PROFILE_FUNCTION() MV_PROFILE_SCOPE(__func__)
//#define MV_PROFILE_FUNCTION() MV_PROFILE_SCOPE(__FUNCSIG__)