#pragma once

#include "mvTypeBases.h"
#include "mvApp.h"
#include "mvModule_Core.h"
#include <string>

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvDragFloatMulti
//     * mvDragIntMulti
//
//-----------------------------------------------------------------------------

namespace Marvel {

    //-----------------------------------------------------------------------------
    // mvDragFloatMulti
    //-----------------------------------------------------------------------------
    MV_REGISTER_WIDGET(mvDragFloatMulti, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Float4, 1);
    class mvDragFloatMulti : public mvFloat4PtrBase
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPITEM_TYPE(mvAppItemType::mvDragFloatMulti, add_drag_floatx)

        MV_CREATE_CONSTANT(mvThemeCol_DragFloatx_Text,         0L, 0L);
        MV_CREATE_CONSTANT(mvThemeCol_DragFloatx_Bg,           7L, 0L);
        MV_CREATE_CONSTANT(mvThemeCol_DragFloatx_BgHovered,    8L, 0L);
        MV_CREATE_CONSTANT(mvThemeCol_DragFloatx_BgActive,     9L, 0L);
        MV_CREATE_CONSTANT(mvThemeCol_DragFloatx_Border,       5L, 0L);
        MV_CREATE_CONSTANT(mvThemeCol_DragFloatx_BorderShadow, 6L, 0L);

        MV_CREATE_CONSTANT(mvThemeStyle_DragFloatx_Rounding,      11L, 0L);
        MV_CREATE_CONSTANT(mvThemeStyle_DragFloatx_BorderSize,    12L, 0L);
        MV_CREATE_CONSTANT(mvThemeStyle_DragFloatx_PaddingX,      10L, 0L);
        MV_CREATE_CONSTANT(mvThemeStyle_DragFloatx_PaddingY,      10L, 1L);
        MV_CREATE_CONSTANT(mvThemeStyle_DragFloatx_InnerSpacingX, 14L, 0L);
        MV_CREATE_CONSTANT(mvThemeStyle_DragFloatx_InnerSpacingY, 14L, 1L);

        MV_START_EXTRA_COMMANDS
        MV_END_EXTRA_COMMANDS

        MV_START_GENERAL_CONSTANTS
        MV_END_GENERAL_CONSTANTS

        MV_START_COLOR_CONSTANTS
        MV_ADD_CONSTANT(mvThemeCol_DragFloatx_Text,         mvColor(255, 255, 255, 255), mvColor(128, 128, 128, 63)),
        MV_ADD_CONSTANT(mvThemeCol_DragFloatx_Bg,           mvColor( 41,  74, 122, 138), mvColor(128, 128, 128, 63)),
        MV_ADD_CONSTANT(mvThemeCol_DragFloatx_BgHovered,    mvColor( 66, 150, 250, 102), mvColor(128, 128, 128, 63)),
        MV_ADD_CONSTANT(mvThemeCol_DragFloatx_BgActive,     mvColor( 66, 150, 250, 171), mvColor(128, 128, 128, 63)),
        MV_ADD_CONSTANT(mvThemeCol_DragFloatx_Border,       mvColor(110, 110, 128, 128), mvColor(128, 128, 128, 63)),
        MV_ADD_CONSTANT(mvThemeCol_DragFloatx_BorderShadow, mvColor(  0,   0,   0,   0), mvColor(128, 128, 128, 63)),
        MV_END_COLOR_CONSTANTS

        MV_START_STYLE_CONSTANTS
        MV_ADD_CONSTANT(mvThemeStyle_DragFloatx_Rounding,      0, 12),
        MV_ADD_CONSTANT(mvThemeStyle_DragFloatx_BorderSize,    0, 1),
        MV_ADD_CONSTANT(mvThemeStyle_DragFloatx_PaddingX,      4, 20),
        MV_ADD_CONSTANT(mvThemeStyle_DragFloatx_PaddingY,      3, 20),
        MV_ADD_CONSTANT(mvThemeStyle_DragFloatx_InnerSpacingX, 4, 20),
        MV_ADD_CONSTANT(mvThemeStyle_DragFloatx_InnerSpacingY, 4, 20),
        MV_END_STYLE_CONSTANTS

