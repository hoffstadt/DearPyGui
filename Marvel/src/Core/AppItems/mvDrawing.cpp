#include "mvDrawing.h"

#include <cmath>
#include <algorithm>

#undef min
#undef max

namespace Marvel {

	void mvDrawImageCommand::draw(mvDrawing* draw, ImDrawList* draw_list)
	{
		if (m_texture == nullptr && !m_file.empty())
		{
			LoadTextureFromFile(m_file.c_str(), &m_texture, &m_width, &m_height);
			if(m_autosize)
				m_pmax = { (float)m_width + m_pmin.x, (float)m_height + m_pmin.y};
		}

		mvVec2 start = draw->getStart();
		if(m_texture)
			draw_list->AddImage(m_texture, m_pmin + start, m_pmax+start, m_uv_min, m_uv_max, m_color);
	}

	void mvDrawLineCommand::draw(mvDrawing* draw, ImDrawList* draw_list)
	{
		mvVec2 start = draw->getStart();
		draw_list->AddLine(m_p1+start, m_p2+start, m_color, m_thickness);
	}

	void mvDrawTriangleCommand::draw(mvDrawing* draw, ImDrawList* draw_list)
	{
		mvVec2 start = draw->getStart();
		if (m_fill.specified)
			draw_list->AddTriangleFilled(m_p1+start, m_p2 + start, m_p3 + start, m_fill);

		draw_list->AddTriangle(m_p1 + start, m_p2 + start, m_p3 + start, m_color, m_thickness);
	}

	void mvDrawCircleCommand::draw(mvDrawing* draw, ImDrawList* draw_list)
	{

		mvVec2 start = draw->getStart();
		if (m_fill.specified)
			draw_list->AddCircleFilled(m_center + start, m_radius, m_fill, m_segments);

		draw_list->AddCircle(m_center+start, m_radius, m_color, m_segments, m_thickness);
	}

	void mvDrawTextCommand::draw(mvDrawing* draw, ImDrawList* draw_list)
	{
		// TODO get imgui color for default
		mvVec2 start = draw->getStart();
		draw_list->AddText(ImGui::GetFont(), (float)m_size, m_pos+start, m_color, m_text.c_str());
	}

	void mvDrawRectCommand::draw(mvDrawing* draw, ImDrawList* draw_list)
	{
		mvVec2 start = draw->getStart();
		if (m_fill.specified)
			draw_list->AddRectFilled(m_pmin + start, m_pmax + start, m_fill, m_rounding, ImDrawCornerFlags_All);
		draw_list->AddRect(m_pmin + start, m_pmax+start, m_color, m_rounding, ImDrawCornerFlags_All,m_thickness);
	}

	void mvDrawQuadCommand::draw(mvDrawing* draw, ImDrawList* draw_list)
	{
		mvVec2 start = draw->getStart();
		if (m_fill.specified)
			draw_list->AddQuadFilled(m_p1 + start, m_p2 + start, m_p3 + start, m_p4 + start, m_fill);
		draw_list->AddQuad(m_p1+start, m_p2+start, m_p3+start, m_p4+start, m_color, m_thickness);
	}

	void mvDrawBezierCurveCommand::draw(mvDrawing* draw, ImDrawList* draw_list)
	{
		mvVec2 start = draw->getStart();
		draw_list->AddBezierCurve(m_p1 + start, m_p2 + start, m_p3 + start, m_p4 + start, m_color, m_thickness);
	}

	void mvDrawPolylineCommand::draw(mvDrawing* draw, ImDrawList* draw_list)
	{
		mvVec2 start = draw->getStart();

		std::vector<mvVec2> points = m_points;

		for (auto& point : points)
			point = point + start;

		draw_list->AddPolyline((const ImVec2*)const_cast<const mvVec2*>(points.data()), m_points.size(), m_color, m_closed, m_thickness);
	}

