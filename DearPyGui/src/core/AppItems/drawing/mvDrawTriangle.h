#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvDrawTriangle, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 2);
	class mvDrawTriangle : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDrawTriangle, draw_triangle)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		mvDrawTriangle(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		bool isParentCompatible(mvAppItemType type) override;

		void handleSpecificRequiredArgs(PyObject* args) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;


	private:

		mvVec2  m_p1 = { 0.0f, 0.0f };
		mvVec2  m_p2 = { 0.0f, 0.0f };
		mvVec2  m_p3 = {0.0f, 0.0f};
		mvColor m_color;
		mvColor m_fill;
		float   m_thickness = 1.0f;

	};

}
