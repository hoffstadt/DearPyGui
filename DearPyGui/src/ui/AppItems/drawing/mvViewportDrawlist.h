#pragma once

#include "mvItemRegistry.h"

class mvViewportDrawlist : public mvAppItem
{

public:

    explicit mvViewportDrawlist(mvUUID uuid);

    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;
    void applySpecificTemplate(mvAppItem* item) override;

private:

    bool _front = true;

};