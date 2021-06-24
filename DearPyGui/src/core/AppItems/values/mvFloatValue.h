#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvFloatValue, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Float, 1);
	class mvFloatValue : public mvFloatPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvFloatValue, add_float_value)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		mvFloatValue(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override {}

	};

}
