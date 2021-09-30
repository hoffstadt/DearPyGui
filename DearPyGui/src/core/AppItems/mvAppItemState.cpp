#include "mvAppItemState.h"
#include <imgui.h>
#include "mvAppItem.h"
#include "mvContext.h"
#include "mvPyObject.h"

namespace Marvel {

    void 
    ResetAppItemState(mvAppItemState& state)
    {
        state.hovered = false;
        state.active = false;
        state.focused = false;
        state.leftclicked = false;
        state.rightclicked = false;
        state.middleclicked = false;
        state.visible = false;
        state.edited = false;
        state.activated = false;
        state.deactivated = false;
        state.deactivatedAfterEdit = false;
        state.toggledOpen = false;
    }

    void
    UpdateAppItemState(mvAppItemState& state)
    {
        state.lastFrameUpdate = GContext->frame;
        state.hovered = ImGui::IsItemHovered();
        state.active = ImGui::IsItemActive();
        state.focused = ImGui::IsItemFocused();
        state.leftclicked = ImGui::IsItemClicked();
        state.rightclicked = ImGui::IsItemClicked(1);
        state.middleclicked = ImGui::IsItemClicked(2);
        state.visible = ImGui::IsItemVisible();
        state.edited = ImGui::IsItemEdited();
        state.activated = ImGui::IsItemActivated();
        state.deactivated = ImGui::IsItemDeactivated();
        state.deactivatedAfterEdit = ImGui::IsItemDeactivatedAfterEdit();
        state.toggledOpen = ImGui::IsItemToggledOpen();
        state.rectMin = { ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y };
        state.rectMax = { ImGui::GetItemRectMax().x, ImGui::GetItemRectMax().y };
        state.rectSize = { ImGui::GetItemRectSize().x, ImGui::GetItemRectSize().y };
        state.contextRegionAvail = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };
    }

    void 
    FillAppItemState(PyObject* dict, mvAppItemState& state, int applicableState)
    {
        if (dict == nullptr)
            return;

        bool valid = state.lastFrameUpdate == GContext->frame;

        PyDict_SetItemString(dict, "ok", mvPyObject(ToPyBool(state.ok)));
        PyDict_SetItemString(dict, "pos", mvPyObject(ToPyPairII((int)state.pos.x, (int)state.pos.y)));

        if(applicableState & MV_STATE_HOVER) PyDict_SetItemString(dict, "hovered", mvPyObject(ToPyBool(valid ? state.hovered : false)));
        if(applicableState & MV_STATE_ACTIVE) PyDict_SetItemString(dict, "active", mvPyObject(ToPyBool(valid ? state.active : false)));
        if(applicableState & MV_STATE_FOCUSED) PyDict_SetItemString(dict, "focused", mvPyObject(ToPyBool(valid ? state.focused : false)));
        if(applicableState & MV_STATE_CLICKED)
        {
            PyDict_SetItemString(dict, "clicked", mvPyObject(ToPyBool(valid ? state.leftclicked || state.rightclicked || state.middleclicked : false)));
            PyDict_SetItemString(dict, "left_clicked", mvPyObject(ToPyBool(valid ? state.leftclicked : false)));
            PyDict_SetItemString(dict, "right_clicked", mvPyObject(ToPyBool(valid ? state.rightclicked : false)));
            PyDict_SetItemString(dict, "middle_clicked", mvPyObject(ToPyBool(valid ? state.middleclicked : false)));
        }
        if(applicableState & MV_STATE_VISIBLE) PyDict_SetItemString(dict, "visible", mvPyObject(ToPyBool(valid ? state.visible : false)));
        if(applicableState & MV_STATE_EDITED) PyDict_SetItemString(dict, "edited", mvPyObject(ToPyBool(valid ? state.edited : false)));
        if(applicableState & MV_STATE_ACTIVATED) PyDict_SetItemString(dict, "activated", mvPyObject(ToPyBool(valid ? state.activated : false)));
        if(applicableState & MV_STATE_DEACTIVATED) PyDict_SetItemString(dict, "deactivated", mvPyObject(ToPyBool(valid ? state.deactivated : false)));
        if(applicableState & MV_STATE_DEACTIVATEDAE) PyDict_SetItemString(dict, "deactivated_after_edit", mvPyObject(ToPyBool(valid ? state.deactivatedAfterEdit : false)));
        if(applicableState & MV_STATE_TOGGLED_OPEN) PyDict_SetItemString(dict, "toggled_open", mvPyObject(ToPyBool(valid ? state.toggledOpen : false)));
        if(applicableState & MV_STATE_RECT_MIN) PyDict_SetItemString(dict, "rect_min", mvPyObject(ToPyPairII((int)state.rectMin.x, (int)state.rectMin.y)));
        if(applicableState & MV_STATE_RECT_MAX) PyDict_SetItemString(dict, "rect_max", mvPyObject(ToPyPairII((int)state.rectMax.x, (int)state.rectMax.y)));
        if(applicableState & MV_STATE_RECT_SIZE) PyDict_SetItemString(dict, "rect_size", mvPyObject(ToPyPairII((int)state.rectSize.x, (int)state.rectSize.y)));
        if(applicableState & MV_STATE_CONT_AVAIL) PyDict_SetItemString(dict, "content_region_avail", mvPyObject(ToPyPairII((int)state.contextRegionAvail.x, (int)state.contextRegionAvail.y)));

    }

    bool 
    IsItemHovered(mvAppItemState& state, int frameDelay)
    { 
        if (state.lastFrameUpdate + frameDelay != GContext->frame)
            return false;
        return state.hovered;
    }

    bool 
    IsItemActive(mvAppItemState& state, int frameDelay)
    { 
        if (state.lastFrameUpdate + frameDelay != GContext->frame)
            return false;
        return state.active;
    }

    bool 
    IsItemFocused(mvAppItemState& state, int frameDelay)
    { 
        if (state.lastFrameUpdate + frameDelay != GContext->frame)
            return false;
        return state.focused;
    }

    bool 
    IsItemLeftClicked(mvAppItemState& state, int frameDelay)
    { 
        if (state.lastFrameUpdate + frameDelay != GContext->frame)
            return false;
        return state.leftclicked;
    }

    bool 
    IsItemRightClicked(mvAppItemState& state, int frameDelay)
    { 
        if (state.lastFrameUpdate + frameDelay != GContext->frame)
            return false;
        return state.rightclicked;
    }

    bool 
    IsItemMiddleClicked(mvAppItemState& state, int frameDelay)
    { 
        if (state.lastFrameUpdate + frameDelay != GContext->frame)
            return false;
        return state.middleclicked;
    }

    bool 
    IsItemVisible(mvAppItemState& state, int frameDelay)
    { 
        if (state.lastFrameUpdate + frameDelay != GContext->frame)
            return false;
        return state.visible;
    }

    bool 
    IsItemEdited(mvAppItemState& state, int frameDelay)
    { 
        if (state.lastFrameUpdate + frameDelay != GContext->frame)
            return false;
        return state.edited;
    }

    bool 
    IsItemActivated(mvAppItemState& state, int frameDelay)
    { 
        if (state.lastFrameUpdate + frameDelay != GContext->frame)
            return false;
        return state.activated;
    }

    bool 
    IsItemDeactivated(mvAppItemState& state, int frameDelay)
    { 
        if (state.lastFrameUpdate + frameDelay != GContext->frame)
            return false;
        return state.deactivated;
    }

    bool 
    IsItemDeactivatedAfterEdit(mvAppItemState& state, int frameDelay)
    { 
        if (state.lastFrameUpdate + frameDelay != GContext->frame)
            return false;
        return state.deactivatedAfterEdit;
    }

    bool 
    IsItemToogledOpen(mvAppItemState& state, int frameDelay)
    { 
        if (state.lastFrameUpdate + frameDelay != GContext->frame)
            return false;
        return state.toggledOpen;
    }
}
