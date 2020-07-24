#pragma once

//-----------------------------------------------------------------------------
// EditorColorScheme
//
//     - This is a temporary class for generating themes. This will be
//       removed once a proper theme system is implemented in v0.2
//     
//-----------------------------------------------------------------------------

namespace Marvel {

	//-----------------------------------------------------------------------------
	// EditorColorScheme
	//-----------------------------------------------------------------------------
	class EditorColorScheme
	{
		// 0xRRGGBBAA
		inline static int BackGroundColor = 0x25213100;
		inline static int TextColor = 0xF4F1DE00;
		inline static int MainColor = 0xDA115E00;
		inline static int MainAccentColor = 0x79235900;
		inline static int HighlightColor = 0xC7EF0000;

		inline static int Black = 0x00000000;
		inline static int White = 0xFFFFFF00;

		inline static int AlphaTransparent = 0x00;
		inline static int Alpha20 = 0x33;
		inline static int Alpha40 = 0x66;
		inline static int Alpha50 = 0x80;
		inline static int Alpha60 = 0x99;
		inline static int Alpha80 = 0xCC;
		inline static int Alpha90 = 0xE6;
		inline static int AlphaFull = 0xFF;

		static float GetR(int colorCode) { return (float)((colorCode & 0xFF000000) >> 24) / (float)(0xFF); }
		static float GetG(int colorCode) { return (float)((colorCode & 0x00FF0000) >> 16) / (float)(0xFF); }
		static float GetB(int colorCode) { return (float)((colorCode & 0x0000FF00) >> 8) / (float)(0xFF); }
		static float GetA(int alphaCode) { return ((float)alphaCode / (float)0xFF); }

		static ImVec4 GetColor(int c, int a = Alpha80) { return ImVec4(GetR(c), GetG(c), GetB(c), GetA(a)); }
		static ImVec4 Darken(ImVec4 c, float p) { return ImVec4(fmax(0.f, c.x - 1.0f * p), fmax(0.f, c.y - 1.0f * p), fmax(0.f, c.z - 1.0f * p), c.w); }
		static ImVec4 Lighten(ImVec4 c, float p) { return ImVec4(fmax(0.f, c.x + 1.0f * p), fmax(0.f, c.y + 1.0f * p), fmax(0.f, c.z + 1.0f * p), c.w); }

		static ImVec4 Disabled(ImVec4 c) { return Darken(c, 0.6f); }
		static ImVec4 Hovered(ImVec4 c) { return Lighten(c, 0.2f); }
		static ImVec4 Active(ImVec4 c) { return Lighten(ImVec4(c.x, c.y, c.z, 1.0f), 0.1f); }
		static ImVec4 Collapsed(ImVec4 c) { return Darken(c, 0.2f); }

	public:

		static void SetColors(int backGroundColor, int textColor, int mainColor, int mainAccentColor, int highlightColor)
		{
			BackGroundColor = backGroundColor;
			TextColor = textColor;
			MainColor = mainColor;
			MainAccentColor = mainAccentColor;
			HighlightColor = highlightColor;
		}

