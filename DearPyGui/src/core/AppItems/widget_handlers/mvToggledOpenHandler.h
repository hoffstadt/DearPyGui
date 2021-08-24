#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvToggledOpenHandler, MV_ITEM_DESC_DEFAULT | MV_ITEM_DESC_HANDLER, StorageValueTypes::None, 1);
	class mvToggledOpenHandler : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvToggledOpenHandler, add_item_toggled_open_handler)
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

		mvToggledOpenHandler(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
	};

}
