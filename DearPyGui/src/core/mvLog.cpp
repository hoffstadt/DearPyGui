#include "mvLog.h"

namespace Marvel {

	bool mvLog::s_initialized = false;

	void mvLog::Init()
	{
#ifdef MV_LOG
		s_initialized = true;
		spdlog::set_pattern("%^[%T] %t %n:%$ %v");
		GetCoreLogger()->set_level(spdlog::level::trace);
		GetEventLogger()->set_level(spdlog::level::info);
		GetItemRegistryLogger()->set_level(spdlog::level::info);
#endif // MV_LOG
	}

	void mvLog::Cleanup()
	{
		s_initialized = false;
#ifdef MV_LOG
		spdlog::shutdown();
#endif // MV_LOG
	}

#ifdef MV_LOG
	std::shared_ptr<spdlog::logger>& mvLog::GetCoreLogger() 
	{ 
		static auto coreLogger = spdlog::create_async<spdlog::sinks::stdout_color_sink_mt>("[CORE]");
		return coreLogger;
	}

	std::shared_ptr<spdlog::logger>& mvLog::GetEventLogger()
	{
		static auto eventLogger = spdlog::create_async<spdlog::sinks::stdout_color_sink_mt>("[EVENT]");
		return eventLogger;
	}

	std::shared_ptr<spdlog::logger>& mvLog::GetItemRegistryLogger()
	{
		static auto eventLogger = spdlog::create_async<spdlog::sinks::stdout_color_sink_mt>("[ITEM REGISTRY]");
		return eventLogger;
	}
#endif // MV_LOG
}
