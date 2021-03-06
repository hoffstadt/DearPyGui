#pragma once
#include<memory>

#ifdef MV_LOG
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#endif // MV_LOG

namespace Marvel {

	class mvLog
	{

	public:

		static void Init();

#ifdef MV_LOG
		static std::shared_ptr<spdlog::logger>& GetCoreLogger();
#endif // MV_LOG
	};
}

#ifdef MV_LOG
	#define MV_CORE_TRACE(...)	Marvel::mvLog::GetCoreLogger()->trace(__VA_ARGS__)
	#define MV_CORE_INFO(...)	Marvel::mvLog::GetCoreLogger()->info(__VA_ARGS__)
	#define MV_CORE_WARN(...)	Marvel::mvLog::GetCoreLogger()->warn(__VA_ARGS__)
	#define MV_CORE_ERROR(...)	Marvel::mvLog::GetCoreLogger()->error(__VA_ARGS__)
	#define MV_CORE_FATAL(...)	Marvel::mvLog::GetCoreLogger()->critical(__VA_ARGS__)
#else
	#define MV_CORE_TRACE(...) //
	#define MV_CORE_INFO(...)  //
	#define MV_CORE_WARN(...)  //
	#define MV_CORE_ERROR(...) //
	#define MV_CORE_FATAL(...) //
#endif
