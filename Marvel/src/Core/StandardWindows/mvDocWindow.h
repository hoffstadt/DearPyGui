#pragma once

#include <vector>
#include <string>

namespace Marvel {

	class mvDocWindow
	{

	public:

		static mvDocWindow* GetWindow();

		void render(bool& show);

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

	};

}