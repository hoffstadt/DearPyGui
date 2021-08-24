#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvActiveHandler, MV_ITEM_DESC_DEFAULT | MV_ITEM_DESC_HANDLER, StorageValueTypes::None, 1);
	class mvActiveHandler : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvActiveHandler, add_item_active_handler)
		MV_NO_COMMANDS
		MV_DEFAULT_CHILDREN
		MV_NO_CONSTANTS

		MV_SET_STATES(MV_STATE_NONE);

		MV_START_PARENTS
			MV_ADD_PARENT(mvAppItemType::mvStage),
			MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry),
			MV_ADD_PARENT(mvAppItemType::mvItemHandlerRegistry)
		MV_END_PARENTS

	public:

		mvActiveHandler(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;

	};

}
