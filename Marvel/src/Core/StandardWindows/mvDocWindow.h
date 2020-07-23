#pragma once

#include <vector>
#include <string>
#include <imgui.h>

#include "mvStandardWindow.h"
#include "mvPythonParser.h"

namespace Marvel {

	class mvDocWindow : public mvStandardWindow
	{

	public:

		static mvStandardWindow* GetWindow();

		virtual void render(bool& show) override;

	private:

		mvDocWindow();
		~mvDocWindow();
		void setup();

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
		std::vector<std::pair<std::string, long>> m_constants;
		std::vector<std::string> m_constantsValues;
		std::map<std::string, mvPythonParser>* m_docmap;
		
		static mvDocWindow* s_instance;

	};

}