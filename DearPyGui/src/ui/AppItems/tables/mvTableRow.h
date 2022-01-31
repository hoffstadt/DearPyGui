#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"
#include <stdint.h>

class mvTableRow : public mvAppItem
{

public:

    explicit mvTableRow(mvUUID uuid);

    void draw(ImDrawList* drawlist, float x, float y) override {}

private:

    int _id = 0;
};