#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"
#include "dearpygui.h"
#include "cpp.hint"

class mvColorMapRegistry : public mvAppItem
{

public:

    explicit mvColorMapRegistry(mvUUID uuid);

    void draw(ImDrawList* drawlist, float x, float y) override;

};