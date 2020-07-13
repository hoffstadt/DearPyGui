#include "mvDocWindow.h"
#include <imgui.h>
#include "mvInterfaceRegistry.h"
#include "Core/mvApp.h"

namespace Marvel {

	mvDocWindow* mvDocWindow::s_instance = nullptr;

	mvDocWindow::mvDocWindow() : mvStandardWindow()
	{
		m_sbWidgets = mvInterfaceRegistry::GetRegistry()->getPythonInterfaceCommands("sbWidgets");
		m_sbApp = mvInterfaceRegistry::GetRegistry()->getPythonInterfaceCommands("sbApp");
		m_sbLog = mvInterfaceRegistry::GetRegistry()->getPythonInterfaceCommands("sbLog");
		m_sbInput = mvInterfaceRegistry::GetRegistry()->getPythonInterfaceCommands("sbInput");
		m_sbPlot = mvInterfaceRegistry::GetRegistry()->getPythonInterfaceCommands("sbPlot");
		m_sbDraw = mvInterfaceRegistry::GetRegistry()->getPythonInterfaceCommands("sbDraw");
		m_sbConstants = mvInterfaceRegistry::GetRegistry()->getConstantsCommands();

		m_sbDocWidgets = mvInterfaceRegistry::GetRegistry()->getPythonInterfaceDoc("sbWidgets");
		m_sbDocApp = mvInterfaceRegistry::GetRegistry()->getPythonInterfaceDoc("sbApp");
		m_sbDocLog = mvInterfaceRegistry::GetRegistry()->getPythonInterfaceDoc("sbLog");
		m_sbDocInput = mvInterfaceRegistry::GetRegistry()->getPythonInterfaceDoc("sbInput");
		m_sbDocPlot = mvInterfaceRegistry::GetRegistry()->getPythonInterfaceDoc("sbPlot");
		m_sbDocDraw = mvInterfaceRegistry::GetRegistry()->getPythonInterfaceDoc("sbDraw");

		//m_flags |= ImGuiWindowFlags_AlwaysAutoResize;

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

		if (!ImGui::Begin("Documentation", &show, m_flags))
		{
			ImGui::End();
			return;
		}

		if (ImGui::BeginTabBar("Main Tabbar"))
		{
			if (ImGui::BeginTabItem("Help"))
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
					ImGui::BulletText("A configuration file called 'sbConfig.json', sits next to the MarvelSandbox executable.");
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
					ImGui::BulletText("There are 3 usecases for the data storage system:");
					ImGui::Indent();
					ImGui::BulletText("1. For widgets to use the same underlying data (i.e. both modify the same data)");
					ImGui::BulletText("2. To store data for use at a later time or in a different callback.");
					ImGui::BulletText("3. A combination of 1 and 2.");
					ImGui::Unindent();
					ImGui::Separator();

					ColorText("USE CASE 1");
					ImGui::BulletText("To share the underlying data of multiple widgets, just use the 'data_source' keyword when adding the widget.");
					ImGui::BulletText("Ensure the underlying data structures match or it will not work.");
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
					CodeColorText("Main    - ran every frame");
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

					ColorText("MAIN CALLBACK:");
					ImGui::BulletText("Useful for live updating (for example a progress bar)");
					ImGui::BulletText("The command is 'set_main_callback(...)'");
					ImGui::BulletText("Notes:");
					ImGui::Indent();

					ImGui::BulletText("Should take the following form:");
					ImGui::Indent();
					CodeColorText("def callbackname(sender):");
					CodeColorText("    ...");
					ImGui::Unindent();
					ImGui::BulletText("The 'sender' parameter is the name of widget the callback was sent from.");
					ImGui::Unindent();
					ImGui::Separator();

					ColorText("INPUT CALLBACKS:");
					ImGui::BulletText("Useful for capturing key strokes, making iteractive drawings, etc.");
					ImGui::BulletText("Commands take the form of 'set_*_callback(...)'");
					ImGui::BulletText("Should take the following form:");
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
					CodeColorText("def callbackname(sender):");
					CodeColorText("    ...");
					ImGui::Unindent();
					ImGui::BulletText("The 'sender' parameter is the name of widget the callback was sent from.");

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

			if (ImGui::BeginTabItem("Commands"))
			{
				ImGui::BeginGroup();
				ImGui::SetNextItemWidth(500);
				ImGui::ListBox("Modules", &m_moduleSelection, &m_modules[0], 7, 7);

				ImGui::SetNextItemWidth(500);
				if (m_moduleSelection == 0)
				{
					static int selection = 0;
					if (ImGui::ListBox("Commands", &selection, m_sbWidgets.data(), m_sbWidgets.size(), 30))
					{
						m_doc = m_sbDocWidgets[selection];
					}
				}

				else if (m_moduleSelection == 1)
				{
					static int selection = 0;
					if (ImGui::ListBox("Commands", &selection, m_sbApp.data(), m_sbApp.size(), 30))
					{
						m_doc = m_sbDocApp[selection];
					}
				}

				else if (m_moduleSelection == 2)
				{
					static int selection = 0;
					if (ImGui::ListBox("Commands", &selection, m_sbLog.data(), m_sbLog.size(), 30))
					{
						m_doc = m_sbDocLog[selection];
					}
				}

				else if (m_moduleSelection == 3)
				{
					static int selection = 0;
					if (ImGui::ListBox("Commands", &selection, m_sbInput.data(), m_sbInput.size(), 30))
					{
						m_doc = m_sbDocInput[selection];
					}
				}

				else if (m_moduleSelection == 4)
				{
					static int selection = 0;
					if (ImGui::ListBox("Commands", &selection, m_sbPlot.data(), m_sbPlot.size(), 30))
					{
						m_doc = m_sbDocPlot[selection];
					}
				}

				else if (m_moduleSelection == 5)
				{
					static int selection = 0;
					if (ImGui::ListBox("Commands", &selection, m_sbDraw.data(), m_sbDraw.size(), 30))
					{
						m_doc = m_sbDocDraw[selection];
					}
				}

				else if (m_moduleSelection == 6)
				{
					static int selection = 0;
					if (ImGui::ListBox("Commands", &selection, m_sbConstants.data(), m_sbConstants.size(), 30))
					{
						m_doc = "A constant.";
					}
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
			ImGui::EndTabBar();
		}

		if (ImGui::IsWindowFocused())
		{

			float titleBarHeight = ImGui::GetStyle().FramePadding.y * 2 + ImGui::GetFontSize();

			// update mouse
			ImVec2 mousePos = ImGui::GetMousePos();
			float x = mousePos.x - ImGui::GetWindowPos().x;
			float y = mousePos.y - ImGui::GetWindowPos().y - titleBarHeight;
			mvApp::GetApp()->setMousePosition(x, y);

			mvApp::GetApp()->setActiveWindow("Documentation");

		}

		ImGui::End();
	}

}