	void mvDrawPolygonCommand::draw(mvDrawing* draw, ImDrawList* draw_list)
	{
		mvVec2 start = draw->getStart();

		if (m_fill.specified)
		{
			int i;
			int y;
			int miny, maxy;
			int x1, y1;
			int x2, y2;
			int ind1, ind2;
			int ints;
			size_t n = m_points.size();
			int* polyints = new int[n];


			/* Determine Y maxima */
			miny = m_points[0].y;
			maxy = m_points[0].y;
			for (i = 1; (i < n); i++)
			{
				miny = std::min(miny, (int)m_points[i].y);
				maxy = std::max(maxy, (int)m_points[i].y);
			}

			/* Draw, scanning y */
			for (y = miny; (y <= maxy); y++) {
				ints = 0;
				for (i = 0; (i < n); i++) {
					if (!i) {
						ind1 = (int)n - 1;
						ind2 = 0;
					}
					else {
						ind1 = i - 1;
						ind2 = i;
					}
					y1 = m_points[ind1].y;
					y2 = m_points[ind2].y;
					if (y1 < y2) {
						x1 = m_points[ind1].x;
						x2 = m_points[ind2].x;
					}
					else if (y1 > y2) {
						y2 = m_points[ind1].y;
						y1 = m_points[ind2].y;
						x2 = m_points[ind1].x;
						x1 = m_points[ind2].x;
					}
					else {
						continue;
					}
					if ((y >= y1) && (y < y2)) {
						polyints[ints++] = (y - y1) * (x2 - x1) / (y2 - y1) + x1;
					}
					else if ((y == maxy) && (y > y1) && (y <= y2)) {
						polyints[ints++] = (y - y1) * (x2 - x1) / (y2 - y1) + x1;
					}
				}

				auto compare_int = [](const void* a, const void* b)
				{
					return (*(const int*)a) - (*(const int*)b);
				};

				qsort(polyints, ints, sizeof(int), compare_int);

				for (i = 0; (i < ints); i += 2) 
				{
					draw_list->AddLine({ polyints[i] + start.x, y + start.y },
						{ polyints[i+1] + start.x, y + start.y }, m_fill, m_thickness);
				}
			}
			delete[] polyints;
		}

		std::vector<mvVec2> points = m_points;

		for (auto& point : points)
			point = point + start;
		draw_list->AddPolyline((const ImVec2*)const_cast<const mvVec2*>(points.data()), m_points.size(), m_color, false, m_thickness);

	}

