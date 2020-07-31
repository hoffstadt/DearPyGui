#include "mvAppLog.h"
#include <string>
#include <mutex>
#include "mvApp.h"
#include "Core/mvInput.h"

typedef std::chrono::high_resolution_clock clock_;
typedef std::chrono::duration<double, std::ratio<1> > second_;

namespace Marvel {
#if defined (_WIN32)
    std::chrono::steady_clock::time_point mvAppLog::s_start = clock_::now();
#elif defined(__APPLE__)
    std::chrono::steady_clock::time_point mvAppLog::s_start = clock_::now();
#else
    std::chrono::system_clock::time_point mvAppLog::s_start = clock_::now();
#endif

	ImGuiTextBuffer mvAppLog::Buf;
	bool mvAppLog::show = false;
	bool mvAppLog::mainmode = false;
	ImGuiTextFilter mvAppLog::Filter;
	ImVector<int>   mvAppLog::LineOffsets;    // Index to lines offset. We maintain this with AddLog() calls, allowing us to have a random access on lines
	bool            mvAppLog::AutoScroll = true;     // Keep scrolling if already at the bottom
	int             mvAppLog::s_loglevel = 1;
	unsigned mvAppLog::s_width = 500;
	unsigned mvAppLog::s_height = 500;
	ImGuiWindowFlags mvAppLog::s_flags = ImGuiWindowFlags_NoSavedSettings;

	void mvAppLog::Clear()
	{
		Buf.clear();
		LineOffsets.clear();
		LineOffsets.push_back(0);
	}

	void mvAppLog::AddLog(const char* fmt, ...)
	{
		static std::mutex mutex;

		std::lock_guard<std::mutex> lock(mutex);

		int old_size = Buf.size();
		va_list args;
		va_start(args, fmt);
		Buf.appendfv(fmt, args);
		va_end(args);
		for (int new_size = Buf.size(); old_size < new_size; old_size++)
			if (Buf[old_size] == '\n')
				LineOffsets.push_back(old_size + 1);
	}

	void mvAppLog::render()
	{
		if (!show)
			return;

		if (mainmode)
		{
			ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
			ImGui::SetNextWindowSize(ImVec2(s_width, s_height));
			
			s_flags = ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoSavedSettings
				| ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;
		}
		else
			ImGui::SetNextWindowSize(ImVec2(s_width, s_width), ImGuiCond_FirstUseEver);

		if (!ImGui::Begin("DearPyGui Logger", &show, s_flags))
		{
			ImGui::End();
			return;
		}

		// Options menu
		if (ImGui::BeginPopup("Options"))
		{
			ImGui::Checkbox("Auto-scroll", &AutoScroll);
			ImGui::EndPopup();
		}

		// Main window
		if (ImGui::Button("Options"))
			ImGui::OpenPopup("Options");
		ImGui::SameLine();
		bool clear = ImGui::Button("Clear");
		ImGui::SameLine();
		bool copy = ImGui::Button("Copy");
		ImGui::SameLine();
		Filter.Draw("Filter", -100.0f);

		ImGui::Separator();
		ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

		if (clear)
			Clear();
		if (copy)
			ImGui::LogToClipboard();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		const char* buf = Buf.begin();
		const char* buf_end = Buf.end();
		if (Filter.IsActive())
		{
			// In this example we don't use the clipper when Filter is enabled.
			// This is because we don't have a random access on the result on our filter.
			// A real application processing logs with ten of thousands of entries may want to store the result of search/filter.
			// especially if the filtering function is not trivial (e.g. reg-exp).
			for (int line_no = 0; line_no < LineOffsets.Size; line_no++)
			{
				const char* line_start = buf + LineOffsets[line_no];
				const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
				if (Filter.PassFilter(line_start, line_end))
					ImGui::TextUnformatted(line_start, line_end);
			}
		}
		else
		{


			// The simplest and easy way to display the entire buffer:
			//   ImGui::TextUnformatted(buf_begin, buf_end);
			// And it'll just work. TextUnformatted() has specialization for large blob of text and will fast-forward to skip non-visible lines.
			// Here we instead demonstrate using the clipper to only process lines that are within the visible area.
			// If you have tens of thousands of items and their processing cost is non-negligible, coarse clipping them on your side is recommended.
			// Using ImGuiListClipper requires A) random access into your data, and B) items all being the  same height,
			// both of which we can handle since we an array pointing to the beginning of each line of text.
			// When using the filter (in the block of code above) we don't have random access into the data to display anymore, which is why we don't use the clipper.
			// Storing or skimming through the search result would make it possible (and would be recommended if you want to search through tens of thousands of entries)
			ImGuiListClipper clipper;
			clipper.Begin(LineOffsets.Size);
			while (clipper.Step())
			{
				for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
				{
					bool pop_color = false;

					const char* line_start = buf + LineOffsets[line_no];
					const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;

					std::string mystring = line_start;

					std::string splitstring = mystring.substr(0, 20);

					if (strstr(splitstring.c_str(), "[ERROR]"))
					{
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
						pop_color = true;
					}
					else if (strstr(splitstring.c_str(), "[WARNING]"))
					{
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
						pop_color = true;
					}

					else if (strstr(splitstring.c_str(), "[TRACE]"))
					{
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
						pop_color = true;
					}

					else if (strstr(splitstring.c_str(), "[INFO]"))
					{
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
						pop_color = true;
					}

					else if (strstr(splitstring.c_str(), "[DEBUG]"))
					{
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.25f, 0.50f, 1.0f, 1.0f));
						pop_color = true;
					}

					else
					{
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 1.0f, 1.0f));
						pop_color = true;
					}


