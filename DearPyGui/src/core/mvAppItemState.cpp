#include "mvAppItemState.h"
#include <imgui.h>
#include "AppItems/mvAppItem.h"

namespace Marvel {

    void mvAppItemState::reset()
    {
        m_hovered = false;
        m_active = false;
        m_focused = false;
        m_clicked = false;
        m_visible = false;
        m_edited = false;
        m_activated = false;
        m_deactivated = false;
        m_deactivatedAfterEdit = false;
        m_toggledOpen = false;
    }

    void mvAppItemState::update()
    {
        m_hovered = ImGui::IsItemHovered();
        m_active = ImGui::IsItemActive();
        m_focused = ImGui::IsItemFocused();
        m_clicked = ImGui::IsItemClicked();
        m_visible = ImGui::IsItemVisible();
        m_edited = ImGui::IsItemEdited();
        m_activated = ImGui::IsItemActivated();
        m_deactivated = ImGui::IsItemDeactivated();
        m_deactivatedAfterEdit = ImGui::IsItemDeactivatedAfterEdit();
        m_toggledOpen = ImGui::IsItemToggledOpen();
        m_rectMin = { ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y };
        m_rectMax = { ImGui::GetItemRectMax().x, ImGui::GetItemRectMax().y };
        m_rectSize = { ImGui::GetItemRectSize().x, ImGui::GetItemRectSize().y };

        if (!m_parent->getDescription().container)
        {
                m_parent->m_actualWidth = m_rectSize.x;
                m_parent->m_actualHeight = m_rectSize.y;
        }
    }

}