#include "mvStyleWindow.h"
#include "mvApp.h"
#include "mvThemeManager.h"

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

        ImGui::BeginChild("##colors", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar | ImGuiWindowFlags_NavFlattened);
        ImGui::PushItemWidth(-350);

        if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
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
                    if (ImGui::SliderFloat("##style", std::get<2>(item), 0.0f, std::get<3>(item)))
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
                    if (ImGui::ColorEdit4("##color(disable)", *std::get<2>(item), ImGuiColorEditFlags_AlphaBar))
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

    }

}
