#pragma once
#include "Core/AppItems/mvTypeBases.h"
#include "mvAppItem.h"
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
			m_commands = GetAllCommands();
		}

		bool canBeDeleted() const override { return false; }

		void draw() override;

		void renderItem(mvAppItem* item);

	private:

		std::string m_selectedItem;
		ImGuiID     m_selectedID;
		std::vector<std::pair<std::string, std::string>> m_commands;
	};

}