#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"
#include <stdint.h>

class mvTableCell : public mvAppItem
{

public:

    explicit mvTableCell(mvUUID uuid);

    void draw(ImDrawList* drawlist, float x, float y) override;

};