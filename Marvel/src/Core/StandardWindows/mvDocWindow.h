#pragma once

#include <vector>
#include <string>
#include <imgui.h>

namespace Marvel {

	class mvDocWindow
	{

	public:

		static mvDocWindow* GetWindow();

		void render(bool& show);

		void setToMainMode() { m_mainMode = true; }
		void setSize(unsigned width, unsigned height) { m_width = width; m_height = height; }

	private:

		mvDocWindow();

	private:

		static mvDocWindow* s_instance;
		int m_moduleSelection = 0;
		const char* m_doc = "None";

		const char* m_modules[7] = {
			"sbWidgets",
			"sbApp",
			"sbLog",
			"sbInput",
			"sbPlot",
			"sbDraw" ,
			"sbConstants" 
		};

		std::vector<const char*> m_sbWidgets;
		std::vector<const char*> m_sbApp;
		std::vector<const char*> m_sbLog;
		std::vector<const char*> m_sbInput;
		std::vector<const char*> m_sbPlot;
		std::vector<const char*> m_sbDraw;
		std::vector<const char*> m_sbConstants;
	
		std::vector<const char*> m_sbDocWidgets;
		std::vector<const char*> m_sbDocApp;
		std::vector<const char*> m_sbDocLog;
		std::vector<const char*> m_sbDocInput;
		std::vector<const char*> m_sbDocPlot;
		std::vector<const char*> m_sbDocDraw;

		bool     m_mainMode = false;
		unsigned m_width = 1280;
		unsigned m_height = 800;
		ImGuiWindowFlags m_flags = ImGuiWindowFlags_NoSavedSettings;

	};

}