#include "mvLogger.h"
#include <chrono>
#include "mvInput.h"
#include "mvGlobalIntepreterLock.h"

typedef std::chrono::high_resolution_clock clock_;
typedef std::chrono::duration<double, std::ratio<1> > second_;

namespace Marvel {
	void mvLoggerItem::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_logger", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "log_level", "", "1"},
			{mvPythonDataType::Bool, "auto_scroll", "auto scroll", "True"},
			{mvPythonDataType::Bool, "auto_scroll_button", "show auto scroll button", "True"},
			{mvPythonDataType::Bool, "clear_button", "show clear button", "True"},
			{mvPythonDataType::Bool, "copy_button", "show copy button", "True"},
			{mvPythonDataType::Bool, "filter", "show filter", "True"},
			{mvPythonDataType::Integer, "width","", "0"},
			{mvPythonDataType::Integer, "height","", "0"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before","This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::Bool, "autosize_x", "", "False"},
			{mvPythonDataType::Bool, "autosize_y", "", "False"},

		}, "Adds a logging widget.", "None", "Adding Widgets") });
	}

#if defined (_WIN32)
	std::chrono::steady_clock::time_point mvLoggerItem::s_start = clock_::now();
#elif defined(__APPLE__)
	std::chrono::steady_clock::time_point mvLoggerItem::s_start = clock_::now();
#else
	std::chrono::system_clock::time_point mvLoggerItem::s_start = clock_::now();
