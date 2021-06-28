#include "mvAppItemState.h"
#include <imgui.h>
#include "mvAppItem.h"

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
        m_contextRegionAvail = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };
    }

    void mvAppItemState::getState(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        PyDict_SetItemString(dict, "ok", ToPyBool(m_ok));
        PyDict_SetItemString(dict, "pos", ToPyPairII(m_pos.x, m_pos.y));

        if(m_applicableState & MV_STATE_HOVER) PyDict_SetItemString(dict, "hovered", ToPyBool(m_hovered));
        if(m_applicableState & MV_STATE_ACTIVE) PyDict_SetItemString(dict, "active", ToPyBool(m_active));
        if(m_applicableState & MV_STATE_FOCUSED) PyDict_SetItemString(dict, "focused", ToPyBool(m_focused));
        if(m_applicableState & MV_STATE_CLICKED) PyDict_SetItemString(dict, "clicked", ToPyBool(m_clicked));
        if(m_applicableState & MV_STATE_VISIBLE) PyDict_SetItemString(dict, "visible", ToPyBool(m_visible));
        if(m_applicableState & MV_STATE_EDITED) PyDict_SetItemString(dict, "edited", ToPyBool(m_edited));
        if(m_applicableState & MV_STATE_ACTIVATED) PyDict_SetItemString(dict, "activated", ToPyBool(m_activated));
        if(m_applicableState & MV_STATE_DEACTIVATED) PyDict_SetItemString(dict, "deactivated", ToPyBool(m_deactivated));
        if(m_applicableState & MV_STATE_DEACTIVATEDAE) PyDict_SetItemString(dict, "deactivated_after_edit", ToPyBool(m_deactivatedAfterEdit));
        if(m_applicableState & MV_STATE_TOGGLED_OPEN) PyDict_SetItemString(dict, "toggled_open", ToPyBool(m_toggledOpen));
        if(m_applicableState & MV_STATE_RECT_MIN) PyDict_SetItemString(dict, "rect_min", ToPyPairII(m_rectMin.x, m_rectMin.y));
        if(m_applicableState & MV_STATE_RECT_MAX) PyDict_SetItemString(dict, "rect_max", ToPyPairII(m_rectMax.x, m_rectMax.y));
        if(m_applicableState & MV_STATE_RECT_SIZE) PyDict_SetItemString(dict, "rect_size", ToPyPairII(m_rectSize.x, m_rectSize.y));
        if(m_applicableState & MV_STATE_CONT_AVAIL) PyDict_SetItemString(dict, "content_region_avail", ToPyPairII(m_contextRegionAvail.x, m_contextRegionAvail.y));

    }

}