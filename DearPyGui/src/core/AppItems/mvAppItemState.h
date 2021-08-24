#pragma once

#include "mvCore.h"

#ifndef PyObject_HEAD
struct _object;
typedef _object PyObject;
#endif

namespace Marvel {

    // forward declarations
    class mvAppItem;

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

    class mvAppItemState
    {

    public:

        void getState(PyObject* dict);

        void reset();
        void update();

        // getters
        [[nodiscard]] bool   isItemHovered(int frameDelay = 0) const;
        [[nodiscard]] bool   isItemActive(int frameDelay = 0) const;
        [[nodiscard]] bool   isItemFocused(int frameDelay = 0) const;
        [[nodiscard]] bool   isItemLeftClicked(int frameDelay = 0) const;
        [[nodiscard]] bool   isItemRightClicked(int frameDelay = 0) const;
        [[nodiscard]] bool   isItemMiddleClicked(int frameDelay = 0) const;
        [[nodiscard]] bool   isItemVisible(int frameDelay = 0) const;
        [[nodiscard]] bool   isItemEdited(int frameDelay = 0) const;
        [[nodiscard]] bool   isItemActivated(int frameDelay = 0) const;
        [[nodiscard]] bool   isItemDeactivated(int frameDelay = 0) const;
        [[nodiscard]] bool   isItemDeactivatedAfterEdit(int frameDelay = 0) const;
        [[nodiscard]] bool   isItemToogledOpen(int frameDelay = 0) const;
        [[nodiscard]] bool   isOk() const;
        [[nodiscard]] mvVec2 getItemRectMin () const;
        [[nodiscard]] mvVec2 getItemRectMax () const;
        [[nodiscard]] mvVec2 getItemRectSize () const;
        [[nodiscard]] mvVec2 getItemPos () const;
        [[nodiscard]] mvVec2 getContextRegionAvail () const;

        // setters
        void setHovered             (bool value)  { _hovered = value; }
        void setActive              (bool value)  { _active = value; }
        void setFocused             (bool value)  { _focused = value; }
        void setLeftClicked         (bool value)  { _leftclicked = value; }
        void setRightClicked        (bool value)  { _rightclicked = value; }
        void setMiddleClicked       (bool value)  { _middleclicked = value; }
        void setVisible             (bool value)  { _visible = value; }
        void setEdited              (bool value)  { _edited = value; }
        void setActivated           (bool value)  { _activated = value; }
        void setDeactivated         (bool value)  { _deactivated = value; }
        void setDeactivatedAfterEdit(bool value)  { _deactivatedAfterEdit = value; }
        void setToggledOpen         (bool value)  { _toggledOpen = value; }
        void setRectMin             (mvVec2 value){ _rectMin = value; }
        void setRectMax             (mvVec2 value){ _rectMax = value; }
        void setRectSize            (mvVec2 value){ _rectSize = value; }
        void setPos                 (mvVec2 value){ _pos = value; }
        void setOk                  (bool value)  { _ok = value; };

    public:

        int    _applicableState      = MV_STATE_ALL;
        bool   _hovered              = false;
        bool   _active               = false;
        bool   _focused              = false;
        bool   _leftclicked          = false;
        bool   _rightclicked         = false;
        bool   _middleclicked        = false;
        bool   _visible              = false;
        bool   _edited               = false;
        bool   _activated            = false;
        bool   _deactivated          = false;
        bool   _deactivatedAfterEdit = false;
        bool   _toggledOpen          = false;
        mvVec2 _rectMin              = { 0.0f, 0.0f };
        mvVec2 _rectMax              = { 0.0f, 0.0f };
        mvVec2 _rectSize             = { 0.0f, 0.0f };
        mvVec2 _pos                  = { 0.0f, 0.0f };
        mvVec2 _contextRegionAvail   = { 0.0f, 0.0f };
        bool   _ok                   = true;
        int    _lastFrameUpdate      = 0; // last frame update occured

    };
}