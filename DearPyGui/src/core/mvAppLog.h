#pragma once

#include <imgui.h>
#include <string>
#include <atomic>
#include <chrono>
#include "mvCore.h"
#include "mvPython.h"

namespace Marvel {

	class mvAppLog
	{

	public:
		
		static void                   AddLog         (const char* fmt, ...);
		static void                   Clear          ();
		static void                   setLogLevel    (int level);
		static void                   Log            (const std::string& text, const std::string& level = "TRACE");
		static void                   LogDebug       (const std::string& text);
		static void                   LogInfo        (const std::string& text);
		static void                   LogWarning     (const std::string& text);
		static void                   LogError       (const std::string& text);
		static void                   ClearLog       ();
		static void                   render         ();
		static void                   setSize        (unsigned width, unsigned height);
		static unsigned               getLogLevel    ();
		static void                   Show           ();
		static void                   ShowMain       ();
		static void                   SetWindowPos   (float x, float y);
		static void                   SetWidth       (int width);
		static void                   SetHeight      (int height);
		[[nodiscard]] static unsigned GetWindowWidth ();
		[[nodiscard]] static unsigned GetWindowHeight();
		static mvVec2                 GetWindowPos   ();
		static void                   SetConfigDict(PyObject* dict);
		static void                   GetConfigDict(PyObject* dict);
		static void                   Focus          ();
		static void                   setTitle       (const std::string& title) { s_title = title; }
		
	private:

		mvAppLog() = default;

	private:

		static ImGuiTextBuffer  Buf;
		static ImGuiTextFilter  Filter;
		static ImVector<int>    LineOffsets; // Index to lines offset. We maintain this with AddLog() calls, allowing us to have a random access on lines
		static bool             AutoScroll;  // Keep scrolling if already at the bottom
		//static bool             ScrollToBottom;  // Keep scrolling if already at the bottom
		static bool             show;        // Keep scrolling if already at the bottom
		static bool             mainmode;
		static int              s_loglevel;
		static unsigned         s_width;
		static unsigned         s_height;
		static ImGuiWindowFlags s_flags;
		static int              s_xpos;
		static int              s_ypos;
		static bool             s_dirty_pos;
		static bool             s_dirty_size;
		static bool             s_focus;
		static std::string      s_title;

#if defined (_WIN32)
        static std::chrono::steady_clock::time_point s_start;
#elif defined(__APPLE__)
        static std::chrono::steady_clock::time_point s_start;
#else
        static std::chrono::system_clock::time_point s_start;
#endif
	};

}