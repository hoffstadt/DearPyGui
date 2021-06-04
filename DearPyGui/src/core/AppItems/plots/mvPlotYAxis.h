#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvPlotYAxis, MV_ITEM_DESC_DEFAULT | MV_ITEM_DESC_CONTAINER, StorageValueTypes::None, 1);
	class mvPlotYAxis : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvPlotYAxis, add_plot_yaxis)

		MV_CREATE_EXTRA_COMMAND(reset_yticks);
		MV_CREATE_EXTRA_COMMAND(set_yticks);
		MV_CREATE_EXTRA_COMMAND(set_plot_ylimits);
		MV_CREATE_EXTRA_COMMAND(get_plot_ylimits);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

	public:

		mvPlotYAxis(const std::string& name);

		void draw(ImDrawList* drawlist, float x, float y) override;
		void customAction() override;
		bool isParentCompatible(mvAppItemType type) override;
		bool canChildBeAdded(mvAppItemType type) override;
		void onChildRemoved(mvRef<mvAppItem> item) override;
		void onChildAdd(mvRef<mvAppItem> item) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;
		void hide() override;
		void show() override;
		void updateBounds();
		void setYTicks(const std::vector<std::string>& labels, const std::vector<double>& locations);
		void resetYTicks();
		void postDraw() override;

		void setYLimits(float y_min, float y_max);
		void setYLimitsAuto();

		ImPlotAxisFlags getFlags() const { return m_flags; }
		const ImVec2& getYLimits() const { return m_ylimits_actual; }

	private:

		ImPlotAxisFlags          m_flags = 0;
		bool                     m_setYLimits = false;
		ImVec2                   m_ylimits;
		ImVec2                   m_ylimits_actual;
		std::vector<std::string> m_ylabels;
		std::vector<double>      m_ylabelLocations;
		std::vector<const char*> m_yclabels; // to prevent conversion from string to char* every frame
		bool                     m_dirty = false;

	};

}
