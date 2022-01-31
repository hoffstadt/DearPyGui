#pragma once

#include "mvItemRegistry.h"

class mvFocusHandler : public mvAppItem
{

public:

    explicit mvFocusHandler(mvUUID uuid);

    void draw(ImDrawList* drawlist, float x, float y) override {}
    void customAction(void* data = nullptr) override;

};