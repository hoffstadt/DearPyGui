#pragma once

#include "mvTypeBases.h"
#include "mvApp.h"
#include "mvModule_Core.h"
#include <string>

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvDragFloat
//     * mvDragInt
//
//-----------------------------------------------------------------------------

namespace Marvel {

    //-----------------------------------------------------------------------------
    // mvDragFloat
    //-----------------------------------------------------------------------------
    MV_REGISTER_WIDGET(mvDragFloat, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Float, 1);
    class mvDragFloat : public mvFloatPtrBase
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPITEM_TYPE(mvAppItemType::mvDragFloat, add_drag_float)

        MV_CREATE_CONSTANT(mvThemeCol_DragFloat_Text,         0L, 0L);
        MV_CREATE_CONSTANT(mvThemeCol_DragFloat_Bg,           7L, 0L);
        MV_CREATE_CONSTANT(mvThemeCol_DragFloat_BgHovered,    8L, 0L);
        MV_CREATE_CONSTANT(mvThemeCol_DragFloat_BgActive,     9L, 0L);
        MV_CREATE_CONSTANT(mvThemeCol_DragFloat_Border,       5L, 0L);
        MV_CREATE_CONSTANT(mvThemeCol_DragFloat_BorderShadow, 6L, 0L);

        MV_CREATE_CONSTANT(mvThemeStyle_DragFloat_Rounding,      11L, 0L);
        MV_CREATE_CONSTANT(mvThemeStyle_DragFloat_BorderSize,    12L, 0L);
        MV_CREATE_CONSTANT(mvThemeStyle_DragFloat_PaddingX,      10L, 0L);
        MV_CREATE_CONSTANT(mvThemeStyle_DragFloat_PaddingY,      10L, 1L);
        MV_CREATE_CONSTANT(mvThemeStyle_DragFloat_InnerSpacingX, 14L, 0L);
        MV_CREATE_CONSTANT(mvThemeStyle_DragFloat_InnerSpacingY, 14L, 1L);

        MV_START_EXTRA_COMMANDS
        MV_END_EXTRA_COMMANDS

        MV_START_GENERAL_CONSTANTS
        MV_END_GENERAL_CONSTANTS

        MV_START_COLOR_CONSTANTS
            MV_ADD_CONSTANT(mvThemeCol_DragFloat_Text,         mvColor(255, 255, 255, 255), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_DragFloat_Bg,           mvColor( 41,  74, 122, 138), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_DragFloat_BgHovered,    mvColor( 66, 150, 250, 102), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_DragFloat_BgActive,     mvColor( 66, 150, 250, 171), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_DragFloat_Border,       mvColor(110, 110, 128, 128), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_DragFloat_BorderShadow, mvColor(  0,   0,   0,   0), mvColor(128, 128, 128, 63)),
        MV_END_COLOR_CONSTANTS

        MV_START_STYLE_CONSTANTS
            MV_ADD_CONSTANT(mvThemeStyle_DragFloat_Rounding,      0, 12),
            MV_ADD_CONSTANT(mvThemeStyle_DragFloat_BorderSize,    0,  1),
            MV_ADD_CONSTANT(mvThemeStyle_DragFloat_PaddingX,      4, 20),
            MV_ADD_CONSTANT(mvThemeStyle_DragFloat_PaddingY,      3, 20),
            MV_ADD_CONSTANT(mvThemeStyle_DragFloat_InnerSpacingX, 4, 20),
            MV_ADD_CONSTANT(mvThemeStyle_DragFloat_InnerSpacingY, 4, 20),
        MV_END_STYLE_CONSTANTS

    public:

        mvDragFloat(const std::string& name, float default_value, const std::string& dataSource);

        void setEnabled(bool value) override;

        void draw(ImDrawList* drawlist, float x, float y) override;

        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;

    private:

