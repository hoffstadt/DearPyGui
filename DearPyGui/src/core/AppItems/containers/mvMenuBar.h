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

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		explicit mvMenuBar(mvUUID uuid);

		bool isParentCompatible(mvAppItemType type) override;

		void draw(ImDrawList* drawlist, float x, float y) override;

	};

}