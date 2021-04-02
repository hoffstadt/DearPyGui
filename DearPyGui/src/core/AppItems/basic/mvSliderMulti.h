#pragma once
#include "mvTypeBases.h"
#include <string>
#include "mvModule_Core.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvSliderFloatMulti
//     * mvSliderIntMulti
//
//-----------------------------------------------------------------------------

namespace Marvel {
  
    //-----------------------------------------------------------------------------
    // mvSliderFloat4
    //-----------------------------------------------------------------------------
    MV_REGISTER_WIDGET(mvSliderFloatMulti, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Float4, 1);
    class mvSliderFloatMulti : public mvFloat4PtrBase
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPITEM_TYPE(mvAppItemType::mvSliderFloatMulti, add_slider_floatx)

        MV_CREATE_CONSTANT(mvThemeCol_SliderFloatx_Text            ,  0L, 0L);
        MV_CREATE_CONSTANT(mvThemeCol_SliderFloatx_Bg              ,  7L, 0L);
        MV_CREATE_CONSTANT(mvThemeCol_SliderFloatx_BgHovered       ,  8L, 0L);
        MV_CREATE_CONSTANT(mvThemeCol_SliderFloatx_BgActive        ,  9L, 0L);
        MV_CREATE_CONSTANT(mvThemeCol_SliderFloatx_Grab            , 19L, 0L);
        MV_CREATE_CONSTANT(mvThemeCol_SliderFloatx_GrabActive      , 20L, 0L);
        MV_CREATE_CONSTANT(mvThemeCol_SliderFloatx_Border          ,  5L, 0L);
        MV_CREATE_CONSTANT(mvThemeCol_SliderFloatx_BorderShadow    ,  6L, 0L);

