#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvMenuBar, MV_ITEM_DESC_CONTAINER, StorageValueTypes::Bool, 1);
	class mvMenuBar : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvMenuBar, add_menu_bar)
		MV_NO_COMMANDS
		MV_DEFAULT_CHILDREN
		MV_NO_CONSTANTS

		MV_SET_STATES(
			MV_STATE_HOVER |
			MV_STATE_ACTIVE |
			MV_STATE_FOCUSED |
			MV_STATE_CLICKED |
			MV_STATE_VISIBLE |
			MV_STATE_EDITED |
			MV_STATE_ACTIVATED |
			MV_STATE_DEACTIVATED |
			MV_STATE_DEACTIVATEDAE |
			MV_STATE_TOGGLED_OPEN |
			MV_STATE_RECT_MIN |
			MV_STATE_RECT_MAX |
			MV_STATE_RECT_SIZE |
			MV_STATE_CONT_AVAIL
		);

		MV_START_PARENTS
			MV_ADD_PARENT(mvAppItemType::mvWindowAppItem),
			MV_ADD_PARENT(mvAppItemType::mvChild),
			MV_ADD_PARENT(mvAppItemType::mvNodeEditor),
			MV_ADD_PARENT(mvAppItemType::mvStage),
			MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry),
		MV_END_PARENTS

	public:

		explicit mvMenuBar(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;

	};

}