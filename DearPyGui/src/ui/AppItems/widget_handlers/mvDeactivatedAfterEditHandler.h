#pragma once

#include "mvItemRegistry.h"

class mvDeactivatedAfterEditHandler : public mvAppItem
{

public:

    explicit mvDeactivatedAfterEditHandler(mvUUID uuid);

    void draw(ImDrawList* drawlist, float x, float y) override {}
    void customAction(void* data = nullptr) override;

};