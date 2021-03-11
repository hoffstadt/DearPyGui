#pragma once

#include <utility>
#include "mvTypeBases.h"

namespace Marvel{

#ifdef MV_CPP
#else
	PyObject* add_simple_plot(PyObject* self, PyObject* args, PyObject* kwargs);
#endif

	MV_REGISTER_WIDGET(mvSimplePlot);
	class mvSimplePlot : public mvFloatVectPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPITEM_TYPE(mvAppItemType::mvSimplePlot, "add_simple_plot")

		MV_CREATE_THEME_CONSTANT(mvThemeCol_SimplePlot_Text					,  0L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_SimplePlot_Border				,  5L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_SimplePlot_BorderShadow			,  6L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_SimplePlot_Bg					,  7L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_SimplePlot_PopupBg				,  4L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_SimplePlot_Lines				, 40L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_SimplePlot_LinesHovered			, 41L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_SimplePlot_Histogram			, 42L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_SimplePlot_HistogramHovered		, 43L, 0L);

		MV_CREATE_THEME_CONSTANT(mvThemeStyle_SimplePlot_Rounding			, 11L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_SimplePlot_BorderSize			, 12L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_SimplePlot_PaddingX			, 10L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_SimplePlot_PaddingY			, 10L, 1L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_SimplePlot_InnerItemSpacingX	, 14L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_SimplePlot_InnerItemSpacingY	, 14L, 1L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_SimplePlot_Text,				mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_SimplePlot_Border,			mvColor(110, 110, 128, 128)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_SimplePlot_BorderShadow,		mvColor(  0,   0,   0,   0)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_SimplePlot_Bg,				mvColor( 41,  74, 122, 138)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_SimplePlot_PopupBg,			mvColor( 20,  20,  20, 240)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_SimplePlot_Lines,			mvColor(156, 156, 156, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_SimplePlot_LinesHovered,		mvColor(255, 110,  89, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_SimplePlot_Histogram,		mvColor(230, 179,   0, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_SimplePlot_HistogramHovered, mvColor(255, 153,   0, 255)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SimplePlot_BorderSize			, 0,  1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SimplePlot_Rounding			, 0, 12),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SimplePlot_PaddingX			, 8, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SimplePlot_PaddingY			, 4, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SimplePlot_InnerItemSpacingX	, 4, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_SimplePlot_InnerItemSpacingY	, 4, 20),
		MV_END_STYLE_CONSTANTS

	public:
		mvSimplePlot(const std::string& name, const std::vector<float>& value);

		void draw() override;

		void setValue(const std::vector<float>& value);
		[[nodiscard]] const std::vector<float>& getValue() const { return *m_value; }

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif

	private:

		std::string        m_overlay;
		float              m_min = 0.0f;
		float              m_max = 0.0f;
		bool               m_histogram = false;

	};

}