#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvDrawing, MV_ITEM_DESC_CONTAINER, StorageValueTypes::None, 1);
	class mvDrawing : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION2(mvAppItemType::mvDrawing, add_drawing)

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

		mvDrawing(const std::string& name);

		void draw(ImDrawList* drawlist, float x, float y) override;
		bool canChildBeAdded(mvAppItemType type) override;

	private:

		float             m_startx = 0.0f;
		float             m_starty = 0.0f;

	};
}