					ImGui::TextUnformatted(line_start, line_end);

					if (pop_color)
						ImGui::PopStyleColor();
				}
			}
			clipper.End();
		}
		ImGui::PopStyleVar();

		if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
			ImGui::SetScrollHereY(1.0f);

		ImGui::EndChild();

		if (ImGui::IsWindowFocused())
		{

			float titleBarHeight = ImGui::GetStyle().FramePadding.y * 2 + ImGui::GetFontSize();

			// update mouse
			ImVec2 mousePos = ImGui::GetMousePos();
			mvInput::setGlobalMousePosition(mousePos.x, mousePos.y);
			float x = mousePos.x - ImGui::GetWindowPos().x;
			float y = mousePos.y - ImGui::GetWindowPos().y - titleBarHeight;
			mvInput::setMousePosition(x, y);
			mvApp::GetApp()->setActiveWindow("Log");

		}
		ImGui::End();
	}

	void mvAppLog::Log(const std::string& text, const std::string& level)
	{
		if (s_loglevel < 1)
		{
			auto now = std::chrono::high_resolution_clock::now();
			AddLog("[%0.2f] [%1s]  %2s\n", std::chrono::duration_cast<second_>(clock_::now()-s_start).count(), 
				level.c_str(), text.c_str());
		}
	}

	void mvAppLog::LogDebug(const std::string& text)
	{
		if (s_loglevel < 2)
			AddLog("[%0.2f] [DEBUG]  %1s\n", std::chrono::duration_cast<second_>(clock_::now() - s_start).count(), 
				text.c_str());
	}

	void mvAppLog::LogInfo(const std::string& text)
	{
		if (s_loglevel < 3)
			AddLog("[%0.2f] [INFO]  %1s\n", std::chrono::duration_cast<second_>(clock_::now() - s_start).count(), 
				text.c_str());
	}

	void mvAppLog::LogWarning(const std::string& text)
	{
		if (s_loglevel < 4)
			AddLog("[%0.2f] [WARNING]  %1s\n", std::chrono::duration_cast<second_>(clock_::now() - s_start).count(), 
				text.c_str());
	}

	void mvAppLog::LogError(const std::string& text)
	{
		if (s_loglevel < 5)
			AddLog("[%0.2f] [ERROR]  %1s\n", std::chrono::duration_cast<second_>(clock_::now() - s_start).count(), 
				text.c_str());
	}

	void mvAppLog::ClearLog()
	{
		Clear();
	}

	void mvAppLog::setSize(unsigned width, unsigned height)
	{
		s_width = width;
		s_height = height;
	}

}