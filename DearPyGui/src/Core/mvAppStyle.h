#pragma once

#include "mvCore.h"
#include <map>
#include <string>

// TODO : this entire system will be reworked.

namespace Marvel {

    typedef std::map<ImGuiStyleVar, ImVec2> mvStyle;

    static mvStyle getAppDefaultStyle()
    {
        mvStyle style;
        style[ImGuiStyleVar_Alpha] = { 1.0f , 0.0f };
        style[ImGuiStyleVar_WindowPadding] = { 8.0f, 8.0f };
        style[ImGuiStyleVar_WindowRounding] = { 0.0f, 0.0f };
        style[ImGuiStyleVar_WindowBorderSize] = { 1.0f, 0.0f };
        style[ImGuiStyleVar_WindowTitleAlign] = { 0.0f, 0.50f };
        style[ImGuiStyleVar_ChildRounding] = { 0.0f, 0.0f };
        style[ImGuiStyleVar_ChildBorderSize] = { 1.0f, 0.0f };
        style[ImGuiStyleVar_PopupRounding] = { 0.0f, 0.0f };
        style[ImGuiStyleVar_PopupBorderSize] = {1.0, 0.0f};
        style[ImGuiStyleVar_FramePadding] = { 4.0f, 4.0f };
        style[ImGuiStyleVar_FrameRounding] = { 0.0f, 0.0f };
        style[ImGuiStyleVar_FrameBorderSize] = { 0.0f, 0.0f };
        style[ImGuiStyleVar_ItemSpacing] = { 8.0f, 4.0f };
        style[ImGuiStyleVar_ItemInnerSpacing] = { 4.0f, 4.0f };
        style[23] = { 0.0f, 0.0f }; // ImGuiStyleVar_TouchExtraPadding
        style[ImGuiStyleVar_IndentSpacing] = { 21.0f, 0.0f };
        style[ImGuiStyleVar_ScrollbarSize] = { 14.0f, 0.0f };
        style[ImGuiStyleVar_ScrollbarRounding] = { 9.0f, 0.0f };
        style[ImGuiStyleVar_GrabMinSize] = { 10.0f, 0.0f };
        style[ImGuiStyleVar_GrabRounding] = { 0.0f, 0.0f };
        style[ImGuiStyleVar_TabRounding] = { 4.0f, 0.0f };
        style[24] = { 0.0f, 0.0f }; // ImGuiStyleVar_TabBorderSize
        style[ImGuiStyleVar_ButtonTextAlign] = { 0.5f, 0.5f };
        style[ImGuiStyleVar_SelectableTextAlign] = { 0.0f, 0.0f };
        style[25] = { 3.0f, 3.0f }; //ImGuiStyleVar_DisplaySafeAreaPadding
        style[26] = { 1.0f, 0.0f }; //ImGuiStyleVar_AntiAliasedLines
        style[27] = { 1.0f, 0.0f }; //ImGuiStyleVar_AntiAliasedFill
        style[28] = { 1.25f, 0.0f }; //ImGuiStyleVar_CurveTessellationTol
        style[29] = { 1.60f, 0.0f }; //ImGuiStyleVar_CircleSegmentMaxError

        return style;
    }

}