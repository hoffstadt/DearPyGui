#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvDoubleValue, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Double, 1);
	class mvDoubleValue : public mvDoublePtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDoubleValue, add_double_value)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		mvDoubleValue(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override {}

	};

}
