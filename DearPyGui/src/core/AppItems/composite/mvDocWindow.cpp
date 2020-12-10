#include "mvDocWindow.h"
#include <imgui.h>
#include "mvApp.h"
#include "mvMarvel.h"
#include "core/mvInput.h"
#include "mvPythonTranslator.h"

namespace Marvel {

	static void ColorText(const char* item)
	{
		ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_PlotLinesHovered), "%s", item);
	}

	static void CodeColorText(const char* item)
	{
		ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_CheckMark), "%s", item);
	}

	static void WidgetTableEntry(const char* widget, const char* container, const char* callback, const char* source, const char* desc)
	{
		ImGui::Separator();
		ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_DragDropTarget), widget); ImGui::NextColumn();
		ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_CheckMark), container); ImGui::NextColumn();
		ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_PlotHistogramHovered), callback); ImGui::NextColumn();
		ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_PlotLinesHovered), source); ImGui::NextColumn();
		ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_Text), desc); ImGui::NextColumn();
	}

	mvDocWindow::mvDocWindow(const std::string& name)
		: mvBaseWindowAppitem(name)
	{
		m_width = 700;
		m_height = 500;
		m_description.deleteAllowed = false;
		setup();
	}

	mvDocWindow::~mvDocWindow()
	{
		m_docmap->clear();
		delete m_docmap;
	}

	void mvDocWindow::setup()
	{
		m_docmap = BuildDearPyGuiInterface();
		m_constants = GetModuleConstants();

		for (auto& item : m_constants)
		{
			m_cconstants.push_back(item.first.c_str());
			m_constantsValues.push_back("Constant with a value of " + std::to_string(item.second));
		}

		for (const auto& item : *m_docmap)
		{
			m_commands.emplace_back(item.first.c_str(), item.second.getDocumentation());

			std::string category = item.second.getCategory();

			if (category == "App")
			{
				m_app.push_back(item.first.c_str());
				m_docApp.push_back(item.second.getDocumentation());
			}

			else if (category == "Logging")
			{
				m_logging.push_back(item.first.c_str());
				m_docLogging.push_back(item.second.getDocumentation());
			}

			else if (category == "Adding Widgets")
			{
				m_widgets.push_back(item.first.c_str());
				m_docWidgets.push_back(item.second.getDocumentation());
			}

			else if (category == "Widget Commands")
			{
				m_widgetsCommands.push_back(item.first.c_str());
				m_docWidgetsCommands.push_back(item.second.getDocumentation());
			}

			else if (category == "Containers")
			{
				m_containers.push_back(item.first.c_str());
				m_docContainers.push_back(item.second.getDocumentation());
			}

			else if (category == "Drawing")
			{
				m_drawing.push_back(item.first.c_str());
				m_docDrawing.push_back(item.second.getDocumentation());
			}

			else if (category == "Plotting")
			{
				m_plotting.push_back(item.first.c_str());
				m_docPlotting.push_back(item.second.getDocumentation());
			}

			else if (category == "Tables")
			{
				m_tables.push_back(item.first.c_str());
				m_docTables.push_back(item.second.getDocumentation());
			}

			else if (category == "Themes and Styles")
			{
				m_themes.push_back(item.first.c_str());
				m_docThemes.push_back(item.second.getDocumentation());
			}

			else if (category == "Input Polling")
			{
				m_inputs.push_back(item.first.c_str());
				m_docInputs.push_back(item.second.getDocumentation());
			}

			else if (category == "Standard Windows")
			{
				m_windows.push_back(item.first.c_str());
				m_docWindows.push_back(item.second.getDocumentation());
			}

		}


	}

	void mvDocWindow::draw()
	{

		if (!prerender())
			return;

		if (ImGui::BeginTabBar("Main Tabbar##doc"))
		{
			if (ImGui::BeginTabItem("Help##doc"))
			{


				ColorText("ABOUT THIS GUIDE:");
				ImGui::BulletText("Sections below cover the basic aspects of DearPyGui Core.");
				ImGui::BulletText("This document can be found by calling the \"show_documentation\" command.");
				ImGui::BulletText("Additional information on specific commands can be found in the commands tab.");
				ImGui::BulletText("The complete documentation can be found at: https://hoffstadt.github.io/DearPyGui/");


				if (ImGui::CollapsingHeader("Logging and Console Output"))
				{

					ColorText("LOGGING");
					ImGui::BulletText("In order to show the logger window, you must use the \"show_logger\" command.");
					ImGui::BulletText("In order to filter what is displayed, you can set the log level.");
					ImGui::BulletText("By Default the logger is set to log level mvDebug.");
					ImGui::BulletText("Can be set like \"set_log_level(mvINFO)\"");
					ImGui::BulletText("The log levels are:");

					ImGui::Columns(3);
					ImGui::Separator();
					ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_DragDropTarget), "level"); ImGui::NextColumn();
					ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_DragDropTarget), "constant"); ImGui::NextColumn();
					ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_DragDropTarget), "command"); ImGui::NextColumn();

					ImGui::Separator();
					CodeColorText("TRACE"); ImGui::NextColumn();
					ColorText("mvTRACE"); ImGui::NextColumn();
					CodeColorText("log('some message')"); ImGui::NextColumn();

					ImGui::Separator();
					CodeColorText("DEBUG"); ImGui::NextColumn();
					ColorText("mvDEBUG"); ImGui::NextColumn();
					CodeColorText("log_debug('some message')"); ImGui::NextColumn();

					ImGui::Separator();
					CodeColorText("INFO"); ImGui::NextColumn();
					ColorText("mvINFO"); ImGui::NextColumn();
					CodeColorText("log_info('some message')"); ImGui::NextColumn();

					ImGui::Separator();
					CodeColorText("WARNING"); ImGui::NextColumn();
					ColorText("mvWARNING"); ImGui::NextColumn();
					CodeColorText("log_warning('some message')"); ImGui::NextColumn();

					ImGui::Separator();
					CodeColorText("ERROR"); ImGui::NextColumn();
					ColorText("ERROR"); ImGui::NextColumn();
					CodeColorText("log_error('some message')"); ImGui::NextColumn();

					ImGui::Separator();
					CodeColorText("OFF"); ImGui::NextColumn();
					ColorText("mvOFF"); ImGui::NextColumn(); ImGui::NextColumn();

					ImGui::Columns(1);


				}


				if (ImGui::CollapsingHeader("Widgets"))
				{

					ColorText("WIDGETS:");
					ImGui::Columns(5);
					WidgetTableEntry("WIDGET", "CONTAINER", "CALLBACK", "SOURCE TYPE", "DESCRIPTION");
					WidgetTableEntry("button", "", "yes", "", "button");
					WidgetTableEntry("checkbox", "", "yes", "bool", "checkbox");
					WidgetTableEntry("child", "yes", "", "bool", "embedded window");
					WidgetTableEntry("collapsing header", "yes", "", "bool", "collapsible header");
					WidgetTableEntry("color button", "", "yes", "", "color button");
					WidgetTableEntry("color edit3", "", "yes", "color", "rgb color editing widget");
					WidgetTableEntry("color edit4", "", "yes", "color", "rgba color editing widget");
					WidgetTableEntry("color picker3", "", "yes", "color", "rgb color picking widget");
					WidgetTableEntry("color picker4", "", "yes", "color", "rgba color picking widget");
					WidgetTableEntry("columns", "yes", "", "", "a layout widget");
					WidgetTableEntry("combo", "", "yes", "string", "dropdown combo box");
					WidgetTableEntry("date picker", "", "yes", "time", "data selector widget");
					WidgetTableEntry("drag float", "", "yes", "float", "drag widget");
					WidgetTableEntry("drag float2", "", "yes", "[float, float]", "drag widget");
					WidgetTableEntry("drag float3", "", "yes", "[float, float, float]", "drag widget");
					WidgetTableEntry("drag float4", "", "yes", "[float, float, float, float]", "drag widget");
					WidgetTableEntry("drag int", "", "yes", "int", "drag widget");
					WidgetTableEntry("drag int2", "", "yes", "[int, int]", "drag widget");
					WidgetTableEntry("drag int3", "", "yes", "[int, int, int]", "drag widget");
					WidgetTableEntry("drag int4", "", "yes", "[int, int, int, int]", "drag widget");
					WidgetTableEntry("drawing", "", "", "", "embeddable canvas");
					WidgetTableEntry("dummy", "", "", "", "spacing widget");
					WidgetTableEntry("group", "yes", "", "", "grouping/layout widget");
					WidgetTableEntry("image", "", "", "", "image widget");
					WidgetTableEntry("image button", "", "yes", "", "clickable image");
					WidgetTableEntry("indent", "", "", "float", "layout widget, indents until unindent called");
					WidgetTableEntry("input float", "", "yes", "float", "input widget");
					WidgetTableEntry("input float2", "", "yes", "[float, float]", "input widget");
					WidgetTableEntry("input float3", "", "yes", "[float, float, float]", "input widget");
					WidgetTableEntry("input float4", "", "yes", "[float, float, float, float]", "input widget");
					WidgetTableEntry("input int", "", "yes", "int", "input widget");
					WidgetTableEntry("input int2", "", "yes", "[int, int]", "input widget");
					WidgetTableEntry("input int3", "", "yes", "[int, int, int]", "input widget");
					WidgetTableEntry("input int4", "", "yes", "[int, int, int, int]", "input widget");
					WidgetTableEntry("input text", "", "yes", "string", "input widget");
					WidgetTableEntry("listbox", "", "yes", "int", "item list widget");
					WidgetTableEntry("logger", "", "", "", "logger widget");
					WidgetTableEntry("managed columns", "yes", "", "", "layout widget");
					WidgetTableEntry("menu", "yes", "", "bool", "menu widget");
					WidgetTableEntry("menu bar", "yes", "", "bool", "menu bar widget");
					WidgetTableEntry("menu item", "yes", "", "bool", "menu item widget");
					WidgetTableEntry("next column", "", "", "", "layout widget");
					WidgetTableEntry("popup", "yes", "", "bool", "popup window");
					WidgetTableEntry("progress bar", "", "", "float", "progress bar widget");
					WidgetTableEntry("radio button", "", "yes", "int", "set of radio buttons");
					WidgetTableEntry("same line", "", "", "", "layout widget, places next item on same line");
					WidgetTableEntry("selectable", "", "yes", "bool", "selectable widget");
					WidgetTableEntry("seperator", "", "", "", "horizontal line");
					WidgetTableEntry("simple plot", "", "", "List[float]", "simple plot for data visualization");
					WidgetTableEntry("slider float", "", "yes", "float", "slider widget");
					WidgetTableEntry("slider float2", "", "yes", "[float, float]", "slider widget");
					WidgetTableEntry("slider float3", "", "yes", "[float, float, float]", "slider widget");
					WidgetTableEntry("slider float4", "", "yes", "[float, float, float, float]", "slider widget");
					WidgetTableEntry("slider int", "", "yes", "int", "slider widget");
					WidgetTableEntry("slider int2", "", "yes", "[int, int]", "slider widget");
					WidgetTableEntry("slider int3", "", "yes", "[int, int, int]", "slider widget");
					WidgetTableEntry("slider int4", "", "yes", "[int, int, int, int]", "slider widget");
					WidgetTableEntry("spacing", "", "", "int", "layout widget, vertical spacing");
					WidgetTableEntry("tab", "yes", "", "bool", "layout widget, tab widget");
					WidgetTableEntry("tab bar", "yes", "", "string", "layout widget, tab container");
					WidgetTableEntry("tab button", "", "yes", "", "button for tab bars");
					WidgetTableEntry("table", "", "yes", "", "simple table widget");
					WidgetTableEntry("text", "", "", "string", "text widget");
					WidgetTableEntry("time picker", "", "yes", "time", "time selecting widget");
					WidgetTableEntry("tooltip", "yes", "", "bool", "window that appears on hovering another item");
					WidgetTableEntry("tree node", "yes", "", "bool", "collapsing node widget");
					WidgetTableEntry("window", "yes", "", "", "new window widget");

					ImGui::Columns(1);

				}

				if (ImGui::CollapsingHeader("Widget Basics"))
				{
					ColorText("BASICS:");
					ImGui::BulletText("Widgets are added with a command of the form \"add_*\"");
					ImGui::BulletText("Some widgets can contain other widgets.");
					ImGui::Separator();

					ColorText("WIDGETS NAMING:");
					ImGui::BulletText("Widgets MUST have unique names.");
					ImGui::BulletText("Most widgets require you to give a name with the exception of:");
					ImGui::Indent();
					CodeColorText("seperator");
					CodeColorText("spacing");
					CodeColorText("same_line");
					CodeColorText("indent");
					CodeColorText("unindent");
					CodeColorText("next_column");
					ImGui::Unindent();
					ImGui::BulletText("The above widget's names are optional (they are auto generated).");
					ImGui::BulletText("If a widget uses the name to display text (i.e. button) and you need");
					ImGui::Text("   multiple buttons to display the same text, you can do the following:");
					ImGui::Indent();
					CodeColorText("add_button('Press me')");
					CodeColorText("add_button('Press me##123')");
					CodeColorText("add_button('Some unique name', label='Press me')");
					ImGui::Unindent();
					ImGui::Separator();

					ColorText("ADDING WIDGETS:");
					ImGui::BulletText("There are 2 methods for adding widgets.");
					ImGui::Indent();
					ImGui::BulletText("METHOD 1 (RECOMMENDED):");
					ImGui::Indent();
					ImGui::Text("You simply call the add_* commands, while creating a nested hierarchy by");
					ImGui::Text("placing widgets between the add_* and end_* of container widgets.");
					ImGui::Text("With this method, widgets automatically know which containers they");
					ImGui::Text("belong to. Example:");
					ImGui::Spacing();
					CodeColorText("add_child(...)");
					CodeColorText("add_button(...)  # belongs to child");
					CodeColorText("add_group(...)   # belongs to child");
					CodeColorText("add_button(...)  # belongs to group");
					CodeColorText("add_button(...)  # belongs to group");
					CodeColorText("end()");
					CodeColorText("end()");
					ImGui::Unindent();
					ImGui::BulletText("METHOD 2:");
					ImGui::Indent();
					ImGui::Text("You add widgets to their parent containers using the \"parent\" keyword.");
					ImGui::Text("Containers are created by calling their add_* command followed immediately");
					ImGui::Text("by an end command.");
					ImGui::Text("Example:");
					ImGui::Spacing();
					CodeColorText("add_child(...)");
					CodeColorText("end()");
					CodeColorText("add_button(..., parent='childname')");
					ImGui::Unindent();
					ImGui::Unindent();

				}

				if (ImGui::CollapsingHeader("Data Storage"))
				{

					ColorText("USE CASE");
					ImGui::BulletText("To store data for later usage, just use the following commands:");
					ImGui::Indent();
					CodeColorText("add_data");
					CodeColorText("get_data");
					ImGui::Unindent();
					ImGui::Separator();

				}

				if (ImGui::CollapsingHeader("Value Storage"))
				{
					ColorText("BASICS");
					ImGui::BulletText("There are 3 use cases for the data storage system:");
					ImGui::Indent();
					ImGui::BulletText("1. For widgets to use the same underlying data (i.e. both modify the same data)");
					ImGui::BulletText("2. To store data for use at a later time or in a different callback.");
					ImGui::BulletText("3. A combination of 1 and 2.");
					ImGui::Unindent();
					ImGui::Separator();

					ColorText("USE CASE 1");
					ImGui::BulletText("To share the underlying data of multiple widgets, just use the 'source' keyword when adding the widget.");
					ImGui::BulletText("Ensure the underlying data structures match or it will not work.");
					ImGui::BulletText("Some widgets have a 'secondary_data_source' (i.e. listbox, which allows modification of the listed items)");
					ImGui::Separator();

					ColorText("USE CASE 2");
					ImGui::BulletText("To store data for later usage, just use the following commands:");
					ImGui::Indent();
					CodeColorText("add_value");
					CodeColorText("get_value");
					CodeColorText("set_value");
					ImGui::Unindent();
					ImGui::Separator();

					ColorText("USE CASE 3");
					ImGui::BulletText("With this use case, you store data with 'add_data' and use that data with the 'ource' keyword.");

				}

				if (ImGui::CollapsingHeader("Callbacks"))
				{
					ColorText("BASICS:");
					ImGui::BulletText("Callbacks are functions that are ran when some event occurs.");
					ImGui::BulletText("There are currently 3 'types' of callbacks used in DearPyGui:");
					ImGui::Indent();
					CodeColorText("Render  - ran every frame");
					CodeColorText("Inputs  - ran when mouse/keyboard events occurr");
					CodeColorText("Widgets - ran when certain widgets are interacted with");
					ImGui::Unindent();
					ImGui::BulletText("Notes:");
					ImGui::Indent();
					ImGui::BulletText("The 'set_*_callback' commands take a string with the name of the callback to use.");
					ImGui::BulletText("Always be ran on the same thread as the GUI.");
					ImGui::BulletText("Should NOT be computationally expensive (because of the note above),");
					ImGui::Text("otherwise you may see a drop in frame rate.");
					ImGui::Unindent();
					ImGui::Separator();

					ColorText("RENDER CALLBACK:");
					ImGui::BulletText("Useful for live updating (for example a progress bar)");
					ImGui::BulletText("The command is 'set_render_callback(...)'");
					ImGui::BulletText("Notes:");
					ImGui::Indent();
					ImGui::BulletText("Should take the following form:");
					ImGui::Indent();
					CodeColorText("def callbackname(sender, data):");
					CodeColorText("    ...");
					ImGui::Unindent();
					ImGui::BulletText("The data parameter depends on the actual callback. Check the documentation.");
					ImGui::Unindent();
					ImGui::Separator();

					ColorText("INPUT CALLBACKS:");
					ImGui::BulletText("Useful for capturing key strokes, making iteractive drawings, etc.");
					ImGui::BulletText("Commands take the form of 'set_*_callback(...)'");
					ImGui::BulletText("The optional 'handler' keyword will set which window triggers the callback.");
					ImGui::BulletText("Most should take the following form:");
					ImGui::Indent();
					CodeColorText("def callbackname(sender, data):");
					CodeColorText("    ...");
					ImGui::Unindent();
					ImGui::BulletText("The 'sender' parameter is the name of widget the callback was sent from.");
					ImGui::BulletText("The data parameter depends on the actual callback. Check the documentation.");
					ImGui::Separator();

					ColorText("WIDGET CALLBACKS:");
					ImGui::BulletText("Useful for performing actions during widget interactions.");
					ImGui::BulletText("Set using the 'callback' keyword.");
					ImGui::BulletText("Should take the following form:");
					ImGui::Indent();
					CodeColorText("def callbackname(sender, data):");
					CodeColorText("    ...");
					ImGui::Unindent();
					ImGui::BulletText("The 'sender' parameter is the name of widget the callback was sent from.");
					ImGui::BulletText("The data parameter depends on the actual callback. Check the documentation.");

				}

				if (ImGui::CollapsingHeader("Concurrency and Asyncronous Functions"))
				{

					ColorText("BASICS:");
					ImGui::BulletText("If a callback needs to perform time consuming calculations, functions can be");
					ImGui::Text("    ran asyncronously (on seperate threads).");
					ImGui::BulletText("Currently, the async function CAN NOT call Marvel API commands.");

					ImGui::BulletText("The following command is used for asyncronous functions:");
					ImGui::Indent();
					CodeColorText("run_async_function(name, data, return_handler)");
					ImGui::Unindent();
					ImGui::BulletText("Notes:");
					ImGui::Indent();
					ImGui::BulletText("name - the function name to run concurrently");
					ImGui::BulletText("data - any data needed by the function");
					ImGui::BulletText("return_hander - the name of a function to run on completion");
					ImGui::Unindent();
					ImGui::Separator();

					ColorText("RETURN HANDLER:");
					ImGui::BulletText("The return handler is used to provide any updates to the GUI following");
					ImGui::Text("    an asyncronous function call. This function is ran on the main thread.");
					ImGui::BulletText("Takes the following form:");
					ImGui::Indent();
					CodeColorText("def ReturnHandler(data):");
					CodeColorText("    ...");
					ImGui::Unindent();
					ImGui::BulletText("Where data is the return value of the async function (if anything is returned).");
					ImGui::Separator();

					ColorText("THREADPOOL:");
					ImGui::BulletText("When an asyncronous function is called, a threadpool is created to manage threads.");
					ImGui::BulletText("There are 2 aspects of this threadpool that can be configured:");
					ImGui::Indent();
					ImGui::BulletText("Number of threads using the following commands:");
					CodeColorText("set_thread_count(...)                # sets number of threads to use (default is 2)");
					CodeColorText("set_threadpool_high_performance(...) # sets thread count to maximum");
					ImGui::BulletText("Timeout using the following command:");
					CodeColorText("set_threadpool_timeout(...) # sets the timeout before destroying threadpool when not in use");
					ImGui::Unindent();

				}

				if (ImGui::CollapsingHeader("Input Polling"))
				{
					ImGui::BulletText("During any callback, you can poll information about the mouse and keyboard.");
					ImGui::BulletText("The following commands are used for this:");
					ImGui::Indent();
					CodeColorText("get_mouse_pos");
					CodeColorText("is_key_pressed");
					CodeColorText("is_key_released");
					CodeColorText("is_key_down");
					CodeColorText("is_mouse_button_dragging");
					CodeColorText("is_mouse_button_down");
					CodeColorText("is_mouse_button_clicked");
					CodeColorText("is_mouse_button_double_clicked");
					CodeColorText("is_mouse_button_released");
					ImGui::Unindent();

				}

				if (ImGui::CollapsingHeader("Drawings"))
				{
					ColorText("BASICS:");
					ImGui::BulletText("The drawing API is a low-level drawing API useful as a canvas or custom widget.");
					ImGui::BulletText("Drawings are added using the 'add_drawing(...)' command.");
					ImGui::BulletText("All drawing commands' first argument is the name of the drawing you are refering to.");
					ImGui::BulletText("The drawings scale and coordinate origin can be set through keywords");
					ImGui::Separator();

					ColorText("TAGS:");
					ImGui::BulletText("To efficiently update a drawing, you should use the tag system (as opposed to clearing and redrawing)");
					ImGui::BulletText("All draw commands of the form 'draw_*' accept an optional tag argument.");
					ImGui::BulletText("The tag argument is a string that acts as an indentifier for the draw item.");
					ImGui::BulletText("On subsequent calls to the same draw command with the given tag, the item will be updated.");
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Commands##doc"))
			{
				ImGui::BeginGroup();
				ImGui::SetNextItemWidth(500);
				ImGui::ListBox("Category", &categorySelection, &m_categories[0], 12, 7);

				ImGui::SetNextItemWidth(500);
				ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(255, 0, 0, 100));
				if (categorySelection == 0)
				{
					static int selection = 0;
					if (ImGui::ListBox("Commands", &selection, m_app.data(), (int)m_app.size(), 30))
						m_doc = m_docApp[selection];
				}

				else if (categorySelection == 1)
				{
					static int selection = 0;
					if (ImGui::ListBox("Commands", &selection, m_logging.data(), (int)m_logging.size(), 30))
						m_doc = m_docLogging[selection];
				}

				else if (categorySelection == 2)
				{
					static int selection = 0;
					if (ImGui::ListBox("Commands", &selection, m_widgets.data(), (int)m_widgets.size(), 30))
						m_doc = m_docWidgets[selection];
				}

				else if (categorySelection == 3)
				{
					static int selection = 0;
					if (ImGui::ListBox("Commands", &selection, m_widgetsCommands.data(), (int)m_widgetsCommands.size(), 30))
						m_doc = m_docWidgetsCommands[selection];
				}

				else if (categorySelection == 4)
				{
					static int selection = 0;
					if (ImGui::ListBox("Commands", &selection, m_containers.data(), (int)m_containers.size(), 30))
						m_doc = m_docContainers[selection];
				}

				else if (categorySelection == 5)
				{
					static int selection = 0;
					if (ImGui::ListBox("Commands", &selection, m_drawing.data(), (int)m_drawing.size(), 30))
						m_doc = m_docDrawing[selection];
				}

				else if (categorySelection == 6)
				{
					static int selection = 0;
					if (ImGui::ListBox("Commands", &selection, m_plotting.data(), (int)m_plotting.size(), 30))
						m_doc = m_docPlotting[selection];
				}

				else if (categorySelection == 7)
				{
					static int selection = 0;
					if (ImGui::ListBox("Commands", &selection, m_tables.data(), (int)m_tables.size(), 30))
						m_doc = m_docTables[selection];
				}

				else if (categorySelection == 8)
				{
					static int selection = 0;
					if (ImGui::ListBox("Commands", &selection, m_themes.data(), (int)m_themes.size(), 30))
						m_doc = m_docThemes[selection];
				}

				else if (categorySelection == 9)
				{
					static int selection = 0;
					if (ImGui::ListBox("Commands", &selection, m_inputs.data(), (int)m_inputs.size(), 30))
						m_doc = m_docInputs[selection];
				}

				else if (categorySelection == 10)
				{
					static int selection = 0;
					if (ImGui::ListBox("Commands", &selection, m_windows.data(), (int)m_windows.size(), 30))
						m_doc = m_docWindows[selection];
				}
				else if (categorySelection == 11)
				{
					static int selection = 0;
					if (ImGui::ListBox("Commands", &selection, m_cconstants.data(), (int)m_cconstants.size(), 30))
						m_doc = m_constantsValues[selection].c_str();
				}

				ImGui::PopStyleColor();
				ImGui::EndGroup();

				ImGui::SameLine();
				ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(0, 0, 255, 100));
				ImGui::BeginChild("DocChild", ImVec2(0, 0), true);
				ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + 400);
				ImGui::Text("%s", m_doc);
				ImGui::PopTextWrapPos();
				ImGui::EndChild();
				ImGui::PopStyleColor();

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Search Commands##doc"))
			{

				static size_t commandselection = 0;
				const char* commanddoc = m_commands[commandselection].second.c_str();
				static ImGuiTextFilter filter;
				filter.Draw();

				ImGui::PushItemWidth(300);
				ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(255, 0, 0, 100));
				ImGui::BeginChild("CommandsChild##debug", ImVec2(500.0f, 0), true);

				for (size_t i = 0; i < m_commands.size(); i++)
				{
					auto& item = m_commands[i];

					if (filter.PassFilter(item.first.c_str()))
					{
						if (ImGui::Selectable(item.first.c_str(), i == commandselection))
							commandselection = i;
					}
				}
				ImGui::PopStyleColor();
				ImGui::EndChild();
				ImGui::SameLine();
				ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(0, 0, 255, 100));
				ImGui::BeginChild("CommandsDoc##debug", ImVec2(0, 0), true);
				ImGui::PushTextWrapPos(500);
				ImGui::Text("%s", commanddoc);
				ImGui::PopStyleColor();
				ImGui::PopTextWrapPos();
				ImGui::EndChild();
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}

		if (ImGui::IsWindowFocused())
		{

			float titleBarHeight = ImGui::GetStyle().FramePadding.y * 2 + ImGui::GetFontSize();

			// update mouse
			mvVec2 oldMousePos = mvInput::getGlobalMousePosition();
			ImVec2 mousePos = ImGui::GetMousePos();
			mvInput::setGlobalMousePosition(mousePos.x, mousePos.y);
			float x = mousePos.x - ImGui::GetWindowPos().x;
			float y = mousePos.y - ImGui::GetWindowPos().y - titleBarHeight;
			mvInput::setMousePosition(x, y);

			if (mvItemRegistry::GetItemRegistry()->getActiveWindow() != "documentation##standard")
				mvEventBus::Publish(mvEVT_CATEGORY_ITEM, mvEVT_ACTIVE_WINDOW, { CreateEventArgument("WINDOW", std::string("documentation##standard")) });

			// mouse move callback
			//if (getMouseMoveCallback() != nullptr)
			//{
			//	if (oldMousePos.x != mousePos.x || oldMousePos.y != mousePos.y)
			//	{
			//		mvCallbackRegistry::GetCallbackRegistry()->runCallback(getMouseMoveCallback(), m_name,
			//			ToPyPair(x, y));
			//	}
			//}

		}

		ImGui::End();
	}

}
