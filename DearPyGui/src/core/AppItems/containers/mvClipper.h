#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

	class mvClipper : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvClipper, add_clipper)
		MV_NO_COMMANDS
		MV_DEFAULT_PARENTS
		MV_DEFAULT_CHILDREN

	public:

		explicit mvClipper(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;

	};

}