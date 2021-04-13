#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvDragPoint, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Float4, 1);
	class mvDragPoint : public mvFloat4PtrBase
	{
	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDragPoint, add_drag_point)

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

	public:

		mvDragPoint(const std::string& name);

		void draw(ImDrawList* drawlist, float x, float y) override;

		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;
		bool isParentCompatible(mvAppItemType type) override;

	private:

		bool    m_show_label = true;
		mvColor m_color = mvColor(0.0f, 0.0f, 0.0f, -1.0f);
		float   m_radius = 4.0f;

	};

}
