#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvFloat4Value, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Float4, 1);
	class mvFloat4Value : public mvFloat4PtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvFloat4Value, add_float4_value)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		mvFloat4Value(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override {}

	};

}
