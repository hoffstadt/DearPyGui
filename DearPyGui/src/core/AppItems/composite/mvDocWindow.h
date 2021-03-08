#pragma once
#include <vector>
#include <map>
#include <string>
#include "mvTypeBases.h"

namespace Marvel {

	struct mvDocWindowConfig : public mvAppItemConfig
	{

		int x_pos = 200;
		int y_pos = 200;
		bool autosize = false;
		bool no_resize = false;
		bool no_title_bar = false;
		bool no_move = false;
		bool no_scrollbar = false;
		bool no_collapse = false;
		bool horizontal_scrollbar = false;
		bool no_focus_on_appearing = false;
		bool no_bring_to_front_on_focus = false;
		bool no_close = false;
		bool no_background = false;

		mvDocWindowConfig()
		{
			width = -1;
			height = -1;
		}
	};

	PyObject* add_doc_window(PyObject* self, PyObject* args, PyObject* kwargs);

	MV_REGISTER_WIDGET(mvDocWindow);
	class mvDocWindow : public mvBaseWindowAppitem
	{

		MV_APPITEM_TYPE(mvAppItemType::mvDocWindow, "add_doc_window")

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvDocWindow(const std::string& name);

		void draw() override;
		void setup();

	private:

		int categorySelection = 0;
		const char* m_doc = "None";

		const char* m_categories[12] = {
			"App",
			"Logging",
			"Adding Widgets",
			"Widget Commands",
			"Containers",
			"Drawing",
			"Plotting",
			"Tables",
			"Themes and Styles",
			"Input Polling",
			"Standard Windows",
			"Constants",
		};

		std::vector<const char*> m_app;
		std::vector<const char*> m_widgets;
		std::vector<const char*> m_widgetsCommands;
		std::vector<const char*> m_containers;
		std::vector<const char*> m_drawing;
		std::vector<const char*> m_plotting;
		std::vector<const char*> m_tables;
		std::vector<const char*> m_inputs;
		std::vector<const char*> m_windows;
		std::vector<const char*> m_logging;
		std::vector<const char*> m_themes;
		std::vector<const char*> m_cconstants;

		std::vector<const char*> m_docApp;
		std::vector<const char*> m_docWidgets;
		std::vector<const char*> m_docWidgetsCommands;
		std::vector<const char*> m_docDrawing;
		std::vector<const char*> m_docPlotting;
		std::vector<const char*> m_docTables;
		std::vector<const char*> m_docInputs;
		std::vector<const char*> m_docWindows;
		std::vector<const char*> m_docLogging;
		std::vector<const char*> m_docThemes;
		std::vector<const char*> m_docContainers;

		std::vector<std::pair<std::string, std::string>> m_commands;
		const std::vector<std::pair<std::string, long>>& m_constants = mvModule_Core::GetModuleConstants();
		std::vector<std::string> m_constantsValues;
		const std::map<std::string, mvPythonParser>& m_docmap = mvModule_Core::GetModuleParsers();

	};

}