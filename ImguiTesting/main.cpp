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
#include <iostream>

int main()
{
    auto window = new mvWindowClass();

    window->show();

    window->setup();
    while (window->m_running)
    {
        window->prerender();

#if 0

        ImGui::Begin("Testing");

        // replace with another widget
        ImGui::Button("Test widget");

        bool m_hovered = ImGui::IsItemHovered();
        bool m_active = ImGui::IsItemActive();
        bool m_focused = ImGui::IsItemFocused();
        bool m_clicked = ImGui::IsItemClicked();
        bool m_visible = ImGui::IsItemVisible();
        bool m_edited = ImGui::IsItemEdited();
        bool m_activated = ImGui::IsItemActivated();
        bool m_deactivated = ImGui::IsItemDeactivated();
        bool m_deactivatedAfterEdit = ImGui::IsItemDeactivatedAfterEdit();
        bool m_toggledOpen = ImGui::IsItemToggledOpen();
        ImVec2 m_rectMin = { ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y };
        ImVec2 m_rectMax = { ImGui::GetItemRectMax().x, ImGui::GetItemRectMax().y };
        ImVec2 m_rectSize = { ImGui::GetItemRectSize().x, ImGui::GetItemRectSize().y };
        ImVec2 m_contextRegionAvail = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };

        if (m_hovered) std::cout << "m_hovered" << std::endl;
        if (m_active) std::cout << "m_active" << std::endl;
        if (m_focused) std::cout << "m_focused" << std::endl;
        if (m_clicked) std::cout << "m_clicked" << std::endl;
        //if (m_visible) std::cout << "m_visible" << std::endl;
        if (m_edited) std::cout << "m_edited" << std::endl;
        if (m_activated) std::cout << "m_activated" << std::endl;
        if (m_deactivated) std::cout << "m_deactivated" << std::endl;
        if (m_deactivatedAfterEdit) std::cout << "m_deactivatedAfterEdit" << std::endl;
        if (m_toggledOpen) std::cout << "m_toggledOpen" << std::endl;

        ImGui::Button("Blah2");

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