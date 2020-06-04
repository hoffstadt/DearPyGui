#include "mvTheme.h"

static std::array<float, 4> ImLerp(std::array<float, 4>& a, std::array<float, 4>& b, float t)
{
    return { a[0] + (b[0] - a[0]) * t, a[1] + (b[1] - a[1]) * t, a[2] + (b[2] - a[2]) * t, a[3] + (b[3] - a[3]) * t  };
}

namespace Marvel {

    mvTheme::mvTheme()
    {
        addThemeItem("ImGuiCol_Text", 1.00f, 1.00f, 1.00f, 1.00f);
        addThemeItem("ImGuiCol_TextDisabled", 0.50f, 0.50f, 0.50f, 1.00f);
        addThemeItem("ImGuiCol_WindowBg", 0.06f, 0.06f, 0.06f, 0.94f);
        addThemeItem("ImGuiCol_ChildBg", 0.00f, 0.00f, 0.00f, 0.00f);
        addThemeItem("ImGuiCol_PopupBg", 0.08f, 0.08f, 0.08f, 0.94f);
        addThemeItem("ImGuiCol_Border", 0.43f, 0.43f, 0.50f, 0.50f);
        addThemeItem("ImGuiCol_BorderShadow", 0.00f, 0.00f, 0.00f, 0.00f);
        addThemeItem("ImGuiCol_FrameBg", 0.16f, 0.29f, 0.48f, 0.54f);
        addThemeItem("ImGuiCol_FrameBgHovered", 0.26f, 0.59f, 0.98f, 0.40f);
        addThemeItem("ImGuiCol_FrameBgActive", 0.26f, 0.59f, 0.98f, 0.67f);
        addThemeItem("ImGuiCol_TitleBg", 0.04f, 0.04f, 0.04f, 1.00f);
        addThemeItem("ImGuiCol_TitleBgActive", 0.16f, 0.29f, 0.48f, 1.00f);
        addThemeItem("ImGuiCol_TitleBgCollapsed", 0.00f, 0.00f, 0.00f, 0.51f);
        addThemeItem("ImGuiCol_MenuBarBg", 0.14f, 0.14f, 0.14f, 1.00f);
        addThemeItem("ImGuiCol_ScrollbarBg", 0.02f, 0.02f, 0.02f, 0.53f);
        addThemeItem("ImGuiCol_ScrollbarGrab", 0.31f, 0.31f, 0.31f, 1.00f);
        addThemeItem("ImGuiCol_ScrollbarGrabHovered", 0.41f, 0.41f, 0.41f, 1.00f);
        addThemeItem("ImGuiCol_ScrollbarGrabActive", 0.51f, 0.51f, 0.51f, 1.00f);
        addThemeItem("ImGuiCol_CheckMark", 0.26f, 0.59f, 0.98f, 1.00f);
        addThemeItem("ImGuiCol_SliderGrab", 0.24f, 0.52f, 0.88f, 1.00f);
        addThemeItem("ImGuiCol_SliderGrabActive", 0.26f, 0.59f, 0.98f, 1.00f);
        addThemeItem("ImGuiCol_Button", 0.26f, 0.59f, 0.98f, 0.40f);
        addThemeItem("ImGuiCol_ButtonHovered", 0.26f, 0.59f, 0.98f, 1.00f);
        addThemeItem("ImGuiCol_ButtonActive", 0.06f, 0.53f, 0.98f, 1.00f);
        addThemeItem("ImGuiCol_Header", 0.26f, 0.59f, 0.98f, 0.31f);
        addThemeItem("ImGuiCol_HeaderHovered", 0.26f, 0.59f, 0.98f, 0.80f);
        addThemeItem("ImGuiCol_HeaderActive", 0.26f, 0.59f, 0.98f, 1.00f);
        addThemeItem("ImGuiCol_Separator", m_colors[m_colorIds["ImGuiCol_Border"]]);
        addThemeItem("ImGuiCol_SeparatorHovered", 0.10f, 0.40f, 0.75f, 0.78f);
        addThemeItem("ImGuiCol_SeparatorActive", 0.10f, 0.40f, 0.75f, 1.00f);
        addThemeItem("ImGuiCol_ResizeGrip", 0.26f, 0.59f, 0.98f, 0.25f);
        addThemeItem("ImGuiCol_ResizeGripHovered", 0.26f, 0.59f, 0.98f, 0.67f);
        addThemeItem("ImGuiCol_ResizeGripActive", 0.26f, 0.59f, 0.98f, 0.95f);
        addThemeItem("ImGuiCol_Tab", ImLerp(m_colors[m_colorIds["ImGuiCol_Header"]], m_colors[m_colorIds["ImGuiCol_TitleBgActive"]], 0.80f));
        addThemeItem("ImGuiCol_TabHovered", m_colors[m_colorIds["ImGuiCol_HeaderHovered"]]);
        addThemeItem("ImGuiCol_TabActive", ImLerp(m_colors[m_colorIds["ImGuiCol_HeaderActive"]], m_colors[m_colorIds["ImGuiCol_TitleBgActive"]], 0.60f));
        addThemeItem("ImGuiCol_TabUnfocused", ImLerp(m_colors[m_colorIds["ImGuiCol_Tab"]], m_colors[m_colorIds["ImGuiCol_TitleBg"]], 0.80f));
        addThemeItem("ImGuiCol_TabUnfocusedActive", ImLerp(m_colors[m_colorIds["ImGuiCol_TabActive"]], m_colors[m_colorIds["ImGuiCol_TitleBg"]], 0.40f));
        addThemeItem("ImGuiCol_PlotLines", 0.61f, 0.61f, 0.61f, 1.00f );
        addThemeItem("ImGuiCol_PlotLinesHovered", 1.00f, 0.43f, 0.35f, 1.00f );
        addThemeItem("ImGuiCol_PlotHistogram", 0.90f, 0.70f, 0.00f, 1.00f );
        addThemeItem("ImGuiCol_PlotHistogramHovered", 1.00f, 0.60f, 0.00f, 1.00f );
        addThemeItem("ImGuiCol_TextSelectedBg", 0.26f, 0.59f, 0.98f, 0.35f );
        addThemeItem("ImGuiCol_DragDropTarget", 1.00f, 1.00f, 0.00f, 0.90f );
        addThemeItem("ImGuiCol_NavHighlight", 0.26f, 0.59f, 0.98f, 1.00f );
        addThemeItem("ImGuiCol_NavWindowingHighlight", 1.00f, 1.00f, 1.00f, 0.70f );
        addThemeItem("ImGuiCol_NavWindowingDimBg", 0.80f, 0.80f, 0.80f, 0.20f );
        addThemeItem("ImGuiCol_ModalWindowDimBg", 0.80f, 0.80f, 0.80f, 0.35f );
        
    }

    void mvTheme::addThemeItem(const char* name, std::array<float, 4> color)
    {
        addThemeItem(name, color[0], color[1], color[2], color[3]);
    }

    void mvTheme::addThemeItem(const char* name, float r, float g, float b, float a)
    {
        m_colorIds[name] = m_items;
        m_colors.push_back({ r, g, b, a });
        m_items++;
    }

    void mvTheme::changeThemeItem(const char* name, float r, float g, float b, float a)
    {
        m_colors[m_colorIds[name]] = { r, g, b, a };
    }

}