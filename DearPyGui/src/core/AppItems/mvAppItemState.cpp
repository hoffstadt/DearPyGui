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

    }

    void mvAppItemState::getState(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        PyDict_SetItemString(dict, "hovered", ToPyBool(m_hovered));
        PyDict_SetItemString(dict, "active", ToPyBool(m_active));
        PyDict_SetItemString(dict, "focused", ToPyBool(m_focused));
        PyDict_SetItemString(dict, "clicked", ToPyBool(m_clicked));
        PyDict_SetItemString(dict, "visible", ToPyBool(m_visible));
        PyDict_SetItemString(dict, "edited", ToPyBool(m_edited));
        PyDict_SetItemString(dict, "activated", ToPyBool(m_activated));
        PyDict_SetItemString(dict, "deactivated", ToPyBool(m_deactivated));
        PyDict_SetItemString(dict, "deactivated_after_edit", ToPyBool(m_deactivatedAfterEdit));
        PyDict_SetItemString(dict, "toggled_open", ToPyBool(m_toggledOpen));
        PyDict_SetItemString(dict, "ok", ToPyBool(m_ok));

        PyDict_SetItemString(dict, "rect_min", ToPyMPair(m_rectMin.x, m_rectMin.y));
        PyDict_SetItemString(dict, "rect_max", ToPyMPair(m_rectMax.x, m_rectMax.y));
        PyDict_SetItemString(dict, "rect_size", ToPyMPair(m_rectSize.x, m_rectSize.y));
        PyDict_SetItemString(dict, "pos", ToPyMPair(m_pos.x, m_pos.y));
    }

}