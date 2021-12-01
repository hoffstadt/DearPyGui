#pragma once

#include <array>
#include "mvItemRegistry.h"

namespace Marvel {

	class mvColorMapButton : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvColorMapButton, add_colormap_button)
		MV_NO_COMMANDS
		MV_DEFAULT_PARENTS
		MV_DEFAULT_CHILDREN

	public:

		explicit mvColorMapButton(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		void applySpecificTemplate(mvAppItem* item) override;
		void setColorMap(ImPlotColormap colormap) { _colormap = colormap; }

	private:

		ImPlotColormap  _colormap = 0;

	};

}
