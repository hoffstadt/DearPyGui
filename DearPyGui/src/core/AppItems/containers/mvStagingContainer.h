#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvStagingContainer, MV_ITEM_DESC_CONTAINER | MV_ITEM_DESC_ROOT, StorageValueTypes::None, 1);
	class mvStagingContainer : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvStagingContainer, add_staging_container)
		MV_NO_COMMANDS
		MV_DEFAULT_PARENTS
		MV_DEFAULT_CHILDREN
		MV_NO_CONSTANTS

	public:

		mvStagingContainer(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override {}

	};

}