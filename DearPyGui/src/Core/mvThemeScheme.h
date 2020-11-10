#pragma once

//-----------------------------------------------------------------------------
// EditorColorScheme
//
//     - This is a temporary class for generating themes.
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

		static void ApplyTheme(ImGuiStyle& style)
		{
			style.Colors[ImGuiCol_Text] = GetColor(TextColor);
			style.Colors[ImGuiCol_TextDisabled] = Disabled(style.Colors[ImGuiCol_Text]);
			style.Colors[ImGuiCol_WindowBg] = GetColor(BackGroundColor);
			style.Colors[ImGuiCol_ChildBg] = GetColor(Black, Alpha20);
			style.Colors[ImGuiCol_PopupBg] = GetColor(BackGroundColor, Alpha90);
			style.Colors[ImGuiCol_Border] = Lighten(GetColor(BackGroundColor), 0.4f);
			style.Colors[ImGuiCol_BorderShadow] = GetColor(Black);
			style.Colors[ImGuiCol_FrameBg] = GetColor(MainAccentColor, Alpha40);
			style.Colors[ImGuiCol_FrameBgHovered] = Hovered(style.Colors[ImGuiCol_FrameBg]);
			style.Colors[ImGuiCol_FrameBgActive] = Active(style.Colors[ImGuiCol_FrameBg]);
			style.Colors[ImGuiCol_TitleBg] = GetColor(BackGroundColor, Alpha80);
			style.Colors[ImGuiCol_TitleBgActive] = Active(style.Colors[ImGuiCol_TitleBg]);
			style.Colors[ImGuiCol_TitleBgCollapsed] = Collapsed(style.Colors[ImGuiCol_TitleBg]);
			style.Colors[ImGuiCol_MenuBarBg] = Darken(GetColor(BackGroundColor), 0.2f);
			style.Colors[ImGuiCol_ScrollbarBg] = Lighten(GetColor(BackGroundColor, Alpha50), 0.4f);
			style.Colors[ImGuiCol_ScrollbarGrab] = Lighten(GetColor(BackGroundColor), 0.3f);
			style.Colors[ImGuiCol_ScrollbarGrabHovered] = Hovered(style.Colors[ImGuiCol_ScrollbarGrab]);
			style.Colors[ImGuiCol_ScrollbarGrabActive] = Active(style.Colors[ImGuiCol_ScrollbarGrab]);
			style.Colors[ImGuiCol_CheckMark] = GetColor(HighlightColor);
			style.Colors[ImGuiCol_SliderGrab] = GetColor(HighlightColor);
			style.Colors[ImGuiCol_SliderGrabActive] = Active(style.Colors[ImGuiCol_SliderGrab]);
			style.Colors[ImGuiCol_Button] = GetColor(MainColor, Alpha80);
			style.Colors[ImGuiCol_ButtonHovered] = Hovered(style.Colors[ImGuiCol_Button]);
			style.Colors[ImGuiCol_ButtonActive] = Active(style.Colors[ImGuiCol_Button]);
			style.Colors[ImGuiCol_Header] = GetColor(MainAccentColor, Alpha80);
			style.Colors[ImGuiCol_HeaderHovered] = Hovered(style.Colors[ImGuiCol_Header]);
			style.Colors[ImGuiCol_HeaderActive] = Active(style.Colors[ImGuiCol_Header]);
			style.Colors[ImGuiCol_Separator] = style.Colors[ImGuiCol_Border];
			style.Colors[ImGuiCol_SeparatorHovered] = Hovered(style.Colors[ImGuiCol_Separator]);
			style.Colors[ImGuiCol_SeparatorActive] = Active(style.Colors[ImGuiCol_Separator]);
			style.Colors[ImGuiCol_ResizeGrip] = GetColor(MainColor, Alpha20);
			style.Colors[ImGuiCol_ResizeGripHovered] = Hovered(style.Colors[ImGuiCol_ResizeGrip]);
			style.Colors[ImGuiCol_ResizeGripActive] = Active(style.Colors[ImGuiCol_ResizeGrip]);
			style.Colors[ImGuiCol_Tab] = GetColor(MainColor, Alpha60);
			style.Colors[ImGuiCol_TabHovered] = Hovered(style.Colors[ImGuiCol_Tab]);
			style.Colors[ImGuiCol_TabActive] = Active(style.Colors[ImGuiCol_Tab]);
			style.Colors[ImGuiCol_TabUnfocused] = style.Colors[ImGuiCol_Tab];
			style.Colors[ImGuiCol_TabUnfocusedActive] = style.Colors[ImGuiCol_TabActive];
			style.Colors[ImGuiCol_PlotLines] = GetColor(HighlightColor);
			style.Colors[ImGuiCol_PlotLinesHovered] = Hovered(style.Colors[ImGuiCol_PlotLines]);
			style.Colors[ImGuiCol_PlotHistogram] = GetColor(HighlightColor);
			style.Colors[ImGuiCol_PlotHistogramHovered] = Hovered(style.Colors[ImGuiCol_PlotHistogram]);
			style.Colors[ImGuiCol_TextSelectedBg] = GetColor(HighlightColor, Alpha40);
			style.Colors[ImGuiCol_DragDropTarget] = GetColor(HighlightColor, Alpha80);;
			style.Colors[ImGuiCol_NavHighlight] = GetColor(White);
			style.Colors[ImGuiCol_NavWindowingHighlight] = GetColor(White, Alpha80);
			style.Colors[ImGuiCol_NavWindowingDimBg] = GetColor(White, Alpha20);
			style.Colors[ImGuiCol_ModalWindowDimBg] = GetColor(Black, Alpha60);

		}
	};

}