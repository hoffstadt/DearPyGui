#pragma once

#include "mvApp.h"
#include "mvAppItem.h"
#include "mvItemRegistry.h"
#include <imgui.h>

namespace Marvel {

	MV_REGISTER_WIDGET(mvLoggerItem, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 1);
	class mvLoggerItem : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvLoggerItem, add_logger)

		MV_CREATE_CONSTANT(mvTRACE,   0L, 0L);
		MV_CREATE_CONSTANT(mvDEBUG,   1L, 0L);
		MV_CREATE_CONSTANT(mvINFO,    2L, 0L);
		MV_CREATE_CONSTANT(mvWARNING, 3L, 0L);
		MV_CREATE_CONSTANT(mvERROR,   4L, 0L);
		MV_CREATE_CONSTANT(mvOFF,     5L, 0L);

		MV_CREATE_EXTRA_COMMAND(get_log_level);
		MV_CREATE_EXTRA_COMMAND(set_log_level);
		MV_CREATE_EXTRA_COMMAND(log);
		MV_CREATE_EXTRA_COMMAND(log_debug);
		MV_CREATE_EXTRA_COMMAND(log_info);
		MV_CREATE_EXTRA_COMMAND(log_warning);
		MV_CREATE_EXTRA_COMMAND(log_error);
		MV_CREATE_EXTRA_COMMAND(clear_log);
		static PyObject* show_logger(PyObject* self, PyObject* args);
		MV_CREATE_EXTRA_COMMAND(set_logger_window_title);

		MV_START_EXTRA_COMMANDS
			MV_ADD_EXTRA_COMMAND(get_log_level);
			MV_ADD_EXTRA_COMMAND(set_log_level);
			MV_ADD_EXTRA_COMMAND(log);
			MV_ADD_EXTRA_COMMAND(log_debug);
			MV_ADD_EXTRA_COMMAND(log_info);
			MV_ADD_EXTRA_COMMAND(log_warning);
			MV_ADD_EXTRA_COMMAND(log_error);
			MV_ADD_EXTRA_COMMAND(clear_log);
			MV_ADD_EXTRA_COMMAND(show_logger);
			MV_ADD_EXTRA_COMMAND(set_logger_window_title);
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
			MV_ADD_GENERAL_CONSTANT(mvTRACE),
			MV_ADD_GENERAL_CONSTANT(mvDEBUG),
            MV_ADD_GENERAL_CONSTANT(mvINFO),
            MV_ADD_GENERAL_CONSTANT(mvWARNING),
            MV_ADD_GENERAL_CONSTANT(mvERROR),
           MV_ADD_GENERAL_CONSTANT(mvOFF),
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

		mvLoggerItem(const std::string& name)
			: mvAppItem(name)
		{
			m_autoScrollButtonName = "Auto-scroll##" + name;
			m_clearButtonName = "Clear##" + name;
			m_copyButtonName = "Copy##" + name;
			ClearLog();
			LogInfo("[Logger] " + m_name);
		}

		void draw(ImDrawList* drawlist, float x, float y) override;

		inline void setLogLevel(int level) { m_loglevel = level; }
		inline int  getLogLevel() const { return m_loglevel; }

		void AddLog(const char* fmt, ...);
		void Log(const std::string& text, const std::string& level = "TRACE");
		void LogDebug(const std::string& text);
		void LogInfo(const std::string& text);
		void LogWarning(const std::string& text);
		void LogError(const std::string& text);
		void ClearLog();

		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;


	private:

		// configurable options
		int  m_loglevel          = 1;
		bool m_autoScrollButton  = true;
		bool m_clearButton       = true;
		bool m_copyButton        = true;
		bool m_filter            = true;
		bool m_autosize_x        = false;
		bool m_autosize_y        = false;

		// 
		std::string m_autoScrollButtonName;
		std::string m_clearButtonName;
		std::string m_copyButtonName;

		// internal
		ImGuiTextBuffer  Buf;
		ImGuiTextFilter  Filter;
		ImVector<int>    LineOffsets; // Index to lines offset. We maintain this with AddLog() calls, allowing us to have a random access on lines
		bool             AutoScroll = true;  // Keep scrolling if already at the bottom
		bool             show = true; // Keep scrolling if already at the bottom
		bool             mainmode = false;

#if defined (_WIN32)
		static std::chrono::steady_clock::time_point s_start;
#elif defined(__APPLE__)
		static std::chrono::steady_clock::time_point s_start;
#else
		static std::chrono::system_clock::time_point s_start;
#endif

	};

}