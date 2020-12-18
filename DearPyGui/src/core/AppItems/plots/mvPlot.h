#pragma once

#include "mvTypeBases.h"
#include <implot.h>
#include <implot_internal.h>
#include <map>
#include <utility>
#include "mvCore.h"
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "mvPythonParser.h"

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
	
	PyObject* add_plot         (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_drag_point   (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* delete_drag_point(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_annotation   (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* delete_annotation(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_drag_line    (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* delete_drag_line (PyObject* self, PyObject* args, PyObject* kwargs);

	// forward declarations
	class mvSeries;
	struct mvPlotAnnotation;
	struct mvDragLine;
	struct mvDragPoint;

	//-----------------------------------------------------------------------------
	// mvPlot
	//-----------------------------------------------------------------------------
	class mvPlot : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		MV_APPITEM_TYPE(mvAppItemType::Plot, "add_plot")

		mvPlot(const std::string& name, PyObject* queryCallback);
		~mvPlot(){clear();}

		// drag lines
		void addDragPoint   (const std::string& name, bool show_label, const mvColor& color, float radius, PyObject* callback, double* dummyValue, const std::string& source);
		void updateDragPoint(const std::string& name, bool show_label, const mvColor& color, float radius, PyObject* callback, double* dummyValue, const std::string& source);
		void deleteDragPoint(const std::string& name);

		// drag lines
		void addDragLine    (const std::string& name, bool show_label, const mvColor& color, float thickness, bool y_line, PyObject* callback, double dummyValue, const std::string& source);
		void updateDragLine (const std::string& name, bool show_label, const mvColor& color, float thickness, bool y_line, PyObject* callback, double dummyValue, const std::string& source);
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
		PyObject*          getQueryCallback ()       { return m_queryCallback; }
		const ImVec2&      getXLimits       () const { return m_xlimits_actual; }
		const ImVec2&      getYLimits       () const { return m_ylimits_actual; }
		const ImVec2&      getY2Limits       () const { return m_y2limits_actual; }
		const ImVec2&      getY3Limits       () const { return m_y3limits_actual; }

		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:

		// new
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


		ImPlotColormap                m_colormap = ImPlotColormap_Default;

		bool                          m_setXLimits = false;
		bool                          m_setYLimits = false;
		ImVec2                        m_xlimits;
		ImVec2                        m_xlimits_actual;
		ImVec2                        m_ylimits;
		ImVec2                        m_ylimits_actual;
		PyObject*                     m_queryCallback;
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
		float*      value;
		bool        show_label;
		mvColor     color;
		float       thickness;
		bool        y_line;
		PyObject*   callback;
		double      dummyValue;
		std::string source;
	};

	//-----------------------------------------------------------------------------
	// mvDragPoint
	//-----------------------------------------------------------------------------
	struct mvDragPoint
	{
		std::string name;
		float*      value;
		bool        show_label;
		mvColor     color;
		float       radius;
		PyObject*   callback;
		double      dummyx;
		double      dummyy;
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
			Stair, Candle
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
