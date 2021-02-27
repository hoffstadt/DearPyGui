#pragma once
#include "mvTypeBases.h"
#include <string>
#include "mvMarvel.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvSliderFloat
//     * mvSliderFloat2
//     * mvSliderFloat3
//     * mvSliderFloat4
//     * mvSliderInt
//     * mvSliderInt2
//     * mvSliderInt3
//     * mvSliderInt4
//
//-----------------------------------------------------------------------------

namespace Marvel {

    //-----------------------------------------------------------------------------
    // mvSliderFloat
    //-----------------------------------------------------------------------------
    
    struct mvSliderFloatConfig : public mvAppItemConfig
    {
        float       default_value   = 0.0;
        float       min_value       = 0.0;
        float       max_value       = 100.0;
        std::string format          = "'%.3f'";
        bool        vertical        = false;
        bool        no_input        = false;
        bool        clamped         = false;
    };
    
    class mvSliderFloat : public mvFloatPtrBase
    {
        
        MV_APPITEM_TYPE(mvAppItemType::SliderFloat, mvSliderFloat, "add_slider_float")

        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat, mvThemeCol_SliderFloat_Text                ,  0L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat, mvThemeCol_SliderFloat_Bg                  ,  7L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat, mvThemeCol_SliderFloat_BgHovered           ,  8L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat, mvThemeCol_SliderFloat_BgActive            ,  9L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat, mvThemeCol_SliderFloat_Grab                , 19L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat, mvThemeCol_SliderFloat_GrabActive          , 20L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat, mvThemeCol_SliderFloat_Border              ,  5L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat, mvThemeCol_SliderFloat_BorderShadow        ,  6L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat, mvThemeStyle_SliderFloat_Rounding          , 11L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat, mvThemeStyle_SliderFloat_BorderSize        , 12L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat, mvThemeStyle_SliderFloat_PaddingX          , 10L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat, mvThemeStyle_SliderFloat_PaddingY          , 10L, 1L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat, mvThemeStyle_SliderFloat_InnerSpacingX     , 14L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat, mvThemeStyle_SliderFloat_InnerSpacingY     , 14L, 1L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat, mvThemeStyle_SliderFloat_GrabMinSize       , 19L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat, mvThemeStyle_SliderFloat_GrabRounding      , 20L, 0L);

