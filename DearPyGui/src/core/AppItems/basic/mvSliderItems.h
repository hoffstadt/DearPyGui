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
    class mvSliderFloat : public mvFloatPtrBase
    {
        
    public:
        
        MV_APPITEM_TYPE_OLD_SYSTEM(mvAppItemType::SliderFloat, "add_slider_float")

        mvSliderFloat(const std::string& name, float default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;
        void draw() override;

#ifndef MV_CPP
        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP
        
    private:

        float               m_min = 0.0f;
        float               m_max = 100.0f;
        std::string         m_format = "%.3f";
        bool                m_vertical = false;
        ImGuiInputTextFlags m_flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags m_stor_flags = ImGuiSliderFlags_None;
        
    };

    //-----------------------------------------------------------------------------
    // mvSliderFloat2
    //-----------------------------------------------------------------------------
    class mvSliderFloat2 : public mvFloat2PtrBase
    {
        
    public:

        MV_APPITEM_TYPE_OLD_SYSTEM(mvAppItemType::SliderFloat2, "add_slider_float2")

        mvSliderFloat2(const std::string& name, float* default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;

        void draw() override;

#ifndef MV_CPP
        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP
        
    private:

        float               m_min = 0.0f;
        float               m_max = 100.0f;
        std::string         m_format = "%.3f";
        ImGuiInputTextFlags m_flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags m_stor_flags = ImGuiSliderFlags_None;
        
    };

    //-----------------------------------------------------------------------------
    // mvSliderFloat3
    //-----------------------------------------------------------------------------
    class mvSliderFloat3 : public mvFloat3PtrBase
    {
        
    public:
        
        MV_APPITEM_TYPE_OLD_SYSTEM(mvAppItemType::SliderFloat3, "add_slider_float3")

        mvSliderFloat3(const std::string& name, float* default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;
        void draw() override;

#ifndef MV_CPP
        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP
        
    private:

        float               m_min = 0.0f;
        float               m_max = 100.0f;
        std::string         m_format = "%.3f";
        ImGuiInputTextFlags m_flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags m_stor_flags = ImGuiSliderFlags_None;
        
    };
    
    //-----------------------------------------------------------------------------
    // mvSliderFloat4
    //-----------------------------------------------------------------------------
    class mvSliderFloat4 : public mvFloat4PtrBase
    {
          
    public:
        
        MV_APPITEM_TYPE_OLD_SYSTEM(mvAppItemType::SliderFloat4, "add_slider_float4")

        mvSliderFloat4(const std::string& name, float* default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;
        void draw() override;

#ifndef MV_CPP
        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP
        
    private:

        float               m_min = 0.0f;
        float               m_max = 100.0f;
        std::string         m_format = "%.3f";
        ImGuiInputTextFlags m_flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags m_stor_flags = ImGuiSliderFlags_None;
        
    };
     
    //-----------------------------------------------------------------------------
    // mvSliderInt
    //-----------------------------------------------------------------------------
    class mvSliderInt : public mvIntPtrBase
    {
            
    public:

        MV_APPITEM_TYPE_OLD_SYSTEM(mvAppItemType::SliderInt, "add_slider_int")

        mvSliderInt(const std::string& name, int default_value, const std::string& dataSource);
            
        void setEnabled(bool value) override;   
        void draw() override;

#ifndef MV_CPP
        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP
            
    private:

        int                 m_min = 0;
        int                 m_max = 100;
        std::string         m_format = "%d";
        bool                m_vertical = false;
        ImGuiInputTextFlags m_flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags m_stor_flags = ImGuiSliderFlags_None;
        
    };
        
    //-----------------------------------------------------------------------------
    // mvSliderInt2
    //-----------------------------------------------------------------------------
    class mvSliderInt2 : public mvInt2PtrBase
    {
        
    public:

        MV_APPITEM_TYPE_OLD_SYSTEM(mvAppItemType::SliderInt2, "add_slider_int2")

        mvSliderInt2(const std::string& name, int* default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;
        void draw() override;

#ifndef MV_CPP
        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP
        
    private:

        int                 m_min = 0;
        int                 m_max = 100;
        std::string         m_format = "%d";
        ImGuiInputTextFlags m_flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags m_stor_flags = ImGuiSliderFlags_None;
        
    };
    
    //-----------------------------------------------------------------------------
    // mvSliderInt3
    //-----------------------------------------------------------------------------
    class mvSliderInt3 : public mvInt3PtrBase
    {
        
    public:
        
        MV_APPITEM_TYPE_OLD_SYSTEM(mvAppItemType::SliderInt3, "add_slider_int3")

        mvSliderInt3(const std::string& name, int* default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;
        void draw() override;

#ifndef MV_CPP
        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP
        
    private:

        int                 m_min = 0;
        int                 m_max = 100;
        std::string         m_format = "%d";
        ImGuiInputTextFlags m_flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags m_stor_flags = ImGuiSliderFlags_None;
        
    };
        
    //-----------------------------------------------------------------------------
    // mvSliderInt4
    //-----------------------------------------------------------------------------
    class mvSliderInt4 : public mvInt4PtrBase
    {
        
    public:

        MV_APPITEM_TYPE_OLD_SYSTEM(mvAppItemType::SliderInt4, "add_slider_int4")

        mvSliderInt4(const std::string& name, int* default_value, const std::string& dataSource);
        
        void setEnabled(bool value) override;
        void draw() override;

#ifndef MV_CPP
        void setExtraConfigDict(PyObject* dict) override;
        void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP
        
    private:

        int                 m_min = 0;
        int                 m_max = 100;
        std::string         m_format = "%d";
        ImGuiInputTextFlags m_flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags m_stor_flags = ImGuiSliderFlags_None;
        
    };

}
