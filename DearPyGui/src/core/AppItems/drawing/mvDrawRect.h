#pragma once

#include "mvAppItem.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvDrawRect, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 0);
	class mvDrawRect : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDrawRect, draw_rectangle)

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

	public:

		mvDrawRect(const std::string& name, const mvVec2& pmin, const mvVec2& pmax, const mvColor& color,
			const mvColor& fill, float rounding, float thickness);

		void draw(ImDrawList* drawlist, float x, float y) override;
		bool isParentCompatible(mvAppItemType type) override;

		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;


	private:

		mvVec2  m_pmin;
		mvVec2  m_pmax;
		float   m_rounding;
		mvColor m_color;
		mvColor m_fill;
		float   m_thickness;

	};

}
