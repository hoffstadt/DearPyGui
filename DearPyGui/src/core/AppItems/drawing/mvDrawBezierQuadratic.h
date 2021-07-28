#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvDrawBezierQuadratic, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 2);
	class mvDrawBezierQuadratic : public mvAppItem
	{
	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDrawBezierQuadratic, draw_bezier_quadratic)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		mvDrawBezierQuadratic(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		bool isParentCompatible(mvAppItemType type) override;

		void handleSpecificRequiredArgs(PyObject* args) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;


	private:

		mvVec2  _p1 = { 0.0f, 0.0f };
		mvVec2  _p2 = { 0.0f, 0.0f };
		mvVec2  _p3 = { 0.0f, 0.0f };
		mvColor _color;
		float   _thickness = 0.0f;
		int     _segments = 0;

	};

}
