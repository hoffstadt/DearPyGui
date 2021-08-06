#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvDrawRect, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 2);
	class mvDrawRect : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDrawRect, draw_rectangle)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		mvDrawRect(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		bool isParentCompatible(mvAppItemType type) override;

		void handleSpecificRequiredArgs(PyObject* args) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;


	private:

		mvVec2  _pmin = { 0.0f, 0.0f };
		mvVec2  _pmax = { 1.0f, 1.0f };
		float   _rounding = 0.0f;
		mvColor _color;
		mvColor _color_upper_left;
		mvColor _color_upper_right;
		mvColor _color_bottom_left;
		mvColor _color_bottom_right;
		mvColor _fill;
		float   _thickness = 1.0f;
		bool    _multicolor = false;

	};

}
