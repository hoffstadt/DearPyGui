#include <imgui.h>
#include "mvApp.h"
#include "mvStyleWindow.h"
#include "mvInput.h"
#include "mvPythonTranslator.h"

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

// [Internal] Display details for a single font, called by ShowStyleEditor().
static void NodeFont(ImFont* font)
{
    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();
    bool font_details_opened = ImGui::TreeNode(font, "Font: \"%s\"\n%.2f px, %d glyphs, %d file(s)",
        font->ConfigData ? font->ConfigData[0].Name : "", font->FontSize, font->Glyphs.Size, font->ConfigDataCount);
    ImGui::SameLine(); if (ImGui::SmallButton("Set as default")) { io.FontDefault = font; }
    if (!font_details_opened)
        return;

    ImGui::PushFont(font);
    ImGui::Text("The quick brown fox jumps over the lazy dog");
    ImGui::PopFont();
    ImGui::DragFloat("Font scale", &font->Scale, 0.005f, 0.3f, 2.0f, "%.1f");   // Scale only this font
    ImGui::SameLine(); HelpMarker(
        "Note than the default embedded font is NOT meant to be scaled.\n\n"
        "Font are currently rendered into bitmaps at a given size at the time of building the atlas. "
        "You may oversample them to get some flexibility with scaling. "
        "You can also render at multiple sizes and select which one to use at runtime.\n\n"
        "(Glimmer of hope: the atlas system will be rewritten in the future to make scaling more flexible.)");
    //ImGui::InputFloat("Font offset", &font->GlyphOffset.y, 1, 1, "%.0f");
    //ImGui::InputInt("Font offset", &font->ConfigData->GlyphOffset.y, 1, 1, "%.0f");
    ImGui::Text("Ascent: %f, Descent: %f, Height: %f", font->Ascent, font->Descent, font->Ascent - font->Descent);
    ImGui::Text("Fallback character: '%c' (U+%04X)", font->FallbackChar, font->FallbackChar);
    ImGui::Text("Ellipsis character: '%c' (U+%04X)", font->EllipsisChar, font->EllipsisChar);
    const int surface_sqrt = (int)sqrtf((float)font->MetricsTotalSurface);
    ImGui::Text("Texture Area: about %d px ~%dx%d px", font->MetricsTotalSurface, surface_sqrt, surface_sqrt);
    for (int config_i = 0; config_i < font->ConfigDataCount; config_i++)
        if (font->ConfigData)
            if (const ImFontConfig* cfg = &font->ConfigData[config_i])
                ImGui::BulletText("Input %d: \'%s\', Oversample: (%d,%d), PixelSnapH: %d",
                    config_i, cfg->Name, cfg->OversampleH, cfg->OversampleV, cfg->PixelSnapH);
    if (ImGui::TreeNode("Glyphs", "Glyphs (%d)", font->Glyphs.Size))
    {
        // Display all glyphs of the fonts in separate pages of 256 characters
        const ImU32 glyph_col = ImGui::GetColorU32(ImGuiCol_Text);
        for (unsigned int base = 0; base <= IM_UNICODE_CODEPOINT_MAX; base += 256)
        {
            // Skip ahead if a large bunch of glyphs are not present in the font (test in chunks of 4k)
            // This is only a small optimization to reduce the number of iterations when IM_UNICODE_MAX_CODEPOINT
            // is large // (if ImWchar==ImWchar32 we will do at least about 272 queries here)
            if (!(base & 4095) && font->IsGlyphRangeUnused(base, base + 4095))
            {
                base += 4096 - 256;
                continue;
            }

            int count = 0;
            for (unsigned int n = 0; n < 256; n++)
                if (font->FindGlyphNoFallback((ImWchar)(base + n)))
                    count++;
            if (count <= 0)
                continue;
            if (!ImGui::TreeNode((void*)(intptr_t)base, "U+%04X..U+%04X (%d %s)", base, base + 255, count, count > 1 ? "glyphs" : "glyph"))
                continue;
            float cell_size = font->FontSize * 1;
            float cell_spacing = style.ItemSpacing.y;
            ImVec2 base_pos = ImGui::GetCursorScreenPos();
            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            for (unsigned int n = 0; n < 256; n++)
            {
                // We use ImFont::RenderChar as a shortcut because we don't have UTF-8 conversion functions
                // available here and thus cannot easily generate a zero-terminated UTF-8 encoded string.
                ImVec2 cell_p1(base_pos.x + (n % 16) * (cell_size + cell_spacing), base_pos.y + (n / 16) * (cell_size + cell_spacing));
                ImVec2 cell_p2(cell_p1.x + cell_size, cell_p1.y + cell_size);
                const ImFontGlyph* glyph = font->FindGlyphNoFallback((ImWchar)(base + n));
                draw_list->AddRect(cell_p1, cell_p2, glyph ? IM_COL32(255, 255, 255, 100) : IM_COL32(255, 255, 255, 50));
                if (glyph)
                    font->RenderChar(draw_list, cell_size, cell_p1, glyph_col, (ImWchar)(base + n));
                if (glyph && ImGui::IsMouseHoveringRect(cell_p1, cell_p2))
                {
                    ImGui::BeginTooltip();
                    ImGui::Text("Codepoint: U+%04X", base + n);
                    ImGui::Separator();
                    ImGui::Text("Visible: %d", glyph->Visible);
                    ImGui::Text("AdvanceX: %.1f", glyph->AdvanceX);
                    ImGui::Text("Pos: (%.2f,%.2f)->(%.2f,%.2f)", glyph->X0, glyph->Y0, glyph->X1, glyph->Y1);
                    ImGui::Text("UV: (%.3f,%.3f)->(%.3f,%.3f)", glyph->U0, glyph->V0, glyph->U1, glyph->V1);
                    ImGui::EndTooltip();
                }
            }
            ImGui::Dummy(ImVec2((cell_size + cell_spacing) * 16, (cell_size + cell_spacing) * 16));
            ImGui::TreePop();
        }
        ImGui::TreePop();
    }
    ImGui::TreePop();
}

