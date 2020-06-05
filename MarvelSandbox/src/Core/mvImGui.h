#pragma once

#include "mvGraph.h"
#include <imgui.h>
#include "events/mvInput.h"

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