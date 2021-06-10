#pragma once

#include "mvTypeBases.h"
#include "mvAppItem.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvMenuBar, MV_ITEM_DESC_CONTAINER, StorageValueTypes::Bool, 1);
	class mvMenuBar : public mvBoolPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvMenuBar, add_menu_bar)

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

	public:

		explicit mvMenuBar(mvUUID uuid);

		bool isParentCompatible(mvAppItemType type) override;

		void draw(ImDrawList* drawlist, float x, float y) override;

	};

}