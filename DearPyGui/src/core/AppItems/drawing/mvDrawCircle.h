#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvDrawCircle, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 2);
	class mvDrawCircle : public mvAppItem
	{
	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDrawCircle, draw_circle)
		MV_NO_COMMANDS
		MV_DEFAULT_CHILDREN
		MV_NO_CONSTANTS

		MV_START_PARENTS
			MV_ADD_PARENT(mvAppItemType::mvStagingContainer),
			MV_ADD_PARENT(mvAppItemType::mvDrawlist),
			MV_ADD_PARENT(mvAppItemType::mvDrawLayer),
			MV_ADD_PARENT(mvAppItemType::mvWindowAppItem),
			MV_ADD_PARENT(mvAppItemType::mvPlot),
			MV_ADD_PARENT(mvAppItemType::mvViewportDrawlist)
		MV_END_PARENTS

	public:

		mvDrawCircle(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		void handleSpecificRequiredArgs(PyObject* args) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;


	private:

		mvVec2  _center = { 0.0f, 0.0f };
		float   _radius = 1.0f;
		int     _segments = 0;
		mvColor _color;
		mvColor _fill;
		float   _thickness = 1.0f;

	};

}
