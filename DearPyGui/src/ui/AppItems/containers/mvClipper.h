#pragma once

#include "mvItemRegistry.h"

class mvClipper : public mvAppItem
{

public:

	explicit mvClipper(mvUUID uuid);

	void draw(ImDrawList* drawlist, float x, float y) override;

};