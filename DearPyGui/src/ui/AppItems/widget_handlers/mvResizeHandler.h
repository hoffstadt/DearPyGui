#pragma once

#include "mvItemRegistry.h"

class mvResizeHandler : public mvAppItem
{

public:

    explicit mvResizeHandler(mvUUID uuid);

    void draw(ImDrawList* drawlist, float x, float y) override {}
    void customAction(void* data = nullptr) override;

};