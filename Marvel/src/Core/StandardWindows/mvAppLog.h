#pragma once

#include <imgui.h>
#include <string>
#include <chrono>
#include "mvStandardWindow.h"
#include "Core/mvCore.h"

namespace Marvel {

	class mvAppLog : public mvStandardWindow
	{

	public:

		static mvAppLog*         getLogger();
		static mvStandardWindow* GetLoggerStandardWindow();
		
		void         AddLog(const char* fmt, ...);
		void         Clear      ();
		void         setLogLevel(unsigned level) { m_loglevel = level; }
		void         Log        (const std::string& text, const std::string& level = "TRACE");
		void         LogDebug   (const std::string& text);
		void         LogInfo    (const std::string& text);
		void         LogWarning (const std::string& text);
		void         LogError   (const std::string& text);
		void         ClearLog   ();
		virtual void render     (bool& show) override;
		unsigned     getLogLevel() const { return m_loglevel; }

	private:

		mvAppLog();

	private:

		static mvAppLog*    s_instance;
		ImGuiTextBuffer     Buf;
		ImGuiTextFilter     Filter;
		ImVector<int>       LineOffsets;    // Index to lines offset. We maintain this with AddLog() calls, allowing us to have a random access on lines
		bool                AutoScroll;     // Keep scrolling if already at the bottom
		int                 m_loglevel =1;

		static std::chrono::steady_clock::time_point s_start;
	};

}