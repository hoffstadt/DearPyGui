#pragma once

#include "mvDrawCmd.h"

namespace Marvel {

	PyObject* draw_line(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvDrawLineCmd : public mvDrawCmd
	{

	public:

		MV_DRAWCMD_TYPE(mvDrawCmdType::DrawLine)

		mvDrawLineCmd(const mvVec2& p1, const mvVec2& p2, const mvColor& color, float thickness);

		void draw         (ImDrawList* drawlist, float x, float y) override;
		void setConfigDict(PyObject* dict)                         override;
		void getConfigDict(PyObject* dict)                         override;

	private:

		mvVec2  m_p1;
		mvVec2  m_p2;
		mvColor m_color;
		float   m_thickness;

	};

}