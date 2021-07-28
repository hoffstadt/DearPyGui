#include "mvAppItemState.h"
#include <imgui.h>
#include "mvAppItem.h"

namespace Marvel {

    void mvAppItemState::reset()
    {
        _hovered = false;
        _active = false;
        _focused = false;
        _clicked = false;
        _visible = false;
        _edited = false;
        _activated = false;
        _deactivated = false;
        _deactivatedAfterEdit = false;
        _toggledOpen = false;
    }

    void mvAppItemState::update()
    {
        _hovered = ImGui::IsItemHovered();
        _active = ImGui::IsItemActive();
        _focused = ImGui::IsItemFocused();
        _clicked = ImGui::IsItemClicked();
        _visible = ImGui::IsItemVisible();
        _edited = ImGui::IsItemEdited();
        _activated = ImGui::IsItemActivated();
        _deactivated = ImGui::IsItemDeactivated();
        _deactivatedAfterEdit = ImGui::IsItemDeactivatedAfterEdit();
        _toggledOpen = ImGui::IsItemToggledOpen();
        _rectMin = { ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y };
        _rectMax = { ImGui::GetItemRectMax().x, ImGui::GetItemRectMax().y };
        _rectSize = { ImGui::GetItemRectSize().x, ImGui::GetItemRectSize().y };
        _contextRegionAvail = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };
    }

    void mvAppItemState::getState(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        PyDict_SetItemString(dict, "ok", ToPyBool(_ok));
        PyDict_SetItemString(dict, "pos", ToPyPairII(_pos.x, _pos.y));

        if(_applicableState & MV_STATE_HOVER) PyDict_SetItemString(dict, "hovered", ToPyBool(_hovered));
        if(_applicableState & MV_STATE_ACTIVE) PyDict_SetItemString(dict, "active", ToPyBool(_active));
        if(_applicableState & MV_STATE_FOCUSED) PyDict_SetItemString(dict, "focused", ToPyBool(_focused));
        if(_applicableState & MV_STATE_CLICKED) PyDict_SetItemString(dict, "clicked", ToPyBool(_clicked));
        if(_applicableState & MV_STATE_VISIBLE) PyDict_SetItemString(dict, "visible", ToPyBool(_visible));
        if(_applicableState & MV_STATE_EDITED) PyDict_SetItemString(dict, "edited", ToPyBool(_edited));
        if(_applicableState & MV_STATE_ACTIVATED) PyDict_SetItemString(dict, "activated", ToPyBool(_activated));
        if(_applicableState & MV_STATE_DEACTIVATED) PyDict_SetItemString(dict, "deactivated", ToPyBool(_deactivated));
        if(_applicableState & MV_STATE_DEACTIVATEDAE) PyDict_SetItemString(dict, "deactivated_after_edit", ToPyBool(_deactivatedAfterEdit));
        if(_applicableState & MV_STATE_TOGGLED_OPEN) PyDict_SetItemString(dict, "toggled_open", ToPyBool(_toggledOpen));
        if(_applicableState & MV_STATE_RECT_MIN) PyDict_SetItemString(dict, "rect_min", ToPyPairII(_rectMin.x, _rectMin.y));
        if(_applicableState & MV_STATE_RECT_MAX) PyDict_SetItemString(dict, "rect_max", ToPyPairII(_rectMax.x, _rectMax.y));
        if(_applicableState & MV_STATE_RECT_SIZE) PyDict_SetItemString(dict, "rect_size", ToPyPairII(_rectSize.x, _rectSize.y));
        if(_applicableState & MV_STATE_CONT_AVAIL) PyDict_SetItemString(dict, "content_region_avail", ToPyPairII(_contextRegionAvail.x, _contextRegionAvail.y));

    }

}