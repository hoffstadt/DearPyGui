#pragma once

#include "mvTypeBases.h"
#include "mvApp.h"
#include "mvModule_Core.h"
#include <string>

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvDragFloat
//     * mvDragFloat2
//     * mvDragFloat3
//     * mvDragFloat4
//     * mvDragInt
//     * mvDragInt2
//     * mvDragInt3
//     * mvDragInt4
//
//-----------------------------------------------------------------------------

namespace Marvel {

    //-----------------------------------------------------------------------------
    // mvDragFloat
    //-----------------------------------------------------------------------------
    
    struct mvDragFloatConfig : public mvAppItemConfig
    {
        float       default_value   = 0.0;
        float       speed           = 1.0;
        float       min_value       = 0.0;
        float       max_value       = 100.0;
        std::string format          = "'%0.3f'";
        bool        no_input        = false;
        bool        clamped         = false;
    };

    PyObject* add_drag_float(PyObject* self, PyObject* args, PyObject* kwargs);
    PyObject* add_drag_float2(PyObject* self, PyObject* args, PyObject* kwargs);
    PyObject* add_drag_float3(PyObject* self, PyObject* args, PyObject* kwargs);
    PyObject* add_drag_float4(PyObject* self, PyObject* args, PyObject* kwargs);
    PyObject* add_drag_int(PyObject* self, PyObject* args, PyObject* kwargs);
    PyObject* add_drag_int2(PyObject* self, PyObject* args, PyObject* kwargs);
    PyObject* add_drag_int3(PyObject* self, PyObject* args, PyObject* kwargs);
    PyObject* add_drag_int4(PyObject* self, PyObject* args, PyObject* kwargs);
    
    MV_REGISTER_WIDGET(mvDragFloat);
    class mvDragFloat : public mvFloatPtrBase
    {

        MV_APPITEM_TYPE(mvAppItemType::mvDragFloat, "add_drag_float")

        MV_CREATE_THEME_CONSTANT(mvThemeCol_DragFloat_Text            ,  0L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_DragFloat_Bg              ,  7L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_DragFloat_BgHovered       ,  8L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_DragFloat_BgActive        ,  9L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_DragFloat_Border          ,  5L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_DragFloat_BorderShadow    ,  6L, 0L);

        MV_CREATE_THEME_CONSTANT(mvThemeStyle_DragFloat_Rounding      , 11L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_DragFloat_BorderSize    , 12L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_DragFloat_PaddingX      , 10L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_DragFloat_PaddingY      , 10L, 1L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_DragFloat_InnerSpacingX , 14L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_DragFloat_InnerSpacingY , 14L, 1L);

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

        MV_START_COLOR_CONSTANTS
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_DragFloat_Text,         mvColor(255, 255, 255, 255), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_DragFloat_Bg,           mvColor( 41,  74, 122, 138), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_DragFloat_BgHovered,    mvColor( 66, 150, 250, 102), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_DragFloat_BgActive,     mvColor( 66, 150, 250, 171), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_DragFloat_Border,       mvColor(110, 110, 128, 128), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_DragFloat_BorderShadow, mvColor(  0,   0,   0,   0), mvColor(128, 128, 128, 63)),
        MV_END_COLOR_CONSTANTS

