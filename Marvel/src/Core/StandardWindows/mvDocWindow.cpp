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

					ColorText("RUNNING AN APP:");
					ImGui::BulletText("An app can be ran 2 ways:");
					ImGui::Indent();
					ImGui::BulletText("Call the MarvelSandbox executable with --app YourPythonFile --path PathToFile");
					ImGui::BulletText("Call the MarvelSandbox executable with --app YourPythonFile (if the file is next to executable)");
					ImGui::Unindent();
					ImGui::BulletText("Notes:");
					ImGui::Indent();
					ImGui::BulletText("Do NOT leave the python extension when using --app i.e. MarvelSandbox.exe --app Blah (not Blah.py).");
					ImGui::BulletText("Do NOT have spacing in the python file name.");
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

				if (ImGui::CollapsingHeader("Callbacks"))
				{
					ColorText("BASICS:");
					ImGui::BulletText("Callbacks are functions that are ran when some event occurs.");
					ImGui::BulletText("There are currently 3 'types' of callbacks used in Sandbox:");
					ImGui::Indent();
					CodeColorText("Main    - ran every frame");
					CodeColorText("Inputs  - ran when mouse/keyboard events occure");
					CodeColorText("Widgets - ran when certain widgets are interacting with");
					ImGui::Unindent();
					ImGui::BulletText("Notes:");
					ImGui::Indent();
					ImGui::BulletText("The 'set_*_callback' commands take a string with the name of the callback to use.");
					ImGui::Unindent();
					ImGui::Separator();

					ColorText("MAIN CALLBACK:");
					ImGui::BulletText("Useful for live updating (for example a progress bar)");
					ImGui::BulletText("The command is 'set_main_callback(...)'");
					ImGui::BulletText("Notes:");
					ImGui::Indent();
					ImGui::BulletText("This function will always be ran on the same thread as the GUI.");
					ImGui::BulletText("Should NOT be computationally expensive (because of the note above),");
					ImGui::Text("otherwise you may see a drop in frame rate.");
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

				if (ImGui::CollapsingHeader("Input Polling"))
				{
					ImGui::BulletText("During any callback, you can poll information about the mouse and keyboard.");
					ImGui::BulletText("The following commands are used for this:");
					ImGui::Indent();
					CodeColorText("get_mouse_pos");
					CodeColorText("is_key_pressed");
					CodeColorText("is_mouse_button_pressed");
					CodeColorText("is_mouse_dragging");
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