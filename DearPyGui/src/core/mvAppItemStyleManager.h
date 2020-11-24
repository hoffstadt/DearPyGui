#pragma once

#include <vector>
#include <imgui.h>
#include "mvCore.h"
#include <unordered_map>


namespace Marvel {

    //-----------------------------------------------------------------------------
    // Forward Declarations
    //-----------------------------------------------------------------------------
    class mvAppItemStyleManager;
    class mvAppItem;

    //-----------------------------------------------------------------------------
    // mvAppItemStyleManagerScope: Automates popping styles
    //-----------------------------------------------------------------------------
    class mvAppItemStyleManagerScope
    {

    public:

        explicit mvAppItemStyleManagerScope(mvAppItemStyleManager& manager);
        mvAppItemStyleManagerScope(mvAppItemStyleManagerScope&& other) noexcept;

        mvAppItemStyleManagerScope(const mvAppItemStyleManagerScope& other) = delete;
        mvAppItemStyleManagerScope operator=(const mvAppItemStyleManagerScope& other) = delete;
        mvAppItemStyleManagerScope& operator=(mvAppItemStyleManagerScope&& other) = delete;

        ~mvAppItemStyleManagerScope();

        void addColorStyle(ImGuiCol item, ImVec4 color);

    private:

        mvAppItemStyleManager& m_manager;
        bool                   m_moved = false;

    };

    //-----------------------------------------------------------------------------
    // mvAppItemStyleManager
    //-----------------------------------------------------------------------------
    class mvAppItemStyleManager
    {
        friend class mvAppItemStyleManagerScope;

        struct StyleColor
        {
            ImGuiCol idx;
            mvColor color;
        };

        struct StyleVar
        {
            ImGuiStyleVar idx;
            std::vector<float> value;
        };

        std::unordered_map<ImGuiStyleVar, int> style_var_sizes{
        { ImGuiStyleVar_Alpha,               1 },
        { ImGuiStyleVar_WindowPadding,       2 },
        { ImGuiStyleVar_WindowRounding,      1 },
        { ImGuiStyleVar_WindowBorderSize,    1 },
        { ImGuiStyleVar_WindowMinSize,       2 },
        { ImGuiStyleVar_WindowTitleAlign,    2 },
        { ImGuiStyleVar_ChildRounding,       1 },
        { ImGuiStyleVar_ChildBorderSize,     1 },
        { ImGuiStyleVar_PopupRounding,       1 },
        { ImGuiStyleVar_PopupBorderSize,     1 },
        { ImGuiStyleVar_FramePadding,        2 },
        { ImGuiStyleVar_FrameRounding,       1 },
        { ImGuiStyleVar_FrameBorderSize,     1 },
        { ImGuiStyleVar_ItemSpacing,         2 },
        { ImGuiStyleVar_ItemInnerSpacing,    2 },
        { ImGuiStyleVar_IndentSpacing,       1 },
        { ImGuiStyleVar_ScrollbarSize,       1 },
        { ImGuiStyleVar_ScrollbarRounding,   1 },
        { ImGuiStyleVar_GrabMinSize,         1 },
        { ImGuiStyleVar_GrabRounding,        1 },
        { ImGuiStyleVar_TabRounding,         1 },
        { ImGuiStyleVar_ButtonTextAlign,     2 },
        { ImGuiStyleVar_SelectableTextAlign, 2 } };

    public:

        mvAppItemStyleManagerScope getScopedStyleManager();

        mvAppItemStyleManager() {};
        mvAppItemStyleManager(mvAppItem* ptr);

        void addColorStyle        (ImGuiCol item, mvColor color);
        void addTempColorStyle    (ImGuiCol item, mvColor color);
        void pushColorStyles      ();
        void popColorStyles       ();
        void clearColors          ();
        void clearTempColors      ();
        void updateAppItemStyle   ();

        void addStyleVar   (ImGuiStyleVar item, std::vector<float> value);
        void pushStyleVars ();
        void popStyleVars  ();
        void clearStyleVars();

    private:

        mvAppItem*              m_appItem = nullptr;
        std::vector<StyleColor> m_colors;
        std::vector<StyleColor> m_colors_temp; // added for disabled colors
        std::vector<StyleVar>   m_style_vars;

        

    };
}