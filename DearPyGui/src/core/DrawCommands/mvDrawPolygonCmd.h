#pragma once

#include <vector>
#include "mvDrawCmd.h"

namespace Marvel {

	class mvDrawPolygonCmd : public mvDrawCmd
	{

	public:

		MV_DRAWCMD_TYPE(mvDrawCmdType::DrawPolygon)

		mvDrawPolygonCmd(const std::vector<mvVec2>& points, const mvColor& color, const mvColor& fill, float thickness);

		void draw(ImDrawList* drawlist, float x, float y) override;

	private:

		std::vector<mvVec2> m_points;
		mvColor             m_color;
		mvColor             m_fill;
		float               m_thickness;

	};

}