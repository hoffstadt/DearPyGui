#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

	class mvTemplateRegistry : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvTemplateRegistry, add_template_registry)

	public:

		explicit mvTemplateRegistry(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override {}

	};

}