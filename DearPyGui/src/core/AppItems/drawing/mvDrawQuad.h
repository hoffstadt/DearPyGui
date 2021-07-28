#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvDrawQuad, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 2);
	class mvDrawQuad : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDrawQuad, draw_quad)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		mvDrawQuad(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		bool isParentCompatible(mvAppItemType type) override;

		void handleSpecificRequiredArgs(PyObject* args) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;


	private:

		mvVec2  _p1 = { 0.0f, 0.0f };
		mvVec2  _p2 = { 0.0f, 0.0f };
		mvVec2  _p3 = { 0.0f, 0.0f };
		mvVec2  _p4 = { 0.0f, 0.0f };
		mvColor _color;
		mvColor _fill;
		float   _thickness = 1.0f;

	};

}
