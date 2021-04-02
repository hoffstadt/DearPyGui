#pragma once

#include "mvAppItem.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvDrawPolyline, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 0);
	class mvDrawPolyline : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPITEM_TYPE(mvAppItemType::mvDrawPolyline, draw_polyline)

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

	public:

		mvDrawPolyline(const std::string& name, const std::vector<mvVec2>& points, const mvColor& color, bool closed,
			float thickness);

		void draw(ImDrawList* drawlist, float x, float y) override;
		bool isParentCompatible(mvAppItemType type) override;

		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;


	private:

		std::vector<mvVec2> m_points;
		mvColor             m_color;
		bool                m_closed;
		float               m_thickness;

	};

}
