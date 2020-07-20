#include "mvDocWindow.h"
#include <imgui.h>
#include "mvInterfaceRegistry.h"
#include "Core/mvApp.h"
#include "Core/mvInput.h"

namespace Marvel {

	mvDocWindow* mvDocWindow::s_instance = nullptr;

	mvDocWindow::mvDocWindow() : mvStandardWindow()
	{
		m_commands = mvInterfaceRegistry::GetRegistry()->getAllCommands();
		m_marvel = mvInterfaceRegistry::GetRegistry()->getPythonInterfaceCommands("marvel");
		m_marvelConstants = mvInterfaceRegistry::GetRegistry()->getConstantsCommands();
		m_docMarvel = mvInterfaceRegistry::GetRegistry()->getPythonInterfaceDoc("marvel");

		for (int i = 0; i<m_marvel.size(); i++)
		{
			std::string category = mvInterfaceRegistry::GetRegistry()->getPythonInterface("marvel")[m_marvel[i]].getCategory();

			if (category == "App")
			{
				m_app.push_back(m_marvel[i]);
				m_docApp.push_back(m_docMarvel[i]);
			}

			else if (category == "Logging")
			{
				m_logging.push_back(m_marvel[i]);
				m_docLogging.push_back(m_docMarvel[i]);
			}

			else if (category == "Adding Widgets")
			{
				m_widgets.push_back(m_marvel[i]);
				m_docWidgets.push_back(m_docMarvel[i]);
			}

			else if (category == "Widget Commands")
			{
				m_widgetsCommands.push_back(m_marvel[i]);
				m_docWidgetsCommands.push_back(m_docMarvel[i]);
			}

			else if (category == "Containers")
			{
				m_containers.push_back(m_marvel[i]);
				m_docContainers.push_back(m_docMarvel[i]);
			}

			else if (category == "Drawing")
			{
				m_drawing.push_back(m_marvel[i]);
				m_docDrawing.push_back(m_docMarvel[i]);
			}

			else if (category == "Plotting")
			{
				m_plotting.push_back(m_marvel[i]);
				m_docPlotting.push_back(m_docMarvel[i]);
			}

			else if (category == "Tables")
			{
				m_tables.push_back(m_marvel[i]);
				m_docTables.push_back(m_docMarvel[i]);
			}

			else if (category == "Themes and Styles")
			{
				m_themes.push_back(m_marvel[i]);
				m_docThemes.push_back(m_docMarvel[i]);
			}

			else if (category == "Input Polling")
			{
				m_inputs.push_back(m_marvel[i]);
				m_docInputs.push_back(m_docMarvel[i]);
			}

			else if (category == "Standard Windows")
			{
				m_windows.push_back(m_marvel[i]);
				m_docWindows.push_back(m_docMarvel[i]);
			}
		}
	}

	mvStandardWindow* mvDocWindow::GetWindow()
	{
		if (s_instance)
			return s_instance;

		s_instance = new mvDocWindow();
		return s_instance;
	}

