#pragma once

#include "mvPlot.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvHeatSeries, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Series, 1);
	class mvHeatSeries : public mvSeriesBase
	{
	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvHeatSeries, add_heat_series)

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

	public:

		mvHeatSeries(const std::string& name);

		void draw(ImDrawList* drawlist, float x, float y) override;

		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:

		int         m_rows = 1;
		int         m_cols = 1;
		double      m_scale_min = 0.0;
		double      m_scale_max = 1.0;
		std::string m_format = "%0.1f";
		mvVec2      m_bounds_min = {0.0f, 0.0f};
		mvVec2      m_bounds_max = {1.0f, 1.0f};

	};

}
