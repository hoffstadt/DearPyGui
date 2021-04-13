#pragma once
#include "mvTypeBases.h"
#include <string>
#include "mvModule_Core.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvSliderFloat
//     * mvSliderInt
//
//-----------------------------------------------------------------------------

namespace Marvel {

    //-----------------------------------------------------------------------------
    // mvSliderFloat
    //-----------------------------------------------------------------------------
    MV_REGISTER_WIDGET(mvSliderFloat, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Float, 1);
    class mvSliderFloat : public mvFloatPtrBase
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvSliderFloat, add_slider_float)

        MV_CREATE_CONSTANT(mvThemeCol_SliderFloat_Text                , ImGuiCol_Text              , 0L);
        MV_CREATE_CONSTANT(mvThemeCol_SliderFloat_Bg                  , ImGuiCol_FrameBg           , 0L);
        MV_CREATE_CONSTANT(mvThemeCol_SliderFloat_BgHovered           , ImGuiCol_FrameBgHovered    , 0L);
        MV_CREATE_CONSTANT(mvThemeCol_SliderFloat_BgActive            , ImGuiCol_FrameBgActive     , 0L);
        MV_CREATE_CONSTANT(mvThemeCol_SliderFloat_Grab                , ImGuiCol_SliderGrab        , 0L);
        MV_CREATE_CONSTANT(mvThemeCol_SliderFloat_GrabActive          , ImGuiCol_SliderGrabActive  , 0L);
        MV_CREATE_CONSTANT(mvThemeCol_SliderFloat_Border              , ImGuiCol_Border            , 0L);
        MV_CREATE_CONSTANT(mvThemeCol_SliderFloat_BorderShadow        , ImGuiCol_BorderShadow      , 0L);

        MV_CREATE_CONSTANT(mvThemeStyle_SliderFloat_Rounding          , ImGuiStyleVar_FrameRounding    , 0L);
        MV_CREATE_CONSTANT(mvThemeStyle_SliderFloat_BorderSize        , ImGuiStyleVar_FrameBorderSize  , 0L);
        MV_CREATE_CONSTANT(mvThemeStyle_SliderFloat_PaddingX          , ImGuiStyleVar_FramePadding     , 0L);
        MV_CREATE_CONSTANT(mvThemeStyle_SliderFloat_PaddingY          , ImGuiStyleVar_FramePadding     , 1L);
        MV_CREATE_CONSTANT(mvThemeStyle_SliderFloat_InnerSpacingX     , ImGuiStyleVar_ItemInnerSpacing , 0L);
        MV_CREATE_CONSTANT(mvThemeStyle_SliderFloat_InnerSpacingY     , ImGuiStyleVar_ItemInnerSpacing , 1L);
        MV_CREATE_CONSTANT(mvThemeStyle_SliderFloat_GrabMinSize       , ImGuiStyleVar_GrabMinSize      , 0L);
        MV_CREATE_CONSTANT(mvThemeStyle_SliderFloat_GrabRounding      , ImGuiStyleVar_GrabRounding     , 0L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

        MV_START_COLOR_CONSTANTS
            MV_ADD_CONSTANT(mvThemeCol_SliderFloat_Text,           mvColor(255, 255, 255, 255), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_SliderFloat_Bg,             mvColor( 41,  74, 122, 138), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_SliderFloat_BgHovered,      mvColor( 66, 150, 250, 102), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_SliderFloat_BgActive,		mvColor( 66, 150, 250, 171), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_SliderFloat_Grab,           mvColor( 61, 133, 224, 255), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_SliderFloat_GrabActive,     mvColor( 66, 150, 250, 255), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_SliderFloat_Border,         mvColor(110, 110, 128, 128), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_SliderFloat_BorderShadow,   mvColor(  0,   0,   0,   0), mvColor(128, 128, 128, 63)),
        MV_END_COLOR_CONSTANTS

        MV_START_STYLE_CONSTANTS
            MV_ADD_CONSTANT_F(mvThemeStyle_SliderFloat_Rounding          , 0, 12),
            MV_ADD_CONSTANT_F(mvThemeStyle_SliderFloat_BorderSize        , 0,  1),
            MV_ADD_CONSTANT_F(mvThemeStyle_SliderFloat_PaddingX          , 4, 20),
            MV_ADD_CONSTANT_F(mvThemeStyle_SliderFloat_PaddingY          , 3, 20),
            MV_ADD_CONSTANT_F(mvThemeStyle_SliderFloat_InnerSpacingX     , 4, 20),
            MV_ADD_CONSTANT_F(mvThemeStyle_SliderFloat_InnerSpacingY     , 4, 20),
            MV_ADD_CONSTANT_F(mvThemeStyle_SliderFloat_GrabMinSize       ,10, 20),
            MV_ADD_CONSTANT_F(mvThemeStyle_SliderFloat_GrabRounding      , 0, 12),
        MV_END_STYLE_CONSTANTS

    public:

        mvSliderFloat(const std::string& name);
        
        void setEnabled(bool value) override;
        void draw(ImDrawList* drawlist, float x, float y) override;

        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;

    private:

        float               m_min = 0.0f;
        float               m_max = 100.0f;
        std::string         m_format = "%.3f";
        bool                m_vertical = false;
        ImGuiInputTextFlags m_flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags m_stor_flags = ImGuiSliderFlags_None;

    };
     
    //-----------------------------------------------------------------------------
    // mvSliderInt
    //-----------------------------------------------------------------------------
    MV_REGISTER_WIDGET(mvSliderInt, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Int, 1);
    class mvSliderInt : public mvIntPtrBase
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);
 
        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvSliderInt, add_slider_int)

        MV_CREATE_CONSTANT(mvThemeCol_SliderInt_Text                , ImGuiCol_Text              , 0L);
        MV_CREATE_CONSTANT(mvThemeCol_SliderInt_Bg                  , ImGuiCol_FrameBg           , 0L);
        MV_CREATE_CONSTANT(mvThemeCol_SliderInt_BgHovered           , ImGuiCol_FrameBgHovered    , 0L);
        MV_CREATE_CONSTANT(mvThemeCol_SliderInt_BgActive            , ImGuiCol_FrameBgActive     , 0L);
        MV_CREATE_CONSTANT(mvThemeCol_SliderInt_Grab                , ImGuiCol_SliderGrab        , 0L);
        MV_CREATE_CONSTANT(mvThemeCol_SliderInt_GrabActive          , ImGuiCol_SliderGrabActive  , 0L);
        MV_CREATE_CONSTANT(mvThemeCol_SliderInt_Border              , ImGuiCol_Border            , 0L);
        MV_CREATE_CONSTANT(mvThemeCol_SliderInt_BorderShadow        , ImGuiCol_BorderShadow      , 0L);

        MV_CREATE_CONSTANT(mvThemeStyle_SliderInt_Rounding          , ImGuiStyleVar_FrameRounding    , 0L);
        MV_CREATE_CONSTANT(mvThemeStyle_SliderInt_BorderSize        , ImGuiStyleVar_FrameBorderSize  , 0L);
        MV_CREATE_CONSTANT(mvThemeStyle_SliderInt_PaddingX          , ImGuiStyleVar_FramePadding     , 0L);
        MV_CREATE_CONSTANT(mvThemeStyle_SliderInt_PaddingY          , ImGuiStyleVar_FramePadding     , 1L);
        MV_CREATE_CONSTANT(mvThemeStyle_SliderInt_InnerSpacingX     , ImGuiStyleVar_ItemInnerSpacing , 0L);
        MV_CREATE_CONSTANT(mvThemeStyle_SliderInt_InnerSpacingY     , ImGuiStyleVar_ItemInnerSpacing , 1L);
        MV_CREATE_CONSTANT(mvThemeStyle_SliderInt_GrabMinSize       , ImGuiStyleVar_GrabMinSize      , 0L);
        MV_CREATE_CONSTANT(mvThemeStyle_SliderInt_GrabRounding      , ImGuiStyleVar_GrabRounding     , 0L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

        MV_START_COLOR_CONSTANTS
            MV_ADD_CONSTANT(mvThemeCol_SliderInt_Text,             mvColor(255, 255, 255, 255), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_SliderInt_Bg,               mvColor( 41,  74, 122, 138), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_SliderInt_BgHovered,        mvColor( 66, 150, 250, 102), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_SliderInt_BgActive,         mvColor( 66, 150, 250, 171), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_SliderInt_Grab,             mvColor( 61, 133, 224, 255), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_SliderInt_GrabActive,       mvColor( 66, 150, 250, 255), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_SliderInt_Border,           mvColor(110, 110, 128, 128), mvColor(128, 128, 128, 63)),
            MV_ADD_CONSTANT(mvThemeCol_SliderInt_BorderShadow,     mvColor(  0,   0,   0,   0), mvColor(128, 128, 128, 63)),
        MV_END_COLOR_CONSTANTS

        MV_START_STYLE_CONSTANTS
            MV_ADD_CONSTANT_F(mvThemeStyle_SliderInt_Rounding        , 0, 12),
            MV_ADD_CONSTANT_F(mvThemeStyle_SliderInt_BorderSize      , 0,  1),
            MV_ADD_CONSTANT_F(mvThemeStyle_SliderInt_PaddingX        , 4, 20),
            MV_ADD_CONSTANT_F(mvThemeStyle_SliderInt_PaddingY        , 3, 20),
            MV_ADD_CONSTANT_F(mvThemeStyle_SliderInt_InnerSpacingX   , 4, 20),
            MV_ADD_CONSTANT_F(mvThemeStyle_SliderInt_InnerSpacingY   , 4, 20),
            MV_ADD_CONSTANT_F(mvThemeStyle_SliderInt_GrabMinSize     ,10, 20),
            MV_ADD_CONSTANT_F(mvThemeStyle_SliderInt_GrabRounding    , 0, 12),
        MV_END_STYLE_CONSTANTS

    public:

        mvSliderInt(const std::string& name);
            
        void setEnabled(bool value) override;   
        void draw(ImDrawList* drawlist, float x, float y) override;

        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;
            
    private:

        int                 m_min = 0;
        int                 m_max = 100;
        std::string         m_format = "%d";
        bool                m_vertical = false;
        ImGuiInputTextFlags m_flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags m_stor_flags = ImGuiSliderFlags_None;
    };


}
