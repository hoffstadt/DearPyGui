#pragma once

#include <array>
#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvColorMapButton);
	class mvColorMapButton : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvColorMapButton, add_colormap_button)
		MV_NO_COMMANDS
		MV_DEFAULT_PARENTS
		MV_DEFAULT_CHILDREN
		MV_NO_CONSTANTS

		MV_SET_STATES(
			MV_STATE_HOVER |
			MV_STATE_ACTIVE |
			MV_STATE_FOCUSED |
			MV_STATE_CLICKED |
			MV_STATE_VISIBLE |
			MV_STATE_ACTIVATED |
			MV_STATE_DEACTIVATED |
			MV_STATE_RECT_MIN |
			MV_STATE_RECT_MAX |
			MV_STATE_RECT_SIZE |
			MV_STATE_CONT_AVAIL
		);

	public:

		explicit mvColorMapButton(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		void applySpecificTemplate(mvAppItem* item) override;
		void setColorMap(ImPlotColormap colormap) { _colormap = colormap; }

	private:

		ImPlotColormap  _colormap = 0;

	};

}
