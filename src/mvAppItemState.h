#pragma once

#include "mvCore.h"
#include <array>

#ifndef PyObject_HEAD
struct _object;
typedef _object PyObject;
#endif

//-----------------------------------------------------------------------------
// forward declarations
//-----------------------------------------------------------------------------
class mvAppItem;
struct mvAppItemState;

enum mvStateItems
{
    MV_STATE_NONE           = 0,
    MV_STATE_HOVER          = 1 << 1,
    MV_STATE_ACTIVE         = 1 << 2,
    MV_STATE_FOCUSED        = 1 << 3,
    MV_STATE_CLICKED        = 1 << 4,
    MV_STATE_VISIBLE        = 1 << 5,
    MV_STATE_EDITED         = 1 << 6,
    MV_STATE_ACTIVATED      = 1 << 7,
    MV_STATE_DEACTIVATED    = 1 << 8,
    MV_STATE_DEACTIVATEDAE  = 1 << 9,
    MV_STATE_TOGGLED_OPEN   = 1 << 10,
    MV_STATE_RECT_MIN       = 1 << 11,
    MV_STATE_RECT_MAX       = 1 << 12,
    MV_STATE_RECT_SIZE      = 1 << 13,
    MV_STATE_CONT_AVAIL     = 1 << 14,
    MV_STATE_ALL = MV_STATE_HOVER |MV_STATE_ACTIVE |MV_STATE_FOCUSED |MV_STATE_CLICKED |MV_STATE_VISIBLE |MV_STATE_EDITED |MV_STATE_ACTIVATED |MV_STATE_DEACTIVATED |MV_STATE_DEACTIVATEDAE |
    MV_STATE_TOGGLED_OPEN | MV_STATE_RECT_MIN |MV_STATE_RECT_MAX |MV_STATE_RECT_SIZE |MV_STATE_CONT_AVAIL
};

//-----------------------------------------------------------------------------
// public API
//-----------------------------------------------------------------------------

void FillAppItemState  (PyObject* dict, mvAppItemState& state, i32 applicableState); // fills python dict with applicable state values
void ResetAppItemState (mvAppItemState& state);                                      // reset values to false
void UpdateAppItemState(mvAppItemState& state);                                      // standard imgui update

// return actual value if frame is active
b8 IsItemHovered             (mvAppItemState& state, i32 frameDelay = 0);
b8 IsItemActive              (mvAppItemState& state, i32 frameDelay = 0);
b8 IsItemFocused             (mvAppItemState& state, i32 frameDelay = 0);
b8 IsItemLeftClicked         (mvAppItemState& state, i32 frameDelay = 0);
b8 IsItemRightClicked        (mvAppItemState& state, i32 frameDelay = 0);
b8 IsItemMiddleClicked       (mvAppItemState& state, i32 frameDelay = 0);
b8 IsItemVisible             (mvAppItemState& state, i32 frameDelay = 0);
b8 IsItemEdited              (mvAppItemState& state, i32 frameDelay = 0);
b8 IsItemActivated           (mvAppItemState& state, i32 frameDelay = 0);
b8 IsItemDeactivated         (mvAppItemState& state, i32 frameDelay = 0);
b8 IsItemDeactivatedAfterEdit(mvAppItemState& state, i32 frameDelay = 0);
b8 IsItemToogledOpen         (mvAppItemState& state, i32 frameDelay = 0);
b8 IsItemRectSizeResized     (mvAppItemState& state, i32 frameDelay = 0);

inline b8 IsItemDoubleClicked(ImGuiMouseButton mouse_button)
{
    return ImGui::GetMouseClickedCount(mouse_button) == 2 && ImGui::IsItemHovered(ImGuiHoveredFlags_None);
}

struct mvAppItemState
{
    b8         hovered              = false;
    b8         active               = false;
    b8         focused              = false;
    b8         leftclicked          = false;
    b8         rightclicked         = false;
    b8         middleclicked        = false;
    std::array<b8, 5> doubleclicked = {};
    b8         visible              = false;
    b8         edited               = false;
    b8         activated            = false;
    b8         deactivated          = false;
    b8         deactivatedAfterEdit = false;
    b8         toggledOpen          = false;
    b8         mvRectSizeResized    = false;
    mvVec2     rectMin              = { 0.0f, 0.0f };
    mvVec2     rectMax              = { 0.0f, 0.0f };
    mvVec2     rectSize             = { 0.0f, 0.0f };
    mvVec2     mvPrevRectSize       = { 0.0f, 0.0f };
    mvVec2     pos                  = { 0.0f, 0.0f };
    mvVec2     contextRegionAvail   = { 0.0f, 0.0f };
    b8         ok                   = true;
    i32        lastFrameUpdate      = 0; // last frame update occured
    mvAppItem* parent               = nullptr; // hacky, but quick fix for widget handlers
};