#endif

	void mvLoggerItem::draw()
	{
		ImGui::BeginGroup();

		auto styleManager = m_styleManager.getScopedStyleManager();
		ImGui::PushID(this);
		
		// auto scroll button
		if (m_autoScrollButton)
		{
			ImGui::Checkbox(m_autoScrollButtonName.c_str(), &AutoScroll);
			ImGui::SameLine();
		}

		if (m_clearButton)
		{
			if (ImGui::Button(m_clearButtonName.c_str()))
				ClearLog();
			ImGui::SameLine();
		}

		if (m_copyButton)
		{
			if (ImGui::Button(m_copyButtonName.c_str()))
				ImGui::LogToClipboard();
			ImGui::SameLine();
		}

		if (m_filter)
		{
			ImGui::NewLine();
			Filter.Draw("Filter", m_width-100.0f);
		}


		//ImGui::NewLine();
		//ImGui::Separator();


		ImGui::BeginChild(m_name.c_str(), ImVec2(m_autosize_x ? 0 : (float)m_width, m_autosize_y ? 0 : (float)m_height), true, ImGuiWindowFlags_AlwaysHorizontalScrollbar);

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

					const char* line_start = buf + LineOffsets[line_no];
					const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;

					std::string mystring = line_start;

					std::string splitstring = mystring.substr(0, 20);

					if (strstr(splitstring.c_str(), "[ERROR]"))
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));

					else if (strstr(splitstring.c_str(), "[WARNING]"))
						ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_PlotHistogramHovered));

					else if (strstr(splitstring.c_str(), "[TRACE]"))
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));

					else if (strstr(splitstring.c_str(), "[INFO]"))
						ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_Text));

					else if (strstr(splitstring.c_str(), "[DEBUG]"))

						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.25f, 0.50f, 1.0f, 1.0f));

					else
						ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_Text));

					ImGui::TextUnformatted(line_start, line_end);
					ImGui::PopStyleColor();
				}
			}
			clipper.End();
		}
		ImGui::PopStyleVar();

		if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
			ImGui::SetScrollHereY(0.0f);

		if (ImGui::IsWindowFocused())
		{

			float titleBarHeight = ImGui::GetStyle().FramePadding.y * 2 + ImGui::GetFontSize();

			// update mouse
			ImVec2 mousePos = ImGui::GetMousePos();
			mvInput::setGlobalMousePosition(mousePos.x, mousePos.y);
			float x = mousePos.x - ImGui::GetWindowPos().x;
			float y = mousePos.y - ImGui::GetWindowPos().y - titleBarHeight;
			mvInput::setMousePosition(x, y);

			if (mvApp::GetApp()->getItemRegistry().getActiveWindow() != m_name)
				mvEventBus::Publish(mvEVT_CATEGORY_ITEM, mvEVT_ACTIVE_WINDOW, { CreateEventArgument("WINDOW", m_name) });


		}

		m_width = (int)ImGui::GetWindowWidth();
		m_height = (int)ImGui::GetWindowHeight();

		ImGui::EndChild();
		ImGui::PopID();
		ImGui::EndGroup();

		if (ImGui::IsWindowFocused())
		{

			float titleBarHeight = ImGui::GetStyle().FramePadding.y * 2 + ImGui::GetFontSize();

			// update mouse
			ImVec2 mousePos = ImGui::GetMousePos();
			mvInput::setGlobalMousePosition(mousePos.x, mousePos.y);
			float x = mousePos.x - ImGui::GetWindowPos().x;
			float y = mousePos.y - ImGui::GetWindowPos().y - titleBarHeight;
			mvInput::setMousePosition(x, y);

			if (mvApp::GetApp()->getItemRegistry().getActiveWindow() != "logger##standard")
				mvEventBus::Publish(mvEVT_CATEGORY_ITEM, mvEVT_ACTIVE_WINDOW, { CreateEventArgument("WINDOW", std::string("logger##standard")) });


		}

	}

	void mvLoggerItem::AddLog(const char* fmt, ...)
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

	void mvLoggerItem::Log(const std::string& text, const std::string& level)
	{
		if (m_loglevel < 1)
		{
			AddLog("[%0.2f] [%1s]  %2s\n", std::chrono::duration_cast<second_>(clock_::now() - s_start).count(),
				level.c_str(), text.c_str());
		}
	}

	void mvLoggerItem::LogDebug(const std::string& text)
	{
		if (m_loglevel < 2)
			AddLog("[%0.2f] [DEBUG]  %1s\n", std::chrono::duration_cast<second_>(clock_::now() - s_start).count(),
				text.c_str());
	}

	void mvLoggerItem::LogInfo(const std::string& text)
	{
		if (m_loglevel < 3)
			AddLog("[%0.2f] [INFO]  %1s\n", std::chrono::duration_cast<second_>(clock_::now() - s_start).count(),
				text.c_str());
	}

	void mvLoggerItem::LogWarning(const std::string& text)
	{
		if (m_loglevel < 4)
			AddLog("[%0.2f] [WARNING]  %1s\n", std::chrono::duration_cast<second_>(clock_::now() - s_start).count(),
				text.c_str());
	}

	void mvLoggerItem::LogError(const std::string& text)
	{
		if (m_loglevel < 5)
			AddLog("[%0.2f] [ERROR]  %1s\n", std::chrono::duration_cast<second_>(clock_::now() - s_start).count(),
				text.c_str());
	}

	void mvLoggerItem::ClearLog()
	{
		Buf.clear();
		LineOffsets.clear();
		LineOffsets.push_back(0);
	}

	void mvLoggerItem::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;
		if (PyObject* item = PyDict_GetItemString(dict, "log_level")) m_loglevel = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "auto_scroll")) m_autoScroll = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "auto_scroll_button")) m_autoScrollButton = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "clear_button")) m_clearButton = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "copy_button")) m_copyButton = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "filter")) m_filter = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "autosize_x")) m_autosize_x = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "autosize_y")) m_autosize_y = ToBool(item);

	}

	void mvLoggerItem::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;

		PyDict_SetItemString(dict, "log_level", ToPyInt(m_loglevel));
		PyDict_SetItemString(dict, "auto_scroll", ToPyBool(m_autoScroll));
		PyDict_SetItemString(dict, "auto_scroll_button", ToPyBool(m_autoScrollButton));
		PyDict_SetItemString(dict, "clear_button", ToPyBool(m_clearButton));
		PyDict_SetItemString(dict, "copy_button", ToPyBool(m_copyButton));
		PyDict_SetItemString(dict, "filter", ToPyBool(m_filter));
		PyDict_SetItemString(dict, "autosize_x", ToPyBool(m_autosize_x));
		PyDict_SetItemString(dict, "autosize_y", ToPyBool(m_autosize_y));
	}

	PyObject* add_logger(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int logLevel = 1;
		int autoScroll = true;
		int autoScrollButton = true;
		int copyButton = true;
		int clearButton = true;
		int filter = true;
		int width = 0;
		int height = 0;
		const char* parent = "";
		const char* before = "";
		int show = true;
		int autosize_x = false;
		int autosize_y = false;

		if (!(*mvApp::GetApp()->getParsers())["add_logger"].parse(args, kwargs, __FUNCTION__,
			&name, &logLevel, &autoScroll, &autoScrollButton, &copyButton, &clearButton,
			&filter, &width, &height, &parent, &before, &show, &autosize_x, &autosize_y))
			return ToPyBool(false);

		auto item = CreateRef<mvLoggerItem>(name);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		return ToPyBool(mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before));
	}

}