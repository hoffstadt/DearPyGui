#pragma once

#include "mvDrawCmd.h"

namespace Marvel {

	PyObject* draw_circle(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvDrawCircleCmd : public mvDrawCmd
	{

	public:

		MV_DRAWCMD_TYPE(mvDrawCmdType::DrawCircle)

		mvDrawCircleCmd(const mvVec2& center, float radius, const mvColor& color,
			int segments, float thickness, const mvColor& fill);

		void draw         (ImDrawList* drawlist, float x, float y) override;
		void setConfigDict(PyObject* dict)                         override;
		void getConfigDict(PyObject* dict)                         override;

	public:

		mvVec2  m_center;
		float   m_radius;
		int     m_segments;
		mvColor m_color;
		mvColor m_fill;
		float   m_thickness;

	};

}