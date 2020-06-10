#pragma once

#include "mvGraph.h"
#include <imgui.h>
#include "events/mvInput.h"
#include "mvCore.h"

namespace marvel {

	//-----------------------------------------------------------------------------
	// ImGuiCanvas
	//-----------------------------------------------------------------------------
	class mvImGuiGraph : public mvGraph
	{

		ImDrawList* draw_list = nullptr;

	public:

		mvImGuiGraph() : mvGraph() {}

		virtual ~mvImGuiGraph() {}

		void drawLine(int x1, int y1, int x2, int y2, const mvColor& color, int thickness = 1) override
		{
			// colors
			ImVec4 colorf = ImVec4(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
			const ImU32 imcolor = ImColor(colorf);

			ImVec2 start = ImVec2(x1 + (float)m_startX, y1 + (float)m_startY);
			ImVec2 end = ImVec2(x2 + (float)m_startX, y2 + (float)m_startY);

			draw_list->AddLine(start, end, imcolor, (float)thickness);
		}

		void drawText(int x, int y, const std::string& text, const mvColor& color, int size = 10) override
		{
			// colors
			ImVec4 colorf = ImVec4(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
			const ImU32 imcolor = ImColor(colorf);

			ImVec2 start = ImVec2(x + (float)m_startX, y + (float)m_startY);

			draw_list->AddText(ImGui::GetFont(), (float)size, start, imcolor, text.c_str());
		}

		void drawRotatedText(int x, int y, double angle, const std::string& text, const mvColor& color, int size = 10) override
		{
			// colors
			ImVec4 colorf = ImVec4(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
			const ImU32 imcolor = ImColor(colorf);

			ImVec2 start = ImVec2(x + (float)m_startX, y + (float)m_startY);

			draw_list->AddText(ImGui::GetFont(), (float)size, start, imcolor, text.c_str(), nullptr, 0.0f);
		}

		virtual void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const mvColor& outline, const mvColor& fill, int thickness = 1) override
		{
			Marvel::mvVec2 start = {(float)m_startX, (float)m_startY};
			Marvel::mvVec2 p1 = { (float)x1, (float)y1 };
			Marvel::mvVec2 p2 = { (float)x2, (float)y2 };
			Marvel::mvVec2 p3 = { (float)x3, (float)y3 };
			Marvel::mvColor color = { outline.r / 255.0f, outline.g / 255.0f, outline.b / 255.0f, outline.a / 255.0f };
			Marvel::mvColor mfill = { fill.r / 255.0f, fill.g / 255.0f, fill.b / 255.0f, fill.a / 255.0f };

			if (fill.a != 0)
				draw_list->AddTriangleFilled(p1 + start, p2 + start, p3 + start, mfill);

			draw_list->AddTriangle(p1 + start, p2 + start, p3 + start, color, thickness);
		}

		virtual void drawRectangle(int x, int y, int width, int height, const mvColor& outline, const mvColor& fill, int thickness = 1) override
		{
			Marvel::mvVec2 start = { (float)m_startX, (float)m_startY };
			Marvel::mvVec2 pmin = { (float)x, (float)y };
			Marvel::mvVec2 pmax = { (float)x+width, (float)y+height };
			Marvel::mvColor color = { outline.r / 255.0f, outline.g / 255.0f, outline.b / 255.0f, outline.a / 255.0f };
			Marvel::mvColor mfill = { fill.r / 255.0f, fill.g / 255.0f, fill.b / 255.0f, fill.a / 255.0f };

			if (fill.a != 0)
				draw_list->AddRectFilled(pmin + start, pmax + start, mfill, 0, ImDrawCornerFlags_All);
			draw_list->AddRect(pmin + start, pmax + start, color, 0, ImDrawCornerFlags_All, thickness);
		}

		virtual void drawCircle(int x, int y, int r, const mvColor& outline, const mvColor& fill, int thickness = 1) override
		{
			Marvel::mvVec2 start = { (float)m_startX, (float)m_startY };
			Marvel::mvVec2 center = { (float)x, (float)y };
			Marvel::mvColor color = { outline.r / 255.0f, outline.g / 255.0f, outline.b / 255.0f, outline.a / 255.0f };
			Marvel::mvColor mfill = { fill.r / 255.0f, fill.g / 255.0f, fill.b / 255.0f, fill.a / 255.0f };

			if (fill.a != 0)
				draw_list->AddCircleFilled(center + start, r, mfill, 12);
			draw_list->AddCircle(center + start, r, color, 12, thickness);
		}

		virtual void drawLines(const std::vector<mvPoint>& data, const mvColor& color, int thickness = 1) override
		{
			Marvel::mvColor mcolor = { color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f };

			Marvel::mvVec2 start = { (float)m_startX, (float)m_startY };

			std::vector<Marvel::mvVec2> points;

			for (auto& point : data)
				points.push_back(Marvel::mvVec2{ (float)point.x, (float)point.y} + start);

			draw_list->AddPolyline((const ImVec2*)const_cast<const Marvel::mvVec2*>(points.data()),
				points.size(), mcolor, false, thickness);
		}

		mvPoint getTextExtent(const std::string& text, int fontsize) override
		{
			ImFont* font = ImGui::GetFont();

			float newfontscale = fontsize / font->FontSize;
			float origfontscale = font->Scale;

			font->Scale = newfontscale;

			ImGui::PushFont(font);
			ImVec2 size = ImGui::CalcTextSize(text.c_str());
			ImGui::PopFont();

			font->Scale = origfontscale;

			return marvel::mvPoint((int)size.x, (int)size.y);
		}

		void PreRender() override
		{
			draw_list = ImGui::GetWindowDrawList();
		}

		void PostRender() override
		{
			ImGui::Dummy(ImVec2((float)m_width, (float)m_height));
		}

	};

	//-----------------------------------------------------------------------------
	// mvImguiInput
	//-----------------------------------------------------------------------------
	class mvImguiInput : public mvInput
	{

	public:

		mvImguiInput() :mvInput() {}

	protected:

		virtual bool IsKeyPressedImpl(int keycode) const override
		{
			ImGuiIO& io = ImGui::GetIO();
			return io.KeysDown[keycode];
		}

		virtual bool IsMouseButtonPressedImpl(int button)  const override
		{
			ImGuiIO& io = ImGui::GetIO();
			return io.MouseDown[button];
		}

		virtual const mvPoint& GetMousePositionImpl() const override
		{
			ImGuiIO& io = ImGui::GetIO();
			return { io.MousePos.x, io.MousePos.y };
		}

		virtual int GetMouseXImpl() const override { return GetMousePositionImpl().x; }

		virtual int GetMouseYImpl() const override { return GetMousePositionImpl().y; }

	};

}