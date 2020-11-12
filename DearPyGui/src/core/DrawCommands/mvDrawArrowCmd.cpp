#include "mvDrawArrowCmd.h"

namespace Marvel {

	mvDrawArrowCmd::mvDrawArrowCmd(const mvVec2& p1, const mvVec2& p2,
		const mvColor& color, float thickness, float size)
		:
		m_p1(p1),
		m_p2(p2),
		m_color(color),
		m_thickness(thickness),
		m_size(size)
	{
		float xsi = p1.x;
		float xfi = p2.x;
		float ysi = p1.y;
		float yfi = p2.y;

		// length of arrow head
		double xoffset = size;
		double yoffset = size;

		// get pointer angle w.r.t +X (in radians)
		double angle = 0.0;
		if (xsi >= xfi && ysi >= yfi) {
			angle = atan((ysi - yfi) / (xsi - xfi));
		}
		else if (xsi < xfi && ysi >= yfi) {
			angle = M_PI + atan((ysi - yfi) / (xsi - xfi));
		}
		else if (xsi < xfi && ysi < yfi) {
			angle = -M_PI + atan((ysi - yfi) / (xsi - xfi));
		}
		else if (xsi >= xfi && ysi < yfi) {
			angle = atan((ysi - yfi) / (xsi - xfi));
		}

		// arrow head points
		auto x1 = (float)(xsi - xoffset * cos(angle));
		auto y1 = (float)(ysi - yoffset * sin(angle));

		m_points.push_back({ xsi, ysi });
		m_points.push_back({ (float)(x1 - 0.5 * size * sin(angle)), (float)(y1 + 0.5 * size * cos(angle)) });
		m_points.push_back({ (float)(x1 + 0.5 * size * cos((M_PI / 2.0) - angle)), (float)(y1 - 0.5 * size * sin((M_PI / 2.0) - angle)) });

	}

	void mvDrawArrowCmd::draw(ImDrawList* drawlist, float x, float y)
	{
		mvVec2 start = {x, y};
		drawlist->AddLine(m_p1 + start, m_p2 + start, m_color, m_thickness);
		drawlist->AddTriangle(m_points[0] + start, m_points[1] + start, m_points[2] + start, m_color, m_thickness);
		drawlist->AddTriangleFilled(m_points[0] + start, m_points[1] + start, m_points[2] + start, m_color);
	}

}