	static void ColorText(const char* item)
	{
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), item);
	}

	static void CodeColorText(const char* item)
	{
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), item);
	}

	void mvDocWindow::render(bool& show)
	{
		if (m_mainMode)
		{
			ImGuiStyle& style = ImGui::GetStyle();
			style.WindowRounding = 0.0f;
			ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
			ImGui::SetNextWindowSize(ImVec2(m_width, m_height));
			m_flags = ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoSavedSettings
				| ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;
		}

		else
		{
			ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowSize(ImVec2(700, 500), ImGuiCond_FirstUseEver);
		}

		if (!ImGui::Begin("Documentation##doc", &show, m_flags))
		{
			ImGui::End();
			return;
		}

		if (ImGui::BeginTabBar("Main Tabbar##doc"))
		{
			if (ImGui::BeginTabItem("Help##doc"))
			{

				if (ImGui::CollapsingHeader("Help"))
				{
					ColorText("ABOUT THIS GUIDE:");
					ImGui::BulletText("Sections below cover various aspects of Marvel Sandbox.");
					ImGui::BulletText("This document can be found by calling the \"show_documentation\" command.");
					ImGui::BulletText("Additional information on specific commands can be found in the commands tab.");
				}

				if (ImGui::CollapsingHeader("Basics"))
				{
					ColorText("MODES:");
					ImGui::BulletText("Marvel Sandbox can be ran in the following modes:");
					ImGui::Indent();
					CodeColorText("Regular - Runs a single app. (default mode)");
					CodeColorText("Editor - Runs a simple IDE to create/test apps. (use --editor flag)");
					CodeColorText("Documentation - Runs the documentation app. (use --documentation flag)");
					ImGui::Unindent();
					ImGui::Separator();

					ColorText("RUNNING AN APP (EXPLICITLY):");
					ImGui::BulletText("An app can be ran explicitly 2 ways:");
					ImGui::Indent();
					ImGui::BulletText("Call the MarvelSandbox executable with --app YourPythonFile --path PathToFile");
					ImGui::BulletText("Call the MarvelSandbox executable with --app YourPythonFile (if the file is next to executable)");
					ImGui::Unindent();
					ImGui::BulletText("Notes:");
					ImGui::Indent();
					ImGui::BulletText("Do NOT leave the python extension when using --app i.e. MarvelSandbox.exe --app Blah (not Blah.py).");
					ImGui::BulletText("Do NOT have spacing in the python file name.");
					ImGui::Unindent();
					ImGui::Separator();

					ColorText("CONFIG FILE:");
					ImGui::BulletText("A configuration file called 'marvel_config.json', sits next to the MarvelSandbox executable.");
					ImGui::BulletText("This file is the default method for passing flags and options to the executable (rather than command line).");
					ImGui::BulletText("The config file is a simple json file.");

					ImGui::BulletText("Config options:");
					ImGui::Indent();
					ImGui::BulletText("App - a string matching the name of the app file (without the .py extension)");
					ImGui::BulletText("Mode - a string for the mode to run (None, Editor, Documentation).");
					ImGui::BulletText("Theme - a string for initial theme (Dark, Light, Classic, Dark 2, Grey, Dark Grey, Cherry, Purple, Gold, Red).");
					ImGui::BulletText("Path - a string with the path to the python file relative to the MarvelSandbox executable.");
					ImGui::BulletText("PythonLibs - a string with the path to 3rd party python libs relative to the MarvelSandbox executable.");
					ImGui::Unindent();
				}

				if (ImGui::CollapsingHeader("Logging and Console Output"))
				{
					ColorText("PYTHON PRINTING");
					ImGui::BulletText("Using the python \"print\" command outputs to the logger window.");
					ImGui::BulletText("In order to show the logger window, you must use the \"show_logger\" command.");
					ImGui::Separator();

					ColorText("LOGGING");
					ImGui::BulletText("All output is routed to the logger window (python, system errors/warnings, etc.).");
					ImGui::BulletText("In order to show the logger window, you must use the \"show_logger\" command.");
					ImGui::BulletText("In order to filter what is displayed, you can set the log level.");
					ImGui::BulletText("Can be set like \"set_log_level(sbConstants.mvINFO)\"");
					ImGui::BulletText("The log levels are:");
					ImGui::Indent();
					CodeColorText("    level   -     constant          -    command");
					CodeColorText("----------------------------------------------------------");
					CodeColorText("TRACE   - sbConstants.mvTRACE   - log('some message')");
					CodeColorText("DEBUG   - sbConstants.mvDEBUG   - log_debug('some message')");
					CodeColorText("INFO    - sbConstants.mvINFO    - log_info('some message')");
					CodeColorText("WARNING - sbConstants.mvWARNING - log_warning('some message')");
					CodeColorText("ERROR   - sbConstants.mvERROR   - log_error('some message')");
					CodeColorText("OFF     - sbConstants.mvOFF");
					ImGui::Unindent();
					
				}

				if (ImGui::CollapsingHeader("Widgets"))
				{
					ColorText("BASICS:");
					ImGui::BulletText("All widgets are added with a command of the form \"add_*\"");
					ImGui::BulletText("Some widgets can contain other widgets. These types of widgets have an additional \"end_*\" command.");
					ImGui::Separator();

					ColorText("CONTAINER WIDGETS:");
					ImGui::BulletText("The following widgets are container types:");
					ImGui::Indent();
					CodeColorText("child             - an embedded window");
					CodeColorText("group             - a group of widgets, useful for layouts");
					CodeColorText("window            - a new window");
					CodeColorText("collapsing header - a collapsible header (like to one used here)");
					CodeColorText("tabbar            - contains tabs");
					CodeColorText("tab               - a tab belonging to a tabbar");
					CodeColorText("menubar           - contains menus");
					CodeColorText("menu              - a menu belonging to a menubar");
					CodeColorText("tree node         - a collapsible tree item");
					CodeColorText("tooltip           - a window that appears when hovering an item");
					CodeColorText("popup             - a window that appears when an item is clicked");
					ImGui::Unindent();
					ImGui::Separator();

					ColorText("WIDGETS NAMING:");
					ImGui::BulletText("Widgets MUST have unique names.");
					ImGui::BulletText("Most widgets require you to give a name with the exception of:");
					ImGui::Indent();
					CodeColorText("seperator");
					CodeColorText("spacing");
					CodeColorText("sameline");
					CodeColorText("indent");
					CodeColorText("unindent");
					ImGui::Unindent();
					ImGui::BulletText("The above widget's names are optional (they are auto generated).");
					ImGui::BulletText("If a widget uses the name to displace text (i.e. button) and you need");
					ImGui::Text("   multiple buttons to display the same, you can do the following:");
					ImGui::Indent();
					CodeColorText("add_button('Press me')");
					CodeColorText("add_button('Press me##123')");
					CodeColorText("add_button('Press me##fish')");
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
					CodeColorText("end_group()");
					CodeColorText("end_child()");
					ImGui::Unindent();
					ImGui::BulletText("METHOD 2:");
					ImGui::Indent();
					ImGui::Text("You add widgets to their parent containers using the \"parent\" keyword.");
					ImGui::Text("Containers are created by calling their add_* command followed immediately");
					ImGui::Text("by their end_* command.");
					ImGui::Text("This method is best used for adding widgets at runtime to just a portion");
					ImGui::Text("of the app, however, it can be used for creating the entire app.");
					ImGui::Text("Example:");
					ImGui::Spacing();
					CodeColorText("add_child(...)");
					CodeColorText("end_child()");
					CodeColorText("add_button(..., parent='childname')");
					ImGui::Unindent();
					ImGui::Unindent();

				}

				if (ImGui::CollapsingHeader("Tables"))
				{
					ColorText("BASICS");
					ImGui::BulletText("Tables are useful for viewing large amounts of data.");
					ImGui::BulletText("The following commands are related to tables:");
					ImGui::Indent();
					CodeColorText("add_table");
					CodeColorText("set_table_item");
					CodeColorText("set_table_selection");
					CodeColorText("get_table_selections");
					CodeColorText("get_table_item");
					CodeColorText("delete_row");
					CodeColorText("delete_column");
					CodeColorText("insert_row");
					CodeColorText("insert_column");
					ImGui::Unindent();

				}

				if (ImGui::CollapsingHeader("Data Storage"))
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
					ImGui::BulletText("To share the underlying data of multiple widgets, just use the 'data_source' keyword when adding the widget.");
					ImGui::BulletText("Ensure the underlying data structures match or it will not work.");
					ImGui::BulletText("Some widgets have a 'secondary_data_source' (i.e. listbox, which allows modification of the listed items)");
					ImGui::Separator();

					ColorText("USE CASE 2");
					ImGui::BulletText("To store data for later usage, just use the following commands:");
					ImGui::Indent();
					CodeColorText("add_data");
					CodeColorText("get_data");
					ImGui::Unindent();
					ImGui::Separator();

					ColorText("USE CASE 3");
					ImGui::BulletText("With this use case, you store data with 'add_data' and use that data with the 'data_source' keyword.");

				}

				if (ImGui::CollapsingHeader("Callbacks"))
				{
					ColorText("BASICS:");
					ImGui::BulletText("Callbacks are functions that are ran when some event occurs.");
					ImGui::BulletText("There are currently 3 'types' of callbacks used in Sandbox:");
					ImGui::Indent();
					CodeColorText("Render  - ran every frame");
					CodeColorText("Inputs  - ran when mouse/keyboard events occure");
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
					ImGui::BulletText("The optional 'handler' keyword will set which window triggers the callback.");
					ImGui::BulletText("Notes:");
					ImGui::Indent();
					ImGui::BulletText("Works for: Window, Child, Popup, Menu");
					ImGui::BulletText("Should take the following form:");
					ImGui::Indent();
					CodeColorText("def callbackname(sender, data):");
					CodeColorText("    ...");
					ImGui::Unindent();
					ImGui::BulletText("The 'sender' parameter is the name of widget the callback was sent from.");
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
					ImGui::BulletText("Commands take the form of 'set_*_callback(...)'");
					ImGui::BulletText("The folling widgets support callbacks:");
					ImGui::Indent();
					CodeColorText(" - button");
					CodeColorText(" - selectable");
					CodeColorText(" - checkbox");
					CodeColorText(" - combo");
					CodeColorText(" - listbox");
					CodeColorText(" - radiobutton");
					CodeColorText(" - coloredit family");
					CodeColorText(" - colorpicker family");
					CodeColorText(" - drag family");
					CodeColorText(" - slide family");
					CodeColorText(" - input family");
					CodeColorText(" - menuitem");
					CodeColorText(" - tab");
					ImGui::Unindent();
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
					ImGui::BulletText("The drawings scale and coordinate origin can be set with the following commands:");
					ImGui::Indent();
					CodeColorText("set_drawing_scale(...)  # default is 1.0 and 1.0");
					CodeColorText("set_drawing_origin(...) # origin is in pixels, default is 0,0 (bottom left corner)");
					ImGui::Unindent();
					ImGui::Separator();

					ColorText("TAGS:");
					ImGui::BulletText("To efficiently update a drawing, you should use the tag system (as opposed to clearing and redrawing)");
					ImGui::BulletText("All draw commands of the form 'draw_*' accept an optional tag argument.");
					ImGui::BulletText("The tag argument is a string that acts as an indentifier for the draw item.");
					ImGui::BulletText("On subsequent calls to the same draw command with the given tag, the item will be updated.");
				}

				if (ImGui::CollapsingHeader("3rd Party Python Libraries"))
				{
					ImGui::BulletText("To use 3rd Party Python Libraries:");
					ImGui::Indent();
					ImGui::BulletText("1. Install Python Version 3.8.3 (must be same version as Marvel's embedded version).");
					ImGui::BulletText("2. Pip install the library you want to use.");
					ImGui::BulletText("3. Run MarvelSandbox with the --libs PathToSitePackages");
					ImGui::BulletText("4. Now you can use any pip installed library.");
					ImGui::Unindent();

					ImGui::BulletText("The path for site packages is typically:");
					ImGui::Indent();
					ColorText("PythonInstallLocation/Lib/site-packages");
					ImGui::Unindent();

				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Commands##doc"))
			{
				ImGui::BeginGroup();
				ImGui::SetNextItemWidth(500);
				ImGui::ListBox("Category", &categorySelection, &m_categories[0], 12, 7);

				ImGui::SetNextItemWidth(500);
				if (categorySelection == 0)
				{
					static int selection = 0;
					if (ImGui::ListBox("Commands", &selection, m_app.data(), m_app.size(), 30))
						m_doc = m_docApp[selection];
				}

				else if (categorySelection == 1)
				{
					static int selection = 0;
					if (ImGui::ListBox("Commands", &selection, m_logging.data(), m_logging.size(), 30))
						m_doc = m_docLogging[selection];
				}

				else if (categorySelection == 2)
				{
					static int selection = 0;
					if (ImGui::ListBox("Commands", &selection, m_widgets.data(), m_widgets.size(), 30))
						m_doc = m_docWidgets[selection];
				}

				else if (categorySelection == 3)
				{
					static int selection = 0;
					if (ImGui::ListBox("Commands", &selection, m_widgetsCommands.data(), m_widgetsCommands.size(), 30))
						m_doc = m_docWidgetsCommands[selection];
				}

				else if (categorySelection == 4)
				{
					static int selection = 0;
					if (ImGui::ListBox("Commands", &selection, m_containers.data(), m_containers.size(), 30))
						m_doc = m_docContainers[selection];
				}

				else if (categorySelection == 5)
				{
					static int selection = 0;
					if (ImGui::ListBox("Commands", &selection, m_drawing.data(), m_drawing.size(), 30))
						m_doc = m_docDrawing[selection];
				}

				else if (categorySelection == 6)
				{
					static int selection = 0;
					if (ImGui::ListBox("Commands", &selection, m_plotting.data(), m_plotting.size(), 30))
						m_doc = m_docPlotting[selection];
				}

				else if (categorySelection == 7)
				{
					static int selection = 0;
					if (ImGui::ListBox("Commands", &selection, m_tables.data(), m_tables.size(), 30))
						m_doc = m_docTables[selection];
				}

				else if (categorySelection == 8)
				{
					static int selection = 0;
					if (ImGui::ListBox("Commands", &selection, m_themes.data(), m_themes.size(), 30))
						m_doc = m_docThemes[selection];
				}

				else if (categorySelection == 9)
				{
					static int selection = 0;
					if (ImGui::ListBox("Commands", &selection, m_inputs.data(), m_inputs.size(), 30))
						m_doc = m_docInputs[selection];
				}

				else if (categorySelection == 10)
				{
					static int selection = 0;
					if (ImGui::ListBox("Commands", &selection, m_windows.data(), m_windows.size(), 30))
						m_doc = m_docWindows[selection];
				}

				else if (categorySelection == 11)
				{
					static int selection = 0;
					if (ImGui::ListBox("Commands", &selection, m_marvelConstants.data(), m_marvelConstants.size(), 30))
						m_doc = "A constant.";
				}

				ImGui::EndGroup();

				ImGui::SameLine();
				ImGui::BeginChild("DocChild", ImVec2(500, 600), true);
				ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + 400);
				ImGui::Text(m_doc);
				ImGui::PopTextWrapPos();
				ImGui::EndChild();

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Search Commands##doc"))
			{

				static int commandselection = 0;
				const char* commanddoc = m_commands[commandselection].second.c_str();
				static ImGuiTextFilter filter;
				filter.Draw();

				ImGui::PushItemWidth(300);
				ImGui::BeginChild("CommandsChild##debug", ImVec2(500.0f, 600.0f), true);
				ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f, 1.0f, 0.0f, 1.0f });
				for (int i = 0; i < m_commands.size(); i++)
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
				ImGui::BeginChild("CommandsDoc##debug", ImVec2(500.0f, 600.0f), true);
				ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f, 0.0f, 1.0f, 1.0f });
				ImGui::PushTextWrapPos(500);
				ImGui::Text(commanddoc);
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
			ImVec2 mousePos = ImGui::GetMousePos();
			mvInput::setGlobalMousePosition(mousePos.x, mousePos.y);
			float x = mousePos.x - ImGui::GetWindowPos().x;
			float y = mousePos.y - ImGui::GetWindowPos().y - titleBarHeight;
			mvInput::setMousePosition(x, y);

			mvApp::GetApp()->setActiveWindow("Documentation");

		}

		ImGui::End();
	}

}