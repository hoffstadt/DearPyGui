#pragma once

//-----------------------------------------------------------------------------
// mvAppItem
//
//     - mvAppItem is the abstract base class for all DearPygui widgets.
//     
//-----------------------------------------------------------------------------

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <string>
#include <vector>
#include <map>
#include <imgui.h>
#include "mvCore.h"

namespace Marvel {

    class mvAppItemState
    {

    public:

        void reset()
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

        void update()
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
        [[nodiscard]] mvVec2 getItemRectMin            () const { return m_rectMin; }
        [[nodiscard]] mvVec2 getItemRectMax            () const { return m_rectMax; }
        [[nodiscard]] mvVec2 getItemRectSize           () const { return m_rectSize; }

        // setters
        inline void          setHovered             (bool value) { m_hovered = value; }
        inline void          setActive              (bool value) { m_active = value; }
        inline void          setFocused             (bool value) { m_focused = value; }
        inline void          setClicked             (bool value) { m_clicked = value; }
        inline void          setVisible             (bool value) { m_visible = value; }
        inline void          setEdited              (bool value) { m_edited = value; }
        inline void          setActivated           (bool value) { m_activated = value; }
        inline void          setDeactivated         (bool value) { m_deactivated = value; }
        inline void          setDeactivatedAfterEdit(bool value) { m_deactivatedAfterEdit = value; }
        inline void          setToggledOpen         (bool value) { m_toggledOpen = value; }
        inline void          setRectMin             (mvVec2 value) { m_rectMin = value; }
        inline void          setRectMax             (mvVec2 value) { m_rectMax = value; }
        inline void          setRectSize            (mvVec2 value) { m_rectSize = value; }

    private:

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
    };
}