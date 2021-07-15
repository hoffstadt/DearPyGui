#include "mvLog.h"

namespace Marvel {

	bool mvLog::s_initialized = false;
	bool mvLog::s_reset = false;

	void mvLog::Init()
	{
#ifdef MV_LOG
		if (s_reset)
			spdlog::init_thread_pool(spdlog::details::default_async_q_size, 1);
		s_initialized = true;
		spdlog::set_pattern("%^[%T] %t %n:%$ %v");
		GetCoreLogger()->set_level(spdlog::level::trace);
		GetEventLogger()->set_level(spdlog::level::info);
		GetItemRegistryLogger()->set_level(spdlog::level::info);

		if (s_reset)
			s_reset = false;
#endif // MV_LOG
	}

	void mvLog::Cleanup()
	{
		s_initialized = false;
		s_reset = true;
#ifdef MV_LOG
		spdlog::shutdown();
#endif // MV_LOG
	}

#ifdef MV_LOG
	std::shared_ptr<spdlog::logger>& mvLog::GetCoreLogger() 
	{ 
		static auto coreLogger = spdlog::create_async<spdlog::sinks::stdout_color_sink_mt>("[CORE]");
		if(s_reset)
			coreLogger = spdlog::create_async<spdlog::sinks::stdout_color_sink_mt>("[CORE]");
		return coreLogger;
	}

	std::shared_ptr<spdlog::logger>& mvLog::GetEventLogger()
	{
		static auto eventLogger = spdlog::create_async<spdlog::sinks::stdout_color_sink_mt>("[EVENT]");
		if (s_reset)
			eventLogger = spdlog::create_async<spdlog::sinks::stdout_color_sink_mt>("[EVENT]");
		return eventLogger;
	}

	std::shared_ptr<spdlog::logger>& mvLog::GetItemRegistryLogger()
	{
		static auto eventLogger = spdlog::create_async<spdlog::sinks::stdout_color_sink_mt>("[ITEM REGISTRY]");
		if (s_reset)
			eventLogger = spdlog::create_async<spdlog::sinks::stdout_color_sink_mt>("[ITEM REGISTRY]");
		return eventLogger;
	}
#endif // MV_LOG
}
