#pragma once

#include "mvTypeBases.h"
#include "mvApp.h"
#include "mvModule_Core.h"
#include <string>

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvInputInt
//     * mvInputInt2
//     * mvInputInt3
//     * mvInputInt4
//     * mvInputFloat
//     * mvInputFloat2
//     * mvInputFloat3
//     * mvInputFloat4
//
//-----------------------------------------------------------------------------

namespace Marvel {

    //-----------------------------------------------------------------------------
    // mvInputInt
    //-----------------------------------------------------------------------------
    
    struct mvInputIntConfig : public mvAppItemConfig
    {
        int     default_value  = 0;
        int     min_value      = 0;
        int     max_value      = 100;
        bool    min_clamped    = false;
        bool    max_clamped    = false;
        bool    on_enter       = false;
        int     step           = 1;
        int     step_fast      = 100;
        bool    readonly       = false;
    };

	PyObject* add_input_int   (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_input_int2  (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_input_int3  (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_input_int4  (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_input_float (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_input_float2(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_input_float3(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_input_float4(PyObject* self, PyObject* args, PyObject* kwargs);
    
    MV_REGISTER_WIDGET(mvInputInt);
    class mvInputInt : public mvIntPtrBase
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);
        
        MV_APPITEM_TYPE(mvAppItemType::mvInputInt, "add_input_int")

        MV_CREATE_THEME_CONSTANT(mvThemeCol_InputInt_Text              ,  0L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_InputInt_TextHighlight     , 44L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_InputInt_Bg                ,  7L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_InputInt_ButtonBg          , 21L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_InputInt_ButtonBgHovered   , 22L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_InputInt_ButtonBgActive    , 23L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_InputInt_Border            ,  5L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_InputInt_BorderShadow      ,  6L, 0L);

        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputInt_Rounding        , 11L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputInt_BorderSize      , 12L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputInt_PaddingX        , 10L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputInt_PaddingY        , 10L, 1L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputInt_InnerSpacingX   , 14L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputInt_InnerSpacingY   , 14L, 1L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputInt_ButtonTextAlignX, 22L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputInt_ButtonTextAlignY, 22L, 1L);

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

        MV_START_COLOR_CONSTANTS
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_InputInt_Text,               mvColor(255, 255, 255, 255), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_InputInt_TextHighlight,      mvColor( 66, 150, 250,  89), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_InputInt_Bg,                 mvColor( 41,  74, 122, 138), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_InputInt_ButtonBg,           mvColor( 66, 150, 250, 102), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_InputInt_ButtonBgHovered,    mvColor( 66, 150, 250, 255), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_InputInt_ButtonBgActive,     mvColor( 15, 135, 250, 255), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_InputInt_Border,             mvColor(110, 110, 128, 128), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_InputInt_BorderShadow,       mvColor(  0,   0,   0,   0), mvColor(128, 128, 128, 63)),
        MV_END_COLOR_CONSTANTS

        MV_START_STYLE_CONSTANTS
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt_Rounding         , 0,   12),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt_BorderSize       , 0,    1),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt_PaddingX         , 4,   20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt_PaddingY         , 3,   20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt_InnerSpacingX    , 4,   20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt_InnerSpacingY    , 4,   20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt_ButtonTextAlignX , 0.5,  1),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt_ButtonTextAlignY , 0.5,  1),
        MV_END_STYLE_CONSTANTS

    public:

        mvInputInt(const std::string& name, int default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;
        void draw() override;

        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;

        // cpp interface
        void updateConfig(mvAppItemConfig* config) override;
        mvAppItemConfig* getConfig() override;
        
    private:

        int                 m_step = 1;
        int                 m_step_fast = 100;
        int                 m_min = 0;
        int                 m_max = 100;
        bool                m_min_clamped = false;
        bool                m_max_clamped = false;
        ImGuiInputTextFlags m_flags = 0;
        ImGuiInputTextFlags m_stor_flags = 0;
        mvInputIntConfig    m_config;
        int                 m_last_value = 0;
        
    };

    //-----------------------------------------------------------------------------
    // mvInputInt2
    //-----------------------------------------------------------------------------

    struct mvInputIntsConfig : public mvAppItemConfig
    {
        std::array<int, 4>  default_value   = { 0, 0, 0, 0 };
        int                 min_value       = 0;
        int                 max_value       = 100;
        bool                min_clamped     = false;
        bool                max_clamped     = false;
        bool                on_enter        = false;
        bool                readonly        = false;

    };

