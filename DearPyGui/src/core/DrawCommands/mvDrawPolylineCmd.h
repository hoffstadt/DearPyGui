#pragma once

#include <vector>
#include "mvDrawCmd.h"

namespace Marvel {

	class mvDrawPolylineCmd : public mvDrawCmd
	{

	public:

		MV_DRAWCMD_TYPE(mvDrawCmdType::DrawPolyline)

		mvDrawPolylineCmd(const std::vector<mvVec2>& points, const mvColor& color, bool closed, float thickness);

		void draw         (ImDrawList* drawlist, float x, float y) override;
		void setConfigDict(PyObject* dict)                         override;
		void getConfigDict(PyObject* dict)                         override;

	private:

		std::vector<mvVec2> m_points;
		mvColor             m_color;
		bool                m_closed;
		float               m_thickness;

	};

}