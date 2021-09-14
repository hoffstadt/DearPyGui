#pragma once

#include "mvCore.h"

#ifndef PyObject_HEAD
struct _object;
typedef _object PyObject;
#endif

namespace Marvel {

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
        MV_STATE_RESIZED        = 1 << 15,
        MV_STATE_ALL = MV_STATE_HOVER |MV_STATE_ACTIVE |MV_STATE_FOCUSED |MV_STATE_CLICKED |MV_STATE_VISIBLE |MV_STATE_EDITED |MV_STATE_ACTIVATED |MV_STATE_DEACTIVATED |MV_STATE_DEACTIVATEDAE |
        MV_STATE_TOGGLED_OPEN | MV_STATE_RECT_MIN |MV_STATE_RECT_MAX |MV_STATE_RECT_SIZE |MV_STATE_CONT_AVAIL
    };

    //-----------------------------------------------------------------------------
    // public API
    //-----------------------------------------------------------------------------

    void FillAppItemState  (PyObject* dict, mvAppItemState& state, int applicableState); // fills python dict with applicable state values
    void ResetAppItemState (mvAppItemState& state);                                      // reset values to false
    void UpdateAppItemState(mvAppItemState& state);                                      // standard imgui update

    // return actual value if frame is active
    bool IsItemHovered             (mvAppItemState& state, int frameDelay = 0);
    bool IsItemActive              (mvAppItemState& state, int frameDelay = 0);
    bool IsItemFocused             (mvAppItemState& state, int frameDelay = 0);
    bool IsItemLeftClicked         (mvAppItemState& state, int frameDelay = 0);
    bool IsItemRightClicked        (mvAppItemState& state, int frameDelay = 0);
    bool IsItemMiddleClicked       (mvAppItemState& state, int frameDelay = 0);
    bool IsItemVisible             (mvAppItemState& state, int frameDelay = 0);
    bool IsItemEdited              (mvAppItemState& state, int frameDelay = 0);
    bool IsItemActivated           (mvAppItemState& state, int frameDelay = 0);
    bool IsItemDeactivated         (mvAppItemState& state, int frameDelay = 0);
    bool IsItemDeactivatedAfterEdit(mvAppItemState& state, int frameDelay = 0);
    bool IsItemToogledOpen         (mvAppItemState& state, int frameDelay = 0);

    struct mvAppItemState
    {
        bool   hovered              = false;
        bool   active               = false;
        bool   focused              = false;
        bool   leftclicked          = false;
        bool   rightclicked         = false;
        bool   middleclicked        = false;
        bool   visible              = false;
        bool   edited               = false;
        bool   activated            = false;
        bool   deactivated          = false;
        bool   deactivatedAfterEdit = false;
        bool   toggledOpen          = false;
        mvVec2 rectMin              = { 0.0f, 0.0f };
        mvVec2 rectMax              = { 0.0f, 0.0f };
        mvVec2 rectSize             = { 0.0f, 0.0f };
        mvVec2 pos                  = { 0.0f, 0.0f };
        mvVec2 contextRegionAvail   = { 0.0f, 0.0f };
        bool   ok                   = true;
        int    lastFrameUpdate      = 0; // last frame update occured
    };

}
