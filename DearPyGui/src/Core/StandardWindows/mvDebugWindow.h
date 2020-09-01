#pragma once

#include "mvStandardWindow.h"
#include "mvPythonTranslator.h"
#include "mvMarvel.h"

namespace Marvel {

	class mvAppItem;

	class mvDebugWindow : public mvStandardWindow
	{

	public:

		mvDebugWindow() : mvStandardWindow("Dear PyGui Debug") 
		{ 
			m_width = 500; 
			m_height = 500;  
			m_flags |= ImGuiWindowFlags_AlwaysAutoResize;

			m_commands = GetAllCommands();
		}

		void render(bool& show) override;
		void renderItem(mvAppItem* item);

	private:

		std::string m_selectedItem = "MainWindow";
		std::vector<std::pair<std::string, std::string>> m_commands;

	};

}