	void mvDrawing::draw()
	{

		draw_list = ImGui::GetWindowDrawList();
		m_startx = (int)ImGui::GetCursorScreenPos().x;
		m_starty = (int)ImGui::GetCursorScreenPos().y;

		if (m_dirty)
			updateCommands();

		ImGui::PushClipRect({ m_startx, m_starty }, { m_startx + (float)m_width, m_starty + (float)m_height }, true);

		for (auto command : m_commands)
			command->draw(this, draw_list);

		ImGui::PopClipRect();
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

	mvVec2 mvDrawing::convertToModelSpace(const mvVec2& point)
	{
		return { point.x*m_scalex + m_originx, m_height - point.y*m_scaley - m_originy };
	}

	void mvDrawing::convertToModelSpace(std::vector<mvVec2>& points, const std::vector<mvVec2>& pointso)
	{
		for (int i = 0; i < points.size(); i++)
		{
			points[i].x = pointso[i].x;

			points[i].x = pointso[i].x * m_scalex + m_originx;
			points[i].y = m_height - pointso[i].y * m_scaley - m_originy;
		}
	}

	void mvDrawing::drawLine(const mvVec2& p1, const mvVec2& p2, const mvColor& color, float thickness, const std::string& tag)
	{

		if (!tag.empty())
		{
			for (auto item : m_commands)
			{
				if (item->tag == tag && item->getType() == mvDrawingCommandType::DrawLine)
				{
					*static_cast<mvDrawLineCommand*>(item) = mvDrawLineCommand(p1, p2, color, thickness);
					item->tag = tag;
					m_dirty = true;
					return;
				}
			}
		}

		mvDrawingCommand* command = new mvDrawLineCommand(p1, p2,  color, thickness);
		command->tag = tag;
		m_commands.push_back(command);
	}

	void mvDrawing::drawTriangle(const mvVec2& p1, const mvVec2& p2, const mvVec2& p3, const mvColor& color, const mvColor& fill, float thickness, const std::string& tag)
	{
		if (!tag.empty())
		{
			for (auto item : m_commands)
			{
				if (item->tag == tag && item->getType() == mvDrawingCommandType::DrawTriangle)
				{
					*static_cast<mvDrawTriangleCommand*>(item) = mvDrawTriangleCommand(p1, 
						p2, p3, color, thickness, fill);
					item->tag = tag;
					m_dirty = true;
					return;
				}
			}
		}

		mvDrawingCommand* command = new mvDrawTriangleCommand(p1,
			p2, p3, color, thickness, fill);
		command->tag = tag;
		m_commands.push_back(command);
	}

	void mvDrawing::drawRectangle(const mvVec2& pmin, const mvVec2& pmax, const mvColor& color, const mvColor& fill, float rounding, float thickness, const std::string& tag)
	{
		if (!tag.empty())
		{
			for (auto item : m_commands)
			{
				if (item->tag == tag && item->getType() == mvDrawingCommandType::DrawRect)
				{
					*static_cast<mvDrawRectCommand*>(item) = mvDrawRectCommand(pmin, pmax, color, fill, rounding, thickness);
					item->tag = tag;
					m_dirty = true;
					return;
				}
			}
		}

		mvDrawingCommand* command = new mvDrawRectCommand(pmin, pmax, color, fill, rounding, thickness);
		command->tag = tag;
		m_commands.push_back(command);
	}

	void mvDrawing::drawQuad(const mvVec2& p1, const mvVec2& p2, const mvVec2& p3, const mvVec2& p4, const mvColor& color, const mvColor& fill, float thickness, const std::string& tag)
	{
		if (!tag.empty())
		{
			for (auto item : m_commands)
			{
				if (item->tag == tag && item->getType() == mvDrawingCommandType::DrawQuad)
				{
					*static_cast<mvDrawQuadCommand*>(item) = mvDrawQuadCommand(p1, p2, 
						p3, p4, color, fill, thickness);
					item->tag = tag;
					m_dirty = true;
					return;
				}
			}
		}

		mvDrawingCommand* command = new mvDrawQuadCommand(p1, p2,
			p3, p4, color, fill, thickness);
		command->tag = tag;
		m_commands.push_back(command);
	}

	void mvDrawing::drawText(const mvVec2& pos, const std::string& text, const mvColor& color, int size, const std::string& tag)
	{
		if (!tag.empty())
		{
			for (auto item : m_commands)
			{
				if (item->tag == tag && item->getType() == mvDrawingCommandType::DrawText)
				{
					*static_cast<mvDrawTextCommand*>(item) = mvDrawTextCommand(pos, text, color, size);
					item->tag = tag;
					m_dirty = true;
					return;
				}
			}
		}

		mvDrawingCommand* command = new mvDrawTextCommand(pos, text, color, size);
		command->tag = tag;
		m_commands.push_back(command);
	}

	void mvDrawing::drawCircle(const mvVec2& center, float radius, const mvColor& color, int segments, float thickness, const mvColor& fill, const std::string& tag)
	{
		if (!tag.empty())
		{
			for (auto item : m_commands)
			{
				if (item->tag == tag && item->getType() == mvDrawingCommandType::DrawCircle)
				{
					*static_cast<mvDrawCircleCommand*>(item) = mvDrawCircleCommand(center, radius, color, segments, thickness, fill);
					item->tag = tag;
					m_dirty = true;
					return;
				}
			}
		}

		mvDrawingCommand* command = new mvDrawCircleCommand(center, radius, color, segments, thickness, fill);
		command->tag = tag;
		m_commands.push_back(command);
	}

	void mvDrawing::drawPolyline(std::vector<mvVec2>& points, const mvColor& color, bool closed, float thickness, const std::string& tag)
	{

		if (!tag.empty())
		{
			for (auto item : m_commands)
			{
				if (item->tag == tag && item->getType() == mvDrawingCommandType::DrawPolyline)
				{
					*static_cast<mvDrawPolylineCommand*>(item) = mvDrawPolylineCommand(points, color, closed, thickness);
					item->tag = tag;
					m_dirty = true;
					return;
				}
			}
		}

		mvDrawingCommand* command = new mvDrawPolylineCommand(points, color, closed, thickness);
		command->tag = tag;
		m_commands.push_back(command);
	}

	void mvDrawing::drawPolygon(std::vector<mvVec2>& points, const mvColor& color, const mvColor& fill, float thickness, const std::string& tag)
	{

		if (!tag.empty())
		{
			for (auto item : m_commands)
			{
				if (item->tag == tag && item->getType() == mvDrawingCommandType::DrawPolygon)
				{
					*static_cast<mvDrawPolygonCommand*>(item) = mvDrawPolygonCommand(points, color, fill, thickness);
					item->tag = tag;
					m_dirty = true;
					return;
				}
			}
		}

		mvDrawingCommand* command = new mvDrawPolygonCommand(points, color, fill, thickness);
		command->tag = tag;
		m_commands.push_back(command);
	}

	void mvDrawing::drawBezierCurve(const mvVec2& p1, const mvVec2& p2, const mvVec2& p3, const mvVec2& p4, const mvColor& color, float thickness, int segments, const std::string& tag)
	{
		if (!tag.empty())
		{
			for (auto item : m_commands)
			{
				if (item->tag == tag && item->getType() == mvDrawingCommandType::DrawBezierCurve)
				{
					*static_cast<mvDrawBezierCurveCommand*>(item) = mvDrawBezierCurveCommand(p1, p2, p3, 
						convertToModelSpace(p4), color, thickness, segments);
					item->tag = tag;
					m_dirty = true;
					return;
				}
			}
		}

		mvDrawingCommand* command = new mvDrawBezierCurveCommand(p1, p2, p3,
			p4, color, thickness, segments);
		command->tag = tag;
		m_commands.push_back(command);
	}

	void mvDrawing::drawImage(const std::string& file, const mvVec2& pmin, const mvVec2& pmax, const mvVec2& uv_min, const mvVec2& uv_max, const mvColor& color, const std::string& tag)
	{
		if (!tag.empty())
		{
			for (auto item : m_commands)
			{
				if (item->tag == tag && item->getType() == mvDrawingCommandType::DrawImage)
				{
					*static_cast<mvDrawImageCommand*>(item) = mvDrawImageCommand(file, pmin, pmax, uv_min, uv_max, color);
					item->tag = tag;
					m_dirty = true;
					return;
				}
			}
		}

		mvDrawingCommand* command = new mvDrawImageCommand(file, pmin, pmax, uv_min, uv_max, color);
		command->tag = tag;
		m_commands.push_back(command);
	}

	void mvDrawing::updateCommands()
	{
		for (auto command : m_commands)
		{
			switch (command->getType())
			{

			case mvDrawingCommandType::DrawLine:
			{
				mvDrawLineCommand& acommand = *static_cast<mvDrawLineCommand*>(command);
				acommand.m_p1 = convertToModelSpace(acommand.m_p1o);
				acommand.m_p2 = convertToModelSpace(acommand.m_p2o);
				break;
			}

			case mvDrawingCommandType::DrawTriangle:
			{
				mvDrawTriangleCommand& acommand = *static_cast<mvDrawTriangleCommand*>(command);
				acommand.m_p1 = convertToModelSpace(acommand.m_p1o);
				acommand.m_p2 = convertToModelSpace(acommand.m_p2o);
				acommand.m_p3 = convertToModelSpace(acommand.m_p3o);
				break;
			}

			case mvDrawingCommandType::DrawCircle:
			{
				mvDrawCircleCommand& acommand = *static_cast<mvDrawCircleCommand*>(command);
				acommand.m_center = convertToModelSpace(acommand.m_centero);
				acommand.m_radius = acommand.m_radiuso * m_scalex;
				break;
			}


			case mvDrawingCommandType::DrawText:
			{
				mvDrawTextCommand& acommand = *static_cast<mvDrawTextCommand*>(command);
				acommand.m_pos = convertToModelSpace(acommand.m_poso);
				break;
			}

			case mvDrawingCommandType::DrawRect:
			{
				mvDrawRectCommand& acommand = *static_cast<mvDrawRectCommand*>(command);
				acommand.m_pmin = convertToModelSpace(acommand.m_pmino);
				acommand.m_pmax = convertToModelSpace(acommand.m_pmaxo);
				break;
			}

			case mvDrawingCommandType::DrawQuad:
			{
				mvDrawQuadCommand& acommand = *static_cast<mvDrawQuadCommand*>(command);
				acommand.m_p1 = convertToModelSpace(acommand.m_p1o);
				acommand.m_p2 = convertToModelSpace(acommand.m_p2o);
				acommand.m_p3 = convertToModelSpace(acommand.m_p3o);
				acommand.m_p4 = convertToModelSpace(acommand.m_p4o);
				break;
			}

			case mvDrawingCommandType::DrawPolyline:
			{
				mvDrawPolylineCommand& acommand = *static_cast<mvDrawPolylineCommand*>(command);
				convertToModelSpace(acommand.m_points, acommand.m_pointso);
				break;
			}

			case mvDrawingCommandType::DrawPolygon:
			{
				mvDrawPolygonCommand& acommand = *static_cast<mvDrawPolygonCommand*>(command);
				convertToModelSpace(acommand.m_points, acommand.m_pointso);
				break;
			}

			case mvDrawingCommandType::DrawBezierCurve:
			{
				mvDrawBezierCurveCommand& acommand = *static_cast<mvDrawBezierCurveCommand*>(command);
				acommand.m_p1 = convertToModelSpace(acommand.m_p1o);
				acommand.m_p2 = convertToModelSpace(acommand.m_p2o);
				acommand.m_p3 = convertToModelSpace(acommand.m_p3o);
				acommand.m_p4 = convertToModelSpace(acommand.m_p4o);
				break;
			}

			}
		}

		m_dirty = false;
	}

	void mvDrawing::setScale(float xscale, float yscale) 
	{ 
		m_scalex = xscale; 
		m_scaley = yscale; 
		m_dirty = true;
	}

	void mvDrawing::setOrigin(float x, float y) 
	{ 
		m_originx = x; 
		m_originy = y; 
		m_dirty = true;
	}
}