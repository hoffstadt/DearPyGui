#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvDummy, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 1);
	class mvDummy : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDummy, add_dummy)
		MV_NO_COMMANDS
		MV_DEFAULT_PARENTS
		MV_DEFAULT_CHILDREN
		MV_NO_CONSTANTS

		mvDummy(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;

	};

}