namespace Marvel {

    void mvStyleWindow::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        parsers->insert({ "add_style_window", mvPythonParser({
            {mvPythonDataType::String, "name"},
            {mvPythonDataType::KeywordOnly},
            {mvPythonDataType::Integer, "width", "", "700"},
            {mvPythonDataType::Integer, "height", "", "500"},
            {mvPythonDataType::Integer, "x_pos", "x position the window will start at", "200"},
            {mvPythonDataType::Integer, "y_pos", "y position the window will start at", "200"},
            {mvPythonDataType::Bool, "autosize", "Autosized the window to fit it's items.", "False"},
            {mvPythonDataType::Bool, "no_resize", "Allows for the window size to be changed or fixed", "False"},
            {mvPythonDataType::Bool, "no_title_bar", "Title name for the title bar of the window", "False"},
            {mvPythonDataType::Bool, "no_move", "Allows for the window's position to be changed or fixed", "False"},
            {mvPythonDataType::Bool, "no_scrollbar" ," Disable scrollbars (window can still scroll with mouse or programmatically)", "False"},
            {mvPythonDataType::Bool, "no_collapse" ,"Disable user collapsing window by double-clicking on it", "False"},
            {mvPythonDataType::Bool, "horizontal_scrollbar" ,"Allow horizontal scrollbar to appear (off by default).", "False"},
            {mvPythonDataType::Bool, "no_focus_on_appearing" ,"Disable taking focus when transitioning from hidden to visible state", "False"},
            {mvPythonDataType::Bool, "no_bring_to_front_on_focus" ,"Disable bringing window to front when taking focus (e.g. clicking on it or programmatically giving it focus)", "False"},
            {mvPythonDataType::Bool, "no_close", "", "False"},
            {mvPythonDataType::Bool, "no_background", "", "False"},
            {mvPythonDataType::String, "label", "", "''"},
            {mvPythonDataType::Bool, "show", "Attempt to render", "True"},
        }, "Creates a style window.",
            "None", "Containers") });
    }

    // Demo helper function to select among loaded fonts.
    // Here we use the regular BeginCombo()/EndCombo() api which is more the more flexible one.
    static void ShowCustomFontSelector(const char* label)
    {
        ImGuiIO& io = ImGui::GetIO();
        ImFont* font_current = ImGui::GetFont();
        if (ImGui::BeginCombo(label, font_current->GetDebugName()))
        {
            for (int n = 0; n < io.Fonts->Fonts.Size; n++)
            {
                ImFont* font = io.Fonts->Fonts[n];
                ImGui::PushID((void*)font);
                if (ImGui::Selectable(font->GetDebugName(), font == font_current))
                    io.FontDefault = font;
                ImGui::PopID();
            }
            ImGui::EndCombo();
        }
    }

    static bool ShowCustomStyleSelector(const char* label)
    {
        static int style_idx = -1;
        if (ImGui::Combo(label, &style_idx, "Classic\0Dark\0Light\0Dark 2\0Grey\0Dark Grey\0Cherry\0Purple\0Gold\0Red"))
        {
            switch (style_idx)
            {
            case 0: mvApp::GetApp()->setAppTheme("Classic"); break;
            case 1: mvApp::GetApp()->setAppTheme("Dark"); break;
            case 2: mvApp::GetApp()->setAppTheme("Light"); break;
            case 3: mvApp::GetApp()->setAppTheme("Dark 2"); break;
            case 4: mvApp::GetApp()->setAppTheme("Grey"); break;
            case 5: mvApp::GetApp()->setAppTheme("Dark Grey"); break;
            case 6: mvApp::GetApp()->setAppTheme("Cherry"); break;
            case 7: mvApp::GetApp()->setAppTheme("Purple"); break;
            case 8: mvApp::GetApp()->setAppTheme("Gold"); break;
            case 9: mvApp::GetApp()->setAppTheme("Red"); break;
            }
            return true;
        }
        return false;
    }

    void mvStyleWindow::draw()
    {
        if (!prerender())
            return;

        ImGuiStyle* ref = nullptr;

        // You can pass in a reference ImGuiStyle structure to compare to, revert to and save to
        // (without a reference style pointer, we will use one compared locally as a reference)
        ImGuiStyle& style = ImGui::GetStyle();
        static ImGuiStyle ref_saved_style;

        // Default to using internal storage as reference
        static bool init = true;
        if (init)
            ref_saved_style = style;
        init = false;
        ref = &ref_saved_style;

        ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.50f);

        if (ShowCustomStyleSelector("Colors##Selector"))
            ref_saved_style = style;
        ShowCustomFontSelector("Fonts##Selector");

        // Simplified Settings (expose floating-pointer border sizes as boolean representing 0.0f or 1.0f)
        if (ImGui::SliderFloat("FrameRounding", &style.FrameRounding, 0.0f, 12.0f, "%.0f"))
            style.GrabRounding = style.FrameRounding; // Make GrabRounding always the same value as FrameRounding
        { bool border = (style.WindowBorderSize > 0.0f); if (ImGui::Checkbox("WindowBorder", &border)) { style.WindowBorderSize = border ? 1.0f : 0.0f; } }
        ImGui::SameLine();
        { bool border = (style.FrameBorderSize > 0.0f);  if (ImGui::Checkbox("FrameBorder", &border)) { style.FrameBorderSize = border ? 1.0f : 0.0f; } }
        ImGui::SameLine();
        { bool border = (style.PopupBorderSize > 0.0f);  if (ImGui::Checkbox("PopupBorder", &border)) { style.PopupBorderSize = border ? 1.0f : 0.0f; } }

        // Save/Revert button
        if (ImGui::Button("Save Ref"))
            *ref = ref_saved_style = style;
        ImGui::SameLine();
        if (ImGui::Button("Revert Ref"))
            style = *ref;
        ImGui::SameLine();
        HelpMarker(
            "Save/Revert in local non-persistent storage. Default Colors definition are not affected. "
            "Use \"Export\" below to save them somewhere.");

        ImGui::Separator();

        if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
        {
            if (ImGui::BeginTabItem("Sizes"))
            {
                HelpMarker(
                    "Export saves commands to your clipboard. Paste into your file.");
                ImGui::SameLine();
                if (ImGui::Button("Export Style"))
                {

                    ImGui::LogToClipboard();

                    ImGui::LogText("set_style_window_padding(%0.2f, %0.2f)\r\n", style.WindowPadding.x, style.WindowPadding.y);
                    ImGui::LogText("set_style_frame_padding(%0.2f, %0.2f)\r\n", style.FramePadding.x, style.FramePadding.y);
                    ImGui::LogText("set_style_item_spacing(%0.2f, %0.2f)\r\n", style.ItemSpacing.x, style.ItemSpacing.y);
                    ImGui::LogText("set_style_item_inner_spacing(%0.2f, %0.2f)\r\n", style.ItemInnerSpacing.x, style.ItemInnerSpacing.y);
                    ImGui::LogText("set_style_touch_extra_padding(%0.2f, %0.2f)\r\n", style.TouchExtraPadding.x, style.TouchExtraPadding.y);
                    ImGui::LogText("set_style_indent_spacing(%0.2f)\r\n", style.IndentSpacing);
                    ImGui::LogText("set_style_scrollbar_size(%0.2f)\r\n", style.ScrollbarSize);
                    ImGui::LogText("set_style_grab_min_size(%0.2f)\r\n", style.GrabMinSize);
                    ImGui::LogText("set_style_window_border_size(%0.2f)\r\n", style.WindowBorderSize);
                    ImGui::LogText("set_style_child_border_size(%0.2f)\r\n", style.ChildBorderSize);
                    ImGui::LogText("set_style_popup_border_size(%0.2f)\r\n", style.PopupBorderSize);
                    ImGui::LogText("set_style_frame_border_size(%0.2f)\r\n", style.FrameBorderSize);
                    ImGui::LogText("set_style_tab_border_size(%0.2f)\r\n", style.TabBorderSize);
                    ImGui::LogText("set_style_window_rounding(%0.2f)\r\n", style.WindowRounding);
                    ImGui::LogText("set_style_child_rounding(%0.2f)\r\n", style.ChildRounding);
                    ImGui::LogText("set_style_frame_rounding(%0.2f)\r\n", style.FrameRounding);
                    ImGui::LogText("set_style_popup_rounding(%0.2f)\r\n", style.PopupRounding);
                    ImGui::LogText("set_style_scrollbar_rounding(%0.2f)\r\n", style.ScrollbarRounding);
                    ImGui::LogText("set_style_grab_rounding(%0.2f)\r\n", style.GrabRounding);
                    ImGui::LogText("set_style_tab_rounding(%0.2f)\r\n", style.TabRounding);
                    ImGui::LogText("set_style_window_title_align(%0.2f, %0.2f)\r\n", style.WindowTitleAlign.x, style.WindowTitleAlign.y);
                    ImGui::LogText("set_style_window_menu_button_position(%s)\r\n", style.WindowMenuButtonPosition == ImGuiDir_Left ? "mvDir_Left" : "mvDir_Right");
                    ImGui::LogText("set_style_color_button_position(%s)\r\n", style.ColorButtonPosition == ImGuiDir_Left ? "mvDir_Left" : "mvDir_Right");
                    ImGui::LogText("set_style_button_text_align(%0.2f, %0.2f)\r\n", style.ButtonTextAlign.x, style.ButtonTextAlign.y);
                    ImGui::LogText("set_style_selectable_text_align(%0.2f, %0.2f)\r\n", style.SelectableTextAlign.x, style.SelectableTextAlign.y);
                    ImGui::LogText("set_style_display_safe_area_padding(%0.2f, %0.2f)\r\n", style.DisplaySafeAreaPadding.x, style.DisplaySafeAreaPadding.y);
                    ImGui::LogText("set_style_global_alpha(%0.2f)\r\n", style.Alpha);
                    ImGui::LogText("set_style_antialiased_lines(%s)\r\n", style.AntiAliasedLines ? "True" : "False");
                    ImGui::LogText("set_style_antialiased_fill(%s)\r\n", style.AntiAliasedFill ? "True" : "False");
                    ImGui::LogText("set_style_curve_tessellation_tolerance(%0.2f)\r\n", style.CurveTessellationTol);
                    ImGui::LogText("set_style_circle_segment_max_error(%0.2f)\r\n", style.CircleSegmentMaxError);
                    ImGui::LogFinish();
                }

                ImGui::Text("Main");
                ImGui::SliderFloat2("WindowPadding", (float*)&style.WindowPadding, 0.0f, 20.0f, "%.0f");
                ImGui::SliderFloat2("FramePadding", (float*)&style.FramePadding, 0.0f, 20.0f, "%.0f");
                ImGui::SliderFloat2("ItemSpacing", (float*)&style.ItemSpacing, 0.0f, 20.0f, "%.0f");
                ImGui::SliderFloat2("ItemInnerSpacing", (float*)&style.ItemInnerSpacing, 0.0f, 20.0f, "%.0f");
                ImGui::SliderFloat2("TouchExtraPadding", (float*)&style.TouchExtraPadding, 0.0f, 10.0f, "%.0f");
                ImGui::SliderFloat("IndentSpacing", &style.IndentSpacing, 0.0f, 30.0f, "%.0f");
                ImGui::SliderFloat("ScrollbarSize", &style.ScrollbarSize, 1.0f, 20.0f, "%.0f");
                ImGui::SliderFloat("GrabMinSize", &style.GrabMinSize, 1.0f, 20.0f, "%.0f");
                ImGui::Text("Borders");
                ImGui::SliderFloat("WindowBorderSize", &style.WindowBorderSize, 0.0f, 1.0f, "%.0f");
                ImGui::SliderFloat("ChildBorderSize", &style.ChildBorderSize, 0.0f, 1.0f, "%.0f");
                ImGui::SliderFloat("PopupBorderSize", &style.PopupBorderSize, 0.0f, 1.0f, "%.0f");
                ImGui::SliderFloat("FrameBorderSize", &style.FrameBorderSize, 0.0f, 1.0f, "%.0f");
                ImGui::SliderFloat("TabBorderSize", &style.TabBorderSize, 0.0f, 1.0f, "%.0f");
                ImGui::Text("Rounding");
                ImGui::SliderFloat("WindowRounding", &style.WindowRounding, 0.0f, 12.0f, "%.0f");
                ImGui::SliderFloat("ChildRounding", &style.ChildRounding, 0.0f, 12.0f, "%.0f");
                ImGui::SliderFloat("FrameRounding", &style.FrameRounding, 0.0f, 12.0f, "%.0f");
                ImGui::SliderFloat("PopupRounding", &style.PopupRounding, 0.0f, 12.0f, "%.0f");
                ImGui::SliderFloat("ScrollbarRounding", &style.ScrollbarRounding, 0.0f, 12.0f, "%.0f");
                ImGui::SliderFloat("GrabRounding", &style.GrabRounding, 0.0f, 12.0f, "%.0f");
                ImGui::SliderFloat("TabRounding", &style.TabRounding, 0.0f, 12.0f, "%.0f");
                ImGui::Text("Alignment");
                ImGui::SliderFloat2("WindowTitleAlign", (float*)&style.WindowTitleAlign, 0.0f, 1.0f, "%.2f");
                int window_menu_button_position = style.WindowMenuButtonPosition + 1;
                if (ImGui::Combo("WindowMenuButtonPosition", (int*)&window_menu_button_position, "None\0Left\0Right\0"))
                    style.WindowMenuButtonPosition = window_menu_button_position - 1;
                ImGui::Combo("ColorButtonPosition", (int*)&style.ColorButtonPosition, "Left\0Right\0");
                ImGui::SliderFloat2("ButtonTextAlign", (float*)&style.ButtonTextAlign, 0.0f, 1.0f, "%.2f");
                ImGui::SameLine(); HelpMarker("Alignment applies when a button is larger than its text content.");
                ImGui::SliderFloat2("SelectableTextAlign", (float*)&style.SelectableTextAlign, 0.0f, 1.0f, "%.2f");
                ImGui::SameLine(); HelpMarker("Alignment applies when a selectable is larger than its text content.");
                ImGui::Text("Safe Area Padding");
                ImGui::SameLine(); HelpMarker("Adjust if you cannot see the edges of your screen (e.g. on a TV where scaling has not been configured).");
                ImGui::SliderFloat2("DisplaySafeAreaPadding", (float*)&style.DisplaySafeAreaPadding, 0.0f, 30.0f, "%.0f");
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Colors"))
            {
                static bool output_only_modified = true;
                HelpMarker(
                    "Export saves commands to your clipboard. Paste into your file.");
                ImGui::SameLine();
                if (ImGui::Button("Export Colors"))
                {

                    ImGui::LogToClipboard();

                    for (int i = 0; i < ImGuiCol_COUNT; i++)
                    {
                        const ImVec4& col = style.Colors[i];
                        const char* name = ImGui::GetStyleColorName(i);
                        if (!output_only_modified || memcmp(&col, &ref->Colors[i], sizeof(ImVec4)) != 0)
                            ImGui::LogText("set_theme_item(mvGuiCol_%s, %i, %i, %i, %i)\r\n",
                                name, (int)(round(col.x*255.0f)), (int)(round(col.y * 255.0f)), (int)(round(col.z * 255.0f)), 
                                (int)(round(col.w * 255.0f)));
                    }
                    ImGui::LogFinish();
                }
                ImGui::SameLine(); ImGui::Checkbox("Only Modified Colors", &output_only_modified);

                static ImGuiTextFilter filter;
                filter.Draw("Filter colors", ImGui::GetFontSize() * 16);

                static ImGuiColorEditFlags alpha_flags = 0;
                if (ImGui::RadioButton("Opaque", alpha_flags == ImGuiColorEditFlags_None)) { alpha_flags = ImGuiColorEditFlags_None; } ImGui::SameLine();
                if (ImGui::RadioButton("Alpha", alpha_flags == ImGuiColorEditFlags_AlphaPreview)) { alpha_flags = ImGuiColorEditFlags_AlphaPreview; } ImGui::SameLine();
                if (ImGui::RadioButton("Both", alpha_flags == ImGuiColorEditFlags_AlphaPreviewHalf)) { alpha_flags = ImGuiColorEditFlags_AlphaPreviewHalf; } ImGui::SameLine();
                HelpMarker(
                    "In the color list:\n"
                    "Left-click on colored square to open color picker,\n"
                    "Right-click to open edit options menu.");

                ImGui::BeginChild("##colors", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar | ImGuiWindowFlags_NavFlattened);
                ImGui::PushItemWidth(-160);
                for (int i = 0; i < ImGuiCol_COUNT; i++)
                {
                    const char* name = ImGui::GetStyleColorName(i);
                    if (!filter.PassFilter(name))
                        continue;
                    ImGui::PushID(i);
                    ImGui::ColorEdit4("##color", (float*)&style.Colors[i], ImGuiColorEditFlags_AlphaBar | alpha_flags);
                    if (memcmp(&style.Colors[i], &ref->Colors[i], sizeof(ImVec4)) != 0)
                    {
                        // Tips: in a real user application, you may want to merge and use an icon font into the main font,
                        // so instead of "Save"/"Revert" you'd use icons!
                        // Read the FAQ and docs/FONTS.md about using icon fonts. It's really easy and super convenient!
                        ImGui::SameLine(0.0f, style.ItemInnerSpacing.x); if (ImGui::Button("Save")) { ref->Colors[i] = style.Colors[i]; }
                        ImGui::SameLine(0.0f, style.ItemInnerSpacing.x); if (ImGui::Button("Revert")) { style.Colors[i] = ref->Colors[i]; }
                    }
                    ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
                    ImGui::TextUnformatted(name);
                    ImGui::PopID();
                }
                ImGui::PopItemWidth();
                ImGui::EndChild();

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Fonts"))
            {
                ImGuiIO& io = ImGui::GetIO();
                ImFontAtlas* atlas = io.Fonts;
                HelpMarker("Read FAQ and docs/FONTS.md for details on font loading.");
                ImGui::PushItemWidth(120);
                for (int i = 0; i < atlas->Fonts.Size; i++)
                {
                    ImFont* font = atlas->Fonts[i];
                    ImGui::PushID(font);
                    NodeFont(font);
                    ImGui::PopID();
                }
                if (ImGui::TreeNode("Atlas texture", "Atlas texture (%dx%d pixels)", atlas->TexWidth, atlas->TexHeight))
                {
                    ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
                    ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f);
                    ImGui::Image(atlas->TexID, ImVec2((float)atlas->TexWidth, (float)atlas->TexHeight), ImVec2(0, 0), ImVec2(1, 1), tint_col, border_col);
                    ImGui::TreePop();
                }

                // Post-baking font scaling. Note that this is NOT the nice way of scaling fonts, read below.
                // (we enforce hard clamping manually as by default DragFloat/SliderFloat allows CTRL+Click text to get out of bounds).
                const float MIN_SCALE = 0.3f;
                const float MAX_SCALE = 2.0f;
                HelpMarker(
                    "Those are old settings provided for convenience.\n"
                    "However, the _correct_ way of scaling your UI is currently to reload your font at the designed size, "
                    "rebuild the font atlas, and call style.ScaleAllSizes() on a reference ImGuiStyle structure.\n"
                    "Using those settings here will give you poor quality results.");
                if (ImGui::DragFloat("global scale", &mvApp::GetApp()->getGlobalFontScale(), 0.005f, MIN_SCALE, MAX_SCALE, "%.2f")) // Scale everything
                    mvApp::GetApp()->getGlobalFontScale() = IM_MAX(mvApp::GetApp()->getGlobalFontScale(), MIN_SCALE);
                ImGui::PopItemWidth();

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Rendering"))
            {
                ImGui::Checkbox("Anti-aliased lines", &style.AntiAliasedLines);
                ImGui::SameLine(); HelpMarker("When disabling anti-aliasing lines, you'll probably want to disable borders in your style as well.");
                ImGui::Checkbox("Anti-aliased fill", &style.AntiAliasedFill);
                ImGui::PushItemWidth(100);
                ImGui::DragFloat("Curve Tessellation Tolerance", &style.CurveTessellationTol, 0.02f, 0.10f, 10.0f, "%.2f");
                if (style.CurveTessellationTol < 0.10f) style.CurveTessellationTol = 0.10f;
                ImGui::DragFloat("Circle segment Max Error", &style.CircleSegmentMaxError, 0.01f, 0.10f, 10.0f, "%.2f");
                ImGui::DragFloat("Global Alpha", &style.Alpha, 0.005f, 0.20f, 1.0f, "%.2f"); // Not exposing zero here so user doesn't "lose" the UI (zero alpha clips all widgets). But application code could have a toggle to switch between zero and non-zero.
                ImGui::PopItemWidth();

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();


            if (ImGui::IsWindowFocused())
            {

                float titleBarHeight = ImGui::GetStyle().FramePadding.y * 2 + ImGui::GetFontSize();

                // update mouse
                ImVec2 mousePos = ImGui::GetMousePos();
                float x = mousePos.x - ImGui::GetWindowPos().x;
                float y = mousePos.y - ImGui::GetWindowPos().y - titleBarHeight;
                mvInput::setMousePosition(x, y);

                if (mvApp::GetApp()->getItemRegistry().getActiveWindow() != "style##standard")
                    mvEventBus::Publish(mvEVT_CATEGORY_ITEM, mvEVT_ACTIVE_WINDOW, { CreateEventArgument("WINDOW", std::string("style##standard")) });

            }

            ImGui::End();
        }

    }

    PyObject* add_style_window(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        const char* name;
        int width = 700;
        int height = 500;
        int x_pos = 200;
        int y_pos = 200;
        int autosize = false;
        int no_resize = false;
        int no_title_bar = false;
        int no_move = false;
        int no_scrollbar = false;
        int no_collapse = false;
        int horizontal_scrollbar = false;
        int no_focus_on_appearing = false;
        int no_bring_to_front_on_focus = false;
        int noclose = false;
        int no_background = false;

        const char* label = "";
        int show = true;

        if (!(*mvApp::GetApp()->getParsers())["add_style_window"].parse(args, kwargs, __FUNCTION__, &name, &width,
            &height, &x_pos, &y_pos, &autosize, &no_resize, &no_title_bar, &no_move, &no_scrollbar,
            &no_collapse, &horizontal_scrollbar, &no_focus_on_appearing, &no_bring_to_front_on_focus,
            &noclose, &no_background, &label, &show))
            return ToPyBool(false);

        auto item = CreateRef<mvStyleWindow>(name);

        item->checkConfigDict(kwargs);
        item->setConfigDict(kwargs);
        item->setExtraConfigDict(kwargs);

        if (mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, "", ""))
        {
            mvApp::GetApp()->getItemRegistry().pushParent(item);

            if (!show)
                item->hide();

            return ToPyBool(true);
        }

        return ToPyBool(false);
    }

}