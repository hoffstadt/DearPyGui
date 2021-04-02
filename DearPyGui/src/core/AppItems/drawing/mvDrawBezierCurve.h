#pragma once

#include "mvAppItem.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvDrawBezierCurve, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 0);
	class mvDrawBezierCurve : public mvAppItem
	{
	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPITEM_TYPE(mvAppItemType::mvDrawBezierCurve, draw_bezier_curve)

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

	public:

		mvDrawBezierCurve(const std::string& name, const mvVec2& p1, const mvVec2& p2,
			const mvVec2& p3, const mvVec2& p4, const mvColor& color, float thickness, int segments);

		void draw(ImDrawList* drawlist, float x, float y) override;
		bool isParentCompatible(mvAppItemType type) override;

		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;


	private:

		mvVec2  m_p1;
		mvVec2  m_p2;
		mvVec2  m_p3;
		mvVec2  m_p4;
		mvColor m_color;
		float   m_thickness;
		int     m_segments;

	};

}
