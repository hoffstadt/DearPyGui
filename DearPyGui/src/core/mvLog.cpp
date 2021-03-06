#include "mvLog.h"

namespace Marvel {

	void mvLog::Init()
	{
#ifdef MV_LOG
		spdlog::set_pattern("%^[%T] %t %n:%$ %v");
		GetCoreLogger()->set_level(spdlog::level::trace);
#endif // MV_LOG
	}

#ifdef MV_LOG
	std::shared_ptr<spdlog::logger>& mvLog::GetCoreLogger() 
	{ 
		static auto coreLogger = spdlog::stdout_color_mt("[CORE]");
		return coreLogger;
	}
#endif // MV_LOG
}