        float               m_speed = 1.0f;
        float               m_min = 0.0f;
        float               m_max = 100.0f;
        std::string         m_format = "%.3f";
        ImGuiInputTextFlags m_flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags m_stor_flags = ImGuiSliderFlags_None;

    };

    //-----------------------------------------------------------------------------
    // mvDragInt
    //----------------------------------------------------------------------------- 
    MV_REGISTER_WIDGET(mvDragInt, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Int, 1);
    class mvDragInt : public mvIntPtrBase
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPITEM_TYPE(mvAppItemType::mvDragInt, add_drag_int)

        MV_CREATE_CONSTANT(mvThemeCol_DragInt_Text,         0L, 0L);
        MV_CREATE_CONSTANT(mvThemeCol_DragInt_Bg,           7L, 0L);
        MV_CREATE_CONSTANT(mvThemeCol_DragInt_BgHovered,    8L, 0L);
        MV_CREATE_CONSTANT(mvThemeCol_DragInt_BgActive,     9L, 0L);
        MV_CREATE_CONSTANT(mvThemeCol_DragInt_Border,       5L, 0L);
        MV_CREATE_CONSTANT(mvThemeCol_DragInt_BorderShadow, 6L, 0L);

        MV_CREATE_CONSTANT(mvThemeStyle_DragInt_Rounding,      11L, 0L);
        MV_CREATE_CONSTANT(mvThemeStyle_DragInt_BorderSize,    12L, 0L);
        MV_CREATE_CONSTANT(mvThemeStyle_DragInt_PaddingX,      10L, 0L);
        MV_CREATE_CONSTANT(mvThemeStyle_DragInt_PaddingY,      10L, 1L);
        MV_CREATE_CONSTANT(mvThemeStyle_DragInt_InnerSpacingX, 14L, 0L);
        MV_CREATE_CONSTANT(mvThemeStyle_DragInt_InnerSpacingY, 14L, 1L);

        MV_START_EXTRA_COMMANDS
        MV_END_EXTRA_COMMANDS

        MV_START_GENERAL_CONSTANTS
        MV_END_GENERAL_CONSTANTS

        MV_START_COLOR_CONSTANTS
            MV_ADD_CONSTANT(mvThemeCol_DragInt_Text,         mvColor(255, 255, 255, 255), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_DragInt_Bg,           mvColor( 41,  74, 122, 138), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_DragInt_BgHovered,    mvColor( 66, 150, 250, 102), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_DragInt_BgActive,     mvColor( 66, 150, 250, 171), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_DragInt_Border,       mvColor(110, 110, 128, 128), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_DragInt_BorderShadow, mvColor(  0,   0,   0,   0), mvColor(128, 128, 128, 63)),
        MV_END_COLOR_CONSTANTS

        MV_START_STYLE_CONSTANTS
            MV_ADD_CONSTANT(mvThemeStyle_DragInt_Rounding,      0, 12),
            MV_ADD_CONSTANT(mvThemeStyle_DragInt_BorderSize,    0,  1),
            MV_ADD_CONSTANT(mvThemeStyle_DragInt_PaddingX,      4, 20),
            MV_ADD_CONSTANT(mvThemeStyle_DragInt_PaddingY,      3, 20),
            MV_ADD_CONSTANT(mvThemeStyle_DragInt_InnerSpacingX, 4, 20),
            MV_ADD_CONSTANT(mvThemeStyle_DragInt_InnerSpacingY, 4, 20),
        MV_END_STYLE_CONSTANTS

    public:

        mvDragInt(const std::string& name, int default_value, const std::string& dataSource);

        void setEnabled(bool value) override;
        void draw(ImDrawList* drawlist, float x, float y) override;

        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;

    private:

        float               m_speed = 1.0f;
        int                 m_min = 0;
        int                 m_max = 100;
        std::string         m_format = "%d";
        ImGuiInputTextFlags m_flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags m_stor_flags = ImGuiSliderFlags_None;

    };

}