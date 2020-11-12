#pragma once

#include "mvDrawCmd.h"

namespace Marvel {

	class mvDrawImageCmd : public mvDrawCmd
	{

	public:

		MV_DRAWCMD_TYPE(mvDrawCmdType::DrawImage)

		mvDrawImageCmd(std::string file, const mvVec2& pmin, const mvVec2& pmax, const mvVec2& uv_min, const mvVec2& uv_max, const mvColor& color);

		void draw(ImDrawList* drawlist, float x, float y) override;

		~mvDrawImageCmd() override;

	public:

		std::string m_file;
		mvVec2		m_pmax;
		mvVec2		m_pmin;
		mvVec2		m_uv_min;
		mvVec2		m_uv_max;
		mvColor		m_color;
		void*       m_texture = nullptr;
		int         m_width = 0;
		int         m_height = 0;
	};

}