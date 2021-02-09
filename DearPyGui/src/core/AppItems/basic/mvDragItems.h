#pragma once

#include "mvTypeBases.h"
#include "mvApp.h"
#include "mvMarvel.h"
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
    class mvDragFloat : public mvFloatPtrBase
    {

        MV_APPITEM_TYPE(mvAppItemType::DragFloat, mvDragFloat, "add_drag_float")

        MV_CREATE_THEME_CONSTANT(mvAppItemType::DragFloat, mvThemeCol_DragFloat_Text            ,  0L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::DragFloat, mvThemeCol_DragFloat_Bg              ,  7L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::DragFloat, mvThemeCol_DragFloat_BgHovered       ,  8L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::DragFloat, mvThemeCol_DragFloat_BgActive        ,  9L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::DragFloat, mvThemeCol_DragFloat_Border          ,  5L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::DragFloat, mvThemeCol_DragFloat_BorderShadow    ,  6L, 0L);

        MV_START_COLOR_CONSTANTS
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_DragFloat_Text),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_DragFloat_Bg),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_DragFloat_BgHovered),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_DragFloat_BgActive),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_DragFloat_Border),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_DragFloat_BorderShadow),
        MV_END_COLOR_CONSTANTS

    public:

        mvDragFloat(const std::string& name, float default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;
        
        void draw() override;
        
#ifndef MV_CPP
        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP
        
    private:

        float               m_speed = 1.0f;
        float               m_min = 0.0f;
        float               m_max = 100.0f;
        std::string         m_format = "%.3f";
        ImGuiInputTextFlags m_flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags m_stor_flags = ImGuiSliderFlags_None;

    };
        
    //-----------------------------------------------------------------------------
    // mvDragFloat2
    //-----------------------------------------------------------------------------
    class mvDragFloat2 : public mvFloat2PtrBase
    {

        MV_APPITEM_TYPE(mvAppItemType::DragFloat2, mvDragFloat2, "add_drag_float2")

        MV_CREATE_THEME_CONSTANT(mvAppItemType::DragFloat2, mvThemeCol_DragFloat2_Text          ,  0L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::DragFloat2, mvThemeCol_DragFloat2_Bg            ,  7L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::DragFloat2, mvThemeCol_DragFloat2_BgHovered     ,  8L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::DragFloat2, mvThemeCol_DragFloat2_BgActive      ,  9L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::DragFloat2, mvThemeCol_DragFloat2_Border        ,  5L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::DragFloat2, mvThemeCol_DragFloat2_BorderShadow  ,  6L, 0L);

        MV_START_COLOR_CONSTANTS
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_DragFloat2_Text),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_DragFloat2_Bg),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_DragFloat2_BgHovered),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_DragFloat2_BgActive),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_DragFloat2_Border),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_DragFloat2_BorderShadow),
        MV_END_COLOR_CONSTANTS

    public:

        mvDragFloat2(const std::string& name, float* default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;
        void draw() override;

#ifndef MV_CPP
        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP
        
    private:

        float               m_speed = 1.0f;
        float               m_min = 0.0f;
        float               m_max = 100.0f;
        std::string         m_format = "%.3f";
        ImGuiInputTextFlags m_flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags m_stor_flags = ImGuiSliderFlags_None;

    };
        
    //-----------------------------------------------------------------------------
    // mvDragFloat3
    //-----------------------------------------------------------------------------
    class mvDragFloat3 : public mvFloat3PtrBase
    {

        MV_APPITEM_TYPE(mvAppItemType::DragFloat3, mvDragFloat3, "add_drag_float3")

        MV_CREATE_THEME_CONSTANT(mvAppItemType::DragFloat3, mvThemeCol_DragFloat3_Text          ,  0L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::DragFloat3, mvThemeCol_DragFloat3_Bg            ,  7L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::DragFloat3, mvThemeCol_DragFloat3_BgHovered     ,  8L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::DragFloat3, mvThemeCol_DragFloat3_BgActive      ,  9L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::DragFloat3, mvThemeCol_DragFloat3_Border        ,  5L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::DragFloat3, mvThemeCol_DragFloat3_BorderShadow  ,  6L, 0L);

        MV_START_COLOR_CONSTANTS
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_DragFloat3_Text),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_DragFloat3_Bg),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_DragFloat3_BgHovered),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_DragFloat3_BgActive),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_DragFloat3_Border),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_DragFloat3_BorderShadow),
        MV_END_COLOR_CONSTANTS

    public:

        mvDragFloat3(const std::string& name, float* default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;
        void draw() override;

#ifndef MV_CPP
        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP
        
    private:

        float               m_speed = 1.0f;
        float               m_min = 0.0f;
        float               m_max = 100.0f;
        std::string         m_format = "%.3f";
        ImGuiInputTextFlags m_flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags m_stor_flags = ImGuiSliderFlags_None;

    };
        
    //-----------------------------------------------------------------------------
    // mvDragFloat4
    //-----------------------------------------------------------------------------
    class mvDragFloat4 : public mvFloat4PtrBase
    {


        MV_APPITEM_TYPE(mvAppItemType::DragFloat4, mvDragFloat4, "add_drag_float4")

        MV_CREATE_THEME_CONSTANT(mvAppItemType::DragFloat4, mvThemeCol_DragFloat4_Text          ,  0L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::DragFloat4, mvThemeCol_DragFloat4_Bg            ,  7L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::DragFloat4, mvThemeCol_DragFloat4_BgHovered     ,  8L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::DragFloat4, mvThemeCol_DragFloat4_BgActive      ,  9L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::DragFloat4, mvThemeCol_DragFloat4_Border        ,  5L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::DragFloat4, mvThemeCol_DragFloat4_BorderShadow  ,  6L, 0L);

        MV_START_COLOR_CONSTANTS
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_DragFloat4_Text),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_DragFloat4_Bg),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_DragFloat4_BgHovered),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_DragFloat4_BgActive),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_DragFloat4_Border),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_DragFloat4_BorderShadow),
        MV_END_COLOR_CONSTANTS

    public:

        mvDragFloat4(const std::string& name, float* default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;
        void draw() override;

#ifndef MV_CPP
        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP
        
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
    class mvDragInt : public mvIntPtrBase
    {

        MV_APPITEM_TYPE(mvAppItemType::DragInt, mvDragInt, "add_drag_int")

        MV_CREATE_THEME_CONSTANT(mvAppItemType::DragInt, mvThemeCol_DragInt_Text            ,  0L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::DragInt, mvThemeCol_DragInt_Bg              ,  7L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::DragInt, mvThemeCol_DragInt_BgHovered       ,  8L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::DragInt, mvThemeCol_DragInt_BgActive        ,  9L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::DragInt, mvThemeCol_DragInt_Border          ,  5L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::DragInt, mvThemeCol_DragInt_BorderShadow    ,  6L, 0L);

        MV_START_COLOR_CONSTANTS
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_DragInt_Text),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_DragInt_Bg),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_DragInt_BgHovered),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_DragInt_BgActive),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_DragInt_Border),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_DragInt_BorderShadow),
        MV_END_COLOR_CONSTANTS

    public:

        mvDragInt(const std::string& name, int default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;
        void draw() override;

#ifndef MV_CPP
        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP
        
    private:

        float               m_speed = 1.0f;
        int                 m_min = 0;
        int                 m_max = 100;
        std::string         m_format = "%d";
        ImGuiInputTextFlags m_flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags m_stor_flags = ImGuiSliderFlags_None;

    };
        
    //-----------------------------------------------------------------------------
    // mvDragInt2
    //-----------------------------------------------------------------------------
    class mvDragInt2 : public mvInt2PtrBase
    {

        MV_APPITEM_TYPE(mvAppItemType::DragInt2, mvDragInt2, "add_drag_int2")

        MV_CREATE_THEME_CONSTANT(mvAppItemType::DragInt2, mvThemeCol_DragInt2_Text          ,  0L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::DragInt2, mvThemeCol_DragInt2_Bg            ,  7L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::DragInt2, mvThemeCol_DragInt2_BgHovered     ,  8L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::DragInt2, mvThemeCol_DragInt2_BgActive      ,  9L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::DragInt2, mvThemeCol_DragInt2_Border        ,  5L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::DragInt2, mvThemeCol_DragInt2_BorderShadow  ,  6L, 0L);

        MV_START_COLOR_CONSTANTS
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_DragInt2_Text),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_DragInt2_Bg),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_DragInt2_BgHovered),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_DragInt2_BgActive),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_DragInt2_Border),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_DragInt2_BorderShadow),
        MV_END_COLOR_CONSTANTS

    public:

        mvDragInt2(const std::string& name, int* default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;
        void draw() override;

#ifndef MV_CPP
        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP
        
    private:

        float               m_speed = 1.0f;
        int                 m_min = 0;
        int                 m_max = 100;
        std::string         m_format = "%d";
        ImGuiInputTextFlags m_flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags m_stor_flags = ImGuiSliderFlags_None;

    };
        
    //-----------------------------------------------------------------------------
    // mvDragInt3
    //-----------------------------------------------------------------------------
    class mvDragInt3 : public mvInt3PtrBase
    {

        MV_APPITEM_TYPE(mvAppItemType::DragInt3, mvDragInt3, "add_drag_int3")

        MV_CREATE_THEME_CONSTANT(mvAppItemType::DragInt3, mvThemeCol_DragInt3_Text          ,  0L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::DragInt3, mvThemeCol_DragInt3_Bg            ,  7L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::DragInt3, mvThemeCol_DragInt3_BgHovered     ,  8L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::DragInt3, mvThemeCol_DragInt3_BgActive      ,  9L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::DragInt3, mvThemeCol_DragInt3_Border        ,  5L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::DragInt3, mvThemeCol_DragInt3_BorderShadow  ,  6L, 0L);

        MV_START_COLOR_CONSTANTS
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_DragInt3_Text),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_DragInt3_Bg),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_DragInt3_BgHovered),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_DragInt3_BgActive),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_DragInt3_Border),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_DragInt3_BorderShadow),
        MV_END_COLOR_CONSTANTS

    public:

        mvDragInt3(const std::string& name, int* default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;
        void draw() override;

#ifndef MV_CPP
        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP
        
    private:

        float               m_speed = 1.0f;
        int                 m_min = 0;
        int                 m_max = 100;
        std::string         m_format = "%d";
        ImGuiInputTextFlags m_flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags m_stor_flags = ImGuiSliderFlags_None;

    };
        
    //-----------------------------------------------------------------------------
    // mvDragInt4
    //-----------------------------------------------------------------------------
    class mvDragInt4 : public mvInt4PtrBase
    {

        MV_APPITEM_TYPE(mvAppItemType::DragInt4, mvDragInt4, "add_drag_int4")

        MV_CREATE_THEME_CONSTANT(mvAppItemType::DragInt4, mvThemeCol_DragInt4_Text          ,  0L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::DragInt4, mvThemeCol_DragInt4_Bg            ,  7L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::DragInt4, mvThemeCol_DragInt4_BgHovered     ,  8L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::DragInt4, mvThemeCol_DragInt4_BgActive      ,  9L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::DragInt4, mvThemeCol_DragInt4_Border        ,  5L, 0L);
        MV_CREATE_THEME_CONSTANT(mvAppItemType::DragInt4, mvThemeCol_DragInt4_BorderShadow  ,  6L, 0L);

        MV_START_COLOR_CONSTANTS
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_DragInt4_Text),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_DragInt4_Bg),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_DragInt4_BgHovered),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_DragInt4_BgActive),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_DragInt4_Border),
            MV_CREATE_CONSTANT_PAIR(mvThemeCol_DragInt4_BorderShadow),
        MV_END_COLOR_CONSTANTS

    public:

        mvDragInt4(const std::string& name, int* default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;
        void draw() override;

#ifndef MV_CPP
        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP
        
    private:

        float               m_speed = 1.0f;
        int                 m_min = 0;
        int                 m_max = 100;
        std::string         m_format = "%d";
        ImGuiInputTextFlags m_flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags m_stor_flags = ImGuiSliderFlags_None;

    };

}