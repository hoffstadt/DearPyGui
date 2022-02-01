#pragma once

#include "mvItemRegistry.h"

class mvTabButton : public mvAppItem
{

public:

    explicit mvTabButton(mvUUID uuid) : mvAppItem(uuid) {}

    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;
    void applySpecificTemplate(mvAppItem* item) override;

private:

    ImGuiTabItemFlags _flags = ImGuiTabItemFlags_None;

};