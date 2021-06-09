#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvPlotAxis, MV_ITEM_DESC_DEFAULT | MV_ITEM_DESC_CONTAINER, StorageValueTypes::None, 1);
	class mvPlotAxis : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvPlotAxis, add_plot_axis)

		MV_CREATE_RAW_CONSTANT(mvXAxis, 0);
		MV_CREATE_RAW_CONSTANT(mvYAxis, 1);

		MV_CREATE_EXTRA_COMMAND(reset_axis_ticks);
		MV_CREATE_EXTRA_COMMAND(set_axis_ticks);
		MV_CREATE_EXTRA_COMMAND(set_axis_limits);
		MV_CREATE_EXTRA_COMMAND(get_axis_limits);

		MV_START_EXTRA_COMMANDS
			MV_ADD_EXTRA_COMMAND(reset_axis_ticks);
			MV_ADD_EXTRA_COMMAND(set_axis_ticks);
			MV_ADD_EXTRA_COMMAND(set_axis_limits);
			MV_ADD_EXTRA_COMMAND(get_axis_limits);
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
			MV_ADD_GENERAL_CONSTANT(mvXAxis),
			MV_ADD_GENERAL_CONSTANT(mvYAxis)
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

	public:

		mvPlotAxis(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		void customAction() override;
		bool isParentCompatible(mvAppItemType type) override;
		bool canChildBeAdded(mvAppItemType type) override;
		void onChildRemoved(mvRef<mvAppItem> item) override;
		void onChildAdd(mvRef<mvAppItem> item) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void handleSpecificRequiredArgs(PyObject* args) override;
		void getSpecificConfiguration(PyObject* dict) override;
		void hide() override;
		void show() override;
		void updateBounds();
		void setYTicks(const std::vector<std::string>& labels, const std::vector<double>& locations);
		void resetYTicks();
		void postDraw() override;

		void setLimits(float y_min, float y_max);
		void setYLimitsAuto();

		ImPlotAxisFlags getFlags() const { return m_flags; }
		const ImVec2& getYLimits() const { return m_limits_actual; }

	private:

		ImPlotAxisFlags          m_flags = 0;
		int                      m_axis = 0;
		bool                     m_setLimits = false;
		ImVec2                   m_limits;
		ImVec2                   m_limits_actual;
		std::vector<std::string> m_labels;
		std::vector<double>      m_labelLocations;
		std::vector<const char*> m_clabels; // to prevent conversion from string to char* every frame
		bool                     m_dirty = false;

	};

}
