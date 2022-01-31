#pragma once

#include <string>
#include <array>
#include "mvItemRegistry.h"
#include "dearpygui.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvSliderFloat
//     * mvSliderFloatMulti
//     * mvSliderInt
//     * mvSliderIntMulti
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// mvSliderFloat
//-----------------------------------------------------------------------------
class mvSliderFloat : public mvAppItem
{

public:

    explicit mvSliderFloat(mvUUID uuid);

    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;
    void applySpecificTemplate(mvAppItem* item) override;

    // values
    void setDataSource(mvUUID dataSource) override;
    void* getValue() override { return &_value; }
    PyObject* getPyValue() override;
    void setPyValue(PyObject* value) override;

private:

    mvRef<float>        _value = CreateRef<float>(0.0f);
    float               _disabled_value = 0.0f;
    float               _min = 0.0f;
    float               _max = 100.0f;
    std::string         _format = "%.3f";
    bool                _vertical = false;
    ImGuiInputTextFlags _flags = ImGuiSliderFlags_None;
    ImGuiInputTextFlags _stor_flags = ImGuiSliderFlags_None;

};
  
//-----------------------------------------------------------------------------
// mvSliderFloat4
//-----------------------------------------------------------------------------
class mvSliderFloatMulti : public mvAppItem
{

public:

    explicit mvSliderFloatMulti(mvUUID uuid);
        
    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;
    void applySpecificTemplate(mvAppItem* item) override;

    // value
    void setDataSource(mvUUID dataSource) override;
    void* getValue() override { return &_value; }
    PyObject* getPyValue() override;
    void setPyValue(PyObject* value) override;
        
private:

    mvRef<std::array<float, 4>> _value = CreateRef<std::array<float, 4>>(std::array<float, 4>{0.0f, 0.0f, 0.0f, 0.0f});
    float                       _disabled_value[4]{};
    float                       _min = 0.0f;
    float                       _max = 100.0f;
    std::string                 _format = "%.3f";
    ImGuiInputTextFlags         _flags = ImGuiSliderFlags_None;
    ImGuiInputTextFlags         _stor_flags = ImGuiSliderFlags_None;
    int                         _size = 4;
};

//-----------------------------------------------------------------------------
// mvSliderInt
//-----------------------------------------------------------------------------
class mvSliderInt : public mvAppItem
{

public:

    explicit mvSliderInt(mvUUID uuid);

    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;
    void applySpecificTemplate(mvAppItem* item) override;

    // values
    void setDataSource(mvUUID dataSource) override;
    void* getValue() override { return &_value; }
    PyObject* getPyValue() override;
    void setPyValue(PyObject* value) override;

private:

    mvRef<int>          _value = CreateRef<int>(0);
    int                 _disabled_value = 0;
    int                 _min = 0;
    int                 _max = 100;
    std::string         _format = "%d";
    bool                _vertical = false;
    ImGuiInputTextFlags _flags = ImGuiSliderFlags_None;
    ImGuiInputTextFlags _stor_flags = ImGuiSliderFlags_None;
};
        
//-----------------------------------------------------------------------------
// mvSliderIntMulti
//-----------------------------------------------------------------------------
class mvSliderIntMulti : public mvAppItem
{

public:

    explicit mvSliderIntMulti(mvUUID uuid);
        
    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;
    void applySpecificTemplate(mvAppItem* item) override;

    // values
    void setDataSource(mvUUID dataSource) override;
    void* getValue() override { return &_value; }
    PyObject* getPyValue() override;
    void setPyValue(PyObject* value) override;
        
private:
    mvRef<std::array<int, 4>> _value = CreateRef<std::array<int, 4>>(std::array<int, 4>{0, 0, 0, 0});
    int                       _disabled_value[4]{};
    int                       _min = 0;
    int                       _max = 100;
    std::string               _format = "%d";
    ImGuiInputTextFlags       _flags = ImGuiSliderFlags_None;
    ImGuiInputTextFlags       _stor_flags = ImGuiSliderFlags_None;
    int                       _size = 4;
};