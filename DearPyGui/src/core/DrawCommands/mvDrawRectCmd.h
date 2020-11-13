#pragma once

#include "mvDrawCmd.h"

namespace Marvel {

	class mvDrawRectCmd : public mvDrawCmd
	{

	public:

		MV_DRAWCMD_TYPE(mvDrawCmdType::DrawRect)

		mvDrawRectCmd(const mvVec2& pmin, const mvVec2& pmax, const mvColor& color, 
			const mvColor& fill, float rounding, float thickness);

		void draw         (ImDrawList* drawlist, float x, float y) override;
		void setConfigDict(PyObject* dict)                         override;
		void getConfigDict(PyObject* dict)                         override;

	private:

		mvVec2  m_pmin;
		mvVec2  m_pmax;
		float   m_rounding;
		mvColor m_color;
		mvColor m_fill;
		float   m_thickness;

	};

}