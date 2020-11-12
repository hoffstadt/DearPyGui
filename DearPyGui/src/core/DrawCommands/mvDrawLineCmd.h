#pragma once

#include "mvDrawCmd.h"

namespace Marvel {

	class mvDrawLineCmd : public mvDrawCmd
	{

	public:

		MV_DRAWCMD_TYPE(mvDrawCmdType::DrawLine)

		mvDrawLineCmd(const mvVec2& p1, const mvVec2& p2, const mvColor& color, float thickness);

		void draw(ImDrawList* drawlist, float x, float y) override;

	private:

		mvVec2  m_p1;
		mvVec2  m_p2;
		mvColor m_color;
		float   m_thickness;

	};

}