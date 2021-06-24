#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvStringValue, MV_ITEM_DESC_DEFAULT, StorageValueTypes::String, 1);
	class mvStringValue : public mvStringPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvStringValue, add_string_value)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		mvStringValue(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override {}

	};

}