    MV_REGISTER_WIDGET(mvInputInt2);
    class mvInputInt2 : public mvInt2PtrBase
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPITEM_TYPE(mvAppItemType::mvInputInt2, "add_input_int2")

        MV_CREATE_THEME_CONSTANT(mvThemeCol_InputInt2_Text            ,  0L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_InputInt2_TextHighlight   , 44L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_InputInt2_Bg              ,  7L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_InputInt2_Border          ,  5L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_InputInt2_BorderShadow    ,  6L, 0L);

        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputInt2_Rounding      , 11L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputInt2_BorderSize    , 12L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputInt2_PaddingX      , 10L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputInt2_PaddingY      , 10L, 1L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputInt2_InnerSpacingX , 14L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputInt2_InnerSpacingY , 14L, 1L);

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

        MV_START_COLOR_CONSTANTS
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_InputInt2_Text,          mvColor(255, 255, 255, 255), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_InputInt2_TextHighlight, mvColor( 66, 150, 250,  89), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_InputInt2_Bg,            mvColor( 41,  74, 122, 138), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_InputInt2_Border,        mvColor(110, 110, 128, 128), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_InputInt2_BorderShadow,  mvColor(  0,   0,   0,   0), mvColor(128, 128, 128, 63)),
        MV_END_COLOR_CONSTANTS

        MV_START_STYLE_CONSTANTS
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt2_Rounding        , 0, 12),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt2_BorderSize      , 0,  1),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt2_PaddingX        , 4, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt2_PaddingY        , 3, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt2_InnerSpacingX   , 4, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt2_InnerSpacingY   , 4, 20),
        MV_END_STYLE_CONSTANTS

    public:

        mvInputInt2(const std::string& name, int* default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;
        void draw() override;

        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;
        
        // cpp interface
        void updateConfig(mvAppItemConfig* config) override;
        mvAppItemConfig* getConfig() override;

    private:
        
        int                 m_min = 0;
        int                 m_max = 100;
        bool                m_min_clamped = false;
        bool                m_max_clamped = false;
        ImGuiInputTextFlags m_flags = 0;
        ImGuiInputTextFlags m_stor_flags = 0;
        mvInputIntsConfig   m_config;
        std::array<int, 2>  m_last_value = { 0, 0 };
        
    };
        
    //-----------------------------------------------------------------------------
    // mvInputInt3
    //-----------------------------------------------------------------------------
    MV_REGISTER_WIDGET(mvInputInt3);
    class mvInputInt3 : public mvInt3PtrBase
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPITEM_TYPE(mvAppItemType::mvInputInt3, "add_input_int3")

        MV_CREATE_THEME_CONSTANT(mvThemeCol_InputInt3_Text            ,  0L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_InputInt3_TextHighlight   , 44L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_InputInt3_Bg              ,  7L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_InputInt3_Border          ,  5L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_InputInt3_BorderShadow    ,  6L, 0L);

        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputInt3_Rounding      , 11L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputInt3_BorderSize    , 12L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputInt3_PaddingX      , 10L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputInt3_PaddingY      , 10L, 1L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputInt3_InnerSpacingX , 14L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputInt3_InnerSpacingY , 14L, 1L);

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

        MV_START_COLOR_CONSTANTS
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_InputInt3_Text,          mvColor(255, 255, 255, 255), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_InputInt3_TextHighlight, mvColor( 66, 150, 250,  89), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_InputInt3_Bg,            mvColor( 41,  74, 122, 138), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_InputInt3_Border,        mvColor(110, 110, 128, 128), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_InputInt3_BorderShadow,  mvColor(  0,   0,   0,   0), mvColor(128, 128, 128, 63)),
        MV_END_COLOR_CONSTANTS

        MV_START_STYLE_CONSTANTS
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt3_Rounding        , 0, 12),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt3_BorderSize      , 0,  1),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt3_PaddingX        , 4, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt3_PaddingY        , 3, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt3_InnerSpacingX   , 4, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt3_InnerSpacingY   , 4, 20),
        MV_END_STYLE_CONSTANTS

        
    public:

        mvInputInt3(const std::string& name, int* default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;
        void draw() override;

        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;

        // cpp interface
        void updateConfig(mvAppItemConfig* config) override;
        mvAppItemConfig* getConfig() override;
        
    private:

        int                 m_min = 0;
        int                 m_max = 100;
        bool                m_min_clamped = false;
        bool                m_max_clamped = false;
        ImGuiInputTextFlags m_flags = 0;
        ImGuiInputTextFlags m_stor_flags = 0;
        mvInputIntsConfig   m_config;
        std::array<int, 3>  m_last_value = { 0, 0, 0 };
        
    };
        
    //-----------------------------------------------------------------------------
    // mvInputInt4
    //-----------------------------------------------------------------------------
    MV_REGISTER_WIDGET(mvInputInt4);
    class mvInputInt4 : public mvInt4PtrBase
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);
    
        MV_APPITEM_TYPE(mvAppItemType::mvInputInt4, "add_input_int4")

        MV_CREATE_THEME_CONSTANT(mvThemeCol_InputInt4_Text            ,  0L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_InputInt4_TextHighlight   , 44L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_InputInt4_Bg              ,  7L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_InputInt4_Border          ,  5L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_InputInt4_BorderShadow    ,  6L, 0L);

        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputInt4_Rounding      , 11L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputInt4_BorderSize    , 12L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputInt4_PaddingX      , 10L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputInt4_PaddingY      , 10L, 1L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputInt4_InnerSpacingX , 14L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputInt4_InnerSpacingY , 14L, 1L);

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

        MV_START_COLOR_CONSTANTS
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_InputInt4_Text,          mvColor(255, 255, 255, 255), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_InputInt4_TextHighlight, mvColor( 66, 150, 250,  89), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_InputInt4_Bg,            mvColor( 41,  74, 122, 138), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_InputInt4_Border,        mvColor(110, 110, 128, 128), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_InputInt4_BorderShadow,  mvColor(  0,   0,   0,   0), mvColor(128, 128, 128, 63)),
        MV_END_COLOR_CONSTANTS

        MV_START_STYLE_CONSTANTS
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt4_Rounding        , 0, 12),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt4_BorderSize      , 0,  1),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt4_PaddingX        , 4, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt4_PaddingY        , 3, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt4_InnerSpacingX   , 4, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt4_InnerSpacingY   , 4, 20),
        MV_END_STYLE_CONSTANTS

    public:

        mvInputInt4(const std::string& name, int* default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;
        void draw() override;

        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;
        
        // cpp interface
        void updateConfig(mvAppItemConfig* config) override;
        mvAppItemConfig* getConfig() override;

    private:

        int                 m_min = 0;
        int                 m_max = 100;
        bool                m_min_clamped = false;
        bool                m_max_clamped = false;
        ImGuiInputTextFlags m_flags = 0;
        ImGuiInputTextFlags m_stor_flags = 0;
        mvInputIntsConfig   m_config;
        std::array<int, 4>  m_last_value = { 0, 0, 0, 0 };
        
    };
        
    //-----------------------------------------------------------------------------
    // mvInputFloat
    //-----------------------------------------------------------------------------
    
    struct mvInputFloatConfig : public mvAppItemConfig
    {
        float       default_value   = 0.0;
        float       min_value       = 0.0;
        float       max_value       = 100.0;
        bool        min_clamped     = false;
        bool        max_clamped     = false;
        std::string format          = "'%.3f'";
        bool        on_enter        = false;
        float       step            = 0.1;
        float       step_fast       = 1.0;
        bool        readonly        = false;
    };
    
    MV_REGISTER_WIDGET(mvInputFloat);
    class mvInputFloat : public mvFloatPtrBase
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);
        
        MV_APPITEM_TYPE(mvAppItemType::mvInputFloat, "add_input_float")

        MV_CREATE_THEME_CONSTANT(mvThemeCol_InputFloat_Text              ,  0L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_InputFloat_TextHighlight     , 44L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_InputFloat_Bg                ,  7L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_InputFloat_ButtonBg          , 21L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_InputFloat_ButtonBgHovered   , 22L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_InputFloat_ButtonBgActive    , 23L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_InputFloat_Border            ,  5L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_InputFloat_BorderShadow      ,  6L, 0L);

        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputFloat_Rounding        , 11L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputFloat_BorderSize      , 12L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputFloat_PaddingX        , 10L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputFloat_PaddingY        , 10L, 1L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputFloat_InnerSpacingX   , 14L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputFloat_InnerSpacingY   , 14L, 1L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputFloat_ButtonTextAlignX, 22L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputFloat_ButtonTextAlignY, 22L, 1L);

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

        MV_START_COLOR_CONSTANTS
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_InputFloat_Text,               mvColor(255, 255, 255, 255), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_InputFloat_TextHighlight,      mvColor( 66, 150, 250,  89), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_InputFloat_Bg,                 mvColor( 41,  74, 122, 138), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_InputFloat_ButtonBg,           mvColor( 66, 150, 250, 102), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_InputFloat_ButtonBgHovered,    mvColor( 66, 150, 250, 255), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_InputFloat_ButtonBgActive,     mvColor( 15, 135, 250, 255), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_InputFloat_Border,             mvColor(110, 110, 128, 128), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_InputFloat_BorderShadow,       mvColor(  0,   0,   0,   0), mvColor(128, 128, 128, 63)),
        MV_END_COLOR_CONSTANTS

        MV_START_STYLE_CONSTANTS
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat_Rounding        , 0,   12),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat_BorderSize      , 0,    1),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat_PaddingX        , 4,   20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat_PaddingY        , 3,   20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat_InnerSpacingX   , 4,   20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat_InnerSpacingY   , 4,   20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat_ButtonTextAlignX, 0.5,  1),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat_ButtonTextAlignY, 0.5,  1),
        MV_END_STYLE_CONSTANTS

    public:

        mvInputFloat(const std::string& name, float default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;
        void draw() override;

        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;
        
        // cpp interface
        void updateConfig(mvAppItemConfig* config) override;
        mvAppItemConfig* getConfig() override;

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
        mvInputFloatConfig  m_config;
        float               m_last_value = 0.0f;
        
    };
        
    //-----------------------------------------------------------------------------
    // mvInputFloat2
    //-----------------------------------------------------------------------------
    
    struct mvInputFloatsConfig : public mvAppItemConfig
    {
        std::array<float, 4>    default_value   = { 0.0f, 0.0f, 0.0f, 0.0f };
        float                   min_value       = 0.0;
        float                   max_value       = 100.0;
        bool                    min_clamped     = false;
        bool                    max_clamped     = false;
        std::string             format          = "'%.3f'";
        bool                    on_enter        = false;
        bool                    readonly        = false;
    };
    
    MV_REGISTER_WIDGET(mvInputFloat2);
    class mvInputFloat2: public mvFloat2PtrBase
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);
    
        MV_APPITEM_TYPE(mvAppItemType::mvInputFloat2, "add_input_float2")

        MV_CREATE_THEME_CONSTANT(mvThemeCol_InputFloat2_Text            ,  0L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_InputFloat2_TextHighlight   , 44L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_InputFloat2_Bg              ,  7L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_InputFloat2_Border          ,  5L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_InputFloat2_BorderShadow    ,  6L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputFloat2_Rounding      , 11L, 0L);

        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputFloat2_BorderSize    , 12L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputFloat2_PaddingX      , 10L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputFloat2_PaddingY      , 10L, 1L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputFloat2_InnerSpacingX , 14L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputFloat2_InnerSpacingY , 14L, 1L);

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

        MV_START_COLOR_CONSTANTS
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_InputFloat2_Text,          mvColor(255, 255, 255, 255), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_InputFloat2_TextHighlight, mvColor( 66, 150, 250,  89), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_InputFloat2_Bg,            mvColor( 41,  74, 122, 138), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_InputFloat2_Border,        mvColor(110, 110, 128, 128), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_InputFloat2_BorderShadow,  mvColor(  0,   0,   0,   0), mvColor(128, 128, 128, 63)),
        MV_END_COLOR_CONSTANTS

        MV_START_STYLE_CONSTANTS
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat2_Rounding      , 0, 12),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat2_BorderSize    , 0,  1),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat2_PaddingX      , 4, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat2_PaddingY      , 3, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat2_InnerSpacingX , 4, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat2_InnerSpacingY , 4, 20),
        MV_END_STYLE_CONSTANTS

    public:

        mvInputFloat2(const std::string& name, float* default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;
        void draw() override;

        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;
        
        // cpp interface
        void updateConfig(mvAppItemConfig* config) override;
        mvAppItemConfig* getConfig() override;

    private:

        float               m_min = 0.0f;
        float               m_max = 100.0f;
        bool                m_min_clamped = false;
        bool                m_max_clamped = false;
        std::string         m_format = "%.3f";
        ImGuiInputTextFlags m_flags = 0;
        ImGuiInputTextFlags m_stor_flags = 0;
        mvInputFloatsConfig m_config;
        std::array<float, 2>m_last_value = { 0.0f, 0.0f };
        
    };
        
    //-----------------------------------------------------------------------------
    // mvInputFloat3
    //-----------------------------------------------------------------------------
    MV_REGISTER_WIDGET(mvInputFloat3);
    class mvInputFloat3 : public mvFloat3PtrBase
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);
        
        MV_APPITEM_TYPE(mvAppItemType::mvInputFloat3, "add_input_float3")

        MV_CREATE_THEME_CONSTANT(mvThemeCol_InputFloat3_Text            ,  0L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_InputFloat3_TextHighlight   , 44L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_InputFloat3_Bg              ,  7L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_InputFloat3_Border          ,  5L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_InputFloat3_BorderShadow    ,  6L, 0L);

        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputFloat3_Rounding      , 11L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputFloat3_BorderSize    , 12L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputFloat3_PaddingX      , 10L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputFloat3_PaddingY      , 10L, 1L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputFloat3_InnerSpacingX , 14L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputFloat3_InnerSpacingY , 14L, 1L);

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

        MV_START_COLOR_CONSTANTS
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_InputFloat3_Text,          mvColor(255, 255, 255, 255), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_InputFloat3_TextHighlight, mvColor( 66, 150, 250,  89), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_InputFloat3_Bg,            mvColor( 41,  74, 122, 138), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_InputFloat3_Border,        mvColor(110, 110, 128, 128), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_InputFloat3_BorderShadow,  mvColor(  0,   0,   0,   0), mvColor(128, 128, 128, 63)),
        MV_END_COLOR_CONSTANTS

        MV_START_STYLE_CONSTANTS
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat3_Rounding      , 0, 12),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat3_BorderSize    , 0,  1),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat3_PaddingX      , 4, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat3_PaddingY      , 3, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat3_InnerSpacingX , 4, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat3_InnerSpacingY , 4, 20),
        MV_END_STYLE_CONSTANTS


    public:

        mvInputFloat3(const std::string& name, float* default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;
        void draw() override;

        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;
        
        // cpp interface
        void updateConfig(mvAppItemConfig* config) override;
        mvAppItemConfig* getConfig() override;

    private:

        float               m_min = 0.0f;
        float               m_max = 100.0f;
        bool                m_min_clamped = false;
        bool                m_max_clamped = false;
        std::string         m_format = "%.3f";
        ImGuiInputTextFlags m_flags = 0;
        ImGuiInputTextFlags m_stor_flags = 0;
        mvInputFloatsConfig m_config;
        std::array<float, 3>m_last_value = { 0.0f, 0.0f, 0.0f };
    };
        
    //-----------------------------------------------------------------------------
    // mvInputFloat4
    //-----------------------------------------------------------------------------
    MV_REGISTER_WIDGET(mvInputFloat4);
    class mvInputFloat4 : public mvFloat4PtrBase
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);
    
        MV_APPITEM_TYPE(mvAppItemType::mvInputFloat4, "add_input_float4")

        MV_CREATE_THEME_CONSTANT(mvThemeCol_InputFloat4_Text            ,  0L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_InputFloat4_TextHighlight   , 44L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_InputFloat4_Bg              ,  7L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_InputFloat4_Border          ,  5L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_InputFloat4_BorderShadow    ,  6L, 0L);

        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputFloat4_Rounding      , 11L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputFloat4_BorderSize    , 12L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputFloat4_PaddingX      , 10L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputFloat4_PaddingY      , 10L, 1L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputFloat4_InnerSpacingX , 14L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputFloat4_InnerSpacingY , 14L, 1L);

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

        MV_START_COLOR_CONSTANTS
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_InputFloat4_Text,          mvColor(255, 255, 255, 255), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_InputFloat4_TextHighlight, mvColor( 66, 150, 250,  89), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_InputFloat4_Bg,            mvColor( 41,  74, 122, 138), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_InputFloat4_Border,        mvColor(110, 110, 128, 128), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_InputFloat4_BorderShadow,  mvColor(  0,   0,   0,   0), mvColor(128, 128, 128, 63)),
        MV_END_COLOR_CONSTANTS

        MV_START_STYLE_CONSTANTS
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat4_Rounding      , 0, 12),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat4_BorderSize    , 0,  1),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat4_PaddingX      , 4, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat4_PaddingY      , 3, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat4_InnerSpacingX , 4, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat4_InnerSpacingY , 4, 20),
        MV_END_STYLE_CONSTANTS

    public:

        mvInputFloat4(const std::string& name, float* default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;
        void draw() override;

        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;

        // cpp interface
        void updateConfig(mvAppItemConfig* config) override;
        mvAppItemConfig* getConfig() override;
        
    private:

        float               m_min = 0.0f;
        float               m_max = 100.0f;
        bool                m_min_clamped = false;
        bool                m_max_clamped = false;
        std::string         m_format = "%.3f";
        ImGuiInputTextFlags m_flags = 0;
        ImGuiInputTextFlags m_stor_flags = 0;
        mvInputFloatsConfig m_config;
        std::array<float, 4>m_last_value = { 0.0f, 0.0f, 0.0f, 0.0f };
    };

}
