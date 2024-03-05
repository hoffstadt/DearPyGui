#include "mvLoadingIndicatorCustom.h"
#include <utility>
#include <imgui.h>
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>

// Posted by @alexsr here: https://github.com/ocornut/imgui/issues/1901
// Sligthly modified to provide default behaviour with default args
void LoadingIndicatorCircle(const char* label, float indicatorRadiusFactor,
    const ImVec4* pOptionalMainColor, const ImVec4* pOptionalBackdropColor,
    int circle_count, const float speed) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) {
        return;
    }

    ImGuiContext& g = *GImGui;
    const ImGuiID id = window->GetID(label);
    const ImGuiStyle& style = ImGui::GetStyle();

    if (circle_count <= 0) circle_count = 12;
    if (indicatorRadiusFactor <= 0.f) indicatorRadiusFactor = 1.f;
    if (!pOptionalMainColor)        pOptionalMainColor = &style.Colors[ImGuiCol_Button];
    if (!pOptionalBackdropColor)    pOptionalBackdropColor = &style.Colors[ImGuiCol_ButtonHovered];

    const float lineHeight = ImGui::GetTextLineHeight(); // or GetTextLineHeight() or GetTextLineHeightWithSpacing() ?
    float indicatorRadiusPixels = indicatorRadiusFactor * lineHeight * 0.5f;

    const ImVec2 pos = window->DC.CursorPos;
    const float circle_radius = indicatorRadiusPixels / 8.f;
    indicatorRadiusPixels -= 2.0f * circle_radius;
    const ImRect bb(pos, ImVec2(pos.x + indicatorRadiusPixels * 2.f + 4.f * circle_radius,
        pos.y + indicatorRadiusPixels * 2.f + 4.f * circle_radius));
    ImGui::ItemSize(bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id)) {
        return;
    }
    const float base_num_segments = circle_radius * 1.f;
    const double t = g.Time;
    const float degree_offset = 2.0f * IM_PI / circle_count;
    for (int i = 0; i < circle_count; ++i) {
        const float sinx = -ImSin(degree_offset * i);
        const float cosx = ImCos(degree_offset * i);
        const float growth = ImMax(0.0f, ImSin((float)(t * (double)(speed * 3.0f) - (double)(i * degree_offset))));
        ImVec4 color;
        color.x = pOptionalMainColor->x * growth + pOptionalBackdropColor->x * (1.0f - growth);
        color.y = pOptionalMainColor->y * growth + pOptionalBackdropColor->y * (1.0f - growth);
        color.z = pOptionalMainColor->z * growth + pOptionalBackdropColor->z * (1.0f - growth);
        color.w = pOptionalMainColor->w * growth + pOptionalBackdropColor->w * (1.0f - growth);
        float grown_circle_radius = circle_radius * (1.0f + growth);
        int num_segments = (int)(base_num_segments * grown_circle_radius);
        if (num_segments < 4) num_segments = 4;
        window->DrawList->AddCircleFilled(ImVec2(pos.x + 2.f * circle_radius + indicatorRadiusPixels * (1.0f + sinx),
            pos.y + 2.f * circle_radius + indicatorRadiusPixels * (1.0f + cosx)),
            grown_circle_radius,
            ImGui::GetColorU32(color), num_segments);
    }
}

// Posted by @zfedoran here: https://github.com/ocornut/imgui/issues/1901
// Sligthly modified to provide default behaviour with default args
void LoadingIndicatorCircle2(const char* label, float indicatorRadiusFactor, float indicatorRadiusThicknessFactor, const ImVec4* pOptionalColor) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);

    if (indicatorRadiusFactor <= 0.f) indicatorRadiusFactor = 1.f;
    if (indicatorRadiusThicknessFactor <= 0.f) indicatorRadiusThicknessFactor = 1.f;
    if (!pOptionalColor)    pOptionalColor = &style.Colors[ImGuiCol_Button];
    const ImU32 color = ImGui::GetColorU32(*pOptionalColor);

    const float lineHeight = ImGui::GetTextLineHeight(); // or GetTextLineHeight() or GetTextLineHeightWithSpacing() ?
    float indicatorRadiusPixels = indicatorRadiusFactor * lineHeight * 0.5f;
    float indicatorThicknessPixels = indicatorRadiusThicknessFactor * indicatorRadiusPixels * 0.6f;
    if (indicatorThicknessPixels > indicatorThicknessPixels * 0.4f) indicatorThicknessPixels = indicatorThicknessPixels * 0.4f;
    indicatorRadiusPixels -= indicatorThicknessPixels;

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size(indicatorRadiusPixels * 2.f, (indicatorRadiusPixels + style.FramePadding.y) * 2.f);

    const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
    ImGui::ItemSize(bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id))
        return;

    // Render
    window->DrawList->PathClear();



    //int num_segments = indicatorRadiusPixels/8.f;
    //if (num_segments<4) num_segments=4;

    int num_segments = 30;

    int start = abs(int(ImSin(g.Time * 1.8f) * (num_segments - 5)));

    const float a_min = IM_PI * 2.0f * ((float)start) / (float)num_segments;
    const float a_max = IM_PI * 2.0f * ((float)num_segments - 3) / (float)num_segments;

    const ImVec2 centre = ImVec2(pos.x + indicatorRadiusPixels, pos.y + indicatorRadiusPixels + style.FramePadding.y);

    for (int i = 0; i < num_segments; i++) {
        const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
        window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a + g.Time * 8) * indicatorRadiusPixels,
            centre.y + ImSin(a + g.Time * 8) * indicatorRadiusPixels));
    }

    window->DrawList->PathStroke(color, false, indicatorThicknessPixels);
}