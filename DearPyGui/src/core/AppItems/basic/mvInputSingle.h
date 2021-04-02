#pragma once

#include "mvTypeBases.h"
#include "mvApp.h"
#include "mvModule_Core.h"
#include <string>

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvInputInt
//     * mvInputFloat
//
//-----------------------------------------------------------------------------

namespace Marvel {

    //-----------------------------------------------------------------------------
    // mvInputInt
    //-----------------------------------------------------------------------------
    MV_REGISTER_WIDGET(mvInputInt, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Int, 1);
    class mvInputInt : public mvIntPtrBase
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPITEM_TYPE(mvAppItemType::mvInputInt, add_input_int)

        MV_CREATE_CONSTANT(mvThemeCol_InputInt_Text              ,  0L, 0L);
        MV_CREATE_CONSTANT(mvThemeCol_InputInt_TextHighlight     , 44L, 0L);
        MV_CREATE_CONSTANT(mvThemeCol_InputInt_Bg                ,  7L, 0L);
        MV_CREATE_CONSTANT(mvThemeCol_InputInt_ButtonBg          , 21L, 0L);
        MV_CREATE_CONSTANT(mvThemeCol_InputInt_ButtonBgHovered   , 22L, 0L);
        MV_CREATE_CONSTANT(mvThemeCol_InputInt_ButtonBgActive    , 23L, 0L);
        MV_CREATE_CONSTANT(mvThemeCol_InputInt_Border            ,  5L, 0L);
        MV_CREATE_CONSTANT(mvThemeCol_InputInt_BorderShadow      ,  6L, 0L);

