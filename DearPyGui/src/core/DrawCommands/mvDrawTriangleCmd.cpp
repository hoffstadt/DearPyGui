#include "mvDrawTriangleCmd.h"

namespace Marvel {

	mvDrawTriangleCmd::mvDrawTriangleCmd(const mvVec2& p1, const mvVec2& p2, const mvVec2& p3,
		const mvColor& color, float thickness, const mvColor& fill)
		:
		m_p1(p1),
		m_p2(p2),
		m_p3(p3),
		m_color(color),
		m_fill(fill),
		m_thickness(thickness)
	{
	}

	void mvDrawTriangleCmd::draw(ImDrawList* drawlist, float x, float y)
	{
		mvVec2 start = {x, y};
		if (m_fill.specified)
			drawlist->AddTriangleFilled(m_p1 + start, m_p2 + start, m_p3 + start, m_fill);

		drawlist->AddTriangle(m_p1 + start, m_p2 + start, m_p3 + start, m_color, m_thickness);
	}

}