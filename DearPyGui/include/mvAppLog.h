#pragma once

#include <imgui.h>
#include <string>
#include <chrono>
#include "mvCore.h"

namespace Marvel {

	class mvAppLog
	{

	public:
		
		static void     AddLog     (const char* fmt, ...);
		static void     Clear      ();
		static void     setLogLevel(unsigned level) { s_loglevel = level; }
		static void     Log        (const std::string& text, const std::string& level = "TRACE");
		static void     LogDebug   (const std::string& text);
		static void     LogInfo    (const std::string& text);
		static void     LogWarning (const std::string& text);
		static void     LogError   (const std::string& text);
		static void     ClearLog   ();
		static void     render     ();
		static void     setSize    (unsigned width, unsigned height);
		static unsigned getLogLevel() { return s_loglevel; }
		static void     Show() { show = true; }
		static void     ShowMain() { show = true; mainmode = true; }

	private:

		mvAppLog() = default;

	private:

		static ImGuiTextBuffer Buf;
		static ImGuiTextFilter Filter;
		static ImVector<int>   LineOffsets;    // Index to lines offset. We maintain this with AddLog() calls, allowing us to have a random access on lines
		static bool            AutoScroll;     // Keep scrolling if already at the bottom
		static bool            show;     // Keep scrolling if already at the bottom
		static bool            mainmode;
		static int             s_loglevel;
		static std::chrono::steady_clock::time_point s_start;
		static unsigned s_width;
		static unsigned s_height;
		static ImGuiWindowFlags s_flags;
	};

}