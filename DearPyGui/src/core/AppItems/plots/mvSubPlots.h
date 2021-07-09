#pragma once

#include "mvTypeBases.h"
#include <implot.h>
#include <implot_internal.h>
#include <map>
#include <utility>
#include "mvCore.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvSubPlots, MV_ITEM_DESC_CONTAINER, StorageValueTypes::None, 1);
	class mvSubPlots : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvSubPlots, add_subplots)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

		mvSubPlots(mvUUID uuid);

		// overriding until we can remove these
		bool preDraw() override { return true; }
		void postDraw() override {}

		void draw (ImDrawList* drawlist, float x, float y) override;

		void onChildRemoved(mvRef<mvAppItem> item) override;
		void onChildAdd(mvRef<mvAppItem> item) override;

		void addFlag(ImPlotSubplotFlags flag);
		void removeFlag(ImPlotSubplotFlags flag);

		void handleSpecificRequiredArgs(PyObject* args) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;
		bool canChildBeAdded(mvAppItemType type) override;

	private:

		int m_rows = 1;
		int m_cols = 1;
		std::vector<float> m_row_ratios;
		std::vector<float> m_col_ratios;
		ImPlotSubplotFlags m_flags = ImPlotSubplotFlags_None;

	
	};

}
