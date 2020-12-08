#pragma once

#include "mvApp.h"
#include "core/AppItems/mvAppItem.h"
#include "PythonUtilities/mvPythonTranslator.h"
#include <imgui.h>


//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvLogger
//
//-----------------------------------------------------------------------------

namespace Marvel {

	class mvLoggerItem : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Logger, "add_logger")

		mvLoggerItem(const std::string& name)
			: mvAppItem(name)
		{
			m_autoScrollButtonName = "Auto-scroll##" + name;
			m_clearButtonName = "Clear##" + name;
			m_copyButtonName = "Copy##" + name;
			LogInfo("[Logger] " + m_name);
			LogInfo("[Logger] " + m_name);
			m_description.ignoreSizeUpdate = true;
		}

		void draw() override;

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
		bool m_autoScroll        = true;
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
		bool             AutoScroll;  // Keep scrolling if already at the bottom
		bool             show;        // Keep scrolling if already at the bottom
		bool             mainmode;

#if defined (_WIN32)
		static std::chrono::steady_clock::time_point s_start;
#elif defined(__APPLE__)
		static std::chrono::steady_clock::time_point s_start;
#else
		static std::chrono::system_clock::time_point s_start;
#endif

	};

}