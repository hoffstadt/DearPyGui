#pragma once

#include "mvPlot.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvVLineSeries
//     * mvHLineSeries
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// mvVLineSeries
//-----------------------------------------------------------------------------
class mvVLineSeries : public mvAppItem
{

public:

    explicit mvVLineSeries(mvUUID uuid) : mvAppItem(uuid) {}

    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificRequiredArgs(PyObject* args) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;
    void applySpecificTemplate(mvAppItem* item) override;

    // values
    void setDataSource(mvUUID dataSource) override;
    void* getValue() override { return &_value; }
    PyObject* getPyValue() override;
    void setPyValue(PyObject* value) override;

private:

    mvRef<std::vector<std::vector<double>>> _value = CreateRef<std::vector<std::vector<double>>>(
        std::vector<std::vector<double>>{ std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{} });

};

//-----------------------------------------------------------------------------
// mvHLineSeries
//-----------------------------------------------------------------------------
class mvHLineSeries : public mvAppItem
{

public:

    explicit mvHLineSeries(mvUUID uuid) : mvAppItem(uuid) {}

    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificRequiredArgs(PyObject* args) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;
    void applySpecificTemplate(mvAppItem* item) override;

    // values
    void setDataSource(mvUUID dataSource) override;
    void* getValue() override { return &_value; }
    PyObject* getPyValue() override;
    void setPyValue(PyObject* value) override;

private:

    mvRef<std::vector<std::vector<double>>> _value = CreateRef<std::vector<std::vector<double>>>(
        std::vector<std::vector<double>>{ std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{} });

};