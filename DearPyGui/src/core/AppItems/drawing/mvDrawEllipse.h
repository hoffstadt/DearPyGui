#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvDrawEllipse, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 2);
	class mvDrawEllipse : public mvAppItem
	{
	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDrawEllipse, draw_ellipse)
		MV_NO_COMMANDS
		MV_DEFAULT_CHILDREN
		MV_NO_CONSTANTS

		MV_START_PARENTS
			MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry),
			MV_ADD_PARENT(mvAppItemType::mvStagingContainer),
			MV_ADD_PARENT(mvAppItemType::mvDrawlist),
			MV_ADD_PARENT(mvAppItemType::mvDrawLayer),
			MV_ADD_PARENT(mvAppItemType::mvWindowAppItem),
			MV_ADD_PARENT(mvAppItemType::mvPlot),
			MV_ADD_PARENT(mvAppItemType::mvViewportDrawlist)
		MV_END_PARENTS

	public:

		mvDrawEllipse(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		void handleSpecificRequiredArgs(PyObject* args) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;


	private:

		mvVec2  _pmin = { 0.0f, 0.0f };
		mvVec2  _pmax = { 1.0f, 1.0f };
		int     _segments = 0;
		mvColor _color;
		mvColor _fill;
		float   _thickness = 1.0f;
		std::vector<mvVec2> _points;
		bool _dirty = true;

	};

}