    public:

        mvDragFloatMulti(const std::string& name, float* default_value, const std::string& dataSource);

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
        int                 m_size = 4;
    };

    //-----------------------------------------------------------------------------
    // mvDragIntMulti
    //-----------------------------------------------------------------------------
    MV_REGISTER_WIDGET(mvDragIntMulti, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Int4, 1);
    class mvDragIntMulti : public mvInt4PtrBase
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPITEM_TYPE(mvAppItemType::mvDragIntMulti, add_drag_intx)

        MV_CREATE_CONSTANT(mvThemeCol_DragIntx_Text,         0L, 0L);
        MV_CREATE_CONSTANT(mvThemeCol_DragIntx_Bg,           7L, 0L);
        MV_CREATE_CONSTANT(mvThemeCol_DragIntx_BgHovered,    8L, 0L);
        MV_CREATE_CONSTANT(mvThemeCol_DragIntx_BgActive,     9L, 0L);
        MV_CREATE_CONSTANT(mvThemeCol_DragIntx_Border,       5L, 0L);
        MV_CREATE_CONSTANT(mvThemeCol_DragIntx_BorderShadow, 6L, 0L);

        MV_CREATE_CONSTANT(mvThemeStyle_DragIntx_Rounding,      11L, 0L);
        MV_CREATE_CONSTANT(mvThemeStyle_DragIntx_BorderSize,    12L, 0L);
        MV_CREATE_CONSTANT(mvThemeStyle_DragIntx_PaddingX,      10L, 0L);
        MV_CREATE_CONSTANT(mvThemeStyle_DragIntx_PaddingY,      10L, 1L);
        MV_CREATE_CONSTANT(mvThemeStyle_DragIntx_InnerSpacingX, 14L, 0L);
        MV_CREATE_CONSTANT(mvThemeStyle_DragIntx_InnerSpacingY, 14L, 1L);

        MV_START_EXTRA_COMMANDS
        MV_END_EXTRA_COMMANDS

        MV_START_GENERAL_CONSTANTS
        MV_END_GENERAL_CONSTANTS

        MV_START_COLOR_CONSTANTS
        MV_ADD_CONSTANT(mvThemeCol_DragIntx_Text,         mvColor(255, 255, 255, 255), mvColor(128, 128, 128, 63)),
        MV_ADD_CONSTANT(mvThemeCol_DragIntx_Bg,           mvColor( 41,  74, 122, 138), mvColor(128, 128, 128, 63)),
        MV_ADD_CONSTANT(mvThemeCol_DragIntx_BgHovered,    mvColor( 66, 150, 250, 102), mvColor(128, 128, 128, 63)),
        MV_ADD_CONSTANT(mvThemeCol_DragIntx_BgActive,     mvColor( 66, 150, 250, 171), mvColor(128, 128, 128, 63)),
        MV_ADD_CONSTANT(mvThemeCol_DragIntx_Border,       mvColor(110, 110, 128, 128), mvColor(128, 128, 128, 63)),
        MV_ADD_CONSTANT(mvThemeCol_DragIntx_BorderShadow, mvColor(  0,   0,   0,   0), mvColor(128, 128, 128, 63)),
        MV_END_COLOR_CONSTANTS

        MV_START_STYLE_CONSTANTS
        MV_ADD_CONSTANT(mvThemeStyle_DragIntx_Rounding,      0, 12),
        MV_ADD_CONSTANT(mvThemeStyle_DragIntx_BorderSize,    0, 1),
        MV_ADD_CONSTANT(mvThemeStyle_DragIntx_PaddingX,      4, 20),
        MV_ADD_CONSTANT(mvThemeStyle_DragIntx_PaddingY,      3, 20),
        MV_ADD_CONSTANT(mvThemeStyle_DragIntx_InnerSpacingX, 4, 20),
        MV_ADD_CONSTANT(mvThemeStyle_DragIntx_InnerSpacingY, 4, 20),
        MV_END_STYLE_CONSTANTS

    public:

        mvDragIntMulti(const std::string& name, int* default_value, const std::string& dataSource);

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
        int                 m_size = 4;
    };
}