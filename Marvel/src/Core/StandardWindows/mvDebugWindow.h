#pragma once

#include "mvStandardWindow.h"

namespace Marvel {

	class mvAppItem;

	class mvDebugWindow : public mvStandardWindow
	{

	public:

		mvDebugWindow() : mvStandardWindow() 
		{ 
			m_width = 500; 
			m_height = 500;  
			m_flags |= ImGuiWindowFlags_AlwaysAutoResize;
		}

		virtual void render(bool& show) override;
		void renderItem(mvAppItem* item);

	private:

		std::string m_selectedItem = "MainWindow";

	};

}