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
        [[nodiscard]] bool   isItemHovered             () const { return m_hovered; }
        [[nodiscard]] bool   isItemActive              () const { return m_active; }
        [[nodiscard]] bool   isItemFocused             () const { return m_focused; }
        [[nodiscard]] bool   isItemClicked             () const { return m_clicked; }
        [[nodiscard]] bool   isItemVisible             () const { return m_visible; }
        [[nodiscard]] bool   isItemEdited              () const { return m_edited; }
        [[nodiscard]] bool   isItemActivated           () const { return m_activated; }
        [[nodiscard]] bool   isItemDeactivated         () const { return m_deactivated; }
        [[nodiscard]] bool   isItemDeactivatedAfterEdit() const { return m_deactivatedAfterEdit; }
        [[nodiscard]] bool   isItemToogledOpen         () const { return m_toggledOpen; }
        [[nodiscard]] bool   isOk                      () const { return m_ok; }
        [[nodiscard]] mvVec2 getItemRectMin            () const { return m_rectMin; }
        [[nodiscard]] mvVec2 getItemRectMax            () const { return m_rectMax; }
        [[nodiscard]] mvVec2 getItemRectSize           () const { return m_rectSize; }
        [[nodiscard]] mvVec2 getItemPos                () const { return m_pos; }
        [[nodiscard]] mvVec2 getContextRegionAvail     () const { return m_contextRegionAvail; }

        // setters
        void setHovered             (bool value)  { m_hovered = value; }
        void setActive              (bool value)  { m_active = value; }
        void setFocused             (bool value)  { m_focused = value; }
        void setClicked             (bool value)  { m_clicked = value; }
        void setVisible             (bool value)  { m_visible = value; }
        void setEdited              (bool value)  { m_edited = value; }
        void setActivated           (bool value)  { m_activated = value; }
        void setDeactivated         (bool value)  { m_deactivated = value; }
        void setDeactivatedAfterEdit(bool value)  { m_deactivatedAfterEdit = value; }
        void setToggledOpen         (bool value)  { m_toggledOpen = value; }
        void setRectMin             (mvVec2 value){ m_rectMin = value; }
        void setRectMax             (mvVec2 value){ m_rectMax = value; }
        void setRectSize            (mvVec2 value){ m_rectSize = value; }
        void setPos                 (mvVec2 value){ m_pos = value; }
        void setOk                  (bool value)  { m_ok = value; };

    public:

        int    m_applicableState      = MV_STATE_ALL;
        bool   m_hovered              = false;
        bool   m_active               = false;
        bool   m_focused              = false;
        bool   m_clicked              = false;
        bool   m_visible              = false;
        bool   m_edited               = false;
        bool   m_activated            = false;
        bool   m_deactivated          = false;
        bool   m_deactivatedAfterEdit = false;
        bool   m_toggledOpen          = false;
        mvVec2 m_rectMin              = { 0.0f, 0.0f };
        mvVec2 m_rectMax              = { 0.0f, 0.0f };
        mvVec2 m_rectSize             = { 0.0f, 0.0f };
        mvVec2 m_pos                  = { 0.0f, 0.0f };
        mvVec2 m_contextRegionAvail   = { 0.0f, 0.0f };
        bool   m_ok                   = true;

    };
}