        MV_START_STYLE_CONSTANTS
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DragFloat_Rounding        , 0, 12),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DragFloat_BorderSize      , 0, 1),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DragFloat_PaddingX        , 4, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DragFloat_PaddingY        , 3, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DragFloat_InnerSpacingX   , 4, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DragFloat_InnerSpacingY   , 4, 20),
        MV_END_STYLE_CONSTANTS

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

    public:

        mvDragFloat(const std::string& name, float default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;
        
        void draw() override;
        
        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;
        
        // cpp interface
        void updateConfig(mvAppItemConfig* config) override;
        mvAppItemConfig* getConfig() override;

    private:

        float               m_speed = 1.0f;
        float               m_min = 0.0f;
        float               m_max = 100.0f;
        std::string         m_format = "%.3f";
        ImGuiInputTextFlags m_flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags m_stor_flags = ImGuiSliderFlags_None;
        mvDragFloatConfig   m_config;

    };
        
    //-----------------------------------------------------------------------------
    // mvDragFloat2
    //-----------------------------------------------------------------------------
    
    struct mvDragFloatsConfig : public mvAppItemConfig
    {
        std::array<float, 4> default_value = { 0.0f, 0.0f, 0.0f, 0.0f };
        float speed = 1.0;
        float min_value = 0.0;
        float max_value = 100.0;
        std::string format = "'%0.3f'";
        bool no_input = false;
        bool clamped = false;
        
    };
    
    MV_REGISTER_WIDGET(mvDragFloat2);
    class mvDragFloat2 : public mvFloat2PtrBase
    {

        MV_APPITEM_TYPE(mvAppItemType::mvDragFloat2, "add_drag_float2")

        MV_CREATE_THEME_CONSTANT(mvThemeCol_DragFloat2_Text              ,  0L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_DragFloat2_Bg                ,  7L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_DragFloat2_BgHovered         ,  8L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_DragFloat2_BgActive          ,  9L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_DragFloat2_Border            ,  5L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_DragFloat2_BorderShadow      ,  6L, 0L);

        MV_CREATE_THEME_CONSTANT(mvThemeStyle_DragFloat2_Rounding        , 11L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_DragFloat2_BorderSize      , 12L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_DragFloat2_PaddingX        , 10L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_DragFloat2_PaddingY        , 10L, 1L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_DragFloat2_InnerSpacingX   , 14L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_DragFloat2_InnerSpacingY   , 14L, 1L);

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

        MV_START_COLOR_CONSTANTS
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_DragFloat2_Text,         mvColor(255, 255, 255, 255), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_DragFloat2_Bg,           mvColor( 41,  74, 122, 138), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_DragFloat2_BgHovered,    mvColor( 66, 150, 250, 102), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_DragFloat2_BgActive,     mvColor( 66, 150, 250, 171), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_DragFloat2_Border,       mvColor(110, 110, 128, 128), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_DragFloat2_BorderShadow, mvColor(  0,   0,   0,   0), mvColor(128, 128, 128, 63)),
        MV_END_COLOR_CONSTANTS

        MV_START_STYLE_CONSTANTS
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DragFloat2_Rounding       , 0, 12),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DragFloat2_BorderSize     , 0,  1),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DragFloat2_PaddingX       , 4, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DragFloat2_PaddingY       , 3, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DragFloat2_InnerSpacingX  , 4, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DragFloat2_InnerSpacingY  , 4, 20),
        MV_END_STYLE_CONSTANTS

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

    public:

        mvDragFloat2(const std::string& name, float* default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;
        void draw() override;

        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;
        
        // cpp interface
        void updateConfig(mvAppItemConfig* config) override;
        mvAppItemConfig* getConfig() override;

    private:

        float               m_speed = 1.0f;
        float               m_min = 0.0f;
        float               m_max = 100.0f;
        std::string         m_format = "%.3f";
        ImGuiInputTextFlags m_flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags m_stor_flags = ImGuiSliderFlags_None;
        mvDragFloatsConfig  m_config;
    };
        
    //-----------------------------------------------------------------------------
    // mvDragFloat3
    //-----------------------------------------------------------------------------
    MV_REGISTER_WIDGET(mvDragFloat3);
    class mvDragFloat3 : public mvFloat3PtrBase
    {

        MV_APPITEM_TYPE(mvAppItemType::mvDragFloat3, "add_drag_float3")

        MV_CREATE_THEME_CONSTANT(mvThemeCol_DragFloat3_Text              ,  0L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_DragFloat3_Bg                ,  7L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_DragFloat3_BgHovered         ,  8L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_DragFloat3_BgActive          ,  9L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_DragFloat3_Border            ,  5L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_DragFloat3_BorderShadow      ,  6L, 0L);

        MV_CREATE_THEME_CONSTANT(mvThemeStyle_DragFloat3_Rounding        , 11L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_DragFloat3_BorderSize      , 12L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_DragFloat3_PaddingX        , 10L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_DragFloat3_PaddingY        , 10L, 1L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_DragFloat3_InnerSpacingX   , 14L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_DragFloat3_InnerSpacingY   , 14L, 1L);

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

        MV_START_COLOR_CONSTANTS
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_DragFloat3_Text,         mvColor(255, 255, 255, 255), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_DragFloat3_Bg,           mvColor( 41,  74, 122, 138), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_DragFloat3_BgHovered,    mvColor( 66, 150, 250, 102), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_DragFloat3_BgActive,     mvColor( 66, 150, 250, 171), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_DragFloat3_Border,       mvColor(110, 110, 128, 128), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_DragFloat3_BorderShadow, mvColor(  0,   0,   0,   0), mvColor(128, 128, 128, 63)),
        MV_END_COLOR_CONSTANTS

        MV_START_STYLE_CONSTANTS
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DragFloat3_Rounding       , 0, 12),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DragFloat3_BorderSize     , 0,  1),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DragFloat3_PaddingX       , 4, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DragFloat3_PaddingY       , 3, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DragFloat3_InnerSpacingX  , 4, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DragFloat3_InnerSpacingY  , 4, 20),
        MV_END_STYLE_CONSTANTS

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

    public:

        mvDragFloat3(const std::string& name, float* default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;
        void draw() override;

        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;
        
        // cpp interface
        void updateConfig(mvAppItemConfig* config) override;
        mvAppItemConfig* getConfig() override;

    private:

        float               m_speed = 1.0f;
        float               m_min = 0.0f;
        float               m_max = 100.0f;
        std::string         m_format = "%.3f";
        ImGuiInputTextFlags m_flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags m_stor_flags = ImGuiSliderFlags_None;
        mvDragFloatsConfig  m_config;
    };
        
    //-----------------------------------------------------------------------------
    // mvDragFloat4
    //-----------------------------------------------------------------------------
    MV_REGISTER_WIDGET(mvDragFloat4);
    class mvDragFloat4 : public mvFloat4PtrBase
    {


        MV_APPITEM_TYPE(mvAppItemType::mvDragFloat4, "add_drag_float4")

        MV_CREATE_THEME_CONSTANT(mvThemeCol_DragFloat4_Text              ,  0L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_DragFloat4_Bg                ,  7L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_DragFloat4_BgHovered         ,  8L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_DragFloat4_BgActive          ,  9L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_DragFloat4_Border            ,  5L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_DragFloat4_BorderShadow      ,  6L, 0L);

        MV_CREATE_THEME_CONSTANT(mvThemeStyle_DragFloat4_Rounding        , 11L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_DragFloat4_BorderSize      , 12L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_DragFloat4_PaddingX        , 10L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_DragFloat4_PaddingY        , 10L, 1L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_DragFloat4_InnerSpacingX   , 14L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_DragFloat4_InnerSpacingY   , 14L, 1L);

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

        MV_START_COLOR_CONSTANTS
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_DragFloat4_Text,         mvColor(255, 255, 255, 255), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_DragFloat4_Bg,           mvColor( 41,  74, 122, 138), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_DragFloat4_BgHovered,    mvColor( 66, 150, 250, 102), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_DragFloat4_BgActive,     mvColor( 66, 150, 250, 171), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_DragFloat4_Border,       mvColor(110, 110, 128, 128), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_DragFloat4_BorderShadow, mvColor(  0,   0,   0,   0), mvColor(128, 128, 128, 63)),
        MV_END_COLOR_CONSTANTS

        MV_START_STYLE_CONSTANTS
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DragFloat4_Rounding       , 0, 12),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DragFloat4_BorderSize     , 0,  1),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DragFloat4_PaddingX       , 4, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DragFloat4_PaddingY       , 3, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DragFloat4_InnerSpacingX  , 4, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DragFloat4_InnerSpacingY  , 4, 20),
        MV_END_STYLE_CONSTANTS

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

    public:

        mvDragFloat4(const std::string& name, float* default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;
        void draw() override;

        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;
        
        // cpp interface
        void updateConfig(mvAppItemConfig* config) override;
        mvAppItemConfig* getConfig() override;

    private:

        float               m_speed = 1.0f;
        float               m_min = 0.0f;
        float               m_max = 100.0f;
        std::string         m_format = "%.3f";
        ImGuiInputTextFlags m_flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags m_stor_flags = ImGuiSliderFlags_None;
        mvDragFloatsConfig  m_config;
    };
        
    //-----------------------------------------------------------------------------
    // mvDragInt
    //-----------------------------------------------------------------------------
    
    struct mvDragIntConfig : public mvAppItemConfig
    {
        int default_value = 0;
        float speed = 1.0;
        int min_value = 0;
        int max_value = 100;
        std::string format = "'%d'";
        bool no_input = false;
        bool clamped = false;
    };
    
    MV_REGISTER_WIDGET(mvDragInt);
    class mvDragInt : public mvIntPtrBase
    {

        MV_APPITEM_TYPE(mvAppItemType::mvDragInt, "add_drag_int")

        MV_CREATE_THEME_CONSTANT(mvThemeCol_DragInt_Text            ,  0L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_DragInt_Bg              ,  7L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_DragInt_BgHovered       ,  8L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_DragInt_BgActive        ,  9L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_DragInt_Border          ,  5L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_DragInt_BorderShadow    ,  6L, 0L);

        MV_CREATE_THEME_CONSTANT(mvThemeStyle_DragInt_Rounding      , 11L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_DragInt_BorderSize    , 12L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_DragInt_PaddingX      , 10L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_DragInt_PaddingY      , 10L, 1L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_DragInt_InnerSpacingX , 14L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_DragInt_InnerSpacingY , 14L, 1L);

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

        MV_START_COLOR_CONSTANTS
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_DragInt_Text,         mvColor(255, 255, 255, 255), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_DragInt_Bg,           mvColor( 41,  74, 122, 138), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_DragInt_BgHovered,    mvColor( 66, 150, 250, 102), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_DragInt_BgActive,     mvColor( 66, 150, 250, 171), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_DragInt_Border,       mvColor(110, 110, 128, 128), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_DragInt_BorderShadow, mvColor(  0,   0,   0,   0), mvColor(128, 128, 128, 63)),
        MV_END_COLOR_CONSTANTS

        MV_START_STYLE_CONSTANTS
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DragInt_Rounding     , 0, 12),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DragInt_BorderSize   , 0,  1),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DragInt_PaddingX     , 4, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DragInt_PaddingY     , 3, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DragInt_InnerSpacingX, 4, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DragInt_InnerSpacingY, 4, 20),
        MV_END_STYLE_CONSTANTS

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

    public:

        mvDragInt(const std::string& name, int default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;
        void draw() override;

        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;
        
        // cpp interface
        void updateConfig(mvAppItemConfig* config) override;
        mvAppItemConfig* getConfig() override;

    private:

        float               m_speed = 1.0f;
        int                 m_min = 0;
        int                 m_max = 100;
        std::string         m_format = "%d";
        ImGuiInputTextFlags m_flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags m_stor_flags = ImGuiSliderFlags_None;
        mvDragIntConfig     m_config;
    };
        
    //-----------------------------------------------------------------------------
    // mvDragInt2
    //-----------------------------------------------------------------------------
    
    struct mvDragIntsConfig : public mvAppItemConfig
    {
        std::array<int, 4> default_value = { 0, 0, 0, 0 };
        float speed = 1.0;
        int min_value = 0;
        int max_value = 100;
        std::string format = "'%d'";
        bool no_input = false;
        bool clamped = false;
    };
    
    MV_REGISTER_WIDGET(mvDragInt2);
    class mvDragInt2 : public mvInt2PtrBase
    {

        MV_APPITEM_TYPE(mvAppItemType::mvDragInt2, "add_drag_int2")

        MV_CREATE_THEME_CONSTANT(mvThemeCol_DragInt2_Text              ,  0L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_DragInt2_Bg                ,  7L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_DragInt2_BgHovered         ,  8L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_DragInt2_BgActive          ,  9L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_DragInt2_Border            ,  5L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_DragInt2_BorderShadow      ,  6L, 0L);

        MV_CREATE_THEME_CONSTANT(mvThemeStyle_DragInt2_Rounding        , 11L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_DragInt2_BorderSize      , 12L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_DragInt2_PaddingX        , 10L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_DragInt2_PaddingY        , 10L, 1L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_DragInt2_InnerSpacingX   , 14L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_DragInt2_InnerSpacingY   , 14L, 1L);

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

        MV_START_COLOR_CONSTANTS
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_DragInt2_Text,         mvColor(255, 255, 255, 255), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_DragInt2_Bg,           mvColor( 41,  74, 122, 138), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_DragInt2_BgHovered,    mvColor( 66, 150, 250, 102), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_DragInt2_BgActive,     mvColor( 66, 150, 250, 171), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_DragInt2_Border,       mvColor(110, 110, 128, 128), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_DragInt2_BorderShadow, mvColor(  0,   0,   0,   0), mvColor(128, 128, 128, 63)),
        MV_END_COLOR_CONSTANTS

        MV_START_STYLE_CONSTANTS
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DragInt2_Rounding     , 0, 12),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DragInt2_BorderSize   , 0,  1),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DragInt2_PaddingX     , 4, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DragInt2_PaddingY     , 3, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DragInt2_InnerSpacingX, 4, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DragInt2_InnerSpacingY, 4, 20),
        MV_END_STYLE_CONSTANTS

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

    public:

        mvDragInt2(const std::string& name, int* default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;
        void draw() override;

        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;
        
        // cpp interface
        void updateConfig(mvAppItemConfig* config) override;
        mvAppItemConfig* getConfig() override;

    private:

        float               m_speed = 1.0f;
        int                 m_min = 0;
        int                 m_max = 100;
        std::string         m_format = "%d";
        ImGuiInputTextFlags m_flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags m_stor_flags = ImGuiSliderFlags_None;
        mvDragIntsConfig    m_config;
    };
        
    //-----------------------------------------------------------------------------
    // mvDragInt3
    //-----------------------------------------------------------------------------
    MV_REGISTER_WIDGET(mvDragInt3);
    class mvDragInt3 : public mvInt3PtrBase
    {

        MV_APPITEM_TYPE(mvAppItemType::mvDragInt3, "add_drag_int3")

        MV_CREATE_THEME_CONSTANT(mvThemeCol_DragInt3_Text              ,  0L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_DragInt3_Bg                ,  7L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_DragInt3_BgHovered         ,  8L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_DragInt3_BgActive          ,  9L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_DragInt3_Border            ,  5L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_DragInt3_BorderShadow      ,  6L, 0L);

        MV_CREATE_THEME_CONSTANT(mvThemeStyle_DragInt3_Rounding        , 11L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_DragInt3_BorderSize      , 12L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_DragInt3_PaddingX        , 10L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_DragInt3_PaddingY        , 10L, 1L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_DragInt3_InnerSpacingX   , 14L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_DragInt3_InnerSpacingY   , 14L, 1L);

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

        MV_START_COLOR_CONSTANTS
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_DragInt3_Text,         mvColor(255, 255, 255, 255), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_DragInt3_Bg,           mvColor( 41,  74, 122, 138), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_DragInt3_BgHovered,    mvColor( 66, 150, 250, 102), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_DragInt3_BgActive,     mvColor( 66, 150, 250, 171), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_DragInt3_Border,       mvColor(110, 110, 128, 128), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_DragInt3_BorderShadow, mvColor(  0,   0,   0,   0), mvColor(128, 128, 128, 63)),
        MV_END_COLOR_CONSTANTS

        MV_START_STYLE_CONSTANTS
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DragInt3_Rounding     , 0, 12),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DragInt3_BorderSize   , 0,  1),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DragInt3_PaddingX     , 4, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DragInt3_PaddingY     , 3, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DragInt3_InnerSpacingX, 4, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DragInt3_InnerSpacingY, 4, 20),
        MV_END_STYLE_CONSTANTS

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

    public:

        mvDragInt3(const std::string& name, int* default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;
        void draw() override;

        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;
        
        // cpp interface
        void updateConfig(mvAppItemConfig* config) override;
        mvAppItemConfig* getConfig() override;

    private:

        float               m_speed = 1.0f;
        int                 m_min = 0;
        int                 m_max = 100;
        std::string         m_format = "%d";
        ImGuiInputTextFlags m_flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags m_stor_flags = ImGuiSliderFlags_None;
        mvDragIntsConfig    m_config;
    };
        
    //-----------------------------------------------------------------------------
    // mvDragInt4
    //-----------------------------------------------------------------------------
    MV_REGISTER_WIDGET(mvDragInt4);
    class mvDragInt4 : public mvInt4PtrBase
    {

        MV_APPITEM_TYPE(mvAppItemType::mvDragInt4, "add_drag_int4")

        MV_CREATE_THEME_CONSTANT(mvThemeCol_DragInt4_Text              ,  0L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_DragInt4_Bg                ,  7L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_DragInt4_BgHovered         ,  8L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_DragInt4_BgActive          ,  9L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_DragInt4_Border            ,  5L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeCol_DragInt4_BorderShadow      ,  6L, 0L);

        MV_CREATE_THEME_CONSTANT(mvThemeStyle_DragInt4_Rounding        , 11L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_DragInt4_BorderSize      , 12L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_DragInt4_PaddingX        , 10L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_DragInt4_PaddingY        , 10L, 1L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_DragInt4_InnerSpacingX   , 14L, 0L);
        MV_CREATE_THEME_CONSTANT(mvThemeStyle_DragInt4_InnerSpacingY   , 14L, 1L);

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

        MV_START_COLOR_CONSTANTS
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_DragInt4_Text,         mvColor(255, 255, 255, 255), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_DragInt4_Bg,           mvColor( 41,  74, 122, 138), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_DragInt4_BgHovered,    mvColor( 66, 150, 250, 102), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_DragInt4_BgActive,     mvColor( 66, 150, 250, 171), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_DragInt4_Border,       mvColor(110, 110, 128, 128), mvColor(128, 128, 128, 63)),
            MV_CREATE_CONSTANT_TUPLE(mvThemeCol_DragInt4_BorderShadow, mvColor(  0,   0,   0,   0), mvColor(128, 128, 128, 63)),
        MV_END_COLOR_CONSTANTS

        MV_START_STYLE_CONSTANTS
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DragInt4_Rounding     , 0, 12),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DragInt4_BorderSize   , 0,  1),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DragInt4_PaddingX     , 4, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DragInt4_PaddingY     , 3, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DragInt4_InnerSpacingX, 4, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DragInt4_InnerSpacingY, 4, 20),
        MV_END_STYLE_CONSTANTS

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

    public:

        mvDragInt4(const std::string& name, int* default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;
        void draw() override;

        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;

        // cpp interface
        void updateConfig(mvAppItemConfig* config) override;
        mvAppItemConfig* getConfig() override;
        
    private:

        float               m_speed = 1.0f;
        int                 m_min = 0;
        int                 m_max = 100;
        std::string         m_format = "%d";
        ImGuiInputTextFlags m_flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags m_stor_flags = ImGuiSliderFlags_None;
        mvDragIntsConfig    m_config;
    };

}