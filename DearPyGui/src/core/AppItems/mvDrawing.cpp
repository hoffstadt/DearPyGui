#include "mvDrawing.h"

namespace Marvel {


	mvDrawing::mvDrawing(const std::string& name)
		: mvAppItem(name)
	{
	}

	void mvDrawing::draw()
	{
		m_startx = (float)ImGui::GetCursorScreenPos().x;
		m_starty = (float)ImGui::GetCursorScreenPos().y;

		ImGui::PushClipRect({ m_startx, m_starty }, { m_startx + (float)m_width, m_starty + (float)m_height }, true);

		m_drawList.draw(ImGui::GetWindowDrawList(), m_startx, m_starty);

		ImGui::PopClipRect();
		ImGui::Dummy(ImVec2((float)m_width, (float)m_height));
	}

	mvDrawList& mvDrawing::getDrawList()
	{
		return m_drawList;
	}
}
