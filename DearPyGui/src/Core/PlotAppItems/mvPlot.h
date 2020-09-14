#pragma once

#include "Core/AppItems/mvTypeBases.h"
#include <implot.h>
#include <implot_internal.h>
#include <map>
#include <utility>
#include "mvCore.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvPlot
//
//-----------------------------------------------------------------------------

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvSeries
	//-----------------------------------------------------------------------------
	class mvSeries
	{

        friend class mvPlot;

	public:

		mvSeries(std::string  name, const std::vector<mvVec2>& points);

		virtual ~mvSeries() = default;

		virtual void draw() = 0;

		const std::string& getName() const { return m_name; }

	protected:

		std::string        m_name;
		std::vector<float> m_xs;
		std::vector<float> m_ys;
		float              m_maxX;
		float              m_maxY;
		float              m_minX;
		float              m_minY;

	};

	//-----------------------------------------------------------------------------
	// mvPlot
	//-----------------------------------------------------------------------------
	class mvPlot : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Plot)

			mvPlot(const std::string& parent, const std::string& name, std::string  xname = "",
				std::string  yname = "", ImPlotFlags flags = 0, ImPlotAxisFlags xflags = 0, ImPlotAxisFlags yflags = 0, 
				PyObject* queryCallback = nullptr);

		void addSeries   (mvSeries* series);
		void updateSeries(mvSeries* series);
		void deleteSeries(const std::string& name);
		void SetColorMap(ImPlotColormap colormap);
		void resetXTicks();
		void resetYTicks();
		void setXTicks(const std::vector<std::string>& labels, const std::vector<double>& locations);
		void setYTicks(const std::vector<std::string>& labels, const std::vector<double>& locations);
		void clear();
		void draw() override;
		void setXLimits(float x_min, float x_max);
		void setYLimits(float y_min, float y_max);
		void setXLimitsAuto();
		void setYLimitsAuto();
		[[nodiscard]] bool isPlotQueried() const;
		float* getPlotQueryArea();

	private:

		std::string     m_xaxisName;
		std::string     m_yaxisName;
		ImPlotFlags     m_flags    = 0;
		ImPlotAxisFlags m_xflags  = 0;
		ImPlotAxisFlags m_yflags  = 0;
		ImPlotColormap  m_colormap = ImPlotColormap_Default;
		bool            m_setXLimits = false;
		bool            m_setYLimits = false;
		ImVec2          m_xlimits;
		ImVec2          m_ylimits;
		PyObject*       m_queryCallback;
		bool            m_queried = false;
		float           m_queryArea[4] = {0.0f , 0.0f, 0.0f, 0.0f};
		bool            m_dirty = false;
		
		std::vector<std::string> m_xlabels;
		std::vector<std::string> m_ylabels;
		std::vector<const char*> m_xclabels; // to prevent conversion from string to char* every frame
		std::vector<const char*> m_yclabels; // to prevent conversion from string to char* every frame
		std::vector<double>      m_xlabelLocations;
		std::vector<double>      m_ylabelLocations;

		std::vector<mvSeries*> m_series;

	};

}