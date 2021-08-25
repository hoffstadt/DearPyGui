#include "mvAppItemState.h"
#include <imgui.h>
#include "mvAppItem.h"
#include "mvApp.h"
#include "mvPyObject.h"

namespace Marvel {

    void mvAppItemState::reset()
    {
        _hovered = false;
        _active = false;
        _focused = false;
        _leftclicked = false;
        _rightclicked = false;
        _middleclicked = false;
        _visible = false;
        _edited = false;
        _activated = false;
        _deactivated = false;
        _deactivatedAfterEdit = false;
        _toggledOpen = false;
    }

    void mvAppItemState::update()
    {
        _lastFrameUpdate = mvApp::s_frame;
        _hovered = ImGui::IsItemHovered();
        _active = ImGui::IsItemActive();
        _focused = ImGui::IsItemFocused();
        _leftclicked = ImGui::IsItemClicked();
        _rightclicked = ImGui::IsItemClicked(1);
        _middleclicked = ImGui::IsItemClicked(2);
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

    void mvAppItemState::getState(PyObject* dict, int applicableState)
    {
        if (dict == nullptr)
            return;

        bool valid = _lastFrameUpdate == mvApp::s_frame;

        PyDict_SetItemString(dict, "ok", mvPyObject(ToPyBool(_ok)));
        PyDict_SetItemString(dict, "pos", mvPyObject(ToPyPairII(_pos.x, _pos.y)));

        if(applicableState & MV_STATE_HOVER) PyDict_SetItemString(dict, "hovered", mvPyObject(ToPyBool(valid ? _hovered : false)));
        if(applicableState & MV_STATE_ACTIVE) PyDict_SetItemString(dict, "active", mvPyObject(ToPyBool(valid ? _active : false)));
        if(applicableState & MV_STATE_FOCUSED) PyDict_SetItemString(dict, "focused", mvPyObject(ToPyBool(valid ? _focused : false)));
        if(applicableState & MV_STATE_CLICKED)
        {
            PyDict_SetItemString(dict, "clicked", mvPyObject(ToPyBool(valid ? _leftclicked || _rightclicked || _middleclicked : false)));
            PyDict_SetItemString(dict, "left_clicked", mvPyObject(ToPyBool(valid ? _leftclicked : false)));
            PyDict_SetItemString(dict, "right_clicked", mvPyObject(ToPyBool(valid ? _rightclicked : false)));
            PyDict_SetItemString(dict, "middle_clicked", mvPyObject(ToPyBool(valid ? _middleclicked : false)));
        }
        if(applicableState & MV_STATE_VISIBLE) PyDict_SetItemString(dict, "visible", mvPyObject(ToPyBool(valid ? _visible : false)));
        if(applicableState & MV_STATE_EDITED) PyDict_SetItemString(dict, "edited", mvPyObject(ToPyBool(valid ? _edited : false)));
        if(applicableState & MV_STATE_ACTIVATED) PyDict_SetItemString(dict, "activated", mvPyObject(ToPyBool(valid ? _activated : false)));
        if(applicableState & MV_STATE_DEACTIVATED) PyDict_SetItemString(dict, "deactivated", mvPyObject(ToPyBool(valid ? _deactivated : false)));
        if(applicableState & MV_STATE_DEACTIVATEDAE) PyDict_SetItemString(dict, "deactivated_after_edit", mvPyObject(ToPyBool(valid ? _deactivatedAfterEdit : false)));
        if(applicableState & MV_STATE_TOGGLED_OPEN) PyDict_SetItemString(dict, "toggled_open", mvPyObject(ToPyBool(valid ? _toggledOpen : false)));
        if(applicableState & MV_STATE_RECT_MIN) PyDict_SetItemString(dict, "rect_min", mvPyObject(ToPyPairII(_rectMin.x, _rectMin.y)));
        if(applicableState & MV_STATE_RECT_MAX) PyDict_SetItemString(dict, "rect_max", mvPyObject(ToPyPairII(_rectMax.x, _rectMax.y)));
        if(applicableState & MV_STATE_RECT_SIZE) PyDict_SetItemString(dict, "rect_size", mvPyObject(ToPyPairII(_rectSize.x, _rectSize.y)));
        if(applicableState & MV_STATE_CONT_AVAIL) PyDict_SetItemString(dict, "content_region_avail", mvPyObject(ToPyPairII(_contextRegionAvail.x, _contextRegionAvail.y)));

    }

    bool mvAppItemState::isItemHovered(int frameDelay) const
    { 
        if (_lastFrameUpdate + frameDelay != mvApp::s_frame)
            return false;
        return _hovered; 
    }

    bool mvAppItemState::isItemActive(int frameDelay) const
    { 
        if (_lastFrameUpdate + frameDelay != mvApp::s_frame)
            return false;
        return _active; 
    }

    bool mvAppItemState::isItemFocused(int frameDelay) const
    { 
        if (_lastFrameUpdate + frameDelay != mvApp::s_frame)
            return false;
        return _focused; 
    }

    bool mvAppItemState::isItemLeftClicked(int frameDelay) const
    { 
        if (_lastFrameUpdate + frameDelay != mvApp::s_frame)
            return false;
        return _leftclicked; 
    }

    bool mvAppItemState::isItemRightClicked(int frameDelay) const
    { 
        if (_lastFrameUpdate + frameDelay != mvApp::s_frame)
            return false;
        return _rightclicked; 
    }

    bool mvAppItemState::isItemMiddleClicked(int frameDelay) const
    { 
        if (_lastFrameUpdate + frameDelay != mvApp::s_frame)
            return false;
        return _middleclicked; 
    }

    bool mvAppItemState::isItemVisible(int frameDelay) const
    { 
        if (_lastFrameUpdate + frameDelay != mvApp::s_frame)
            return false;
        return _visible; 
    }

    bool mvAppItemState::isItemEdited(int frameDelay) const
    { 
        if (_lastFrameUpdate + frameDelay != mvApp::s_frame)
            return false;
        return _edited; 
    }

    bool mvAppItemState::isItemActivated(int frameDelay) const
    { 
        if (_lastFrameUpdate + frameDelay != mvApp::s_frame)
            return false;
        return _activated; 
    }

    bool mvAppItemState::isItemDeactivated(int frameDelay) const
    { 
        if (_lastFrameUpdate + frameDelay != mvApp::s_frame)
            return false;
        return _deactivated; 
    }

    bool mvAppItemState::isItemDeactivatedAfterEdit(int frameDelay) const
    { 
        if (_lastFrameUpdate + frameDelay != mvApp::s_frame)
            return false;
        return _deactivatedAfterEdit; 
    }

    bool mvAppItemState::isItemToogledOpen(int frameDelay) const
    { 
        if (_lastFrameUpdate + frameDelay != mvApp::s_frame)
            return false;
        return _toggledOpen; 
    }

    bool mvAppItemState::isOk() const 
    { 
        return _ok; 
    }

    mvVec2 mvAppItemState::getItemRectMin() const 
    { 
        return _rectMin; 
    }

    mvVec2 mvAppItemState::getItemRectMax() const 
    { 
        return _rectMax; 
    }

    mvVec2 mvAppItemState::getItemRectSize() const 
    { 
        return _rectSize; 
    }

    mvVec2 mvAppItemState::getItemPos() const 
    { 
        return _pos; 
    }

    mvVec2 mvAppItemState::getContextRegionAvail() const 
    { 
        return _contextRegionAvail; 
    }

}