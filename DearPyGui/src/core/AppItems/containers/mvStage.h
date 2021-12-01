#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

	class mvStage : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvStage, add_stage)
		MV_NO_COMMANDS
		MV_DEFAULT_PARENTS
		MV_DEFAULT_CHILDREN

	public:

		explicit mvStage(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override {}

	};

}