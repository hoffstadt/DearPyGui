#pragma once

#include "mvItemRegistry.h"

class mvMouseMoveHandler : public mvAppItem
{

public:

    explicit mvMouseMoveHandler(mvUUID uuid);

    void draw(ImDrawList* drawlist, float x, float y) override;

private:

    ImVec2 _oldPos = {};

};