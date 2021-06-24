#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvFloatVectValue, MV_ITEM_DESC_DEFAULT, StorageValueTypes::FloatVect, 1);
	class mvFloatVectValue : public mvFloatVectPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvFloatVectValue, add_float_vect_value)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		mvFloatVectValue(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override {}

	};

}
