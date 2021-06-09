#pragma once

#include <utility>
#include "mvTypeBases.h"

namespace Marvel{

	MV_REGISTER_WIDGET(mvSimplePlot, MV_ITEM_DESC_DEFAULT, StorageValueTypes::FloatVect, 1);
	class mvSimplePlot : public mvFloatVectPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvSimplePlot, add_simple_plot)

		MV_CREATE_CONSTANT(mvThemeCol_SimplePlot_Text					,  0L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_SimplePlot_Border				,  5L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_SimplePlot_BorderShadow			,  6L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_SimplePlot_Bg					,  7L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_SimplePlot_PopupBg				,  4L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_SimplePlot_Lines				, 40L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_SimplePlot_LinesHovered			, 41L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_SimplePlot_Histogram			, 42L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_SimplePlot_HistogramHovered		, 43L, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_SimplePlot_Rounding			, 11L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_SimplePlot_BorderSize			, 12L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_SimplePlot_PaddingX			, 10L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_SimplePlot_PaddingY			, 10L, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_SimplePlot_InnerItemSpacingX	, 14L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_SimplePlot_InnerItemSpacingY	, 14L, 1L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_SimplePlot_Text, mvImGuiCol_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_SimplePlot_Border, mvImGuiCol_Border),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_SimplePlot_BorderShadow, mvImGuiCol_BorderShadow),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_SimplePlot_Bg, mvImGuiCol_FrameBg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_SimplePlot_PopupBg, mvImGuiCol_PopupBg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_SimplePlot_Lines, mvImGuiCol_PlotLines),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_SimplePlot_LinesHovered, mvImGuiCol_PlotLinesHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_SimplePlot_Histogram, mvImGuiCol_PlotHistogram),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_SimplePlot_HistogramHovered, mvImGuiCol_PlotHistogramHovered),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT_F(mvThemeStyle_SimplePlot_BorderSize, 0,  1),
			MV_ADD_CONSTANT_F(mvThemeStyle_SimplePlot_Rounding, 0, 12),
			MV_ADD_CONSTANT_F(mvThemeStyle_SimplePlot_PaddingX, 8, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_SimplePlot_PaddingY, 4, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_SimplePlot_InnerItemSpacingX, 4, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_SimplePlot_InnerItemSpacingY, 4, 20),
		MV_END_STYLE_CONSTANTS

	public:

		mvSimplePlot(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;

		void setValue(const std::vector<float>& value);
		void setPyValue(PyObject* value) override;
		[[nodiscard]] const std::vector<float>& getValue() const { return *m_value; }

		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:

		std::string        m_overlay;
		float              m_min = 0.0f;
		float              m_max = 0.0f;
		bool               m_histogram = false;
		bool               m_autosize = true;

	};

}