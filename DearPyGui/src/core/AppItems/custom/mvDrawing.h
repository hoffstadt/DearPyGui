#pragma once

#include "mvTypeBases.h"
#include "mvDrawList.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvDrawing);
	class mvDrawing : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		MV_APPITEM_TYPE(mvAppItemType::mvDrawing, add_drawing)

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

		mvDrawing(const std::string& name);

		void draw() override;

		mvRef<mvDrawList> getDrawList();


	private:

		mvRef<mvDrawList> m_drawList;
		float             m_startx = 0.0f;
		float             m_starty = 0.0f;

	};
}