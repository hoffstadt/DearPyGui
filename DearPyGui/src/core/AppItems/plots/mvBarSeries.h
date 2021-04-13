#pragma once

#include "mvPlot.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvBarSeries, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Series, 1);
	class mvBarSeries : public mvSeriesBase
	{
	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvBarSeries, add_bar_series)

		MV_CREATE_CONSTANT(mvThemeCol_Plot_Bar, ImPlotCol_Line, 0L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_ADD_CONSTANT(mvThemeCol_Plot_Bar, mvColor(0, 0, 0, -255), mvColor(0, 0, 0, -255)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

	public:

		mvBarSeries(const std::string& name);

		void draw(ImDrawList* drawlist, float x, float y) override;

		void handleSpecificRequiredArgs(PyObject* args) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:

		bool m_horizontal = false;
		float m_weight = 1.0f;

	};

}
