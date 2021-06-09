#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvDrawBezierCubic, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 2);
	class mvDrawBezierCubic : public mvAppItem
	{
	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDrawBezierCubic, draw_bezier_cubic)

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

	public:

		mvDrawBezierCubic(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		bool isParentCompatible(mvAppItemType type) override;

		void handleSpecificRequiredArgs(PyObject* args) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;


	private:

		mvVec2  m_p1 = { 0.0f, 0.0f };
		mvVec2  m_p2 = { 0.0f, 0.0f };
		mvVec2  m_p3 = { 0.0f, 0.0f };
		mvVec2  m_p4 = { 0.0f, 0.0f };
		mvColor m_color;
		float   m_thickness = 0.0f;
		int     m_segments = 0;

	};

}
