#pragma once

#include "mvTypeBases.h"
#include <implot.h>
#include <implot_internal.h>
#include <map>
#include <utility>
#include "mvCore.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvPlot, MV_ITEM_DESC_CONTAINER, StorageValueTypes::None, 1);
	class mvPlot : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvPlot, add_plot)

		MV_CREATE_EXTRA_COMMAND(is_plot_queried);
		MV_CREATE_EXTRA_COMMAND(get_plot_query_area);

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
			MV_ADD_EXTRA_COMMAND(is_plot_queried);
			MV_ADD_EXTRA_COMMAND(get_plot_query_area);
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
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Plot_PlotBorder,   mvImGuiCol_Border),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Plot_TitleText,    mvImGuiCol_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Plot_InlayText,    mvImGuiCol_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Plot_XAxis,        mvImGuiCol_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Plot_XAxisGrid,    mvImGuiCol_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Plot_YAxis,        mvImGuiCol_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Plot_YAxisGrid,    mvImGuiCol_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Plot_YAxis2,       mvImGuiCol_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Plot_YAxisGrid2,   mvImGuiCol_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Plot_YAxis3,       mvImGuiCol_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Plot_YAxisGrid3,   mvImGuiCol_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Plot_Selection,    mvColor(255, 153,   0, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Plot_Query,        mvColor(  0, 255, 112, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Plot_Crosshairs,   mvColor(255, 255, 255, 128)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT_F(mvThemeStyle_Plot_FillAlpha,			   1,   1),
			MV_ADD_CONSTANT_F(mvThemeStyle_Plot_BorderSize,			   1,  10),
			MV_ADD_CONSTANT_F(mvThemeStyle_Plot_MinorAlpha,			0.25,   1),
			MV_ADD_CONSTANT_F(mvThemeStyle_Plot_MajorTickLenX,		  10,  20),
			MV_ADD_CONSTANT_F(mvThemeStyle_Plot_MajorTickLenY,		  10,  20),
			MV_ADD_CONSTANT_F(mvThemeStyle_Plot_MinorTickLenX,		   5,  10),
			MV_ADD_CONSTANT_F(mvThemeStyle_Plot_MinorTickLenY,		   5,  10),
			MV_ADD_CONSTANT_F(mvThemeStyle_Plot_MajorTickSizeX,		   1,  10),
			MV_ADD_CONSTANT_F(mvThemeStyle_Plot_MajorTickSizeY,		   1,  10),
			MV_ADD_CONSTANT_F(mvThemeStyle_Plot_MinorTickSizeX,		   1,  10),
			MV_ADD_CONSTANT_F(mvThemeStyle_Plot_MinorTickSizeY,		   1,  10),
			MV_ADD_CONSTANT_F(mvThemeStyle_Plot_MajorGridSizeX,		   1,  10),
			MV_ADD_CONSTANT_F(mvThemeStyle_Plot_MajorGridSizeY,		   1,  10),
			MV_ADD_CONSTANT_F(mvThemeStyle_Plot_MinorGridSizeX,		   1,  10),
			MV_ADD_CONSTANT_F(mvThemeStyle_Plot_MinorGridSizeY,		   1,  10),
			MV_ADD_CONSTANT_F(mvThemeStyle_Plot_PlotPaddingX,		  10,  10),
			MV_ADD_CONSTANT_F(mvThemeStyle_Plot_PlotPaddingY,		  10,  10),
			MV_ADD_CONSTANT_F(mvThemeStyle_Plot_LabelPaddingX,		   5,  10),
			MV_ADD_CONSTANT_F(mvThemeStyle_Plot_LabelPaddingY,		   5,  10),
			MV_ADD_CONSTANT_F(mvThemeStyle_Plot_MousePosPaddingX,	  10,  20),
			MV_ADD_CONSTANT_F(mvThemeStyle_Plot_MousePosPaddingY,	  10,  20),
			MV_ADD_CONSTANT_F(mvThemeStyle_Plot_AnnotationPaddingX,	   2,   4),
			MV_ADD_CONSTANT_F(mvThemeStyle_Plot_AnnotationPaddingY,	   2,   4),
			MV_ADD_CONSTANT_F(mvThemeStyle_Plot_FitPaddingX,			   0,   1),
			MV_ADD_CONSTANT_F(mvThemeStyle_Plot_FitPaddingY,			   0,   1),
			MV_ADD_CONSTANT_F(mvThemeStyle_Plot_PlotDefaultSizeX,	 400, 800),
			MV_ADD_CONSTANT_F(mvThemeStyle_Plot_PlotDefaultSizeY,	 300, 600),
			MV_ADD_CONSTANT_F(mvThemeStyle_Plot_PlotMinSizeX,		 300, 600),
			MV_ADD_CONSTANT_F(mvThemeStyle_Plot_PlotMinSizeY,		 255, 510),
		MV_END_STYLE_CONSTANTS

		mvPlot(const std::string& name);

		void updateFlags();
		void updateAxesNames();

		// settings
		void SetColorMap    (ImPlotColormap colormap);
		void draw           (ImDrawList* drawlist, float x, float y) override;

		void addFlag         (ImPlotFlags flag);
		void removeFlag      (ImPlotFlags flag);

		[[nodiscard]] bool isPlotQueried() const;
		double* getPlotQueryArea();
		
		ImPlotFlags        getFlags         () const { return m_flags; }

		void onChildRemoved(mvRef<mvAppItem> item) override;
		void onChildAdd(mvRef<mvAppItem> item) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;
		bool canChildBeAdded(mvAppItemType type) override;
		void postDraw() override;

	private:

		std::string                   m_xaxisName;
		std::string                   m_y1axisName;
		std::string                   m_y2axisName;
		std::string                   m_y3axisName;
		ImPlotFlags                   m_flags    = ImPlotFlags_NoLegend;
		ImPlotAxisFlags               m_xflags  = 0;
		ImPlotAxisFlags               m_yflags  = 0;
		ImPlotAxisFlags               m_y1flags  = 0;
		ImPlotAxisFlags               m_y2flags  = 0;

		ImPlotColormap                m_colormap = ImPlotColormap_Deep;

		bool                          m_equalAspectRatios = false;

		bool                          m_queried = false;
		double                        m_queryArea[4] = {0.0, 0.0, 0.0, 0.0};
		bool                          m_dirty = false;
	
	};

}
