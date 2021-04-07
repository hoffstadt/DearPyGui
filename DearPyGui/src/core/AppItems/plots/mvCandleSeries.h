#pragma once

#include "mvPlot.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvCandleSeries, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Series, 1);
	class mvCandleSeries : public mvSeriesBase
	{

		public:

			static void InsertParser(std::map<std::string, mvPythonParser>*parsers);

			MV_APPLY_WIDGET_REGISTRATION2(mvAppItemType::mvCandleSeries, add_candle_series)


			MV_START_EXTRA_COMMANDS
			MV_END_EXTRA_COMMANDS

			MV_START_GENERAL_CONSTANTS
			MV_END_GENERAL_CONSTANTS

			MV_START_COLOR_CONSTANTS
			MV_END_COLOR_CONSTANTS

			MV_START_STYLE_CONSTANTS
			MV_END_STYLE_CONSTANTS

	public:

		mvCandleSeries(const std::string& name);

		void draw(ImDrawList* drawlist, float x, float y) override;

		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:

		float m_width = 0.25f;
		bool m_tooltip = true;
		mvColor m_bullColor = { 0, 255, 113, 255};
		mvColor m_bearColor = { 218, 13, 79, 255 };

	};

}
