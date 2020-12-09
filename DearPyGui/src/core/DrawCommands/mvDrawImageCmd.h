#pragma once

#include "mvDrawCmd.h"
#include "mvEvents.h"

namespace Marvel {

	class mvDrawImageCmd : public mvDrawCmd, public mvEventHandler
	{

	public:

		MV_DRAWCMD_TYPE(mvDrawCmdType::DrawImage)

		mvDrawImageCmd(std::string file, const mvVec2& pmin, const mvVec2& pmax, const mvVec2& uv_min, const mvVec2& uv_max, const mvColor& color);

		bool onEvent(mvEvent& event) override;
		bool onTextureDeleted(mvEvent& event);

		void draw         (ImDrawList* drawlist, float x, float y) override;
		void setConfigDict(PyObject* dict)                         override;
		void getConfigDict(PyObject* dict)                         override;

		~mvDrawImageCmd() override;

	private:

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