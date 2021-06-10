#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvDummy, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 1);
	class mvDummy : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDummy, add_dummy)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

		mvDummy(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;

	};

}