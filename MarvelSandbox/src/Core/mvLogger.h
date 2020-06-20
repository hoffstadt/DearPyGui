#pragma once

#include <imgui.h>
#include <string>

namespace Marvel {

	class AppLog
	{

	public:

		static AppLog* getLogger();

		void Clear ();
		void AddLog(const char* fmt, ...);
		void setLogLevel(unsigned level) { m_loglevel = level; }
		void Log(const std::string& text, const std::string& level = "TRACE");
		void LogDebug(const std::string& text);
		void LogInfo(const std::string& text);
		void LogWarning(const std::string& text);
		void LogError(const std::string& text);
		void ClearLog();
		void Draw  (const char* title, bool* p_open = NULL);

	private:

		AppLog();

	private:

		static AppLog* s_instance;
		ImGuiTextBuffer     Buf;
		ImGuiTextFilter     Filter;
		ImVector<int>       LineOffsets;    // Index to lines offset. We maintain this with AddLog() calls, allowing us to have a random access on lines
		bool                AutoScroll;     // Keep scrolling if already at the bottom
		int                 m_loglevel =0;

	};

}