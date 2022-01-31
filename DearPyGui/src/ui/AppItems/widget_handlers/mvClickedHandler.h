#pragma once

#include "mvItemRegistry.h"

class mvClickedHandler : public mvAppItem
{

public:

    explicit mvClickedHandler(mvUUID uuid);

    void draw(ImDrawList* drawlist, float x, float y) override {}
    void customAction(void* data = nullptr) override;
    void handleSpecificRequiredArgs(PyObject* dict) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;
    void applySpecificTemplate(mvAppItem* item) override;

private:

    int _button = -1;

};