#pragma once

#include "mvTypeBases.h"
#include "mvAppItem.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvViewportMenuBar, MV_ITEM_DESC_CONTAINER | MV_ITEM_DESC_ROOT, StorageValueTypes::None, 1);
	class mvViewportMenuBar : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvViewportMenuBar, add_viewport_menu_bar)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		explicit mvViewportMenuBar(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;

	};

}