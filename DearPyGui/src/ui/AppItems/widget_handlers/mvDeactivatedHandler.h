#pragma once

#include "mvItemRegistry.h"

class mvDeactivatedHandler : public mvAppItem
{

public:

    explicit mvDeactivatedHandler(mvUUID uuid);

    void draw(ImDrawList* drawlist, float x, float y) override {}
    void customAction(void* data = nullptr) override;

};