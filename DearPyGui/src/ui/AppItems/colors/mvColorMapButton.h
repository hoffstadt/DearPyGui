#pragma once

#include <array>
#include "mvItemRegistry.h"

class mvColorMapButton : public mvAppItem
{

public:

	explicit mvColorMapButton(mvUUID uuid) : mvAppItem(uuid) {}

	void draw(ImDrawList* drawlist, float x, float y) override;
	void applySpecificTemplate(mvAppItem* item) override;
	void setColorMap(ImPlotColormap colormap) { _colormap = colormap; }

private:

	ImPlotColormap  _colormap = 0;

};