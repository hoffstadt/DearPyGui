#include "mvDrawCircleCmd.h"

namespace Marvel {

	mvDrawCircleCmd::mvDrawCircleCmd(const mvVec2& center, float radius, const mvColor& color,
		int segments, float thickness, const mvColor& fill)
		:
		m_center(center),
		m_radius(radius),
		m_segments(segments),
		m_color(color),
		m_fill(fill),
		m_thickness(thickness)
	{
	}

	void mvDrawCircleCmd::draw(ImDrawList* drawlist, float x, float y)
	{
		mvVec2 start = {x, y};
		if (m_fill.specified)
			drawlist->AddCircleFilled(m_center + start, m_radius, m_fill, m_segments);

		drawlist->AddCircle(m_center + start, m_radius, m_color, m_segments, m_thickness);
	}

}