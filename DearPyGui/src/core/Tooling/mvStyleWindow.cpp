#include "mvStyleWindow.h"
#include "mvApp.h"
#include "imnodes.h"

// Play it nice with Windows users (Update: May 2018, Notepad now supports Unix-style carriage returns!)
#ifdef _WIN32
#define IM_NEWLINE  "\r\n"
#else
#define IM_NEWLINE  "\n"
#endif

#define IM_MIN(A, B)            (((A) < (B)) ? (A) : (B))
#define IM_MAX(A, B)            (((A) >= (B)) ? (A) : (B))

// Helper to display a little (?) mark which shows a tooltip when hovered.
// In your own code you may want to display an actual icon if you are using a merged icon fonts (see docs/FONTS.md)
static void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

namespace Marvel {


    mvStyleWindow::mvStyleWindow()
    {
        m_windowflags = ImGuiWindowFlags_NoSavedSettings;
    }

    void mvStyleWindow::drawWidgets()
    {


        ImGuiStyle* ref = NULL;

        ImPlotStyle& plotstyle = ImPlot::GetStyle();

        // You can pass in a reference ImGuiStyle structure to compare to, revert to and save to
        // (without a reference style pointer, we will use one compared locally as a reference)
        ImGuiStyle& style = ImGui::GetStyle();
        static ImGuiStyle ref_saved_style;

        // Default to using internal storage as reference
        static bool init = true;
        //if (init && ref == NULL)
            ref_saved_style = style;
        init = false;
        //if (ref == NULL)
            ref = &ref_saved_style;

        ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.50f);

        //if (ImGui::ShowStyleSelector("Colors##Selector"))
        //    ref_saved_style = style;
        ImGui::ShowFontSelector("Fonts##Selector");

        // Simplified Settings (expose floating-pointer border sizes as boolean representing 0.0f or 1.0f)
        if (ImGui::SliderFloat("FrameRounding", &style.FrameRounding, 0.0f, 12.0f, "%.0f"))
            style.GrabRounding = style.FrameRounding; // Make GrabRounding always the same value as FrameRounding
        { bool border = (style.WindowBorderSize > 0.0f); if (ImGui::Checkbox("WindowBorder", &border)) { style.WindowBorderSize = border ? 1.0f : 0.0f; } }
        ImGui::SameLine();
        { bool border = (style.FrameBorderSize > 0.0f);  if (ImGui::Checkbox("FrameBorder", &border)) { style.FrameBorderSize = border ? 1.0f : 0.0f; } }
        ImGui::SameLine();
        { bool border = (style.PopupBorderSize > 0.0f);  if (ImGui::Checkbox("PopupBorder", &border)) { style.PopupBorderSize = border ? 1.0f : 0.0f; } }

        // Save/Revert button
        //if (ImGui::Button("Save Ref"))
        //    *ref = ref_saved_style = style;
        //ImGui::SameLine();
        //if (ImGui::Button("Revert Ref"))
        //    style = *ref;
        //ImGui::SameLine();
        //HelpMarker(
        //    "Save/Revert in local non-persistent storage. Default Colors definition are not affected. "
        //    "Use \"Export\" below to save them somewhere.");

        ImGui::Separator();

