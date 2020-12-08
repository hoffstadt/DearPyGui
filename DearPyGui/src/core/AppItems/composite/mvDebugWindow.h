#pragma once
#include "core/AppItems/mvTypeBases.h"
#include "mvMarvel.h"

namespace Marvel {

	class mvDebugWindow : public mvBaseWindowAppitem
	{

		MV_APPITEM_TYPE(mvAppItemType::DebugWindow, "add_debug_window")

	public:

		mvDebugWindow(const std::string& name)
			: mvBaseWindowAppitem(name)
		{
			m_width = 700;
			m_height = 500;
			m_description.deleteAllowed = false;
			m_commands = GetAllCommands();
		}

		void draw() override;

		void renderItem(mvAppItem* item);

	private:

		std::string m_selectedItem;
		ImGuiID     m_selectedID;
		std::vector<std::pair<std::string, std::string>> m_commands;
	};

}