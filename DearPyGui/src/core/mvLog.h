#pragma once
#include<memory>

#ifdef MV_LOG
#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#endif // MV_LOG

namespace Marvel {

	class mvLog
	{

	public:

		static void Init();
		static void Cleanup();
		static bool IsInitialized() { return s_initialized; }

#ifdef MV_LOG
		static std::shared_ptr<spdlog::logger>& GetCoreLogger();
		static std::shared_ptr<spdlog::logger>& GetEventLogger();
#endif // MV_LOG

	private:

		static bool s_initialized;
	};
}

#ifdef MV_LOG
	#define MV_CORE_TRACE(...)	if(Marvel::mvLog::IsInitialized())Marvel::mvLog::GetCoreLogger()->trace(__VA_ARGS__)
	#define MV_CORE_INFO(...)	if(Marvel::mvLog::IsInitialized())Marvel::mvLog::GetCoreLogger()->info(__VA_ARGS__)
	#define MV_CORE_WARN(...)	if(Marvel::mvLog::IsInitialized())Marvel::mvLog::GetCoreLogger()->warn(__VA_ARGS__)
	#define MV_CORE_ERROR(...)	if(Marvel::mvLog::IsInitialized())Marvel::mvLog::GetCoreLogger()->error(__VA_ARGS__)
	#define MV_CORE_FATAL(...)	if(Marvel::mvLog::IsInitialized())Marvel::mvLog::GetCoreLogger()->critical(__VA_ARGS__)

	#define MV_EVENT_TRACE(...)	if(Marvel::mvLog::IsInitialized())Marvel::mvLog::GetEventLogger()->trace(__VA_ARGS__)
	#define MV_EVENT_INFO(...)	if(Marvel::mvLog::IsInitialized())Marvel::mvLog::GetEventLogger()->info(__VA_ARGS__)
	#define MV_EVENT_WARN(...)	if(Marvel::mvLog::IsInitialized())Marvel::mvLog::GetEventLogger()->warn(__VA_ARGS__)
	#define MV_EVENT_ERROR(...)	if(Marvel::mvLog::IsInitialized())Marvel::mvLog::GetEventLogger()->error(__VA_ARGS__)
	#define MV_EVENT_FATAL(...)	if(Marvel::mvLog::IsInitialized())Marvel::mvLog::GetEventLogger()->critical(__VA_ARGS__)
#else
	#define MV_CORE_TRACE(...) //
	#define MV_CORE_INFO(...)  //
	#define MV_CORE_WARN(...)  //
	#define MV_CORE_ERROR(...) //
	#define MV_CORE_FATAL(...) //

	#define MV_EVENT_TRACE(...) //
	#define MV_EVENT_INFO(...)  //
	#define MV_EVENT_WARN(...)  //
	#define MV_EVENT_ERROR(...) //
	#define MV_EVENT_FATAL(...) //
#endif