        if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
        {
            if (ImGui::BeginTabItem("Styles"))
            {
                ImGui::Text("Main");
                ImGui::SliderFloat2("mvStyleVar_WindowPadding", (float*)&style.WindowPadding, 0.0f, 20.0f, "%.0f");
                ImGui::SliderFloat2("mvStyleVar_FramePadding", (float*)&style.FramePadding, 0.0f, 20.0f, "%.0f");
                ImGui::SliderFloat2("mvStyleVar_CellPadding", (float*)&style.CellPadding, 0.0f, 20.0f, "%.0f");
                ImGui::SliderFloat2("mvStyleVar_ItemSpacing", (float*)&style.ItemSpacing, 0.0f, 20.0f, "%.0f");
                ImGui::SliderFloat2("mvStyleVar_ItemInnerSpacing", (float*)&style.ItemInnerSpacing, 0.0f, 20.0f, "%.0f");
                ImGui::SliderFloat2("mvStyleVar_TouchExtraPadding", (float*)&style.TouchExtraPadding, 0.0f, 10.0f, "%.0f");
                ImGui::SliderFloat("mvStyleVar_IndentSpacing", &style.IndentSpacing, 0.0f, 30.0f, "%.0f");
                ImGui::SliderFloat("mvStyleVar_ScrollbarSize", &style.ScrollbarSize, 1.0f, 20.0f, "%.0f");
                ImGui::SliderFloat("mvStyleVar_GrabMinSize", &style.GrabMinSize, 1.0f, 20.0f, "%.0f");
                ImGui::Text("Borders");
                ImGui::SliderFloat("mvStyleVar_WindowBorderSize", &style.WindowBorderSize, 0.0f, 1.0f, "%.0f");
                ImGui::SliderFloat("mvStyleVar_ChildBorderSize", &style.ChildBorderSize, 0.0f, 1.0f, "%.0f");
                ImGui::SliderFloat("mvStyleVar_PopupBorderSize", &style.PopupBorderSize, 0.0f, 1.0f, "%.0f");
                ImGui::SliderFloat("mvStyleVar_FrameBorderSize", &style.FrameBorderSize, 0.0f, 1.0f, "%.0f");
                ImGui::SliderFloat("mvStyleVar_TabBorderSize", &style.TabBorderSize, 0.0f, 1.0f, "%.0f");
                ImGui::Text("Rounding");
                ImGui::SliderFloat("mvStyleVar_WindowRounding", &style.WindowRounding, 0.0f, 12.0f, "%.0f");
                ImGui::SliderFloat("mvStyleVar_ChildRounding", &style.ChildRounding, 0.0f, 12.0f, "%.0f");
                ImGui::SliderFloat("mvStyleVar_FrameRounding", &style.FrameRounding, 0.0f, 12.0f, "%.0f");
                ImGui::SliderFloat("mvStyleVar_PopupRounding", &style.PopupRounding, 0.0f, 12.0f, "%.0f");
                ImGui::SliderFloat("mvStyleVar_ScrollbarRounding", &style.ScrollbarRounding, 0.0f, 12.0f, "%.0f");
                ImGui::SliderFloat("mvStyleVar_GrabRounding", &style.GrabRounding, 0.0f, 12.0f, "%.0f");
                ImGui::SliderFloat("mvStyleVar_LogSliderDeadzone", &style.LogSliderDeadzone, 0.0f, 12.0f, "%.0f");
                ImGui::SliderFloat("mvStyleVar_TabRounding", &style.TabRounding, 0.0f, 12.0f, "%.0f");
                ImGui::Text("Alignment");
                ImGui::SliderFloat2("mvStyleVar_WindowTitleAlign", (float*)&style.WindowTitleAlign, 0.0f, 1.0f, "%.2f");
                //int window_menu_button_position = style.WindowMenuButtonPosition + 1;
                //if (ImGui::Combo("WindowMenuButtonPosition", (int*)&window_menu_button_position, "None\0Left\0Right\0"))
                //    style.WindowMenuButtonPosition = window_menu_button_position - 1;
                //ImGui::Combo("mvStyleVar_ColorButtonPosition", (int*)&style.ColorButtonPosition, "Left\0Right\0");
                ImGui::SliderFloat2("mvStyleVar_ButtonTextAlign", (float*)&style.ButtonTextAlign, 0.0f, 1.0f, "%.2f");
                ImGui::SameLine(); HelpMarker("mvStyleVar_Alignment applies when a button is larger than its text content.");
                ImGui::SliderFloat2("mvStyleVar_SelectableTextAlign", (float*)&style.SelectableTextAlign, 0.0f, 1.0f, "%.2f");
                ImGui::SameLine(); HelpMarker("mvStyleVar_Alignment applies when a selectable is larger than its text content.");
                ImGui::Text("Safe Area Padding");
                ImGui::SameLine(); HelpMarker("Adjust if you cannot see the edges of your screen (e.g. on a TV where scaling has not been configured).");
                ImGui::SliderFloat2("mvStyleVar_DisplaySafeAreaPadding", (float*)&style.DisplaySafeAreaPadding, 0.0f, 30.0f, "%.0f");
                
                ImGui::Text("Item Styling");
                ImGui::SliderFloat("LineWeight", &plotstyle.LineWeight, 0.0f, 5.0f, "%.1f");
                ImGui::SliderFloat("MarkerSize", &plotstyle.MarkerSize, 2.0f, 10.0f, "%.1f");
                ImGui::SliderFloat("MarkerWeight", &plotstyle.MarkerWeight, 0.0f, 5.0f, "%.1f");
                ImGui::SliderFloat("FillAlpha", &plotstyle.FillAlpha, 0.0f, 1.0f, "%.2f");
                ImGui::SliderFloat("ErrorBarSize", &plotstyle.ErrorBarSize, 0.0f, 10.0f, "%.1f");
                ImGui::SliderFloat("ErrorBarWeight", &plotstyle.ErrorBarWeight, 0.0f, 5.0f, "%.1f");
                ImGui::SliderFloat("DigitalBitHeight", &plotstyle.DigitalBitHeight, 0.0f, 20.0f, "%.1f");
                ImGui::SliderFloat("DigitalBitGap", &plotstyle.DigitalBitGap, 0.0f, 20.0f, "%.1f");
                float indent = ImGui::CalcItemWidth() - ImGui::GetFrameHeight();
                ImGui::Indent(ImGui::CalcItemWidth() - ImGui::GetFrameHeight());
                ImGui::Checkbox("AntiAliasedLines", &plotstyle.AntiAliasedLines);
                ImGui::Unindent(indent);
                ImGui::Text("Plot Styling");
                ImGui::SliderFloat("PlotBorderSize", &plotstyle.PlotBorderSize, 0.0f, 2.0f, "%.0f");
                ImGui::SliderFloat("MinorAlpha", &plotstyle.MinorAlpha, 0.0f, 1.0f, "%.2f");
                ImGui::SliderFloat2("MajorTickLen", (float*)&plotstyle.MajorTickLen, 0.0f, 20.0f, "%.0f");
                ImGui::SliderFloat2("MinorTickLen", (float*)&plotstyle.MinorTickLen, 0.0f, 20.0f, "%.0f");
                ImGui::SliderFloat2("MajorTickSize", (float*)&plotstyle.MajorTickSize, 0.0f, 2.0f, "%.1f");
                ImGui::SliderFloat2("MinorTickSize", (float*)&plotstyle.MinorTickSize, 0.0f, 2.0f, "%.1f");
                ImGui::SliderFloat2("MajorGridSize", (float*)&plotstyle.MajorGridSize, 0.0f, 2.0f, "%.1f");
                ImGui::SliderFloat2("MinorGridSize", (float*)&plotstyle.MinorGridSize, 0.0f, 2.0f, "%.1f");
                ImGui::SliderFloat2("PlotDefaultSize", (float*)&plotstyle.PlotDefaultSize, 0.0f, 1000, "%.0f");
                ImGui::SliderFloat2("PlotMinSize", (float*)&plotstyle.PlotMinSize, 0.0f, 300, "%.0f");
                ImGui::Text("Plot Padding");
                ImGui::SliderFloat2("PlotPadding", (float*)&plotstyle.PlotPadding, 0.0f, 20.0f, "%.0f");
                ImGui::SliderFloat2("LabelPadding", (float*)&plotstyle.LabelPadding, 0.0f, 20.0f, "%.0f");
                ImGui::SliderFloat2("LegendPadding", (float*)&plotstyle.LegendPadding, 0.0f, 20.0f, "%.0f");
                ImGui::SliderFloat2("LegendInnerPadding", (float*)&plotstyle.LegendInnerPadding, 0.0f, 10.0f, "%.0f");
                ImGui::SliderFloat2("LegendSpacing", (float*)&plotstyle.LegendSpacing, 0.0f, 5.0f, "%.0f");
                ImGui::SliderFloat2("MousePosPadding", (float*)&plotstyle.MousePosPadding, 0.0f, 20.0f, "%.0f");
                ImGui::SliderFloat2("AnnotationPadding", (float*)&plotstyle.AnnotationPadding, 0.0f, 5.0f, "%.0f");
                ImGui::SliderFloat2("FitPadding", (float*)&plotstyle.FitPadding, 0, 0.2f, "%.2f");
                ImGui::Text("Nodes");
                ImGui::SliderFloat("mvNodeStyleVar_GridSpacing", &imnodes::GetStyle().grid_spacing, 0.0f, 32.0f, "%.0f");
                ImGui::SliderFloat("mvNodeStyleVar_NodeCornerRounding", &imnodes::GetStyle().node_corner_rounding, 0.0f, 10.0f, "%.0f");
                ImGui::SliderFloat("mvNodeStyleVar_NodePaddingHorizontal", &imnodes::GetStyle().node_padding_horizontal, 0.0f, 10.0f, "%.0f");
                ImGui::SliderFloat("mvNodeStyleVar_NodePaddingVertical", &imnodes::GetStyle().node_padding_vertical, 0.0f, 10.0f, "%.0f");
                ImGui::SliderFloat("mvNodeStyleVar_NodeBorderThickness", &imnodes::GetStyle().node_border_thickness, 0.0f, 10.0f, "%.0f");
                ImGui::SliderFloat("mvNodeStyleVar_LinkThickness", &imnodes::GetStyle().link_thickness, 0.0f, 10.0f, "%.0f");
                ImGui::SliderFloat("mvNodeStyleVar_LinkLineSegmentsPerLength", &imnodes::GetStyle().link_line_segments_per_length, 0.0f, 10.0f, "%.0f");
                ImGui::SliderFloat("mvNodeStyleVar_LinkHoverDistance", &imnodes::GetStyle().link_hover_distance, 0.0f, 10.0f, "%.0f");
                ImGui::SliderFloat("mvNodeStyleVar_PinCircleRadius", &imnodes::GetStyle().pin_circle_radius, 0.0f, 10.0f, "%.0f");
                ImGui::SliderFloat("mvNodeStyleVar_PinQuadSideLength", &imnodes::GetStyle().pin_quad_side_length, 0.0f, 10.0f, "%.0f");
                ImGui::SliderFloat("mvNodeStyleVar_PinTriangleSideLength", &imnodes::GetStyle().pin_triangle_side_length, 0.0f, 10.0f, "%.0f");
                ImGui::SliderFloat("mvNodeStyleVar_PinLineThickness", &imnodes::GetStyle().pin_line_thickness, 0.0f, 10.0f, "%.0f");
                ImGui::SliderFloat("mvNodeStyleVar_PinHoverRadius", &imnodes::GetStyle().pin_hover_radius, 0.0f, 10.0f, "%.0f");
                ImGui::SliderFloat("mvNodeStyleVar_PinOffset", &imnodes::GetStyle().pin_offset, 0.0f, 10.0f, "%.0f");

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Colors"))
            {
                static int output_dest = 0;
                static bool output_only_modified = true;
                //if (ImGui::Button("Export"))
                //{
                //    if (output_dest == 0)
                //        ImGui::LogToClipboard();
                //    else
                //        ImGui::LogToTTY();
                //    ImGui::LogText("ImVec4* colors = ImGui::GetStyle().Colors;" IM_NEWLINE);
                //    for (int i = 0; i < ImGuiCol_COUNT; i++)
                //    {
                //        const ImVec4& col = style.Colors[i];
                //        const char* name = ImGui::GetStyleColorName(i);
                //        if (!output_only_modified || memcmp(&col, &ref->Colors[i], sizeof(ImVec4)) != 0)
                //            ImGui::LogText("colors[ImGuiCol_%s]%*s= ImVec4(%.2ff, %.2ff, %.2ff, %.2ff);" IM_NEWLINE,
                //                name, 23 - (int)strlen(name), "", col.x, col.y, col.z, col.w);
                //    }
                //    ImGui::LogFinish();
                //}
                //ImGui::SameLine(); ImGui::SetNextItemWidth(120); ImGui::Combo("##output_type", &output_dest, "To Clipboard\0To TTY\0");
                //ImGui::Checkbox("Only Modified Colors", &output_only_modified);

                static ImGuiTextFilter filter;
                filter.Draw("Filter colors", ImGui::GetFontSize() * 16);

                static ImGuiColorEditFlags alpha_flags = 0;
                if (ImGui::RadioButton("Opaque", alpha_flags == ImGuiColorEditFlags_None)) { alpha_flags = ImGuiColorEditFlags_None; } ImGui::SameLine();
                if (ImGui::RadioButton("Alpha", alpha_flags == ImGuiColorEditFlags_AlphaPreview)) { alpha_flags = ImGuiColorEditFlags_AlphaPreview; } ImGui::SameLine();
                if (ImGui::RadioButton("Both", alpha_flags == ImGuiColorEditFlags_AlphaPreviewHalf)) { alpha_flags = ImGuiColorEditFlags_AlphaPreviewHalf; } ImGui::SameLine();
                HelpMarker(
                    "In the color list:\n"
                    "Left-click on color square to open color picker,\n"
                    "Right-click to open edit options menu.");

                ImGui::BeginChild("##colors", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar | ImGuiWindowFlags_NavFlattened);
                ImGui::PushItemWidth(-300);
                for (int i = 0; i < ImGuiCol_COUNT; i++)
                {
                    const char* name = ImGui::GetStyleColorName(i);
                    if (!filter.PassFilter(name))
                        continue;
                    ImGui::PushID(i);
                    ImGui::ColorEdit4("##color", (float*)&style.Colors[i], ImGuiColorEditFlags_AlphaBar | alpha_flags); 
                    ImGui::SameLine(0.0f, style.ItemInnerSpacing.x); 
                    ImGui::Text("mvThemeCol_%s", name);

                    ImGui::PopID();
                }

                for (int i = 0; i < ImPlotCol_COUNT; i++)
                {
                    const char* name = ImPlot::GetStyleColorName(i);
                    if (!filter.PassFilter(name))
                        continue;
                    ImGui::PushID(i);
                    ImGui::ColorEdit4("##color123", (float*)&plotstyle.Colors[i].x, ImGuiColorEditFlags_AlphaBar | alpha_flags);
                    ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
                    ImGui::Text("mvPlotCol_%s", name);

                    ImGui::PopID();
                }

                // imnodes
                static std::string imnodesNames[16] = {
                    "mvNodeCol_NodeBackground",
                    "mvNodeCol_NodeBackgroundHovered",
                    "mvNodeCol_NodeBackgroundSelected",
                    "mvNodeCol_NodeOutline",
                    "mvNodeCol_TitleBar",
                    "mvNodeCol_TitleBarHovered",
                    "mvNodeCol_TitleBarSelected",
                    "mvNodeCol_Link",
                    "mvNodeCol_LinkHovered",
                    "mvNodeCol_LinkSelected",
                    "mvNodeCol_Pin",
                    "mvNodeCol_PinHovered",
                    "mvNodeCol_BoxSelector",
                    "mvNodeCol_BoxSelectorOutline",
                    "mvNodeCol_GridBackground",
                    "mvNodeCol_GridLine"
                };

                for (int i = 0; i < imnodes::ColorStyle_Count; i++)
                {
                    const char* name = imnodesNames[i].c_str();
                    if (!filter.PassFilter(name))
                        continue;
                    ImGui::PushID(i);
                    ImVec4 color = ImGui::ColorConvertU32ToFloat4(imnodes::GetStyle().colors[i]);
                    if (ImGui::ColorEdit4("##color1234", (float*)&color.x, ImGuiColorEditFlags_AlphaBar | alpha_flags))
                    {
                        imnodes::GetStyle().colors[i] = ImGui::ColorConvertFloat4ToU32(color);
                    }

                    ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
                    ImGui::Text("%s", name);

                    ImGui::PopID();
                }

                ImGui::PopItemWidth();
                ImGui::EndChild();

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Rendering"))
            {
                ImGui::Checkbox("Anti-aliased lines", &style.AntiAliasedLines);
                ImGui::SameLine();
                HelpMarker("When disabling anti-aliasing lines, you'll probably want to disable borders in your style as well.");

                ImGui::Checkbox("Anti-aliased lines use texture", &style.AntiAliasedLinesUseTex);
                ImGui::SameLine();
                HelpMarker("Faster lines using texture data. Require backend to render with bilinear filtering (not point/nearest filtering).");

                ImGui::Checkbox("Anti-aliased fill", &style.AntiAliasedFill);
                ImGui::PushItemWidth(ImGui::GetFontSize() * 8);
                ImGui::DragFloat("Curve Tessellation Tolerance", &style.CurveTessellationTol, 0.02f, 0.10f, 10.0f, "%.2f");
                if (style.CurveTessellationTol < 0.10f) style.CurveTessellationTol = 0.10f;

                // When editing the "Circle Segment Max Error" value, draw a preview of its effect on auto-tessellated circles.
                ImGui::DragFloat("Circle Tessellation Max Error", &style.CircleTessellationMaxError, 0.005f, 0.10f, 5.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
                if (ImGui::IsItemActive())
                {
                    ImGui::SetNextWindowPos(ImGui::GetCursorScreenPos());
                    ImGui::BeginTooltip();
                    ImGui::TextUnformatted("(R = radius, N = number of segments)");
                    ImGui::Spacing();
                    ImDrawList* draw_list = ImGui::GetWindowDrawList();
                    const float min_widget_width = ImGui::CalcTextSize("N: MMM\nR: MMM").x;
                    for (int n = 0; n < 8; n++)
                    {
                        const float RAD_MIN = 5.0f;
                        const float RAD_MAX = 70.0f;
                        const float rad = RAD_MIN + (RAD_MAX - RAD_MIN) * (float)n / (8.0f - 1.0f);

                        ImGui::BeginGroup();

                        ImGui::Text("R: %.f\nN: %d", rad, draw_list->_CalcCircleAutoSegmentCount(rad));

                        const float canvas_width = IM_MAX(min_widget_width, rad * 2.0f);
                        const float offset_x = floorf(canvas_width * 0.5f);
                        const float offset_y = floorf(RAD_MAX);

                        const ImVec2 p1 = ImGui::GetCursorScreenPos();
                        draw_list->AddCircle(ImVec2(p1.x + offset_x, p1.y + offset_y), rad, ImGui::GetColorU32(ImGuiCol_Text));
                        ImGui::Dummy(ImVec2(canvas_width, RAD_MAX * 2));

                        /*
                        const ImVec2 p2 = ImGui::GetCursorScreenPos();
                        draw_list->AddCircleFilled(ImVec2(p2.x + offset_x, p2.y + offset_y), rad, ImGui::GetColorU32(ImGuiCol_Text));
                        ImGui::Dummy(ImVec2(canvas_width, RAD_MAX * 2));
                        */

                        ImGui::EndGroup();
                        ImGui::SameLine();
                    }
                    ImGui::EndTooltip();
                }
                ImGui::SameLine();
                HelpMarker("When drawing circle primitives with \"num_segments == 0\" tesselation will be calculated automatically.");

                ImGui::DragFloat("Global Alpha", &style.Alpha, 0.005f, 0.20f, 1.0f, "%.2f"); // Not exposing zero here so user doesn't "lose" the UI (zero alpha clips all widgets). But application code could have a toggle to switch between zero and non-zero.
                ImGui::PopItemWidth();

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        ImGui::PopItemWidth();

    }

}