        MV_START_COLOR_CONSTANTS
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderFloat_Text),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderFloat_Bg),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderFloat_BgHovered),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderFloat_BgActive),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderFloat_Grab),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderFloat_GrabActive),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderFloat_Border),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderFloat_BorderShadow),
        MV_END_COLOR_CONSTANTS

        MV_START_STYLE_CONSTANTS
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderFloat_Rounding          , 0, 12),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderFloat_BorderSize        , 0,  1),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderFloat_PaddingX          , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderFloat_PaddingY          , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderFloat_InnerSpacingX     , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderFloat_InnerSpacingY     , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderFloat_GrabMinSize       , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderFloat_GrabRounding      , 0, 12),
        MV_END_STYLE_CONSTANTS

    public:

        mvSliderFloat(const std::string& name, float default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;
        void draw() override;

#ifndef MV_CPP
        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP
        
        // cpp interface
        void updateConfig(mvAppItemConfig* config) override;
        mvAppItemConfig* getConfig() override;

    private:

        float               m_min = 0.0f;
        float               m_max = 100.0f;
        std::string         m_format = "%.3f";
        bool                m_vertical = false;
        ImGuiInputTextFlags m_flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags m_stor_flags = ImGuiSliderFlags_None;
        mvSliderFloatConfig m_config;
    };

    //-----------------------------------------------------------------------------
    // mvSliderFloat2
    //-----------------------------------------------------------------------------
    
    struct mvSliderFloatsConfig : public mvAppItemConfig
    {
        std::array<float,4> default_value   = { 0.0f, 0.0f, 0.0f, 0.0f };
        float               min_value       = 0.0;
        float               max_value       = 100.0;
        std::string         format          = "'%.3f'";
        bool                no_input        = false;
        bool                clamped         = false;
    };
    
    class mvSliderFloat2 : public mvFloat2PtrBase
    {
        
        MV_APPITEM_TYPE(mvAppItemType::SliderFloat2, mvSliderFloat2, "add_slider_float2")

        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat2, mvThemeCol_SliderFloat2_Text              ,  0L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat2, mvThemeCol_SliderFloat2_Bg                ,  7L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat2, mvThemeCol_SliderFloat2_BgHovered         ,  8L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat2, mvThemeCol_SliderFloat2_BgActive          ,  9L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat2, mvThemeCol_SliderFloat2_Grab              , 19L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat2, mvThemeCol_SliderFloat2_GrabActive        , 20L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat2, mvThemeCol_SliderFloat2_Border            ,  5L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat2, mvThemeCol_SliderFloat2_BorderShadow      ,  6L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat2, mvThemeStyle_SliderFloat2_Rounding        , 11L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat2, mvThemeStyle_SliderFloat2_BorderSize      , 12L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat2, mvThemeStyle_SliderFloat2_PaddingX        , 10L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat2, mvThemeStyle_SliderFloat2_PaddingY        , 10L, 1L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat2, mvThemeStyle_SliderFloat2_InnerSpacingX   , 14L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat2, mvThemeStyle_SliderFloat2_InnerSpacingY   , 14L, 1L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat2, mvThemeStyle_SliderFloat2_GrabMinSize     , 19L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat2, mvThemeStyle_SliderFloat2_GrabRounding    , 20L, 0L);

        MV_START_COLOR_CONSTANTS
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderFloat2_Text),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderFloat2_Bg),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderFloat2_BgHovered),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderFloat2_BgActive),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderFloat2_Grab),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderFloat2_GrabActive),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderFloat2_Border),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderFloat2_BorderShadow),
        MV_END_COLOR_CONSTANTS

        MV_START_STYLE_CONSTANTS
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderFloat2_Rounding      , 0, 12),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderFloat2_BorderSize    , 0,  1),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderFloat2_PaddingX      , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderFloat2_PaddingY      , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderFloat2_InnerSpacingX , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderFloat2_InnerSpacingY , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderFloat2_GrabMinSize   , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderFloat2_GrabRounding  , 0, 12),
        MV_END_STYLE_CONSTANTS

    public:

        mvSliderFloat2(const std::string& name, float* default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;

        void draw() override;

#ifndef MV_CPP
        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP
        
        // cpp interface
        void updateConfig(mvAppItemConfig* config) override;
        mvAppItemConfig* getConfig() override;

    private:

        float                   m_min = 0.0f;
        float                   m_max = 100.0f;
        std::string             m_format = "%.3f";
        ImGuiInputTextFlags     m_flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags     m_stor_flags = ImGuiSliderFlags_None;
        mvSliderFloatsConfig    m_config;
    };

    //-----------------------------------------------------------------------------
    // mvSliderFloat3
    //-----------------------------------------------------------------------------
    class mvSliderFloat3 : public mvFloat3PtrBase
    {
        
        MV_APPITEM_TYPE(mvAppItemType::SliderFloat3, mvSliderFloat3, "add_slider_float3")

        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat3, mvThemeCol_SliderFloat3_Text              ,  0L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat3, mvThemeCol_SliderFloat3_Bg                ,  7L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat3, mvThemeCol_SliderFloat3_BgHovered         ,  8L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat3, mvThemeCol_SliderFloat3_BgActive          ,  9L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat3, mvThemeCol_SliderFloat3_Grab              , 19L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat3, mvThemeCol_SliderFloat3_GrabActive        , 20L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat3, mvThemeCol_SliderFloat3_Border            ,  5L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat3, mvThemeCol_SliderFloat3_BorderShadow      ,  6L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat3, mvThemeStyle_SliderFloat3_Rounding        , 11L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat3, mvThemeStyle_SliderFloat3_BorderSize      , 12L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat3, mvThemeStyle_SliderFloat3_PaddingX        , 10L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat3, mvThemeStyle_SliderFloat3_PaddingY        , 10L, 1L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat3, mvThemeStyle_SliderFloat3_InnerSpacingX   , 14L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat3, mvThemeStyle_SliderFloat3_InnerSpacingY   , 14L, 1L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat3, mvThemeStyle_SliderFloat3_GrabMinSize     , 19L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat3, mvThemeStyle_SliderFloat3_GrabRounding    , 20L, 0L);

        MV_START_COLOR_CONSTANTS
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderFloat3_Text),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderFloat3_Bg),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderFloat3_BgHovered),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderFloat3_BgActive),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderFloat3_Grab),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderFloat3_GrabActive),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderFloat3_Border),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderFloat3_BorderShadow),
        MV_END_COLOR_CONSTANTS

        MV_START_STYLE_CONSTANTS
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderFloat3_Rounding     , 0, 12),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderFloat3_BorderSize   , 0,  1),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderFloat3_PaddingX     , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderFloat3_PaddingY     , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderFloat3_InnerSpacingX, 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderFloat3_InnerSpacingY, 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderFloat3_GrabMinSize  , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderFloat3_GrabRounding , 0, 12),
        MV_END_STYLE_CONSTANTS

    public:

        mvSliderFloat3(const std::string& name, float* default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;
        void draw() override;

#ifndef MV_CPP
        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

        // cpp interface
        void updateConfig(mvAppItemConfig* config) override;
        mvAppItemConfig* getConfig() override;
        
    private:

        float                   m_min = 0.0f;
        float                   m_max = 100.0f;
        std::string             m_format = "%.3f";
        ImGuiInputTextFlags     m_flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags     m_stor_flags = ImGuiSliderFlags_None;
        mvSliderFloatsConfig    m_config;
    };
    
    //-----------------------------------------------------------------------------
    // mvSliderFloat4
    //-----------------------------------------------------------------------------
    class mvSliderFloat4 : public mvFloat4PtrBase
    {
          
        MV_APPITEM_TYPE(mvAppItemType::SliderFloat4, mvSliderFloat4, "add_slider_float4")

        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat4, mvThemeCol_SliderFloat4_Text            ,  0L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat4, mvThemeCol_SliderFloat4_Bg              ,  7L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat4, mvThemeCol_SliderFloat4_BgHovered       ,  8L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat4, mvThemeCol_SliderFloat4_BgActive        ,  9L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat4, mvThemeCol_SliderFloat4_Grab            , 19L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat4, mvThemeCol_SliderFloat4_GrabActive      , 20L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat4, mvThemeCol_SliderFloat4_Border          ,  5L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat4, mvThemeCol_SliderFloat4_BorderShadow    ,  6L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat4, mvThemeStyle_SliderFloat4_Rounding      , 11L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat4, mvThemeStyle_SliderFloat4_BorderSize    , 12L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat4, mvThemeStyle_SliderFloat4_PaddingX      , 10L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat4, mvThemeStyle_SliderFloat4_PaddingY      , 10L, 1L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat4, mvThemeStyle_SliderFloat4_InnerSpacingX , 14L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat4, mvThemeStyle_SliderFloat4_InnerSpacingY , 14L, 1L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat4, mvThemeStyle_SliderFloat4_GrabMinSize   , 19L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderFloat4, mvThemeStyle_SliderFloat4_GrabRounding  , 20L, 0L);

        MV_START_COLOR_CONSTANTS
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderFloat4_Text),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderFloat4_Bg),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderFloat4_BgHovered),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderFloat4_BgActive),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderFloat4_Grab),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderFloat4_GrabActive),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderFloat4_Border),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderFloat4_BorderShadow),
        MV_END_COLOR_CONSTANTS

        MV_START_STYLE_CONSTANTS
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderFloat4_Rounding     , 0, 12),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderFloat4_BorderSize   , 0, 1),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderFloat4_PaddingX     , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderFloat4_PaddingY     , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderFloat4_InnerSpacingX, 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderFloat4_InnerSpacingY, 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderFloat4_GrabMinSize  , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderFloat4_GrabRounding , 0, 12),
        MV_END_STYLE_CONSTANTS

    public:

        mvSliderFloat4(const std::string& name, float* default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;
        void draw() override;

#ifndef MV_CPP
        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

        // cpp interface
        void updateConfig(mvAppItemConfig* config) override;
        mvAppItemConfig* getConfig() override;
        
    private:

        float                   m_min = 0.0f;
        float                   m_max = 100.0f;
        std::string             m_format = "%.3f";
        ImGuiInputTextFlags     m_flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags     m_stor_flags = ImGuiSliderFlags_None;
        mvSliderFloatsConfig    m_config;
    };
     
    //-----------------------------------------------------------------------------
    // mvSliderInt
    //-----------------------------------------------------------------------------
    
    struct mvSliderIntConfig : public mvAppItemConfig
    {
        int         default_value;
        int         min_value;
        int         max_value   = 100;
        std::string format      = "'%d'";
        bool        vertical    = false;
        bool        no_input    = false;
        bool        clamped     = false;
    };
    
    class mvSliderInt : public mvIntPtrBase
    {
            
        MV_APPITEM_TYPE(mvAppItemType::SliderInt, mvSliderInt, "add_slider_int")

        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt, mvThemeCol_SliderInt_Text                ,  0L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt, mvThemeCol_SliderInt_Bg                  ,  7L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt, mvThemeCol_SliderInt_BgHovered           ,  8L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt, mvThemeCol_SliderInt_BgActive            ,  9L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt, mvThemeCol_SliderInt_Grab                , 19L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt, mvThemeCol_SliderInt_GrabActive          , 20L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt, mvThemeCol_SliderInt_Border              ,  5L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt, mvThemeCol_SliderInt_BorderShadow        ,  6L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt, mvThemeStyle_SliderInt_Rounding          , 11L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt, mvThemeStyle_SliderInt_BorderSize        , 12L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt, mvThemeStyle_SliderInt_PaddingX          , 10L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt, mvThemeStyle_SliderInt_PaddingY          , 10L, 1L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt, mvThemeStyle_SliderInt_InnerSpacingX     , 14L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt, mvThemeStyle_SliderInt_InnerSpacingY     , 14L, 1L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt, mvThemeStyle_SliderInt_GrabMinSize       , 19L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt, mvThemeStyle_SliderInt_GrabRounding      , 20L, 0L);


        MV_START_COLOR_CONSTANTS
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderInt_Text),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderInt_Bg),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderInt_BgHovered),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderInt_BgActive),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderInt_Grab),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderInt_GrabActive),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderInt_Border),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderInt_BorderShadow),
        MV_END_COLOR_CONSTANTS

        MV_START_STYLE_CONSTANTS
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderInt_Rounding        , 0, 12),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderInt_BorderSize      , 0,  1),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderInt_PaddingX        , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderInt_PaddingY        , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderInt_InnerSpacingX   , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderInt_InnerSpacingY   , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderInt_GrabMinSize     , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderInt_GrabRounding    , 0, 12),
        MV_END_STYLE_CONSTANTS

    public:

        mvSliderInt(const std::string& name, int default_value, const std::string& dataSource);
            
        void setEnabled(bool value) override;   
        void draw() override;

