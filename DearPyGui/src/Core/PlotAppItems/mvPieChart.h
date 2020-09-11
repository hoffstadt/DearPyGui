#pragma once

#include "Core/AppItems/mvTypeBases.h"
#include <implot.h>
#include <implot_internal.h>
#include <utility>
#include "mvCore.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvPieChart
//
//-----------------------------------------------------------------------------

namespace Marvel {


	//-----------------------------------------------------------------------------
	// mvPieChart
	//-----------------------------------------------------------------------------
	class mvPieChart : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::PieChart)

		mvPieChart(const std::string& parent, const std::string& name, bool normalize, const std::string& format)
			: mvAppItem(parent, name), m_normalize(normalize), m_format(format)
		{

		}

		void draw() override
		{
			ImPlot::PushColormap(m_colormap);
			ImPlot::SetNextPlotLimits(0, 1, 0, 1, ImGuiCond_Always);
			if (ImPlot::BeginPlot(m_name.c_str(), NULL, NULL, ImVec2(m_width, m_height), 
				ImPlotFlags_NoMousePos, ImPlotAxisFlags_NoDecorations, 
				ImPlotAxisFlags_NoDecorations)) {
				ImPlot::PlotPieChart(m_clabels.data(), m_values.data(), m_labels.size(), 
					0.5, 0.5, 0.4, m_normalize, m_format.c_str(), 180);
				ImPlot::EndPlot();
			}
			ImPlot::PopColormap();
		}

		void setColorMap(ImPlotColormap colormap){ m_colormap = colormap;}

		void addData(const std::vector<std::pair<std::string, float>>& data)
		{
			clearData();
			for (const auto& item : data)
			{
				m_labels.push_back(item.first);
				m_values.push_back(item.second);
			}

			for (const auto& item : m_labels)
				m_clabels.push_back(item.c_str());

		}

		void clearData()
		{
			m_values.clear();
			m_labels.clear();
			m_clabels.clear();
		}


	private:

		bool                     m_normalize;
		std::string              m_format;
		std::vector<std::string> m_labels;
		std::vector<const char*> m_clabels;
		std::vector<float>       m_values;
		ImPlotColormap           m_colormap = ImPlotColormap_Default;

	};

}