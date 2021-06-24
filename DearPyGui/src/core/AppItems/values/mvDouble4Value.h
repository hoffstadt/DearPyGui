#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvDouble4Value, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Double4, 1);
	class mvDouble4Value : public mvDouble4PtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDouble4Value, add_double4_value)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		mvDouble4Value(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override {}

	};

}
