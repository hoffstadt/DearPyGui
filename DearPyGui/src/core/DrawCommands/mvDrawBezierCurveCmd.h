#pragma once

#include "mvDrawCmd.h"

namespace Marvel {

	class mvDrawBezierCurveCmd : public mvDrawCmd
	{

	public:

		MV_DRAWCMD_TYPE(mvDrawCmdType::DrawBezierCurve)

		mvDrawBezierCurveCmd(const mvVec2& p1, const mvVec2& p2, 
			const mvVec2& p3, const mvVec2& p4, const mvColor& color, float thickness, int segments);

		void draw(ImDrawList* drawlist, float x, float y) override;

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