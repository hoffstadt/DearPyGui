#include"mvDrawRectCmd.h"

namespace Marvel {

	mvDrawRectCmd::mvDrawRectCmd(const mvVec2& pmin, const mvVec2& pmax, const mvColor& color, const mvColor& fill, float rounding, float thickness)
		:
		m_pmin(pmin),
		m_pmax(pmax),
		m_rounding(rounding),
		m_color(color),
		m_fill(fill),
		m_thickness(thickness)
	{
	}

	void mvDrawRectCmd::draw(ImDrawList* drawlist, float x, float y)
	{
		mvVec2 start = {x, y};
		if (m_fill.specified)
			drawlist->AddRectFilled(m_pmin + start, m_pmax + start, m_fill, m_rounding, ImDrawCornerFlags_All);
		drawlist->AddRect(m_pmin + start, m_pmax + start, m_color, m_rounding, ImDrawCornerFlags_All, m_thickness);
	}

}