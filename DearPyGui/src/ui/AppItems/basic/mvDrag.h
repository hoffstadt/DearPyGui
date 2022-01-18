#pragma once

#include <string>
#include <array>
#include "mvItemRegistry.h"
#include "mvContext.h"
#include "dearpygui.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvDragFloat
//     * mvDragFloatMulti
//     * mvDragInt
//     * mvDragIntMulti
//
//-----------------------------------------------------------------------------

namespace Marvel {

    //-----------------------------------------------------------------------------
    // mvDragFloat
    //-----------------------------------------------------------------------------

    struct mvDragFloatConfig
    {
        float               speed = 1.0f;
        float               minv = 0.0f;
        float               maxv = 100.0f;
        std::string         format = "%.3f";
        ImGuiInputTextFlags flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags stor_flags = ImGuiSliderFlags_None;
    };

    class mvDragFloat : public mvAppItem
    {

    public:

        explicit mvDragFloat(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;

        // values
        void setDataSource(mvUUID dataSource) override;
        void* getValue() override { return &value; }
        PyObject* getPyValue() override;
        void setPyValue(PyObject* value) override;

        mvRef<float>      value = CreateRef<float>(0.0f);
        float             disabled_value = 0.0f;
        mvDragFloatConfig configData{};

    };

    //-----------------------------------------------------------------------------
    // mvDragFloatMulti
    //-----------------------------------------------------------------------------
    struct mvDragFloatMultiConfig
    {
        float               speed = 1.0f;
        float               minv = 0.0f;
        float               maxv = 100.0f;
        std::string         format = "%.3f";
        ImGuiInputTextFlags flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags stor_flags = ImGuiSliderFlags_None;
        int                 size = 4;
    };

    class mvDragFloatMulti : public mvAppItem
    {

    public:

        explicit mvDragFloatMulti(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;
        
        // values
        void setDataSource(mvUUID dataSource) override;
        void* getValue() override { return &value; }
        PyObject* getPyValue() override;
        void setPyValue(PyObject* value) override;

    private:

        mvRef<std::array<float, 4>> value = CreateRef<std::array<float, 4>>(std::array<float, 4>{0.0f, 0.0f, 0.0f, 0.0f});
        float                       disabled_value[4]{};
        mvDragFloatMultiConfig      configData{};

    };

    //-----------------------------------------------------------------------------
    // mvDragInt
    //----------------------------------------------------------------------------- 
    struct mvDragIntConfig
    {
        float               speed = 1.0f;
        int                 minv = 0;
        int                 maxv = 100;
        std::string         format = "%d";
        ImGuiInputTextFlags flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags stor_flags = ImGuiSliderFlags_None;
    };

    class mvDragInt : public mvAppItem
    {

    public:

        explicit mvDragInt(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;

        // values
        void setDataSource(mvUUID dataSource) override;
        void* getValue() override { return &value; }
        PyObject* getPyValue() override;
        void setPyValue(PyObject* value) override;

    private:

        mvRef<int>      value = CreateRef<int>(0);
        int             disabled_value = 0;
        mvDragIntConfig configData{};

    };

    //-----------------------------------------------------------------------------
    // mvDragIntMulti
    //-----------------------------------------------------------------------------
    struct mvDragIntMultiConfig
    {
        float               speed = 1.0f;
        int                 minv = 0;
        int                 maxv = 100;
        std::string         format = "%d";
        ImGuiInputTextFlags flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags stor_flags = ImGuiSliderFlags_None;
        int                 size = 4;
    };

    class mvDragIntMulti : public mvAppItem
    {

    public:

        explicit mvDragIntMulti(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;

        // values
        void setDataSource(mvUUID dataSource) override;
        void* getValue() override { return &value; }
        PyObject* getPyValue() override;
        void setPyValue(PyObject* value) override;

    private:

        mvRef<std::array<int, 4>> value = CreateRef<std::array<int, 4>>(std::array<int, 4>{0, 0, 0, 0});
        int                       disabled_value[4]{};
        mvDragIntMultiConfig      configData{};

    };
}
