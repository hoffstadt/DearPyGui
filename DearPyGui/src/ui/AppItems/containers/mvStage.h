#pragma once

#include "mvItemRegistry.h"

class mvStage : public mvAppItem
{

public:

	explicit mvStage(mvUUID uuid);

	void draw(ImDrawList* drawlist, float x, float y) override {}

};