        MV_CREATE_CONSTANT(mvThemeStyle_InputInt_Rounding        , 11L, 0L);
        MV_CREATE_CONSTANT(mvThemeStyle_InputInt_BorderSize      , 12L, 0L);
        MV_CREATE_CONSTANT(mvThemeStyle_InputInt_PaddingX        , 10L, 0L);
        MV_CREATE_CONSTANT(mvThemeStyle_InputInt_PaddingY        , 10L, 1L);
        MV_CREATE_CONSTANT(mvThemeStyle_InputInt_InnerSpacingX   , 14L, 0L);
        MV_CREATE_CONSTANT(mvThemeStyle_InputInt_InnerSpacingY   , 14L, 1L);
        MV_CREATE_CONSTANT(mvThemeStyle_InputInt_ButtonTextAlignX, 22L, 0L);
        MV_CREATE_CONSTANT(mvThemeStyle_InputInt_ButtonTextAlignY, 22L, 1L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

        MV_START_COLOR_CONSTANTS
            MV_ADD_CONSTANT(mvThemeCol_InputInt_Text,               mvColor(255, 255, 255, 255), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_InputInt_TextHighlight,      mvColor( 66, 150, 250,  89), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_InputInt_Bg,                 mvColor( 41,  74, 122, 138), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_InputInt_ButtonBg,           mvColor( 66, 150, 250, 102), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_InputInt_ButtonBgHovered,    mvColor( 66, 150, 250, 255), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_InputInt_ButtonBgActive,     mvColor( 15, 135, 250, 255), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_InputInt_Border,             mvColor(110, 110, 128, 128), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_InputInt_BorderShadow,       mvColor(  0,   0,   0,   0), mvColor(128, 128, 128, 63)),
        MV_END_COLOR_CONSTANTS

        MV_START_STYLE_CONSTANTS
            MV_ADD_CONSTANT(mvThemeStyle_InputInt_Rounding         , 0,   12),
            MV_ADD_CONSTANT(mvThemeStyle_InputInt_BorderSize       , 0,    1),
            MV_ADD_CONSTANT(mvThemeStyle_InputInt_PaddingX         , 4,   20),
            MV_ADD_CONSTANT(mvThemeStyle_InputInt_PaddingY         , 3,   20),
            MV_ADD_CONSTANT(mvThemeStyle_InputInt_InnerSpacingX    , 4,   20),
            MV_ADD_CONSTANT(mvThemeStyle_InputInt_InnerSpacingY    , 4,   20),
            MV_ADD_CONSTANT(mvThemeStyle_InputInt_ButtonTextAlignX , 0.5,  1),
            MV_ADD_CONSTANT(mvThemeStyle_InputInt_ButtonTextAlignY , 0.5,  1),
        MV_END_STYLE_CONSTANTS

    public:

        mvInputInt(const std::string& name, int default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;
        void draw(ImDrawList* drawlist, float x, float y) override;

        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;
        
    private:

        int                 m_step = 1;
        int                 m_step_fast = 100;
        int                 m_min = 0;
        int                 m_max = 100;
        bool                m_min_clamped = false;
        bool                m_max_clamped = false;
        ImGuiInputTextFlags m_flags = 0;
        ImGuiInputTextFlags m_stor_flags = 0;
        int                 m_last_value = 0;
        
    };
        
    //-----------------------------------------------------------------------------
    // mvInputFloat
    //-----------------------------------------------------------------------------
    MV_REGISTER_WIDGET(mvInputFloat, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Float, 1);
    class mvInputFloat : public mvFloatPtrBase
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPITEM_TYPE(mvAppItemType::mvInputFloat, add_input_float)

        MV_CREATE_CONSTANT(mvThemeCol_InputFloat_Text              ,  0L, 0L);
        MV_CREATE_CONSTANT(mvThemeCol_InputFloat_TextHighlight     , 44L, 0L);
        MV_CREATE_CONSTANT(mvThemeCol_InputFloat_Bg                ,  7L, 0L);
        MV_CREATE_CONSTANT(mvThemeCol_InputFloat_ButtonBg          , 21L, 0L);
        MV_CREATE_CONSTANT(mvThemeCol_InputFloat_ButtonBgHovered   , 22L, 0L);
        MV_CREATE_CONSTANT(mvThemeCol_InputFloat_ButtonBgActive    , 23L, 0L);
        MV_CREATE_CONSTANT(mvThemeCol_InputFloat_Border            ,  5L, 0L);
        MV_CREATE_CONSTANT(mvThemeCol_InputFloat_BorderShadow      ,  6L, 0L);

        MV_CREATE_CONSTANT(mvThemeStyle_InputFloat_Rounding        , 11L, 0L);
        MV_CREATE_CONSTANT(mvThemeStyle_InputFloat_BorderSize      , 12L, 0L);
        MV_CREATE_CONSTANT(mvThemeStyle_InputFloat_PaddingX        , 10L, 0L);
        MV_CREATE_CONSTANT(mvThemeStyle_InputFloat_PaddingY        , 10L, 1L);
        MV_CREATE_CONSTANT(mvThemeStyle_InputFloat_InnerSpacingX   , 14L, 0L);
        MV_CREATE_CONSTANT(mvThemeStyle_InputFloat_InnerSpacingY   , 14L, 1L);
        MV_CREATE_CONSTANT(mvThemeStyle_InputFloat_ButtonTextAlignX, 22L, 0L);
        MV_CREATE_CONSTANT(mvThemeStyle_InputFloat_ButtonTextAlignY, 22L, 1L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

        MV_START_COLOR_CONSTANTS
            MV_ADD_CONSTANT(mvThemeCol_InputFloat_Text,               mvColor(255, 255, 255, 255), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_InputFloat_TextHighlight,      mvColor( 66, 150, 250,  89), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_InputFloat_Bg,                 mvColor( 41,  74, 122, 138), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_InputFloat_ButtonBg,           mvColor( 66, 150, 250, 102), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_InputFloat_ButtonBgHovered,    mvColor( 66, 150, 250, 255), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_InputFloat_ButtonBgActive,     mvColor( 15, 135, 250, 255), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_InputFloat_Border,             mvColor(110, 110, 128, 128), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_InputFloat_BorderShadow,       mvColor(  0,   0,   0,   0), mvColor(128, 128, 128, 63)),
        MV_END_COLOR_CONSTANTS

        MV_START_STYLE_CONSTANTS
            MV_ADD_CONSTANT(mvThemeStyle_InputFloat_Rounding        , 0,   12),
            MV_ADD_CONSTANT(mvThemeStyle_InputFloat_BorderSize      , 0,    1),
            MV_ADD_CONSTANT(mvThemeStyle_InputFloat_PaddingX        , 4,   20),
            MV_ADD_CONSTANT(mvThemeStyle_InputFloat_PaddingY        , 3,   20),
            MV_ADD_CONSTANT(mvThemeStyle_InputFloat_InnerSpacingX   , 4,   20),
            MV_ADD_CONSTANT(mvThemeStyle_InputFloat_InnerSpacingY   , 4,   20),
            MV_ADD_CONSTANT(mvThemeStyle_InputFloat_ButtonTextAlignX, 0.5,  1),
            MV_ADD_CONSTANT(mvThemeStyle_InputFloat_ButtonTextAlignY, 0.5,  1),
        MV_END_STYLE_CONSTANTS

    public:

        mvInputFloat(const std::string& name, float default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;
        void draw(ImDrawList* drawlist, float x, float y) override;

        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;

    private:

        float               m_min = 0.0f;
        float               m_max = 100.0f;
        bool                m_min_clamped = false;
        bool                m_max_clamped = false;
        std::string         m_format = "%.3f";
        float               m_step = 0.1f;
        float               m_step_fast = 1.0f;
        ImGuiInputTextFlags m_flags = 0;
        ImGuiInputTextFlags m_stor_flags = 0;
        float               m_last_value = 0.0f;
        
    };
 
}
