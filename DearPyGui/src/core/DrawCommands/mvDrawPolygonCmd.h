#pragma once

#include <vector>
#include "mvDrawCmd.h"

namespace Marvel {

	PyObject* draw_polygon(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvDrawPolygonCmd : public mvDrawCmd
	{

	public:

		MV_DRAWCMD_TYPE(mvDrawCmdType::DrawPolygon)

		mvDrawPolygonCmd(const std::vector<mvVec2>& points, const mvColor& color, const mvColor& fill, float thickness);

		void draw         (ImDrawList* drawlist, float x, float y) override;
		void setConfigDict(PyObject* dict)                         override;
		void getConfigDict(PyObject* dict)                         override;

	private:

		std::vector<mvVec2> m_points;
		mvColor             m_color;
		mvColor             m_fill;
		float               m_thickness;

	};

}