		static void ApplyTheme()
		{
			ImVec4* colors = ImGui::GetStyle().Colors;

			colors[ImGuiCol_Text] = GetColor(TextColor);
			colors[ImGuiCol_TextDisabled] = Disabled(colors[ImGuiCol_Text]);
			colors[ImGuiCol_WindowBg] = GetColor(BackGroundColor);
			colors[ImGuiCol_ChildBg] = GetColor(Black, Alpha20);
			colors[ImGuiCol_PopupBg] = GetColor(BackGroundColor, Alpha90);
			colors[ImGuiCol_Border] = Lighten(GetColor(BackGroundColor), 0.4f);
			colors[ImGuiCol_BorderShadow] = GetColor(Black);
			colors[ImGuiCol_FrameBg] = GetColor(MainAccentColor, Alpha40);
			colors[ImGuiCol_FrameBgHovered] = Hovered(colors[ImGuiCol_FrameBg]);
			colors[ImGuiCol_FrameBgActive] = Active(colors[ImGuiCol_FrameBg]);
			colors[ImGuiCol_TitleBg] = GetColor(BackGroundColor, Alpha80);
			colors[ImGuiCol_TitleBgActive] = Active(colors[ImGuiCol_TitleBg]);
			colors[ImGuiCol_TitleBgCollapsed] = Collapsed(colors[ImGuiCol_TitleBg]);
			colors[ImGuiCol_MenuBarBg] = Darken(GetColor(BackGroundColor), 0.2f);
			colors[ImGuiCol_ScrollbarBg] = Lighten(GetColor(BackGroundColor, Alpha50), 0.4f);
			colors[ImGuiCol_ScrollbarGrab] = Lighten(GetColor(BackGroundColor), 0.3f);
			colors[ImGuiCol_ScrollbarGrabHovered] = Hovered(colors[ImGuiCol_ScrollbarGrab]);
			colors[ImGuiCol_ScrollbarGrabActive] = Active(colors[ImGuiCol_ScrollbarGrab]);
			colors[ImGuiCol_CheckMark] = GetColor(HighlightColor);
			colors[ImGuiCol_SliderGrab] = GetColor(HighlightColor);
			colors[ImGuiCol_SliderGrabActive] = Active(colors[ImGuiCol_SliderGrab]);
			colors[ImGuiCol_Button] = GetColor(MainColor, Alpha80);
			colors[ImGuiCol_ButtonHovered] = Hovered(colors[ImGuiCol_Button]);
			colors[ImGuiCol_ButtonActive] = Active(colors[ImGuiCol_Button]);
			colors[ImGuiCol_Header] = GetColor(MainAccentColor, Alpha80);
			colors[ImGuiCol_HeaderHovered] = Hovered(colors[ImGuiCol_Header]);
			colors[ImGuiCol_HeaderActive] = Active(colors[ImGuiCol_Header]);
			colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
			colors[ImGuiCol_SeparatorHovered] = Hovered(colors[ImGuiCol_Separator]);
			colors[ImGuiCol_SeparatorActive] = Active(colors[ImGuiCol_Separator]);
			colors[ImGuiCol_ResizeGrip] = GetColor(MainColor, Alpha20);
			colors[ImGuiCol_ResizeGripHovered] = Hovered(colors[ImGuiCol_ResizeGrip]);
			colors[ImGuiCol_ResizeGripActive] = Active(colors[ImGuiCol_ResizeGrip]);
			colors[ImGuiCol_Tab] = GetColor(MainColor, Alpha60);
			colors[ImGuiCol_TabHovered] = Hovered(colors[ImGuiCol_Tab]);
			colors[ImGuiCol_TabActive] = Active(colors[ImGuiCol_Tab]);
			colors[ImGuiCol_TabUnfocused] = colors[ImGuiCol_Tab];
			colors[ImGuiCol_TabUnfocusedActive] = colors[ImGuiCol_TabActive];
			colors[ImGuiCol_PlotLines] = GetColor(HighlightColor);
			colors[ImGuiCol_PlotLinesHovered] = Hovered(colors[ImGuiCol_PlotLines]);
			colors[ImGuiCol_PlotHistogram] = GetColor(HighlightColor);
			colors[ImGuiCol_PlotHistogramHovered] = Hovered(colors[ImGuiCol_PlotHistogram]);
			colors[ImGuiCol_TextSelectedBg] = GetColor(HighlightColor, Alpha40);
			colors[ImGuiCol_DragDropTarget] = GetColor(HighlightColor, Alpha80);;
			colors[ImGuiCol_NavHighlight] = GetColor(White);
			colors[ImGuiCol_NavWindowingHighlight] = GetColor(White, Alpha80);
			colors[ImGuiCol_NavWindowingDimBg] = GetColor(White, Alpha20);
			colors[ImGuiCol_ModalWindowDimBg] = GetColor(Black, Alpha60);

			ImGui::GetStyle().WindowMenuButtonPosition = ImGuiDir_Right;
		}
	};

}