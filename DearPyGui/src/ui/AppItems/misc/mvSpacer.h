#pragma once

#include "mvItemRegistry.h"

class mvSpacer : public mvAppItem
{

public:

    explicit mvSpacer(mvUUID uuid);

    void draw(ImDrawList* drawlist, float x, float y) override;

};