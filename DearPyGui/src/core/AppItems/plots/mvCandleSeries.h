#pragma once

#include "mvPlot.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvCandleSeries, MV_ITEM_DESC_DEFAULT | MV_ITEM_DESC_CONTAINER, StorageValueTypes::Series, 1);
	class mvCandleSeries : public mvSeriesBase
	{

		public:

			static void InsertParser(std::map<std::string, mvPythonParser>*parsers);

			MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvCandleSeries, add_candle_series)


			MV_START_COMMANDS
			MV_END_COMMANDS

			MV_START_CONSTANTS
			MV_END_CONSTANTS

	public:

		mvCandleSeries(mvUUID uuid);

		// overriding until we can remove these
		bool preDraw() override { return true; }
		void postDraw() override {}

		void draw(ImDrawList* drawlist, float x, float y) override;
		bool isParentCompatible(mvAppItemType type) override;
		void handleSpecificRequiredArgs(PyObject* args) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:

		float m_weight = 0.25f;
		bool m_tooltip = true;
		mvColor m_bullColor = { 0, 255, 113, 255};
		mvColor m_bearColor = { 218, 13, 79, 255 };

	};

}
