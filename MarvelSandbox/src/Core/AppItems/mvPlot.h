#pragma once

#include "Core/AppItems/mvTypeBases.h"
#include <implot.h>
#include <map>
#include "Core/mvCore.h"

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

		mvSeries(const std::string& name, const std::vector<mvVec2>& points)
			: m_name(name)
		{
			for (auto point : points)
			{
				m_xs.push_back(point.x);
				m_ys.push_back(point.y);
			}

		}

		virtual ~mvSeries() = default;

		virtual void draw() = 0;

		void predraw()
		{
			for(auto item : m_variables)
				ImPlot::PushStyleVar(item.first, item.second);

			for (auto item : m_colors)
				ImPlot::PushStyleColor(item.first, item.second);
		}

		void postdraw()
		{
			ImPlot::PopStyleVar(m_variables.size());
			ImPlot::PopStyleColor(m_colors.size());
		}

		inline void setPlotColors(ImPlotCol var, int value) { m_colors.emplace_back(var, value); }
		inline void setPlotStyleVariable(ImPlotStyleVar var, int value) { m_variables.emplace_back(var, value); }

	protected:

		std::string        m_name;
		std::vector<float> m_xs;
		std::vector<float> m_ys;
		std::vector<std::pair<ImPlotStyleVar, int>> m_variables;
		std::vector<std::pair<ImPlotCol, int>> m_colors;

	};

	//-----------------------------------------------------------------------------
	// mvPlot
	//-----------------------------------------------------------------------------
	class mvPlot : public mvNoneItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Plot)

		mvPlot(const std::string& parent, const std::string& name, const std::string& xname="", 
			const std::string& yname="", int width = -1, int height = 0)
			: mvNoneItemBase(parent, name), m_xaxisName(xname), m_yaxisName(yname)
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

		void clear()
		{
			for (auto series : m_series)
			{
				delete series;
				series = nullptr;
			}

			m_series.clear();
		}

		virtual void draw() override
		{
			ImGui::PushID(m_colormap);

			if (ImPlot::BeginPlot(m_name.c_str(), m_xaxisName.c_str(), m_yaxisName.c_str(), ImVec2(m_width, m_height)) )
			{
				ImPlot::SetColormap(m_colormap);

				for (auto series : m_series)
				{
					series->predraw();
					series->draw();
					series->postdraw();
				}

				ImPlot::SetColormap(ImPlotColormap_Default);

				ImPlot::EndPlot();
			}

			ImGui::PopID();
		}


	private:

		std::string     m_xaxisName;
		std::string     m_yaxisName;
		ImPlotFlags     m_flags    = ImPlotFlags_Default;
		ImPlotAxisFlags m_x_flags  = ImPlotAxisFlags_Default;
		ImPlotAxisFlags m_y_flags  = ImPlotAxisFlags_Default;
		ImPlotAxisFlags m_y2_flags = ImPlotAxisFlags_Auxiliary;
		ImPlotAxisFlags m_y3_flags = ImPlotAxisFlags_Auxiliary;
		ImPlotColormap  m_colormap = ImPlotColormap_Default;

		std::vector<mvSeries*> m_series;

	};

	//-----------------------------------------------------------------------------
	// mvLineSeries
	//-----------------------------------------------------------------------------
	class mvLineSeries: public mvSeries
	{

	public:

		mvLineSeries(const std::string& name, const std::vector<mvVec2>& points)
			: mvSeries(name, points)
		{
		}

		virtual void draw() override
		{
			ImPlot::PlotLine(m_name.c_str(), m_xs.data(), m_ys.data(), m_xs.size());
		}

	};

	//-----------------------------------------------------------------------------
	// mvScatterSeries
	//-----------------------------------------------------------------------------
	class mvScatterSeries : public mvSeries
	{

	public:

		mvScatterSeries(const std::string& name, const std::vector<mvVec2>& points)
			: mvSeries(name, points)
		{
		}

		virtual void draw() override
		{
			ImPlot::PlotScatter(m_name.c_str(), m_xs.data(), m_ys.data(), m_xs.size());
		}

	};

}