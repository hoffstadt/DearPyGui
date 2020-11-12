#include "mvDrawBezierCurveCmd.h"

namespace Marvel {

	mvDrawBezierCurveCmd::mvDrawBezierCurveCmd(const mvVec2& p1, const mvVec2& p2, const mvVec2& p3, const mvVec2& p4, const mvColor& color, float thickness, int segments)
		:
		m_p1(p1),
		m_p2(p2),
		m_p3(p3),
		m_p4(p4),
		m_color(color),
		m_thickness(thickness),
		m_segments(segments)
	{
	}

	void mvDrawBezierCurveCmd::draw(ImDrawList* drawlist, float x, float y)
	{
		mvVec2 start = {x, y};
		drawlist->AddBezierCurve(m_p1 + start, m_p2 + start, m_p3 + start, m_p4 + start, m_color, m_thickness);
	}
}