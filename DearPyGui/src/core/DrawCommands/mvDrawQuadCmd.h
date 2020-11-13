#pragma once

#include "mvDrawCmd.h"

namespace Marvel {

	class mvDrawQuadCmd : public mvDrawCmd
	{

	public:

		MV_DRAWCMD_TYPE(mvDrawCmdType::DrawQuad)

		mvDrawQuadCmd(const mvVec2& p1, const mvVec2& p2, const mvVec2& p3, const mvVec2& p4, 
			const mvColor& color, const mvColor& fill, float thickness);

		void draw         (ImDrawList* drawlist, float x, float y) override;
		void setConfigDict(PyObject* dict)                         override;
		void getConfigDict(PyObject* dict)                         override;

	private:

		mvVec2  m_p1;
		mvVec2  m_p2;
		mvVec2  m_p3;
		mvVec2  m_p4;
		mvColor m_color;
		mvColor m_fill;
		float   m_thickness;

	};

}