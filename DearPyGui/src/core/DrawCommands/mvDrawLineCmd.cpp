#include "mvDrawLineCmd.h"

namespace Marvel {

	mvDrawLineCmd::mvDrawLineCmd(const mvVec2& p1, const mvVec2& p2, const mvColor& color, float thickness)
		:
		m_p1(p1),
		m_p2(p2),
		m_color(color),
		m_thickness(thickness)
	{
	}

	void mvDrawLineCmd::draw(ImDrawList* drawlist, float x, float y)
	{
		mvVec2 start = {x, y};
		drawlist->AddLine(m_p1 + start, m_p2 + start, m_color, m_thickness);
	}

}