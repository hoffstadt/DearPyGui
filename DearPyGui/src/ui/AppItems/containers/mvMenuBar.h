#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"

class mvMenuBar : public mvAppItem
{

public:

    explicit mvMenuBar(mvUUID uuid);

    void draw(ImDrawList* drawlist, float x, float y) override;

};