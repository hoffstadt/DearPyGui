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

    public:

        MV_APPITEM_TYPE(mvAppItemType::DragFloat, "add_drag_float")

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

    public:

        MV_APPITEM_TYPE(mvAppItemType::DragFloat2, "add_drag_float2")

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

    public:

        MV_APPITEM_TYPE(mvAppItemType::DragFloat3, "add_drag_float3")

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

    public:

        MV_APPITEM_TYPE(mvAppItemType::DragFloat4, "add_drag_float4")

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

    public:

        MV_APPITEM_TYPE(mvAppItemType::DragInt, "add_drag_int")

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

    public:

        MV_APPITEM_TYPE(mvAppItemType::DragInt2, "add_drag_int2")

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

    public:

        MV_APPITEM_TYPE(mvAppItemType::DragInt3, "add_drag_int3")

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

    public:

        MV_APPITEM_TYPE(mvAppItemType::DragInt4, "add_drag_int4")

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