#pragma once

#include "mvTypeBases.h"
#include <implot.h>
#include <implot_internal.h>
#include <map>
#include <utility>
#include "mvCore.h"
#include "mvPython.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvPlot, MV_ITEM_DESC_CONTAINER, StorageValueTypes::None, 1);
	class mvPlot : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvPlot, add_plot_stack)

		MV_CREATE_EXTRA_COMMAND(add_plot);
		MV_CREATE_EXTRA_COMMAND(reset_xticks);
		MV_CREATE_EXTRA_COMMAND(reset_yticks);
		MV_CREATE_EXTRA_COMMAND(set_xticks);
		MV_CREATE_EXTRA_COMMAND(set_yticks);
		MV_CREATE_EXTRA_COMMAND(is_plot_queried);
		MV_CREATE_EXTRA_COMMAND(get_plot_query_area);
		MV_CREATE_EXTRA_COMMAND(set_plot_xlimits_auto);
		MV_CREATE_EXTRA_COMMAND(set_plot_ylimits_auto);
		MV_CREATE_EXTRA_COMMAND(set_plot_xlimits);
		MV_CREATE_EXTRA_COMMAND(set_plot_ylimits);
		MV_CREATE_EXTRA_COMMAND(get_plot_xlimits);
		MV_CREATE_EXTRA_COMMAND(get_plot_ylimits);

		//-----------------------------------------------------------------------------
		// Plot Marker Specifications
		//-----------------------------------------------------------------------------
		MV_CREATE_CONSTANT(mvPlotMarker_None    , -1L, 0L);  // no marker
		MV_CREATE_CONSTANT(mvPlotMarker_Circle  ,  0L, 0L);  // a circle marker will be rendered at each point
		MV_CREATE_CONSTANT(mvPlotMarker_Square  ,  1L, 0L);  // a square maker will be rendered at each point
		MV_CREATE_CONSTANT(mvPlotMarker_Diamond ,  2L, 0L);  // a diamond marker will be rendered at each point
		MV_CREATE_CONSTANT(mvPlotMarker_Up      ,  3L, 0L);  // an upward-pointing triangle marker will up rendered at each point
		MV_CREATE_CONSTANT(mvPlotMarker_Down    ,  4L, 0L);  // an downward-pointing triangle marker will up rendered at each point
		MV_CREATE_CONSTANT(mvPlotMarker_Left    ,  5L, 0L);  // an leftward-pointing triangle marker will up rendered at each point
		MV_CREATE_CONSTANT(mvPlotMarker_Right   ,  6L, 0L);  // an rightward-pointing triangle marker will up rendered at each point
		MV_CREATE_CONSTANT(mvPlotMarker_Cross   ,  7L, 0L);  // a cross marker will be rendered at each point (not filled)
		MV_CREATE_CONSTANT(mvPlotMarker_Plus    ,  8L, 0L);  // a plus marker will be rendered at each point (not filled)
		MV_CREATE_CONSTANT(mvPlotMarker_Asterisk,  9L, 0L); // a asterisk marker will be rendered at each point (not filled)

		//-----------------------------------------------------------------------------
		// Built-in ImPlot Color maps
		//-----------------------------------------------------------------------------
		MV_CREATE_CONSTANT(mvPlotColormap_Default,   0L, 0L); // ImPlot default colormap         (n=10)
		MV_CREATE_CONSTANT(mvPlotColormap_Deep   ,   1L, 0L); // a.k.a. matplotlib "Set1"        (n=9)
		MV_CREATE_CONSTANT(mvPlotColormap_Dark   ,   2L, 0L); // a.k.a. matplotlib "Set1"        (n=9)
		MV_CREATE_CONSTANT(mvPlotColormap_Pastel ,   3L, 0L); // a.k.a. matplotlib "Pastel1"     (n=9)
		MV_CREATE_CONSTANT(mvPlotColormap_Paired ,   4L, 0L); // a.k.a. matplotlib "Paired"      (n=12)
		MV_CREATE_CONSTANT(mvPlotColormap_Viridis,   5L, 0L); // a.k.a. matplotlib "viridis"     (n=11)
		MV_CREATE_CONSTANT(mvPlotColormap_Plasma ,   6L, 0L); // a.k.a. matplotlib "plasma"      (n=11)
		MV_CREATE_CONSTANT(mvPlotColormap_Hot    ,   7L, 0L); // a.k.a. matplotlib/MATLAB "hot"  (n=11)
		MV_CREATE_CONSTANT(mvPlotColormap_Cool   ,   8L, 0L); // a.k.a. matplotlib/MATLAB "cool" (n=11)
		MV_CREATE_CONSTANT(mvPlotColormap_Pink   ,   9L, 0L); // a.k.a. matplotlib/MATLAB "pink" (n=11)
		MV_CREATE_CONSTANT(mvPlotColormap_Jet    ,  10L, 0L); // a.k.a. MATLAB "jet"             (n=11)

		MV_CREATE_CONSTANT(mvThemeCol_Plot_FrameBg,					 5L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Plot_PlotBg,					 6L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Plot_PlotBorder,				 7L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Plot_LegendBg,					 8L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Plot_LegendBorder,				 9L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Plot_LegendText,				10L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Plot_TitleText,					11L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Plot_InlayText,					12L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Plot_XAxis,						13L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Plot_XAxisGrid,					14L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Plot_YAxis,						15L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Plot_YAxisGrid,					16L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Plot_YAxis2,					17L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Plot_YAxisGrid2,				18L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Plot_YAxis3,					19L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Plot_YAxisGrid3,				20L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Plot_Selection,					21L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Plot_Query,						22L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Plot_Crosshairs,				23L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Plot_FillAlpha,				 4L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Plot_BorderSize,				 9L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Plot_MinorAlpha,				10L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Plot_MajorTickLenX,			11L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Plot_MajorTickLenY,			11L, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Plot_MinorTickLenX,			12L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Plot_MinorTickLenY,			12L, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Plot_MajorTickSizeX,			13L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Plot_MajorTickSizeY,			13L, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Plot_MinorTickSizeX,			14L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Plot_MinorTickSizeY,			14L, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Plot_MajorGridSizeX,			15L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Plot_MajorGridSizeY,			15L, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Plot_MinorGridSizeX,			16L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Plot_MinorGridSizeY,			16L, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Plot_PlotPaddingX,			17L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Plot_PlotPaddingY,			17L, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Plot_LabelPaddingX,			18L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Plot_LabelPaddingY,			18L, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Plot_LegendPaddingX,			19L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Plot_LegendPaddingY,			19L, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Plot_LegendInnerPaddingX,		20L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Plot_LegendInnerPaddingY,		20L, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Plot_LegendSpacingX,			21L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Plot_LegendSpacingY,			21L, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Plot_MousePosPaddingX,		22L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Plot_MousePosPaddingY,		22L, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Plot_AnnotationPaddingX,		23L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Plot_AnnotationPaddingY,		23L, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Plot_FitPaddingX,				24L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Plot_FitPaddingY,				24L, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Plot_PlotDefaultSizeX,		25L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Plot_PlotDefaultSizeY,		25L, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Plot_PlotMinSizeX,			26L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Plot_PlotMinSizeY,			26L, 1L);

		MV_START_EXTRA_COMMANDS
			MV_ADD_EXTRA_COMMAND(add_plot);
			MV_ADD_EXTRA_COMMAND(reset_xticks);
			MV_ADD_EXTRA_COMMAND(reset_yticks);
			MV_ADD_EXTRA_COMMAND(set_xticks);
			MV_ADD_EXTRA_COMMAND(set_yticks);
			MV_ADD_EXTRA_COMMAND(is_plot_queried);
			MV_ADD_EXTRA_COMMAND(get_plot_query_area);
			MV_ADD_EXTRA_COMMAND(set_plot_xlimits_auto);
			MV_ADD_EXTRA_COMMAND(set_plot_ylimits_auto);
			MV_ADD_EXTRA_COMMAND(set_plot_xlimits);
			MV_ADD_EXTRA_COMMAND(set_plot_ylimits);
			MV_ADD_EXTRA_COMMAND(get_plot_xlimits);
			MV_ADD_EXTRA_COMMAND(get_plot_ylimits);
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_ADD_GENERAL_CONSTANT(mvPlotMarker_None),
		MV_ADD_GENERAL_CONSTANT(mvPlotMarker_Circle),
		MV_ADD_GENERAL_CONSTANT(mvPlotMarker_Square),
		MV_ADD_GENERAL_CONSTANT(mvPlotMarker_Diamond),
		MV_ADD_GENERAL_CONSTANT(mvPlotMarker_Up),
		MV_ADD_GENERAL_CONSTANT(mvPlotMarker_Down),
		MV_ADD_GENERAL_CONSTANT(mvPlotMarker_Left),
		MV_ADD_GENERAL_CONSTANT(mvPlotMarker_Right),
		MV_ADD_GENERAL_CONSTANT(mvPlotMarker_Cross),
		MV_ADD_GENERAL_CONSTANT(mvPlotMarker_Plus),
		MV_ADD_GENERAL_CONSTANT(mvPlotMarker_Asterisk),
		MV_ADD_GENERAL_CONSTANT(mvPlotColormap_Default),
		MV_ADD_GENERAL_CONSTANT(mvPlotColormap_Deep),
		MV_ADD_GENERAL_CONSTANT(mvPlotColormap_Dark),
		MV_ADD_GENERAL_CONSTANT(mvPlotColormap_Pastel),
		MV_ADD_GENERAL_CONSTANT(mvPlotColormap_Paired),
		MV_ADD_GENERAL_CONSTANT(mvPlotColormap_Viridis),
		MV_ADD_GENERAL_CONSTANT(mvPlotColormap_Plasma),
		MV_ADD_GENERAL_CONSTANT(mvPlotColormap_Hot),
		MV_ADD_GENERAL_CONSTANT(mvPlotColormap_Cool),
		MV_ADD_GENERAL_CONSTANT(mvPlotColormap_Pink),
		MV_ADD_GENERAL_CONSTANT(mvPlotColormap_Jet),
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Plot_FrameBg,      mvColor(255, 255, 255,  18)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Plot_PlotBg,       mvColor(  0,   0,   0, 128)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Plot_PlotBorder,   mvColor(110, 110, 128, 128)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Plot_LegendBg,     mvColor( 20,  20,  20, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Plot_LegendBorder, mvColor(110, 110, 128, 128)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Plot_LegendText,   mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Plot_TitleText,    mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Plot_InlayText,    mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Plot_XAxis,        mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Plot_XAxisGrid,    mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Plot_YAxis,        mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Plot_YAxisGrid,    mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Plot_YAxis2,       mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Plot_YAxisGrid2,   mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Plot_YAxis3,       mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Plot_YAxisGrid3,   mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Plot_Selection,    mvColor(255, 153,   0, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Plot_Query,        mvColor(  0, 255, 112, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Plot_Crosshairs,   mvColor(255, 255, 255, 128)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT(mvThemeStyle_Plot_FillAlpha,			   1,   1),
			MV_ADD_CONSTANT(mvThemeStyle_Plot_BorderSize,			   1,  10),
			MV_ADD_CONSTANT(mvThemeStyle_Plot_MinorAlpha,			0.25,   1),
			MV_ADD_CONSTANT(mvThemeStyle_Plot_MajorTickLenX,		  10,  20),
			MV_ADD_CONSTANT(mvThemeStyle_Plot_MajorTickLenY,		  10,  20),
			MV_ADD_CONSTANT(mvThemeStyle_Plot_MinorTickLenX,		   5,  10),
			MV_ADD_CONSTANT(mvThemeStyle_Plot_MinorTickLenY,		   5,  10),
			MV_ADD_CONSTANT(mvThemeStyle_Plot_MajorTickSizeX,		   1,  10),
			MV_ADD_CONSTANT(mvThemeStyle_Plot_MajorTickSizeY,		   1,  10),
			MV_ADD_CONSTANT(mvThemeStyle_Plot_MinorTickSizeX,		   1,  10),
			MV_ADD_CONSTANT(mvThemeStyle_Plot_MinorTickSizeY,		   1,  10),
			MV_ADD_CONSTANT(mvThemeStyle_Plot_MajorGridSizeX,		   1,  10),
			MV_ADD_CONSTANT(mvThemeStyle_Plot_MajorGridSizeY,		   1,  10),
			MV_ADD_CONSTANT(mvThemeStyle_Plot_MinorGridSizeX,		   1,  10),
			MV_ADD_CONSTANT(mvThemeStyle_Plot_MinorGridSizeY,		   1,  10),
			MV_ADD_CONSTANT(mvThemeStyle_Plot_PlotPaddingX,		  10,  10),
			MV_ADD_CONSTANT(mvThemeStyle_Plot_PlotPaddingY,		  10,  10),
			MV_ADD_CONSTANT(mvThemeStyle_Plot_LabelPaddingX,		   5,  10),
			MV_ADD_CONSTANT(mvThemeStyle_Plot_LabelPaddingY,		   5,  10),
			MV_ADD_CONSTANT(mvThemeStyle_Plot_LegendPaddingX,		  10,  20),
			MV_ADD_CONSTANT(mvThemeStyle_Plot_LegendPaddingY,		  10,  20),
			MV_ADD_CONSTANT(mvThemeStyle_Plot_LegendInnerPaddingX,	   5,  10),
			MV_ADD_CONSTANT(mvThemeStyle_Plot_LegendInnerPaddingY,	   5,  10),
			MV_ADD_CONSTANT(mvThemeStyle_Plot_LegendSpacingX,		   5,  10),
			MV_ADD_CONSTANT(mvThemeStyle_Plot_LegendSpacingY,		   0,  10),
			MV_ADD_CONSTANT(mvThemeStyle_Plot_MousePosPaddingX,	  10,  20),
			MV_ADD_CONSTANT(mvThemeStyle_Plot_MousePosPaddingY,	  10,  20),
			MV_ADD_CONSTANT(mvThemeStyle_Plot_AnnotationPaddingX,	   2,   4),
			MV_ADD_CONSTANT(mvThemeStyle_Plot_AnnotationPaddingY,	   2,   4),
			MV_ADD_CONSTANT(mvThemeStyle_Plot_FitPaddingX,			   0,   1),
			MV_ADD_CONSTANT(mvThemeStyle_Plot_FitPaddingY,			   0,   1),
			MV_ADD_CONSTANT(mvThemeStyle_Plot_PlotDefaultSizeX,	 400, 800),
			MV_ADD_CONSTANT(mvThemeStyle_Plot_PlotDefaultSizeY,	 300, 600),
			MV_ADD_CONSTANT(mvThemeStyle_Plot_PlotMinSizeX,		 300, 600),
			MV_ADD_CONSTANT(mvThemeStyle_Plot_PlotMinSizeY,		 255, 510),
		MV_END_STYLE_CONSTANTS

		mvPlot(const std::string& name);

		// settings
		void SetColorMap    (ImPlotColormap colormap);
		void resetXTicks    ();
		void resetYTicks    ();
		void setXTicks      (const std::vector<std::string>& labels, const std::vector<double>& locations);
		void setYTicks      (const std::vector<std::string>& labels, const std::vector<double>& locations);
		void draw           (ImDrawList* drawlist, float x, float y) override;
		void setXLimits     (float x_min, float x_max);
		void setYLimits     (float y_min, float y_max);
		void setY2Limits     (float y_min, float y_max);
		void setY3Limits     (float y_min, float y_max);
		void setXLimitsAuto ();
		void setYLimitsAuto ();
		void setY2LimitsAuto ();
		void setY3LimitsAuto ();

		[[nodiscard]] bool isPlotQueried() const;
		float* getPlotQueryArea();
		
		ImPlotFlags        getFlags         () const { return m_flags; }
		ImPlotAxisFlags    getXFlags        () const { return m_xflags; }
		ImPlotAxisFlags    getYFlags        () const { return m_yflags; }
		ImPlotAxisFlags    getY2Flags        () const { return m_y2flags; }
		ImPlotAxisFlags    getY3Flags        () const { return m_y3flags; }
		const std::string& getXAxisName     () const { return m_xaxisName; }
		const std::string& getYAxisName     () const { return m_yaxisName; }
		const ImVec2&      getXLimits       () const { return m_xlimits_actual; }
		const ImVec2&      getYLimits       () const { return m_ylimits_actual; }
		const ImVec2&      getY2Limits       () const { return m_y2limits_actual; }
		const ImVec2&      getY3Limits       () const { return m_y3limits_actual; }

		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
		bool canChildBeAdded(mvAppItemType type) override;

	private:

		std::string                   m_xaxisName;
		std::string                   m_yaxisName;
		ImPlotFlags                   m_flags    = 0;
		ImPlotAxisFlags               m_xflags  = 0;
		ImPlotAxisFlags               m_yflags  = 0;
		ImPlotAxisFlags               m_y2flags  = 0;
		ImPlotAxisFlags               m_y3flags  = 0;

		ImPlotColormap                m_colormap = ImPlotColormap_Deep;

		bool                          m_setXLimits = false;
		bool                          m_setYLimits = false;
		bool                          m_equalAspectRatios = false;
		ImVec2                        m_xlimits;
		ImVec2                        m_xlimits_actual;
		ImVec2                        m_ylimits;
		ImVec2                        m_ylimits_actual;
		PyObject*                    m_queryCallback = nullptr;
		bool                          m_queried = false;
		float                         m_queryArea[4] = {0.0f , 0.0f, 0.0f, 0.0f};
		bool                          m_dirty = false;
		
		std::vector<std::string>      m_xlabels;
		std::vector<std::string>      m_ylabels;
		std::vector<const char*>      m_xclabels; // to prevent conversion from string to char* every frame
		std::vector<const char*>      m_yclabels; // to prevent conversion from string to char* every frame
		std::vector<double>           m_xlabelLocations;
		std::vector<double>           m_ylabelLocations;

		// y axis 2
		bool                          m_setY2Limits = false;
		ImVec2                        m_y2limits;
		ImVec2                        m_y2limits_actual;

		// y axis 3
		bool                          m_setY3Limits = false;
		ImVec2                        m_y3limits;
		ImVec2                        m_y3limits_actual;
	};

}
