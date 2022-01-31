#pragma once

#include "mvItemRegistry.h"

class mvViewportMenuBar : public mvAppItem
{

public:

    explicit mvViewportMenuBar(mvUUID uuid);

    void draw(ImDrawList* drawlist, float x, float y) override;

};