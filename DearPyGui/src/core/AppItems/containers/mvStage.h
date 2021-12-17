#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

	class mvStage : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvStage, add_stage)

	public:

		explicit mvStage(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override {}

	};

}