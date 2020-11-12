#pragma once

#include <vector>
#include "mvDrawCmd.h"
#include "mvCore.h"

namespace Marvel {

	class mvDrawArrowCmd : public mvDrawCmd
	{

	public:

		MV_DRAWCMD_TYPE(mvDrawCmdType::DrawArrow)

		mvDrawArrowCmd(const mvVec2& p1, const mvVec2& p2,
			const mvColor& color, float thickness, float size);

		void draw(ImDrawList* drawlist, float x, float y) override;

	public:

		mvVec2              m_p1;
		mvVec2              m_p2;
		mvColor             m_color;
		float               m_thickness;
		float               m_size;
		std::vector<mvVec2> m_points;

	};

}