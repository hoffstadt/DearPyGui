#pragma once

#include "Core/AppItems/mvTypeBases.h"
#include <implot.h>
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

		mvSeries(std::string  name, const std::vector<mvVec2>& points)
			: m_name(std::move(name))
		{
		    if(!points.empty())
            {
		        m_maxX = points[0].x;
		        m_minX = points[0].x;
		        m_maxY = points[0].y;
		        m_minY = points[0].y;
            }

			for (auto& point : points)
			{
			    if(point.x > m_maxX) m_maxX = point.x;
			    if(point.y > m_maxY) m_maxY = point.y;
			    if(point.x < m_minX) m_minX = point.x;
			    if(point.y < m_minY) m_minY = point.y;
				m_xs.push_back(point.x);
				m_ys.push_back(point.y);
			}

		}

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
				std::string  yname = "", int width = -1, int height = 0, ImPlotFlags flags = ImPlotFlags_Default,
				ImPlotAxisFlags xflags = ImPlotAxisFlags_Default, ImPlotAxisFlags yflags = ImPlotAxisFlags_Default, 
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
		ImPlotFlags     m_flags    = ImPlotFlags_Default;
		ImPlotAxisFlags m_xflags  = ImPlotAxisFlags_Default;
		ImPlotAxisFlags m_yflags  = ImPlotAxisFlags_Default;
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

	//-----------------------------------------------------------------------------
	// mvLineSeries
	//-----------------------------------------------------------------------------
	class mvLineSeries: public mvSeries
	{

	public:

		mvLineSeries(const std::string& name, const std::vector<mvVec2>& points, float weight=1.0f,
			mvColor color = MV_DEFAULT_COLOR, mvColor fill = MV_DEFAULT_COLOR)
			: mvSeries(name, points), m_lineWeight(weight), m_color(color), m_fill(fill)
		{
		}

		void draw() override
		{
			if (m_color.specified)
				ImPlot::PushStyleColor(ImPlotCol_Line, m_color);
			if (m_fill.specified)
				ImPlot::PushStyleColor(ImPlotCol_Fill, m_fill);

			ImPlot::PushStyleVar(ImPlotStyleVar_LineWeight, m_lineWeight);

			if (m_fill.specified)
				ImPlot::PlotShaded(m_name.c_str(), m_xs.data(), m_ys.data(), m_xs.size());
			else
				ImPlot::PlotLine(m_name.c_str(), m_xs.data(), m_ys.data(), m_xs.size());

			if (m_color.specified)
				ImPlot::PopStyleColor();
			if (m_fill.specified)
				ImPlot::PopStyleColor();

			ImPlot::PopStyleVar();
		}

	private:

		float   m_lineWeight;
		mvColor m_color;
		mvColor m_fill;

	};

	//-----------------------------------------------------------------------------
	// mvScatterSeries
	//-----------------------------------------------------------------------------
	class mvScatterSeries : public mvSeries
	{

	public:

		mvScatterSeries(const std::string& name, const std::vector<mvVec2>& points, int marker=2, float markerSize=4.0f, float markerWeight =1.0f,
			mvColor markerOutlineColor = MV_DEFAULT_COLOR, mvColor markerFillColor = MV_DEFAULT_COLOR)
			: mvSeries(name, points), m_marker(marker), m_markerSize(markerSize), m_markerWeight(markerWeight),
			m_markerOutlineColor(markerOutlineColor), m_markerFillColor(markerFillColor)
		{
		}

		void draw() override
		{
			if (m_markerOutlineColor.specified)
				ImPlot::PushStyleColor(ImPlotCol_MarkerOutline, m_markerOutlineColor);
			if (m_markerFillColor.specified)
				ImPlot::PushStyleColor(ImPlotCol_MarkerFill, m_markerFillColor);
				
			ImPlot::PushStyleVar(ImPlotStyleVar_Marker, m_marker);
			ImPlot::PushStyleVar(ImPlotStyleVar_MarkerSize, m_markerSize);
			ImPlot::PushStyleVar(ImPlotStyleVar_MarkerWeight, m_markerWeight);

			ImPlot::PlotScatter(m_name.c_str(), m_xs.data(), m_ys.data(), m_xs.size());

			if (m_markerOutlineColor.specified)
				ImPlot::PopStyleColor();
			if (m_markerFillColor.specified)
				ImPlot::PopStyleColor();

			ImPlot::PopStyleVar(3);
		}

	private:

		int   m_marker = 2;
		float m_markerSize = 4.0f;
		float m_markerWeight = 1.0f;
		mvColor m_markerOutlineColor = MV_DEFAULT_COLOR;
		mvColor m_markerFillColor = MV_DEFAULT_COLOR;

	};

	//-----------------------------------------------------------------------------
	// mvLabelSeries
	//-----------------------------------------------------------------------------
	class mvLabelSeries : public mvSeries
	{

	public:

		mvLabelSeries(const std::string& name, const std::vector<mvVec2>& points, int xoffset=0, int yoffset=0, bool vertical = false)
			: mvSeries(name, points), m_xoffset(xoffset), m_yoffset(yoffset), m_vertical(vertical)
		{
		}

		void draw() override
		{
			ImPlot::PlotText(m_name.c_str(), m_xs[0], m_ys[0], m_vertical,
                    ImVec2((float)m_xoffset, (float)m_yoffset));
		}

	private:

		int m_xoffset;
		int m_yoffset;
		bool m_vertical;

	};

	//-----------------------------------------------------------------------------
	// mvAreaSeries
	//-----------------------------------------------------------------------------
	class mvAreaSeries : public mvSeries
	{

	public:

		mvAreaSeries(const std::string& name, const std::vector<mvVec2>& points, float weight = 1.0f,
			mvColor color = MV_DEFAULT_COLOR, mvColor fill = MV_DEFAULT_COLOR)
			: mvSeries(name, points), m_weight(weight), m_color(color), m_fill(fill)
		{
		}

		void draw() override
		{

			ImPlot::PushPlotClipRect();
			auto item = ImPlot::RegisterItem(m_name.c_str());
			if (item->Show)
				drawPolygon();
			ImPlot::PopPlotClipRect();

		}

		void drawPolygon();

	private:

		float m_weight = 1.0f;
		mvColor m_color = MV_DEFAULT_COLOR;
		mvColor m_fill = MV_DEFAULT_COLOR;
	};

}