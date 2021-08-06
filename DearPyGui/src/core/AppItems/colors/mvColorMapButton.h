#pragma once

#include <array>
#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvColorMapButton, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 1);
	class mvColorMapButton : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvColorMapButton, add_colormap_button)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		explicit mvColorMapButton(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		void setColorMap(ImPlotColormap colormap) { _colormap = colormap; }

	private:

		ImPlotColormap  _colormap = 0;

	};

}
