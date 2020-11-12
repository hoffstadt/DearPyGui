#include "mvDrawPolylineCmd.h"

namespace Marvel {

	mvDrawPolylineCmd::mvDrawPolylineCmd(const std::vector<mvVec2>& points, const mvColor& color, bool closed, float thickness)
		:
		m_points(points),
		m_color(color),
		m_closed(closed),
		m_thickness(thickness)
	{
	}

	void mvDrawPolylineCmd::draw(ImDrawList* drawlist, float x, float y)
	{
		mvVec2 start = {x, y};

		std::vector<mvVec2> points = m_points;

		for (auto& point : points)
			point = point + start;

		drawlist->AddPolyline((const ImVec2*)const_cast<const mvVec2*>(points.data()), (int)m_points.size(), m_color, m_closed, m_thickness);
	}
}