#pragma once

#include "mvDrawCmd.h"

namespace Marvel {

	class mvDrawTextCmd : public mvDrawCmd
	{

	public:

		MV_DRAWCMD_TYPE(mvDrawCmdType::DrawText)

		mvDrawTextCmd(const mvVec2& pos, std::string  text, const mvColor& color, int size);

		void draw(ImDrawList* drawlist, float x, float y) override;

	private:

		mvVec2      m_pos;
		std::string m_text;
		mvColor     m_color;
		int         m_size;

	};

}