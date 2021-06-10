#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvClipper, MV_ITEM_DESC_CONTAINER, StorageValueTypes::None, 1);
	class mvClipper : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvClipper, add_clipper)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		mvClipper(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;

	};

}