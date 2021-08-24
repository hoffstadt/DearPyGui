#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvDrawlist, MV_ITEM_DESC_CONTAINER, StorageValueTypes::None, 1);
	class mvDrawlist : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDrawlist, add_drawlist)
		MV_NO_COMMANDS
		MV_DEFAULT_PARENTS
		MV_NO_CONSTANTS

		MV_SET_STATES(MV_STATE_NONE);

		MV_START_CHILDREN
			MV_ADD_CHILD(mvAppItemType::mvDrawLine),
			MV_ADD_CHILD(mvAppItemType::mvDrawArrow),
			MV_ADD_CHILD(mvAppItemType::mvDrawTriangle),
			MV_ADD_CHILD(mvAppItemType::mvDrawCircle),
			MV_ADD_CHILD(mvAppItemType::mvDrawEllipse),
			MV_ADD_CHILD(mvAppItemType::mvDrawBezierCubic),
			MV_ADD_CHILD(mvAppItemType::mvDrawBezierQuadratic),
			MV_ADD_CHILD(mvAppItemType::mvDrawQuad),
			MV_ADD_CHILD(mvAppItemType::mvDrawRect),
			MV_ADD_CHILD(mvAppItemType::mvDrawText),
			MV_ADD_CHILD(mvAppItemType::mvDrawPolygon),
			MV_ADD_CHILD(mvAppItemType::mvDrawPolyline),
			MV_ADD_CHILD(mvAppItemType::mvDrawImage)
		MV_END_CHILDREN

	public:

		mvDrawlist(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;

	private:

		float             _startx = 0.0f;
		float             _starty = 0.0f;

	};
}