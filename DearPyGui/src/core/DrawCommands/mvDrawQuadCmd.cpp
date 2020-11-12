#include "mvDrawQuadCmd.h"

namespace Marvel {

	mvDrawQuadCmd::mvDrawQuadCmd(const mvVec2& p1, const mvVec2& p2, const mvVec2& p3, const mvVec2& p4, const mvColor& color, const mvColor& fill, float thickness)
		:
		m_p1(p1),
		m_p2(p2),
		m_p3(p3),
		m_p4(p4),
		m_color(color),
		m_fill(fill),
		m_thickness(thickness)
	{
	}

	void mvDrawQuadCmd::draw(ImDrawList* drawlist, float x, float y)
	{
		mvVec2 start = {x, y};
		if (m_fill.specified)
			drawlist->AddQuadFilled(m_p1 + start, m_p2 + start, m_p3 + start, m_p4 + start, m_fill);
		drawlist->AddQuad(m_p1 + start, m_p2 + start, m_p3 + start, m_p4 + start, m_color, m_thickness);
	}

}