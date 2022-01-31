#pragma once

#include "mvItemRegistry.h"

class mvToggledOpenHandler : public mvAppItem
{

public:

    explicit mvToggledOpenHandler(mvUUID uuid);

    void draw(ImDrawList* drawlist, float x, float y) override {}
    void customAction(void* data = nullptr) override;
};
