#pragma once

#include "mvItemRegistry.h"

class mvHoverHandler : public mvAppItem
{

public:

    explicit mvHoverHandler(mvUUID uuid);

    void draw(ImDrawList* drawlist, float x, float y) override {}
    void customAction(void* data = nullptr) override;

};