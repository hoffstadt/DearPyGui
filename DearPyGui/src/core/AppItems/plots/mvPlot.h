#pragma once

#include "mvTypeBases.h"
#include <implot.h>
#include <implot_internal.h>
#include <map>
#include <utility>
#include "mvCore.h"
#include "mvPython.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvPlot
//     * mvPlotAnnotation
//     * mvDragLine
//     * mvDragPoint
//     * mvSeries
//
//-----------------------------------------------------------------------------

namespace Marvel {

	// forward declarations
	class mvSeries;
	struct mvPlotAnnotation;
	struct mvDragLine;
	struct mvDragPoint;
	class mvDrawList;

#ifdef MV_CPP
#else
	
	PyObject* add_plot         (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_drag_point   (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* delete_drag_point(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_annotation   (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* delete_annotation(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_drag_line    (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* delete_drag_line (PyObject* self, PyObject* args, PyObject* kwargs);

	// data removal
	PyObject* clear_plot(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* delete_series(PyObject* self, PyObject* args, PyObject* kwargs);

	// ticks
	PyObject* reset_xticks(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* reset_yticks(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_xticks(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_yticks(PyObject* self, PyObject* args, PyObject* kwargs);

	// query
	PyObject* is_plot_queried(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_plot_query_area(PyObject* self, PyObject* args, PyObject* kwargs);

	// limits
	PyObject* set_plot_xlimits_auto(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_plot_ylimits_auto(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_plot_xlimits(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_plot_ylimits(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_plot_xlimits(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_plot_ylimits(PyObject* self, PyObject* args, PyObject* kwargs);

	// series
	PyObject* add_image_series(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_pie_series(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_line_series(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_bar_series(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_shade_series(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_scatter_series(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_stem_series(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_text_point(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_area_series(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_error_series(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_heat_series(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_stair_series(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_candle_series(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_vline_series(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_hline_series(PyObject* self, PyObject* args, PyObject* kwargs);
#endif


	//-----------------------------------------------------------------------------
	// mvPlot
	//-----------------------------------------------------------------------------
	MV_REGISTER_WIDGET(mvPlot);
	class mvPlot : public mvAppItem
	{

	public:

		static void InsertParser   (std::map<std::string, mvPythonParser>* parsers);
		static void InsertConstants(std::vector<std::pair<std::string, long>>& constants);

	public:

		MV_APPITEM_TYPE(mvAppItemType::mvPlot, "add_plot")

		MV_CREATE_THEME_CONSTANT(mvThemeCol_Plot_FrameBg,					 5L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Plot_PlotBg,					 6L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Plot_PlotBorder,				 7L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Plot_LegendBg,					 8L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Plot_LegendBorder,				 9L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Plot_LegendText,				10L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Plot_TitleText,					11L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Plot_InlayText,					12L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Plot_XAxis,						13L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Plot_XAxisGrid,					14L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Plot_YAxis,						15L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Plot_YAxisGrid,					16L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Plot_YAxis2,					17L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Plot_YAxisGrid2,				18L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Plot_YAxis3,					19L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Plot_YAxisGrid3,				20L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Plot_Selection,					21L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Plot_Query,						22L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Plot_Crosshairs,				23L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Plot_FillAlpha,				 4L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Plot_ErrorBarSize,			 5L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Plot_ErrorBarWeight,			 6L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Plot_BorderSize,				 9L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Plot_MinorAlpha,				10L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Plot_MajorTickLenX,			11L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Plot_MajorTickLenY,			11L, 1L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Plot_MinorTickLenX,			12L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Plot_MinorTickLenY,			12L, 1L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Plot_MajorTickSizeX,			13L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Plot_MajorTickSizeY,			13L, 1L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Plot_MinorTickSizeX,			14L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Plot_MinorTickSizeY,			14L, 1L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Plot_MajorGridSizeX,			15L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Plot_MajorGridSizeY,			15L, 1L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Plot_MinorGridSizeX,			16L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Plot_MinorGridSizeY,			16L, 1L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Plot_PlotPaddingX,			17L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Plot_PlotPaddingY,			17L, 1L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Plot_LabelPaddingX,			18L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Plot_LabelPaddingY,			18L, 1L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Plot_LegendPaddingX,			19L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Plot_LegendPaddingY,			19L, 1L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Plot_LegendInnerPaddingX,		20L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Plot_LegendInnerPaddingY,		20L, 1L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Plot_LegendSpacingX,			21L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Plot_LegendSpacingY,			21L, 1L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Plot_MousePosPaddingX,		22L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Plot_MousePosPaddingY,		22L, 1L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Plot_AnnotationPaddingX,		23L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Plot_AnnotationPaddingY,		23L, 1L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Plot_FitPaddingX,				24L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Plot_FitPaddingY,				24L, 1L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Plot_PlotDefaultSizeX,		25L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Plot_PlotDefaultSizeY,		25L, 1L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Plot_PlotMinSizeX,			26L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Plot_PlotMinSizeY,			26L, 1L);

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
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Plot_FillAlpha,			   1,   1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Plot_ErrorBarSize,		   5,  10),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Plot_ErrorBarWeight,		 1.5,  10),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Plot_BorderSize,			   1,  10),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Plot_MinorAlpha,			0.25,   1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Plot_MajorTickLenX,		  10,  20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Plot_MajorTickLenY,		  10,  20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Plot_MinorTickLenX,		   5,  10),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Plot_MinorTickLenY,		   5,  10),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Plot_MajorTickSizeX,		   1,  10),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Plot_MajorTickSizeY,		   1,  10),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Plot_MinorTickSizeX,		   1,  10),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Plot_MinorTickSizeY,		   1,  10),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Plot_MajorGridSizeX,		   1,  10),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Plot_MajorGridSizeY,		   1,  10),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Plot_MinorGridSizeX,		   1,  10),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Plot_MinorGridSizeY,		   1,  10),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Plot_PlotPaddingX,		  10,  10),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Plot_PlotPaddingY,		  10,  10),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Plot_LabelPaddingX,		   5,  10),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Plot_LabelPaddingY,		   5,  10),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Plot_LegendPaddingX,		  10,  20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Plot_LegendPaddingY,		  10,  20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Plot_LegendInnerPaddingX,	   5,  10),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Plot_LegendInnerPaddingY,	   5,  10),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Plot_LegendSpacingX,		   5,  10),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Plot_LegendSpacingY,		   0,  10),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Plot_MousePosPaddingX,	  10,  20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Plot_MousePosPaddingY,	  10,  20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Plot_AnnotationPaddingX,	   2,   4),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Plot_AnnotationPaddingY,	   2,   4),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Plot_FitPaddingX,			   0,   1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Plot_FitPaddingY,			   0,   1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Plot_PlotDefaultSizeX,	 400, 800),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Plot_PlotDefaultSizeY,	 300, 600),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Plot_PlotMinSizeX,		 300, 600),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Plot_PlotMinSizeY,		 255, 510),
		MV_END_STYLE_CONSTANTS

		mvPlot(const std::string& name, mvCallable queryCallback);
		~mvPlot(){clear();}

		// drag lines
		void addDragPoint   (const std::string& name, bool show_label, const mvColor& color, float radius, mvCallable callback, const double* dummyValue, const std::string& source);
		void updateDragPoint(const std::string& name, bool show_label, const mvColor& color, float radius, mvCallable callback, const double* dummyValue, const std::string& source);
		void deleteDragPoint(const std::string& name);

		// drag lines
		void addDragLine    (const std::string& name, bool show_label, const mvColor& color, float thickness, bool y_line, mvCallable callback, double dummyValue, const std::string& source);
		void updateDragLine (const std::string& name, bool show_label, const mvColor& color, float thickness, bool y_line, mvCallable callback, double dummyValue, const std::string& source);
		void deleteDragLine (const std::string& name);

		// annotations
		void addAnnotation  (const std::string& name, double x, double y, float xoffset, float yoffset, const mvColor& color, const std::string& text, bool clamped);
		void updateAnnotation(const std::string& name, double x, double y, float xoffset, float yoffset, const mvColor& color, const std::string& text, bool clamped);
		void deleteAnnotation(const std::string& name);

		// series
		void addSeries      (mvRef<mvSeries> series, bool updateBounds);
		void updateSeries   (mvRef<mvSeries> series, bool updateBounds);
		void deleteSeries   (const std::string& name);

		// settings
		void SetColorMap    (ImPlotColormap colormap);
		void resetXTicks    ();
		void resetYTicks    ();
		void setXTicks      (const std::vector<std::string>& labels, const std::vector<double>& locations);
		void setYTicks      (const std::vector<std::string>& labels, const std::vector<double>& locations);
		void clear          ();
		void draw           () override;
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
		mvRef<mvDrawList> getDrawList() { return m_drawList; }
		
		ImPlotFlags        getFlags         () const { return m_flags; }
		ImPlotAxisFlags    getXFlags        () const { return m_xflags; }
		ImPlotAxisFlags    getYFlags        () const { return m_yflags; }
		ImPlotAxisFlags    getY2Flags        () const { return m_y2flags; }
		ImPlotAxisFlags    getY3Flags        () const { return m_y3flags; }
		bool               isColorScaleShown() const { return m_colormapscale; }
		int                getScaleHeight   () const { return m_scale_height; }
		float              getScaleMin      () const { return m_scale_min; }
		float              getScaleMax      () const { return m_scale_max; }
		const std::string& getXAxisName     () const { return m_xaxisName; }
		const std::string& getYAxisName     () const { return m_yaxisName; }
		mvCallable         getQueryCallback ()       { return m_queryCallback; }
		const ImVec2&      getXLimits       () const { return m_xlimits_actual; }
		const ImVec2&      getYLimits       () const { return m_ylimits_actual; }
		const ImVec2&      getY2Limits       () const { return m_y2limits_actual; }
		const ImVec2&      getY3Limits       () const { return m_y3limits_actual; }

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

	private:

		mvRef<mvDrawList>             m_drawList;
		
		std::string                   m_xaxisName;
		std::string                   m_yaxisName;
		ImPlotFlags                   m_flags    = 0;
		ImPlotAxisFlags               m_xflags  = 0;
		ImPlotAxisFlags               m_yflags  = 0;
		ImPlotAxisFlags               m_y2flags  = 0;
		ImPlotAxisFlags               m_y3flags  = 0;
		bool                          m_showAnnotations = true;
		bool                          m_showDragLines = true;
		bool                          m_showDragPoints = true;


		ImPlotColormap                m_colormap = ImPlotColormap_Deep;

		bool                          m_setXLimits = false;
		bool                          m_setYLimits = false;
		bool                          m_equalAspectRatios = false;
		ImVec2                        m_xlimits;
		ImVec2                        m_xlimits_actual;
		ImVec2                        m_ylimits;
		ImVec2                        m_ylimits_actual;
		mvCallable                    m_queryCallback;
		bool                          m_queried = false;
		float                         m_queryArea[4] = {0.0f , 0.0f, 0.0f, 0.0f};
		bool                          m_dirty = false;
		bool                          m_colormapscale = false;
		float                         m_scale_min = 0.0f;
		float                         m_scale_max = 1.0f;
		int                           m_scale_height = 100;
		
		std::vector<std::string>      m_xlabels;
		std::vector<std::string>      m_ylabels;
		std::vector<const char*>      m_xclabels; // to prevent conversion from string to char* every frame
		std::vector<const char*>      m_yclabels; // to prevent conversion from string to char* every frame
		std::vector<double>           m_xlabelLocations;
		std::vector<double>           m_ylabelLocations;

		std::vector<mvRef<mvSeries>>  m_series;
		std::vector<mvPlotAnnotation> m_annotations;
		std::vector<mvDragLine>       m_dragLines;
		std::vector<mvDragPoint>      m_dragPoints;

		// y axis 2
		bool                          m_setY2Limits = false;
		ImVec2                        m_y2limits;
		ImVec2                        m_y2limits_actual;

		// y axis 3
		bool                          m_setY3Limits = false;
		ImVec2                        m_y3limits;
		ImVec2                        m_y3limits_actual;
	};

	//-----------------------------------------------------------------------------
	// mvPlotAnnotation
	//-----------------------------------------------------------------------------
	struct mvPlotAnnotation
	{
		std::string name;
		double      x;
		double      y;
		ImVec2      pix_offset;
		mvColor     color;
		std::string text;
		bool        clamped;
	};

	//-----------------------------------------------------------------------------
	// mvDragLine
	//-----------------------------------------------------------------------------
	struct mvDragLine
	{
		std::string name;
		std::shared_ptr<float> value;
		bool        show_label;
		mvColor     color;
		float       thickness;
		bool        y_line;
		mvCallable  callback;
		std::string source;
	};

	//-----------------------------------------------------------------------------
	// mvDragPoint
	//-----------------------------------------------------------------------------
	struct mvDragPoint
	{
		std::string name;
		std::shared_ptr<std::array<float, 2>> value;
		bool        show_label;
		mvColor     color;
		float       radius;
		mvCallable  callback;
		std::string source;
	};

	//-----------------------------------------------------------------------------
	// mvSeries
	//-----------------------------------------------------------------------------
	class mvSeries
	{

		friend class mvPlot;

	public:

		enum class mvSeriesType
		{
			None = 0, Line, Area, Bar, Error, Heat, 
			Image, Label, Pie, Scatter, Stem, Shade, 
			Stair, Candle, VLine, HLine
		};

		mvSeries(std::string name, const std::vector<const std::vector<float>*>& data, ImPlotYAxis_ axis = ImPlotYAxis_1);

		mvSeries(std::string name, const ImPlotPoint& boundsMin, const ImPlotPoint& boundsMax, ImPlotYAxis_ axis = ImPlotYAxis_1);

		virtual ~mvSeries() = default;

		virtual void draw() = 0;
		virtual mvSeriesType getSeriesType() = 0;

		const std::string& getName() const { return m_name; }

		void setWeight(float weight) { m_weight = weight; }

	protected:

		std::string                     m_name;
		ImPlotYAxis_                    m_axis = ImPlotYAxis_1;
		std::vector<std::vector<float>> m_data;

		float                           m_maxX;
		float                           m_maxY;
		float                           m_minX;
		float                           m_minY;
		float                           m_weight = 1.0f;
		
	};
}
