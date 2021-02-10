#pragma once

#include "mvTypeBases.h"
#include "mvApp.h"
#include "mvMarvel.h"
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
    class mvInputInt : public mvIntPtrBase
    {
        
        MV_APPITEM_TYPE(mvAppItemType::InputInt, mvInputInt, "add_input_int")

        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputInt, mvThemeCol_InputInt_Text              ,  0L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputInt, mvThemeCol_InputInt_TextHighlight     , 44L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputInt, mvThemeCol_InputInt_Bg                ,  7L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputInt, mvThemeCol_InputInt_ButtonBg          , 21L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputInt, mvThemeCol_InputInt_ButtonBgHovered   , 22L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputInt, mvThemeCol_InputInt_ButtonBgActive    , 23L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputInt, mvThemeCol_InputInt_Border            ,  5L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputInt, mvThemeCol_InputInt_BorderShadow      ,  6L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputInt, mvThemeStyle_InputInt_Rounding        , 11L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputInt, mvThemeStyle_InputInt_BorderSize      , 12L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputInt, mvThemeStyle_InputInt_PaddingX        , 10L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputInt, mvThemeStyle_InputInt_PaddingY        , 10L, 1L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputInt, mvThemeStyle_InputInt_InnerSpacingX   , 14L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputInt, mvThemeStyle_InputInt_InnerSpacingY   , 14L, 1L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputInt, mvThemeStyle_InputInt_ButtonTextAlign , 22L, 0L);

        MV_START_COLOR_CONSTANTS
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputInt_Text),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputInt_TextHighlight),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputInt_Bg),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputInt_ButtonBg),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputInt_ButtonBgHovered),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputInt_ButtonBgActive),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputInt_Border),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputInt_BorderShadow),
        MV_END_COLOR_CONSTANTS

        MV_START_STYLE_CONSTANTS
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt_Rounding         , 0, 12),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt_BorderSize       , 0,  1),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt_PaddingX         , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt_PaddingY         , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt_InnerSpacingX    , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt_InnerSpacingY    , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt_ButtonTextAlign  , 0,  1),
        MV_END_STYLE_CONSTANTS

    public:

        mvInputInt(const std::string& name, int default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;
        void draw() override;

#ifndef MV_CPP
        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP
        
    private:

        int                 m_step = 1;
        int                 m_step_fast = 100;
        int                 m_min = 0;
        int                 m_max = 100;
        bool                m_min_clamped = false;
        bool                m_max_clamped = false;
        ImGuiInputTextFlags m_flags = 0;
        ImGuiInputTextFlags m_stor_flags = 0;
        
    };

    //-----------------------------------------------------------------------------
    // mvInputInt2
    //-----------------------------------------------------------------------------
    class mvInputInt2 : public mvInt2PtrBase
    {

        MV_APPITEM_TYPE(mvAppItemType::InputInt2, mvInputInt2, "add_input_int2")

        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputInt2, mvThemeCol_InputInt2_Text            ,  0L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputInt2, mvThemeCol_InputInt2_TextHighlight   , 44L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputInt2, mvThemeCol_InputInt2_Bg              ,  7L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputInt2, mvThemeCol_InputInt2_Border          ,  5L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputInt2, mvThemeCol_InputInt2_BorderShadow    ,  6L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputInt2, mvThemeStyle_InputInt2_Rounding      , 11L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputInt2, mvThemeStyle_InputInt2_BorderSize    , 12L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputInt2, mvThemeStyle_InputInt2_PaddingX      , 10L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputInt2, mvThemeStyle_InputInt2_PaddingY      , 10L, 1L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputInt2, mvThemeStyle_InputInt2_InnerSpacingX , 14L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputInt2, mvThemeStyle_InputInt2_InnerSpacingY , 14L, 1L);

        MV_START_COLOR_CONSTANTS
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputInt2_Text),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputInt2_TextHighlight),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputInt2_Bg),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputInt2_Border),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputInt2_BorderShadow),
        MV_END_COLOR_CONSTANTS

        MV_START_STYLE_CONSTANTS
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt2_Rounding        , 0, 12),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt2_BorderSize      , 0,  1),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt2_PaddingX        , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt2_PaddingY        , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt2_InnerSpacingX   , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt2_InnerSpacingY   , 0, 20),
        MV_END_STYLE_CONSTANTS

    public:

        mvInputInt2(const std::string& name, int* default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;
        void draw() override;

#ifndef MV_CPP
        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP
        
    private:
        
        int                 m_min = 0;
        int                 m_max = 100;
        bool                m_min_clamped = false;
        bool                m_max_clamped = false;
        ImGuiInputTextFlags m_flags = 0;
        ImGuiInputTextFlags m_stor_flags = 0;
        
    };
        
    //-----------------------------------------------------------------------------
    // mvInputInt3
    //-----------------------------------------------------------------------------
    class mvInputInt3 : public mvInt3PtrBase
    {

        MV_APPITEM_TYPE(mvAppItemType::InputInt3, mvInputInt3, "add_input_int3")

        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputInt3, mvThemeCol_InputInt3_Text            ,  0L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputInt3, mvThemeCol_InputInt3_TextHighlight   , 44L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputInt3, mvThemeCol_InputInt3_Bg              ,  7L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputInt3, mvThemeCol_InputInt3_Border          ,  5L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputInt3, mvThemeCol_InputInt3_BorderShadow    ,  6L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputInt3, mvThemeStyle_InputInt3_Rounding      , 11L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputInt3, mvThemeStyle_InputInt3_BorderSize    , 12L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputInt3, mvThemeStyle_InputInt3_PaddingX      , 10L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputInt3, mvThemeStyle_InputInt3_PaddingY      , 10L, 1L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputInt3, mvThemeStyle_InputInt3_InnerSpacingX , 14L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputInt3, mvThemeStyle_InputInt3_InnerSpacingY , 14L, 1L);

        MV_START_COLOR_CONSTANTS
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputInt3_Text),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputInt3_TextHighlight),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputInt3_Bg),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputInt3_Border),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputInt3_BorderShadow),
        MV_END_COLOR_CONSTANTS

        MV_START_STYLE_CONSTANTS
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt3_Rounding        , 0, 12),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt3_BorderSize      , 0,  1),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt3_PaddingX        , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt3_PaddingY        , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt3_InnerSpacingX   , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt3_InnerSpacingY   , 0, 20),
        MV_END_STYLE_CONSTANTS

        
    public:

        mvInputInt3(const std::string& name, int* default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;
        void draw() override;

#ifndef MV_CPP
        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP
        
    private:

        int                 m_min = 0;
        int                 m_max = 100;
        bool                m_min_clamped = false;
        bool                m_max_clamped = false;
        ImGuiInputTextFlags m_flags = 0;
        ImGuiInputTextFlags m_stor_flags = 0;
        
    };
        
    //-----------------------------------------------------------------------------
    // mvInputInt4
    //-----------------------------------------------------------------------------
    class mvInputInt4 : public mvInt4PtrBase
    {
    
        MV_APPITEM_TYPE(mvAppItemType::InputInt4, mvInputInt4, "add_input_int4")

        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputInt4, mvThemeCol_InputInt4_Text            ,  0L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputInt4, mvThemeCol_InputInt4_TextHighlight   , 44L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputInt4, mvThemeCol_InputInt4_Bg              ,  7L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputInt4, mvThemeCol_InputInt4_Border          ,  5L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputInt4, mvThemeCol_InputInt4_BorderShadow    ,  6L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputInt4, mvThemeStyle_InputInt4_Rounding      , 11L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputInt4, mvThemeStyle_InputInt4_BorderSize    , 12L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputInt4, mvThemeStyle_InputInt4_PaddingX      , 10L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputInt4, mvThemeStyle_InputInt4_PaddingY      , 10L, 1L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputInt4, mvThemeStyle_InputInt4_InnerSpacingX , 14L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputInt4, mvThemeStyle_InputInt4_InnerSpacingY , 14L, 1L);

        MV_START_COLOR_CONSTANTS
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputInt4_Text),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputInt4_TextHighlight),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputInt4_Bg),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputInt4_Border),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputInt4_BorderShadow),
        MV_END_COLOR_CONSTANTS

        MV_START_STYLE_CONSTANTS
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt4_Rounding        , 0, 12),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt4_BorderSize      , 0,  1),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt4_PaddingX        , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt4_PaddingY        , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt4_InnerSpacingX   , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputInt4_InnerSpacingY   , 0, 20),
        MV_END_STYLE_CONSTANTS

    public:

        mvInputInt4(const std::string& name, int* default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;
        void draw() override;

#ifndef MV_CPP
        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP
        
    private:

        int                 m_min = 0;
        int                 m_max = 100;
        bool                m_min_clamped = false;
        bool                m_max_clamped = false;
        ImGuiInputTextFlags m_flags = 0;
        ImGuiInputTextFlags m_stor_flags = 0;
        
    };
        
    //-----------------------------------------------------------------------------
    // mvInputFloat
    //-----------------------------------------------------------------------------
    class mvInputFloat : public mvFloatPtrBase
    {
        
        MV_APPITEM_TYPE(mvAppItemType::InputFloat, mvInputFloat, "add_input_float")

        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputFloat, mvThemeCol_InputFloat_Text              ,  0L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputFloat, mvThemeCol_InputFloat_TextHighlight     , 44L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputFloat, mvThemeCol_InputFloat_Bg                ,  7L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputFloat, mvThemeCol_InputFloat_ButtonBg          , 21L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputFloat, mvThemeCol_InputFloat_ButtonBgHovered   , 22L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputFloat, mvThemeCol_InputFloat_ButtonBgActive    , 23L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputFloat, mvThemeCol_InputFloat_Border            ,  5L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputFloat, mvThemeCol_InputFloat_BorderShadow      ,  6L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputFloat, mvThemeStyle_InputFloat_Rounding        , 11L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputFloat, mvThemeStyle_InputFloat_BorderSize      , 12L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputFloat, mvThemeStyle_InputFloat_PaddingX        , 10L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputFloat, mvThemeStyle_InputFloat_PaddingY        , 10L, 1L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputFloat, mvThemeStyle_InputFloat_InnerSpacingX   , 14L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputFloat, mvThemeStyle_InputFloat_InnerSpacingY   , 14L, 1L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputFloat, mvThemeStyle_InputFloat_ButtonTextAlign , 22L, 0L);

        MV_START_COLOR_CONSTANTS
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputFloat_Text),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputFloat_TextHighlight),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputFloat_Bg),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputFloat_ButtonBg),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputFloat_ButtonBgHovered),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputFloat_ButtonBgActive),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputFloat_Border),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputFloat_BorderShadow),
        MV_END_COLOR_CONSTANTS

        MV_START_STYLE_CONSTANTS
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat_Rounding       , 0, 12),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat_BorderSize     , 0,  1),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat_PaddingX       , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat_PaddingY       , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat_InnerSpacingX  , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat_InnerSpacingY  , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat_ButtonTextAlign, 0,  1),
        MV_END_STYLE_CONSTANTS

    public:

        mvInputFloat(const std::string& name, float default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;
        void draw() override;

#ifndef MV_CPP
        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP
        
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
        
    };
        
    //-----------------------------------------------------------------------------
    // mvInputFloat2
    //-----------------------------------------------------------------------------
    class mvInputFloat2: public mvFloat2PtrBase
    {
    
        MV_APPITEM_TYPE(mvAppItemType::InputFloat2, mvInputFloat2, "add_input_float2")

        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputFloat2, mvThemeCol_InputFloat2_Text            ,  0L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputFloat2, mvThemeCol_InputFloat2_TextHighlight   , 44L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputFloat2, mvThemeCol_InputFloat2_Bg              ,  7L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputFloat2, mvThemeCol_InputFloat2_Border          ,  5L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputFloat2, mvThemeCol_InputFloat2_BorderShadow    ,  6L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputFloat2, mvThemeStyle_InputFloat2_Rounding      , 11L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputFloat2, mvThemeStyle_InputFloat2_BorderSize    , 12L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputFloat2, mvThemeStyle_InputFloat2_PaddingX      , 10L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputFloat2, mvThemeStyle_InputFloat2_PaddingY      , 10L, 1L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputFloat2, mvThemeStyle_InputFloat2_InnerSpacingX , 14L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputFloat2, mvThemeStyle_InputFloat2_InnerSpacingY , 14L, 1L);

        MV_START_COLOR_CONSTANTS
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputFloat2_Text),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputFloat2_TextHighlight),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputFloat2_Bg),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputFloat2_Border),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputFloat2_BorderShadow),
        MV_END_COLOR_CONSTANTS

        MV_START_STYLE_CONSTANTS
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat2_Rounding      , 0, 12),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat2_BorderSize    , 0,  1),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat2_PaddingX      , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat2_PaddingY      , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat2_InnerSpacingX , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat2_InnerSpacingY , 0, 20),
        MV_END_STYLE_CONSTANTS

    public:

        mvInputFloat2(const std::string& name, float* default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;
        void draw() override;

#ifndef MV_CPP
        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP
        
    private:

        float               m_min = 0.0f;
        float               m_max = 100.0f;
        bool                m_min_clamped = false;
        bool                m_max_clamped = false;
        std::string         m_format = "%.3f";
        ImGuiInputTextFlags m_flags = 0;
        ImGuiInputTextFlags m_stor_flags = 0;
        
    };
        
    //-----------------------------------------------------------------------------
    // mvInputFloat3
    //-----------------------------------------------------------------------------
    class mvInputFloat3 : public mvFloat3PtrBase
    {
        
        MV_APPITEM_TYPE(mvAppItemType::InputFloat3, mvInputFloat3, "add_input_float3")

        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputFloat3, mvThemeCol_InputFloat3_Text            ,  0L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputFloat3, mvThemeCol_InputFloat3_TextHighlight   , 44L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputFloat3, mvThemeCol_InputFloat3_Bg              ,  7L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputFloat3, mvThemeCol_InputFloat3_Border          ,  5L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputFloat3, mvThemeCol_InputFloat3_BorderShadow    ,  6L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputFloat3, mvThemeStyle_InputFloat3_Rounding      , 11L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputFloat3, mvThemeStyle_InputFloat3_BorderSize    , 12L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputFloat3, mvThemeStyle_InputFloat3_PaddingX      , 10L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputFloat3, mvThemeStyle_InputFloat3_PaddingY      , 10L, 1L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputFloat3, mvThemeStyle_InputFloat3_InnerSpacingX , 14L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputFloat3, mvThemeStyle_InputFloat3_InnerSpacingY , 14L, 1L);

        MV_START_COLOR_CONSTANTS
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputFloat3_Text),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputFloat3_TextHighlight),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputFloat3_Bg),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputFloat3_Border),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputFloat3_BorderShadow),
        MV_END_COLOR_CONSTANTS

        MV_START_STYLE_CONSTANTS
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat3_Rounding      , 0, 12),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat3_BorderSize    , 0,  1),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat3_PaddingX      , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat3_PaddingY      , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat3_InnerSpacingX , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat3_InnerSpacingY , 0, 20),
        MV_END_STYLE_CONSTANTS


    public:

        mvInputFloat3(const std::string& name, float* default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;
        void draw() override;

#ifndef MV_CPP
        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP
        
    private:

        float               m_min = 0.0f;
        float               m_max = 100.0f;
        bool                m_min_clamped = false;
        bool                m_max_clamped = false;
        std::string         m_format = "%.3f";
        ImGuiInputTextFlags m_flags = 0;
        ImGuiInputTextFlags m_stor_flags = 0;
        
    };
        
    //-----------------------------------------------------------------------------
    // mvInputFloat4
    //-----------------------------------------------------------------------------
    class mvInputFloat4 : public mvFloat4PtrBase
    {
    
        MV_APPITEM_TYPE(mvAppItemType::InputFloat4, mvInputFloat3, "add_input_float4")

        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputFloat4, mvThemeCol_InputFloat4_Text            ,  0L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputFloat4, mvThemeCol_InputFloat4_TextHighlight   , 44L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputFloat4, mvThemeCol_InputFloat4_Bg              ,  7L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputFloat4, mvThemeCol_InputFloat4_Border          ,  5L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputFloat4, mvThemeCol_InputFloat4_BorderShadow    ,  6L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputFloat4, mvThemeStyle_InputFloat4_Rounding      , 11L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputFloat4, mvThemeStyle_InputFloat4_BorderSize    , 12L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputFloat4, mvThemeStyle_InputFloat4_PaddingX      , 10L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputFloat4, mvThemeStyle_InputFloat4_PaddingY      , 10L, 1L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputFloat4, mvThemeStyle_InputFloat4_InnerSpacingX , 14L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::InputFloat4, mvThemeStyle_InputFloat4_InnerSpacingY , 14L, 1L);

        MV_START_COLOR_CONSTANTS
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputFloat4_Text),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputFloat4_TextHighlight),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputFloat4_Bg),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputFloat4_Border),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputFloat4_BorderShadow),
        MV_END_COLOR_CONSTANTS

        MV_START_STYLE_CONSTANTS
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat4_Rounding      , 0, 12),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat4_BorderSize    , 0,  1),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat4_PaddingX      , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat4_PaddingY      , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat4_InnerSpacingX , 0, 20),
            MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputFloat4_InnerSpacingY , 0, 20),
        MV_END_STYLE_CONSTANTS

    public:

        mvInputFloat4(const std::string& name, float* default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;
        void draw() override;

#ifndef MV_CPP
        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP
        
    private:

        float               m_min = 0.0f;
        float               m_max = 100.0f;
        bool                m_min_clamped = false;
        bool                m_max_clamped = false;
        std::string         m_format = "%.3f";
        ImGuiInputTextFlags m_flags = 0;
        ImGuiInputTextFlags m_stor_flags = 0;
        
    };

}
