#pragma once

#include "mvPlot.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvHeatSeries, MV_ITEM_DESC_DEFAULT, StorageValueTypes::VectFloatVect, 1);
	class mvHeatSeries : public mvSeriesBase
	{
	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPITEM_TYPE(mvAppItemType::mvHeatSeries, add_heat_series)

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

	public:

		mvHeatSeries(const std::string& name, const std::vector<std::vector<float>>& default_value);

		void draw(ImDrawList* drawlist, float x, float y) override;

		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:

		int         m_rows;
		int         m_cols;
		double      m_scale_min;
		double      m_scale_max;
		std::string m_format;
		mvVec2      m_bounds_min = {0.0f, 0.0f};
		mvVec2      m_bounds_max = {1.0f, 1.0f};

	};

}
