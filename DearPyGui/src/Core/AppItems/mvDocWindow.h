#pragma once
#include "Core/AppItems/mvTypeBases.h"
#include "mvAppItem.h"

namespace Marvel {

	class mvDocWindow : public mvBaseWindowAppitem
	{

		MV_APPITEM_TYPE(mvAppItemType::DocWindow, "add_doc_window")

	public:

		mvDocWindow(const std::string& name)
			: mvBaseWindowAppitem(name)
		{
			m_width = 700;
			m_height = 500;
			setup();
		}

		~mvDocWindow();

		bool canBeDeleted() const override { return false; }

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
		std::vector<std::pair<std::string, long>> m_constants;
		std::vector<std::string> m_constantsValues;
		std::map<std::string, mvPythonParser>* m_docmap = nullptr;

	};

}