#ifndef MV_CPP
        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

        // cpp interface
        void updateConfig(mvAppItemConfig* config) override;
        mvAppItemConfig* getConfig() override;
            
    private:

        int                 m_min = 0;
        int                 m_max = 100;
        std::string         m_format = "%d";
        bool                m_vertical = false;
        ImGuiInputTextFlags m_flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags m_stor_flags = ImGuiSliderFlags_None;
        mvSliderIntConfig   m_config;
    };
        
    //-----------------------------------------------------------------------------
    // mvSliderInt2
    //-----------------------------------------------------------------------------
    
    struct mvSliderIntsConfig : public mvAppItemConfig
    {
        std::array<int, 4>  default_value   = { 0, 0, 0, 0 };
        int                 min_value;
        int                 max_value       = 100;
        std::string         format          = "'%d'";
        bool                no_input        = false;
        bool                clamped         = false;
    };
    
    class mvSliderInt2 : public mvInt2PtrBase
    {
        
        MV_APPITEM_TYPE(mvAppItemType::SliderInt2, mvSliderInt2, "add_slider_int2")

        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt2, mvThemeCol_SliderInt2_Text              ,  0L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt2, mvThemeCol_SliderInt2_Bg                ,  7L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt2, mvThemeCol_SliderInt2_BgHovered         ,  8L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt2, mvThemeCol_SliderInt2_BgActive          ,  9L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt2, mvThemeCol_SliderInt2_Grab              , 19L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt2, mvThemeCol_SliderInt2_GrabActive        , 20L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt2, mvThemeCol_SliderInt2_Border            ,  5L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt2, mvThemeCol_SliderInt2_BorderShadow      ,  6L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt2, mvThemeStyle_SliderInt2_Rounding        , 11L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt2, mvThemeStyle_SliderInt2_BorderSize      , 12L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt2, mvThemeStyle_SliderInt2_PaddingX        , 10L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt2, mvThemeStyle_SliderInt2_PaddingY        , 10L, 1L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt2, mvThemeStyle_SliderInt2_InnerSpacingX   , 14L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt2, mvThemeStyle_SliderInt2_InnerSpacingY   , 14L, 1L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt2, mvThemeStyle_SliderInt2_GrabMinSize     , 19L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt2, mvThemeStyle_SliderInt2_GrabRounding    , 20L, 0L);

        MV_START_COLOR_CONSTANTS
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderInt2_Text),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderInt2_Bg),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderInt2_BgHovered),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderInt2_BgActive),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderInt2_Grab),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderInt2_GrabActive),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderInt2_Border),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderInt2_BorderShadow),
        MV_END_COLOR_CONSTANTS

        MV_START_STYLE_CONSTANTS
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderInt2_Rounding       , 0, 12),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderInt2_BorderSize     , 0,  1),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderInt2_PaddingX       , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderInt2_PaddingY       , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderInt2_InnerSpacingX  , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderInt2_InnerSpacingY  , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderInt2_GrabMinSize    , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderInt2_GrabRounding   , 0, 12),
        MV_END_STYLE_CONSTANTS

    public:

        mvSliderInt2(const std::string& name, int* default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;
        void draw() override;

#ifndef MV_CPP
        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

        // cpp interface
        void updateConfig(mvAppItemConfig* config) override;
        mvAppItemConfig* getConfig() override;
        
    private:

        int                 m_min = 0;
        int                 m_max = 100;
        std::string         m_format = "%d";
        ImGuiInputTextFlags m_flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags m_stor_flags = ImGuiSliderFlags_None;
        mvSliderIntsConfig  m_config;
    };
    
    //-----------------------------------------------------------------------------
    // mvSliderInt3
    //-----------------------------------------------------------------------------
    class mvSliderInt3 : public mvInt3PtrBase
    {
        
        MV_APPITEM_TYPE(mvAppItemType::SliderInt3, mvSliderInt3, "add_slider_int3")

        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt3, mvThemeCol_SliderInt3_Text              ,  0L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt3, mvThemeCol_SliderInt3_Bg                ,  7L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt3, mvThemeCol_SliderInt3_BgHovered         ,  8L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt3, mvThemeCol_SliderInt3_BgActive          ,  9L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt3, mvThemeCol_SliderInt3_Grab              , 19L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt3, mvThemeCol_SliderInt3_GrabActive        , 20L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt3, mvThemeCol_SliderInt3_Border            ,  5L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt3, mvThemeCol_SliderInt3_BorderShadow      ,  6L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt3, mvThemeStyle_SliderInt3_Rounding        , 11L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt3, mvThemeStyle_SliderInt3_BorderSize      , 12L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt3, mvThemeStyle_SliderInt3_PaddingX        , 10L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt3, mvThemeStyle_SliderInt3_PaddingY        , 10L, 1L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt3, mvThemeStyle_SliderInt3_InnerSpacingX   , 14L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt3, mvThemeStyle_SliderInt3_InnerSpacingY   , 14L, 1L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt3, mvThemeStyle_SliderInt3_GrabMinSize     , 19L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt3, mvThemeStyle_SliderInt3_GrabRounding    , 20L, 0L);

        MV_START_COLOR_CONSTANTS
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderInt3_Text),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderInt3_Bg),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderInt3_BgHovered),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderInt3_BgActive),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderInt3_Grab),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderInt3_GrabActive),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderInt3_Border),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderInt3_BorderShadow),
        MV_END_COLOR_CONSTANTS

        MV_START_STYLE_CONSTANTS
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderInt3_Rounding       , 0, 12),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderInt3_BorderSize     , 0,  1),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderInt3_PaddingX       , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderInt3_PaddingY       , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderInt3_InnerSpacingX  , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderInt3_InnerSpacingY  , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderInt3_GrabMinSize    , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderInt3_GrabRounding   , 0, 12),
        MV_END_STYLE_CONSTANTS

    public:

        mvSliderInt3(const std::string& name, int* default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;
        void draw() override;

#ifndef MV_CPP
        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

        // cpp interface
        void updateConfig(mvAppItemConfig* config) override;
        mvAppItemConfig* getConfig() override;
        
    private:

        int                 m_min = 0;
        int                 m_max = 100;
        std::string         m_format = "%d";
        ImGuiInputTextFlags m_flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags m_stor_flags = ImGuiSliderFlags_None;
        mvSliderIntsConfig  m_config;
    };
        
    //-----------------------------------------------------------------------------
    // mvSliderInt4
    //-----------------------------------------------------------------------------
    class mvSliderInt4 : public mvInt4PtrBase
    {
        
        MV_APPITEM_TYPE(mvAppItemType::SliderInt4, mvSliderInt4, "add_slider_int4")

        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt4, mvThemeCol_SliderInt4_Text              ,  0L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt4, mvThemeCol_SliderInt4_Bg                ,  7L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt4, mvThemeCol_SliderInt4_BgHovered         ,  8L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt4, mvThemeCol_SliderInt4_BgActive          ,  9L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt4, mvThemeCol_SliderInt4_Grab              , 19L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt4, mvThemeCol_SliderInt4_GrabActive        , 20L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt4, mvThemeCol_SliderInt4_Border            ,  5L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt4, mvThemeCol_SliderInt4_BorderShadow      ,  6L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt4, mvThemeStyle_SliderInt4_Rounding        , 11L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt4, mvThemeStyle_SliderInt4_BorderSize      , 12L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt4, mvThemeStyle_SliderInt4_PaddingX        , 10L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt4, mvThemeStyle_SliderInt4_PaddingY        , 10L, 1L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt4, mvThemeStyle_SliderInt4_InnerSpacingX   , 14L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt4, mvThemeStyle_SliderInt4_InnerSpacingY   , 14L, 1L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt4, mvThemeStyle_SliderInt4_GrabMinSize     , 19L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::SliderInt4, mvThemeStyle_SliderInt4_GrabRounding    , 20L, 0L);

        MV_START_COLOR_CONSTANTS
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderInt4_Text),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderInt4_Bg),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderInt4_BgHovered),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderInt4_BgActive),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderInt4_Grab),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderInt4_GrabActive),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderInt4_Border),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_SliderInt4_BorderShadow),
        MV_END_COLOR_CONSTANTS

        MV_START_STYLE_CONSTANTS
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderInt4_Rounding       , 0, 12),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderInt4_BorderSize     , 0,  1),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderInt4_PaddingX       , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderInt4_PaddingY       , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderInt4_InnerSpacingX  , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderInt4_InnerSpacingY  , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderInt4_GrabMinSize    , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SliderInt4_GrabRounding   , 0, 12),
        MV_END_STYLE_CONSTANTS

    public:

        mvSliderInt4(const std::string& name, int* default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;
        void draw() override;

#ifndef MV_CPP
        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

        // cpp interface
        void updateConfig(mvAppItemConfig* config) override;
        mvAppItemConfig* getConfig() override;
        
    private:

        int                 m_min = 0;
        int                 m_max = 100;
        std::string         m_format = "%d";
        ImGuiInputTextFlags m_flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags m_stor_flags = ImGuiSliderFlags_None;
        mvSliderIntsConfig  m_config;
    };

}