        MV_CREATE_CONSTANT(mvThemeStyle_SliderFloatx_Rounding      , 11L, 0L);
        MV_CREATE_CONSTANT(mvThemeStyle_SliderFloatx_BorderSize    , 12L, 0L);
        MV_CREATE_CONSTANT(mvThemeStyle_SliderFloatx_PaddingX      , 10L, 0L);
        MV_CREATE_CONSTANT(mvThemeStyle_SliderFloatx_PaddingY      , 10L, 1L);
        MV_CREATE_CONSTANT(mvThemeStyle_SliderFloatx_InnerSpacingX , 14L, 0L);
        MV_CREATE_CONSTANT(mvThemeStyle_SliderFloatx_InnerSpacingY , 14L, 1L);
        MV_CREATE_CONSTANT(mvThemeStyle_SliderFloatx_GrabMinSize   , 19L, 0L);
        MV_CREATE_CONSTANT(mvThemeStyle_SliderFloatx_GrabRounding  , 20L, 0L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

        MV_START_COLOR_CONSTANTS
            MV_ADD_CONSTANT(mvThemeCol_SliderFloatx_Text,          mvColor(255, 255, 255, 255), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_SliderFloatx_Bg,            mvColor( 41,  74, 122, 138), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_SliderFloatx_BgHovered,     mvColor( 66, 150, 250, 102), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_SliderFloatx_BgActive,      mvColor( 66, 150, 250, 171), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_SliderFloatx_Grab,          mvColor( 61, 133, 224, 255), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_SliderFloatx_GrabActive,    mvColor( 66, 150, 250, 255), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_SliderFloatx_Border,        mvColor(110, 110, 128, 128), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_SliderFloatx_BorderShadow,  mvColor(  0,   0,   0,   0), mvColor(128, 128, 128, 63)),
        MV_END_COLOR_CONSTANTS

        MV_START_STYLE_CONSTANTS
            MV_ADD_CONSTANT(mvThemeStyle_SliderFloatx_Rounding     , 0, 12),
            MV_ADD_CONSTANT(mvThemeStyle_SliderFloatx_BorderSize   , 0,  1),
            MV_ADD_CONSTANT(mvThemeStyle_SliderFloatx_PaddingX     , 4, 20),
            MV_ADD_CONSTANT(mvThemeStyle_SliderFloatx_PaddingY     , 3, 20),
            MV_ADD_CONSTANT(mvThemeStyle_SliderFloatx_InnerSpacingX, 4, 20),
            MV_ADD_CONSTANT(mvThemeStyle_SliderFloatx_InnerSpacingY, 4, 20),
            MV_ADD_CONSTANT(mvThemeStyle_SliderFloatx_GrabMinSize  ,10, 20),
            MV_ADD_CONSTANT(mvThemeStyle_SliderFloatx_GrabRounding , 0, 12),
        MV_END_STYLE_CONSTANTS

    public:

        mvSliderFloatMulti(const std::string& name, float* default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;
        void draw(ImDrawList* drawlist, float x, float y) override;

        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;
        
    private:

        float                   m_min = 0.0f;
        float                   m_max = 100.0f;
        std::string             m_format = "%.3f";
        ImGuiInputTextFlags     m_flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags     m_stor_flags = ImGuiSliderFlags_None;
        int                     m_size = 4;
    };
        
    //-----------------------------------------------------------------------------
    // mvSliderIntMulti
    //-----------------------------------------------------------------------------
    MV_REGISTER_WIDGET(mvSliderIntMulti, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Int4, 1);
    class mvSliderIntMulti : public mvInt4PtrBase
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPITEM_TYPE(mvAppItemType::mvSliderIntMulti, add_slider_intx)

        MV_CREATE_CONSTANT(mvThemeCol_SliderIntx_Text              ,  0L, 0L);
        MV_CREATE_CONSTANT(mvThemeCol_SliderIntx_Bg                ,  7L, 0L);
        MV_CREATE_CONSTANT(mvThemeCol_SliderIntx_BgHovered         ,  8L, 0L);
        MV_CREATE_CONSTANT(mvThemeCol_SliderIntx_BgActive          ,  9L, 0L);
        MV_CREATE_CONSTANT(mvThemeCol_SliderIntx_Grab              , 19L, 0L);
        MV_CREATE_CONSTANT(mvThemeCol_SliderIntx_GrabActive        , 20L, 0L);
        MV_CREATE_CONSTANT(mvThemeCol_SliderIntx_Border            ,  5L, 0L);
        MV_CREATE_CONSTANT(mvThemeCol_SliderIntx_BorderShadow      ,  6L, 0L);

        MV_CREATE_CONSTANT(mvThemeStyle_SliderIntx_Rounding        , 11L, 0L);
        MV_CREATE_CONSTANT(mvThemeStyle_SliderIntx_BorderSize      , 12L, 0L);
        MV_CREATE_CONSTANT(mvThemeStyle_SliderIntx_PaddingX        , 10L, 0L);
        MV_CREATE_CONSTANT(mvThemeStyle_SliderIntx_PaddingY        , 10L, 1L);
        MV_CREATE_CONSTANT(mvThemeStyle_SliderIntx_InnerSpacingX   , 14L, 0L);
        MV_CREATE_CONSTANT(mvThemeStyle_SliderIntx_InnerSpacingY   , 14L, 1L);
        MV_CREATE_CONSTANT(mvThemeStyle_SliderIntx_GrabMinSize     , 19L, 0L);
        MV_CREATE_CONSTANT(mvThemeStyle_SliderIntx_GrabRounding    , 20L, 0L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

        MV_START_COLOR_CONSTANTS
            MV_ADD_CONSTANT(mvThemeCol_SliderIntx_Text,         mvColor(255, 255, 255, 255), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_SliderIntx_Bg,           mvColor( 41,  74, 122, 138), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_SliderIntx_BgHovered,    mvColor( 66, 150, 250, 102), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_SliderIntx_BgActive,     mvColor( 66, 150, 250, 171), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_SliderIntx_Grab,         mvColor( 61, 133, 224, 255), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_SliderIntx_GrabActive,   mvColor( 66, 150, 250, 255), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_SliderIntx_Border,       mvColor(110, 110, 128, 128), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_SliderIntx_BorderShadow, mvColor(  0,   0,   0,   0), mvColor(128, 128, 128, 63)),
        MV_END_COLOR_CONSTANTS

        MV_START_STYLE_CONSTANTS
            MV_ADD_CONSTANT(mvThemeStyle_SliderIntx_Rounding       , 0, 12),
            MV_ADD_CONSTANT(mvThemeStyle_SliderIntx_BorderSize     , 0,  1),
            MV_ADD_CONSTANT(mvThemeStyle_SliderIntx_PaddingX       , 4, 20),
            MV_ADD_CONSTANT(mvThemeStyle_SliderIntx_PaddingY       , 3, 20),
            MV_ADD_CONSTANT(mvThemeStyle_SliderIntx_InnerSpacingX  , 4, 20),
            MV_ADD_CONSTANT(mvThemeStyle_SliderIntx_InnerSpacingY  , 4, 20),
            MV_ADD_CONSTANT(mvThemeStyle_SliderIntx_GrabMinSize    ,10, 20),
            MV_ADD_CONSTANT(mvThemeStyle_SliderIntx_GrabRounding   , 0, 12),
        MV_END_STYLE_CONSTANTS

    public:

        mvSliderIntMulti(const std::string& name, int* default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;
        void draw(ImDrawList* drawlist, float x, float y) override;

        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;
        
    private:

        int                 m_min = 0;
        int                 m_max = 100;
        std::string         m_format = "%d";
        ImGuiInputTextFlags m_flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags m_stor_flags = ImGuiSliderFlags_None;
        int                 m_size = 4;
    };

}
