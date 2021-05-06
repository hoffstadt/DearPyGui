#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvDrawlist, MV_ITEM_DESC_CONTAINER, StorageValueTypes::None, 1);
	class mvDrawlist : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDrawlist, add_drawlist)

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

		mvDrawlist(const std::string& name);

		void draw(ImDrawList* drawlist, float x, float y) override;
		bool canChildBeAdded(mvAppItemType type) override;

	private:

		float             m_startx = 0.0f;
		float             m_starty = 0.0f;

	};
}