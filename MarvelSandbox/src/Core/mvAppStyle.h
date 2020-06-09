#pragma once

#include "mvCore.h"
#include <map>
#include <string>

namespace Marvel {

    typedef std::map<std::string, ImVec2> mvStyle;

    static mvStyle getAppDefaultStyle()
    {
        mvStyle style;
        style["Alpha"] = { 1.0f , 0.0f };                
        style["WindowPadding"] = { 8.0f, 8.0f };
        style["WindowRounding"] = { 0.0, 0.0f };
        style["WindowBorderSize"] = { 1.0, 0.0f };
        style["WindowTitleAlign"] = { 0.0f, 0.50f };
        style["ChildRounding"] = { 0.0, 0.0f };
        style["ChildBorderSize"] = { 1.0, 0.0f };
        style["PopupRounding"] = { 0.0, 0.0f };
        style["PopupBorderSize"] = {1.0, 0.0f};
        style["FramePadding"] = { 4.0f, 4.0f };
        style["FrameRounding"] = { 0.0, 0.0f };
        style["FrameBorderSize"] = { 0.0, 0.0f };
        style["ItemSpacing"] = { 8.0f, 4.0f };
        style["ItemInnerSpacing"] = { 4.0f, 4.0f };
        style["TouchExtraPadding"] = { 0.0f, 0.0f };
        style["IndentSpacing"] = { 21.0, 0.0f };
        style["ScrollbarSize"] = { 14.0, 0.0f };
        style["ScrollbarRounding"] = { 9.0, 0.0f };
        style["GrabMinSize"] = { 10.0, 0.0f };
        style["GrabRounding"] = { 0.0, 0.0f };
        style["TabRounding"] = { 4.0, 0.0f };
        style["TabBorderSize"] = { 0.0, 0.0f };
        style["ButtonTextAlign"] = { 0.5f, 0.5f };
        style["SelectableTextAlign"] = { 0.0f, 0.0f };
        style["DisplaySafeAreaPadding"] = { 3.0f, 3.0f };
        style["AntiAliasedLines"] = { 1.0f, 0.0f };
        style["AntiAliasedFill"] = { 1.0f, 0.0f };
        style["CurveTessellationTol"] = { 1.25, 0.0f };
        style["CircleSegmentMaxError"] = { 1.60, 0.0f };

        return style;
    }

}