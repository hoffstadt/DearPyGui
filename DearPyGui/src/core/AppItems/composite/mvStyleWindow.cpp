#include <imgui.h>
#include "mvApp.h"
#include "mvAppItems.h"
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
        parsers->insert({ "add_style_window", mvPythonParser({
            {mvPythonDataType::Optional},
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

    void mvStyleWindow::draw()
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

                for (auto& item : mvThemeManager::GetColorsPtr())
                {
                    if (!filter3.PassFilter(std::get<0>(item).c_str()))
                        continue;

                    ImGui::PushID(&item);
                    if(ImGui::ColorEdit4("##color(disable)", *std::get<3>(item), ImGuiColorEditFlags_AlphaBar))
                        mvThemeManager::InValidateColorTheme();
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

    PyObject* mvStyleWindow::add_style_window(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        static int i = 0; i++;
        std::string sname = std::string(std::string("$$DPG_") + s_internal_id + std::to_string(i));
        const char* name = sname.c_str();
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

        if (!(mvApp::GetApp()->getParsers())["add_style_window"].parse(args, kwargs, __FUNCTION__, &name, &width,
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

        }

        return ToPyString(name);
    }

}