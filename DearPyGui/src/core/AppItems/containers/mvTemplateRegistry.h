#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

	class mvTemplateRegistry : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvTemplateRegistry, add_template_registry)
		MV_NO_COMMANDS
		MV_DEFAULT_PARENTS
		MV_DEFAULT_CHILDREN
		MV_NO_CONSTANTS

		MV_SET_STATES(MV_STATE_NONE);

	public:

		explicit mvTemplateRegistry(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override {}

	};

}