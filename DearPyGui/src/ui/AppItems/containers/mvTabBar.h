#pragma once

#include "mvItemRegistry.h"

class mvTabBar : public mvAppItem
{

public:

    explicit mvTabBar(mvUUID uuid) : mvAppItem(uuid) {}

    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;
    void applySpecificTemplate(mvAppItem* item) override;

    // values
    void setDataSource(mvUUID dataSource) override;
    void* getValue() override { return &_value; }
    PyObject* getPyValue() override;
    void setPyValue(PyObject* value) override;

    mvUUID getSpecificValue();
    void setValue(mvUUID value);

private:
        
    mvRef<mvUUID>    _value = CreateRef<mvUUID>(0);
    mvUUID           _disabled_value = 0;
    ImGuiTabBarFlags _flags = ImGuiTabBarFlags_None;
    mvUUID           _lastValue = 0;
    mvUUID           _uiValue = 0; // value suggested from UI

};