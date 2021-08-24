#if defined (_WIN32)
#include "mvWindowsWindow.h"
#define mvWindowClass mvWindowsWindow
#elif defined(__APPLE__)
#include "mvAppleWindow.h"
#define mvWindowClass mvAppleWindow
#else
#include "mvLinuxWindow.h"
#define mvWindowClass mvLinuxWindow
#endif

#include <implot.h>
#include <imnodes.h>
#include <iostream>

int main()
{
    auto window = new mvWindowClass();

    bool hoveredApplicable = false;
    bool activeApplicable = false;
    bool focusedApplicable = false;
    bool clickedApplicable = false;
    bool visibleApplicable = false;
    bool editedApplicable = false;
    bool activatedApplicable = false;
    bool deactivatedApplicable = false;
    bool deactivatedAfterEditApplicable = false;
    bool toggledOpenApplicable = false;

    static char ts[6] = "True";
    static char fs[6] = "    ";

    char var[255] = "Help me";

    window->show();

    window->setup();
    while (window->m_running)
    {
        window->prerender();

#if 1

        ImGui::Begin("Testing");

        // replace with another widget
        ImGui::InputText("Test widget", var, 255);

        if (ImGui::IsItemHovered() && !hoveredApplicable) hoveredApplicable = true;
        if (ImGui::IsItemActive() && !activeApplicable) activeApplicable = true;
        if (ImGui::IsItemFocused() && !focusedApplicable) focusedApplicable = true;
        if (ImGui::IsItemClicked() && !clickedApplicable) clickedApplicable = true;
        if (ImGui::IsItemVisible() && !visibleApplicable) visibleApplicable = true;
        if (ImGui::IsItemEdited() && !editedApplicable) editedApplicable = true;
        if (ImGui::IsItemActivated() && !activatedApplicable) activatedApplicable = true;
        if (ImGui::IsItemDeactivated() && !deactivatedApplicable) deactivatedApplicable = true;
        if (ImGui::IsItemDeactivatedAfterEdit() && !deactivatedAfterEditApplicable) deactivatedAfterEditApplicable = true;
        if (ImGui::IsItemToggledOpen() && !toggledOpenApplicable) toggledOpenApplicable = true;

        ImVec2 m_rectMin = { ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y };
        ImVec2 m_rectMax = { ImGui::GetItemRectMax().x, ImGui::GetItemRectMax().y };
        ImVec2 m_rectSize = { ImGui::GetItemRectSize().x, ImGui::GetItemRectSize().y };
        ImVec2 m_contextRegionAvail = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };

        ImGui::Text("%s: %s", "Item Hovered", hoveredApplicable ? ts : fs);
        ImGui::Text("%s: %s", "Item Active", activeApplicable ? ts : fs);
        ImGui::Text("%s: %s", "Item Focused", focusedApplicable ? ts : fs);
        ImGui::Text("%s: %s", "Item Clicked", clickedApplicable ? ts : fs);
        ImGui::Text("%s: %s", "Item Visible", visibleApplicable ? ts : fs);
        ImGui::Text("%s: %s", "Item Edited", editedApplicable ? ts : fs);
        ImGui::Text("%s: %s", "Item Activated", activatedApplicable ? ts : fs);
        ImGui::Text("%s: %s", "Item Deactivated", deactivatedApplicable ? ts : fs);
        ImGui::Text("%s: %s", "Item DeactivatedAfterEdit", deactivatedAfterEditApplicable ? ts : fs);
        ImGui::Text("%s: %s", "Item ToggledOpen", toggledOpenApplicable ? ts : fs);

        ImGui::End();
#endif

#if defined (_WIN32)
        ImGui::ShowDemoWindow();
        ImPlot::ShowDemoWindow();
        window->postrender();
#elif defined(__APPLE__)
#else
        ImGui::ShowDemoWindow();
        ImPlot::ShowDemoWindow();
        window->postrender();
#endif


    }

}