#pragma once

#include "mvItemRegistry.h"

class mvVisibleHandler : public mvAppItem
{

public:

    explicit mvVisibleHandler(mvUUID uuid);

    void draw(ImDrawList* drawlist, float x, float y) override {}
    void customAction(void* data = nullptr) override;

};