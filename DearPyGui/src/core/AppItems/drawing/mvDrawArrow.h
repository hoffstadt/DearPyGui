#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvDrawArrow, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 0);
	class mvDrawArrow : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDrawArrow, draw_arrow)

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

	public:

		mvDrawArrow(const std::string& name);

		void draw(ImDrawList* drawlist, float x, float y) override;
		bool isParentCompatible(mvAppItemType type) override;

		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:

		void updatePoints();

	private:

		mvVec2              m_p1 = { 0.0f, 0.0f };
		mvVec2              m_p2 = { 0.0f, 0.0f };
		mvColor             m_color;
		float               m_thickness = 1.0f;
		float               m_size = 4;
		std::vector<mvVec2> m_points;

	};

}
