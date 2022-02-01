#pragma once

#include "mvItemRegistry.h"

class mvFilterSet : public mvAppItem
{

public:

    explicit mvFilterSet(mvUUID uuid) : mvAppItem(uuid) {}

    void draw(ImDrawList* drawlist, float x, float y) override;

    // values
    void setPyValue(PyObject* value) override;
    PyObject* getPyValue() override;

private:

    ImGuiTextFilter _imguiFilter;
};