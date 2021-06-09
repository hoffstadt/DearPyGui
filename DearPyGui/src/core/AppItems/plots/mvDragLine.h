#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvDragLine, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Double, 0);
	class mvDragLine : public mvDoublePtrBase
	{
	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDragLine, add_drag_line)

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

	public:

		mvDragLine(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;

		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;
		bool isParentCompatible(mvAppItemType type) override;

	private:

		bool    m_show_label = true;
		mvColor m_color = mvColor(0.0f, 0.0f, 0.0f, -1.0f);
		float   m_thickness = 1.0f;
		bool    m_vertical = true;

	};

}
