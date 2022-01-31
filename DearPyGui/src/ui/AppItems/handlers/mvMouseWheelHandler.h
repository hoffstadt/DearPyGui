#pragma once

#include "mvItemRegistry.h"

class mvMouseWheelHandler : public mvAppItem
{

public:

    explicit mvMouseWheelHandler(mvUUID uuid);

    void draw(ImDrawList* drawlist, float x, float y) override;

};