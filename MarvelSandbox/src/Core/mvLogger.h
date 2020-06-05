#pragma once

#include <imgui.h>


namespace Marvel {

	class AppLog
	{

	public:

		static AppLog* getLogger();

		void Clear ();
		void AddLog(const char* fmt, ...);
		void Draw  (const char* title, bool* p_open = NULL);

	private:

		AppLog();

	private:

		static AppLog* s_instance;
		ImGuiTextBuffer     Buf;
		ImGuiTextFilter     Filter;
		ImVector<int>       LineOffsets;    // Index to lines offset. We maintain this with AddLog() calls, allowing us to have a random access on lines
		bool                AutoScroll;     // Keep scrolling if already at the bottom

	};

}