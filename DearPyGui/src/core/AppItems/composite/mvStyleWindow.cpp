#include <imgui.h>
#include "mvApp.h"
#include "mvAppItemCommons.h"
#include "mvStyleWindow.h"
#include "mvInput.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

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

    void mvStyleWindow::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        mvPythonParser parser(mvPyDataType::String);
        mvAppItem::AddCommonArgs(parser);
        parser.removeArg("parent");
        parser.removeArg("before");
        parser.removeArg("source");
        parser.removeArg("callback");
        parser.removeArg("callback_data");
        parser.removeArg("enabled");

        parser.addArg<mvPyDataType::Integer>("x_pos", mvArgType::KEYWORD_ARG, "200");
        parser.addArg<mvPyDataType::Integer>("y_pos", mvArgType::KEYWORD_ARG, "200");

        parser.addArg<mvPyDataType::Bool>("autosize", mvArgType::KEYWORD_ARG, "False", "Autosized the window to fit it's items.");
        parser.addArg<mvPyDataType::Bool>("no_resize", mvArgType::KEYWORD_ARG, "False", "Allows for the window size to be changed or fixed");
        parser.addArg<mvPyDataType::Bool>("no_title_bar", mvArgType::KEYWORD_ARG, "False", "Title name for the title bar of the window");
        parser.addArg<mvPyDataType::Bool>("no_move", mvArgType::KEYWORD_ARG, "False", "Allows for the window's position to be changed or fixed");
        parser.addArg<mvPyDataType::Bool>("no_scrollbar", mvArgType::KEYWORD_ARG, "False", " Disable scrollbars (window can still scroll with mouse or programmatically)");
        parser.addArg<mvPyDataType::Bool>("no_collapse", mvArgType::KEYWORD_ARG, "False", "Disable user collapsing window by double-clicking on it");
        parser.addArg<mvPyDataType::Bool>("horizontal_scrollbar", mvArgType::KEYWORD_ARG, "False", "Allow horizontal scrollbar to appear (off by default).");
        parser.addArg<mvPyDataType::Bool>("no_focus_on_appearing", mvArgType::KEYWORD_ARG, "False", "Disable taking focus when transitioning from hidden to visible state");
        parser.addArg<mvPyDataType::Bool>("no_bring_to_front_on_focus", mvArgType::KEYWORD_ARG, "False", "Disable bringing window to front when taking focus (e.g. clicking on it or programmatically giving it focus)");
        parser.addArg<mvPyDataType::Bool>("no_close", mvArgType::KEYWORD_ARG, "False");
        parser.addArg<mvPyDataType::Bool>("no_background", mvArgType::KEYWORD_ARG, "False");

        parser.finalize();

        parsers->insert({ s_command, parser });
    }

    void mvStyleWindow::draw(ImDrawList* drawlist, float x, float y)
    {

        mvFontScope fscope(this);

        if (!prerender())
            return;

        ImGui::BeginChild("##colors", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar | ImGuiWindowFlags_NavFlattened);
        ImGui::PushItemWidth(-350);

        if(ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
        {

            if (ImGui::BeginTabItem("Styles"))
            {


                HelpMarker(
                    "Export saves commands to your clipboard. Paste into your file.");
                ImGui::SameLine();
                if (ImGui::Button("Export Styles"))
                {

                    ImGui::LogToClipboard();

                    for (auto item : mvThemeManager::GetStylesPtr())
                    {
                        // Uncomment and replace with new command

                        //ImGui::LogText("set_theme_item(mvGuiCol_%s, %i, %i, %i, %i)\r\n",
                        //    name, (int)(round(col.x * 255.0f)), (int)(round(col.y * 255.0f)), (int)(round(col.z * 255.0f)),
                        //    (int)(round(col.w * 255.0f)));
                    }
                    ImGui::LogFinish();
                }

                static ImGuiTextFilter filter1;
                filter1.Draw("Filter Styles", ImGui::GetFontSize() * 16);

                ImGui::Separator();

                for (auto& item : mvThemeManager::GetStylesPtr())
                {
                    if (!filter1.PassFilter(std::get<0>(item).c_str()))
                        continue;

                    ImGui::PushID(&item);
                    if(ImGui::SliderFloat("##style", std::get<2>(item), 0.0f, std::get<3>(item)))
                        mvThemeManager::InValidateStyleTheme();
                    ImGui::SameLine();
                    ImGui::TextUnformatted(std::get<0>(item).c_str());
                    ImGui::PopID();
                }

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Colors"))
            {

                HelpMarker(
                    "Export saves commands to your clipboard. Paste into your file.");
                ImGui::SameLine();
                if (ImGui::Button("Export Colors"))
                {

                    ImGui::LogToClipboard();

                    for (auto item : mvThemeManager::GetColorsPtr())
                    {
                        // Uncomment and replace with new command

                        //ImGui::LogText("set_theme_item(mvGuiCol_%s, %i, %i, %i, %i)\r\n",
                        //    name, (int)(round(col.x * 255.0f)), (int)(round(col.y * 255.0f)), (int)(round(col.z * 255.0f)),
                        //    (int)(round(col.w * 255.0f)));
                    }
                    ImGui::LogFinish();
                }

                static ImGuiTextFilter filter2;
                filter2.Draw("Filter Colors", ImGui::GetFontSize() * 16);

                ImGui::Separator();

                for (auto& item : mvThemeManager::GetColorsPtr())
                {
                    if (!filter2.PassFilter(std::get<0>(item).c_str()))
                        continue;

                    ImGui::PushID(&item);
                    if (ImGui::ColorEdit4("##color", *std::get<2>(item), ImGuiColorEditFlags_AlphaBar))
                        mvThemeManager::InValidateColorTheme();
                    ImGui::SameLine();
                    ImGui::TextUnformatted(std::get<0>(item).c_str());
                    ImGui::PopID();
                }

                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Colors (disabled)"))
            {

                HelpMarker(
                    "Export saves commands to your clipboard. Paste into your file.");
                ImGui::SameLine();
                if (ImGui::Button("Export Colors (disabled)"))
                {

                    ImGui::LogToClipboard();

                    for (auto item : mvThemeManager::GetColorsPtr())
                    {
                        // Uncomment and replace with new command

                        //ImGui::LogText("set_theme_item(mvGuiCol_%s, %i, %i, %i, %i)\r\n",
                        //    name, (int)(round(col.x * 255.0f)), (int)(round(col.y * 255.0f)), (int)(round(col.z * 255.0f)),
                        //    (int)(round(col.w * 255.0f)));
                    }
                    ImGui::LogFinish();
                }

                static ImGuiTextFilter filter3;
                filter3.Draw("Filter Colors##(disabled)", ImGui::GetFontSize() * 16);

                ImGui::Separator();

                for (auto& item : mvThemeManager::GetDisabledColorsPtr())
                {
                    if (!filter3.PassFilter(std::get<0>(item).c_str()))
                        continue;

                    ImGui::PushID(&item);
                    if(ImGui::ColorEdit4("##color(disable)", *std::get<2>(item), ImGuiColorEditFlags_AlphaBar))
                        mvThemeManager::InValidateDisabledColorTheme();
                    ImGui::SameLine();
                    ImGui::TextUnformatted(std::get<0>(item).c_str());
                    ImGui::PopID();
                }

                ImGui::EndTabItem();
            }

        }

        ImGui::EndTabBar();

        ImGui::PopItemWidth();
        ImGui::EndChild();

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