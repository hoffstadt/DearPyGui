#include "mvDrawTextCmd.h"

namespace Marvel {

	mvDrawTextCmd::mvDrawTextCmd(const mvVec2& pos, std::string  text, const mvColor& color, int size)
		:
		m_pos(pos),
		m_text(std::move(text)),
		m_color(color),
		m_size(size)
	{
	}

	void mvDrawTextCmd::draw(ImDrawList* drawlist, float x, float y)
	{
		mvVec2 start = {x, y};
		drawlist->AddText(ImGui::GetFont(), (float)m_size, m_pos + start, m_color, m_text.c_str());
	}
}