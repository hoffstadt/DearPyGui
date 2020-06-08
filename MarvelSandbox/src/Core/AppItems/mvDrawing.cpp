#include "mvDrawing.h"

namespace Marvel {

	void mvDrawLineCommand::draw(mvDrawing* draw, ImDrawList* draw_list)
	{
		float startX = draw->getStartX();
		float startY = draw->getStartY();

		// colors
		ImVec4 colorf = ImVec4(m_color.r, m_color.g, m_color.b, m_color.a);
		const ImU32 imcolor = ImColor(colorf);

		ImVec2 start = ImVec2(m_x1 + startX, m_y1 + startY);
		ImVec2 end = ImVec2(m_x2 + startX, m_y2 + startY);

		draw_list->AddLine(start, end, imcolor, m_thickness);
	}

	void mvDrawing::draw()
	{
		draw_list = ImGui::GetWindowDrawList();
		m_startx = (int)ImGui::GetCursorScreenPos().x;
		m_starty = (int)ImGui::GetCursorScreenPos().y;

		for (auto command : m_commands)
			command->draw(this, draw_list);

		ImGui::Dummy(ImVec2((float)m_width, m_height));
	}

	void mvDrawing::clear()
	{
		for (mvDrawingCommand* item : m_commands)
		{
			delete item;
			item = nullptr;
		}

		m_commands.clear();
	}

	void mvDrawing::drawLine(int x1, int y1, int x2, int y2, const mvColor& color, float thickness)
	{
		mvDrawingCommand* command = new mvDrawLineCommand(x1, y1, x2, y2, color, thickness);
		m_commands.push_back(command);
	}
}