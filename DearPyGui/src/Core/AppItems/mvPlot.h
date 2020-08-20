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

	public:

		mvSeries(std::string  name, const std::vector<mvVec2>& points)
			: m_name(std::move(name))
		{
			for (auto& point : points)
			{
				m_xs.push_back(point.x);
				m_ys.push_back(point.y);
			}

		}

		virtual ~mvSeries() = default;

		virtual void draw() = 0;

	protected:

		std::string        m_name;
		std::vector<float> m_xs;
		std::vector<float> m_ys;

	};

	//-----------------------------------------------------------------------------
	// mvPlot
	//-----------------------------------------------------------------------------
	class mvPlot : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Plot)

		mvPlot(const std::string& parent, const std::string& name, std::string  xname="",
			std::string  yname="", int width = -1, int height = 0, ImPlotFlags flags = ImPlotFlags_Default,
			ImPlotAxisFlags xflags = ImPlotAxisFlags_Default, ImPlotAxisFlags yflags = ImPlotAxisFlags_Default, std::string  queryCallback = "")
			: mvAppItem(parent, name), m_xaxisName(std::move(xname)), m_yaxisName(std::move(yname)),
			m_flags(flags), m_xflags(xflags), m_yflags(yflags), m_queryCallback(std::move(queryCallback))
		{
			m_width = width;
			m_height = height;
		}

		void addSeries(mvSeries* series)
		{
			m_series.push_back(series);
		}

		void SetColorMap(ImPlotColormap colormap)
		{
			m_colormap = colormap;
		}

		void resetXTicks()
		{
			m_xlabels.clear();
			m_xclabels.clear();
			m_xlabelLocations.clear();
		}

		void resetYTicks()
		{
			m_ylabels.clear();
			m_yclabels.clear();
			m_ylabelLocations.clear();
		}

		void setXTicks(const std::vector<std::string>& labels, const std::vector<double>& locations)
		{
			m_xlabels = labels;
			m_xlabelLocations = locations;

			for (const auto& item : m_xlabels)
				m_xclabels.push_back(item.data());
		}

		void setYTicks(const std::vector<std::string>& labels, const std::vector<double>& locations)
		{
			m_ylabels = labels;
			m_ylabelLocations = locations;

			for (const auto& item : m_ylabels)
				m_yclabels.push_back(item.data());
		}

		void clear()
		{
			for (auto& series : m_series)
			{
				delete series;
				series = nullptr;
			}

			m_series.clear();
		}

		void draw() override
		{
			ImGui::PushID(m_colormap);

			if (m_setXLimits)
				ImPlot::SetNextPlotLimitsX(m_xlimits.x, m_xlimits.y, ImGuiCond_Always);

			if (m_setYLimits)
				ImPlot::SetNextPlotLimitsY(m_ylimits.x, m_ylimits.y, ImGuiCond_Always);

			if (!m_xlabels.empty())
			{
				// TODO: Checks
				ImPlot::SetNextPlotTicksX(m_xlabelLocations.data(), (int)m_xlabels.size(), m_xclabels.data());
			}
			if (!m_ylabels.empty())
			{
				// TODO: Checks
				ImPlot::SetNextPlotTicksY(m_ylabelLocations.data(), (int)m_ylabels.size(), m_yclabels.data());
			}

			if (ImPlot::BeginPlot(m_name.c_str(), m_xaxisName.c_str(), m_yaxisName.c_str(),
                         ImVec2((float)m_width, (float)m_height), m_flags,
				m_xflags, m_yflags) )
			{
				ImPlot::SetColormap(m_colormap);

				for (auto series : m_series)
					series->draw();
		

				//ImPlot::SetColormap(ImPlotColormap_Default);

				m_queried = ImPlot::IsPlotQueried();

				if (m_queried)
				{
					auto area = ImPlot::GetPlotQuery();
					m_queryArea[0] = (float)area.X.Min;
					m_queryArea[1] = (float)area.X.Max;
					m_queryArea[2] = (float)area.Y.Min;
					m_queryArea[3] = (float)area.Y.Max;
				}

				if (!m_queryCallback.empty() && m_queried)
				{
					PyGILState_STATE gstate = PyGILState_Ensure();
					PyObject* area = PyTuple_New(4);
					PyTuple_SetItem(area, 0, PyFloat_FromDouble(m_queryArea[0]));
					PyTuple_SetItem(area, 1, PyFloat_FromDouble(m_queryArea[1]));
					PyTuple_SetItem(area, 2, PyFloat_FromDouble(m_queryArea[2]));
					PyTuple_SetItem(area, 3, PyFloat_FromDouble(m_queryArea[3]));
					PyGILState_Release(gstate);
					mvApp::GetApp()->runCallback(m_queryCallback, m_name, area);
				}

				ImPlot::EndPlot();
			}

			ImGui::PopID();
		}

		void setXLimits(float x_min, float x_max)
		{
			m_setXLimits = true;
			m_xlimits = ImVec2(x_min, x_max);
		}

		void setYLimits(float y_min, float y_max)
		{
			m_setYLimits = true;
			m_ylimits = ImVec2(y_min, y_max);
		}

		void setXLimitsAuto()
		{
			m_setXLimits = false;
		}

		void setYLimitsAuto()
		{
			m_setYLimits = false;
		}

		[[nodiscard]] bool isPlotQueried() const
		{
			return m_queried;
		}

		float* getPlotQueryArea()
		{
			return m_queryArea;
		}

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
		std::string     m_queryCallback;
		bool            m_queried = false;
		float           m_queryArea[4] = {0.0f , 0.0f, 0.0f, 0.0f};
		
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