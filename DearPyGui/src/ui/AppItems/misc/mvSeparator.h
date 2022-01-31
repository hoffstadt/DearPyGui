#pragma once

#include "mvItemRegistry.h"

class mvSeparator : public mvAppItem
{

public:

    explicit mvSeparator(mvUUID uuid);

    void draw(ImDrawList* drawlist, float x, float y) override;

};