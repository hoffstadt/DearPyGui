#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvInt4Value, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Int4, 1);
	class mvInt4Value : public mvInt4PtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvInt4Value, add_int4_value)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		mvInt4Value(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override {}

	};

}
