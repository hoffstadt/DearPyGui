#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"
#include "dearpygui.h"
#include "cpp.hint"

class mvHandlerRegistry : public mvAppItem
{

public:

    explicit mvHandlerRegistry(mvUUID uuid);

    void draw(ImDrawList* drawlist, float x, float y) override;

};