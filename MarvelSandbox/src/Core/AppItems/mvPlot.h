#pragma once

#include "Core/mvAppItem.h"

#include "events/mvMouseEvent.h"
#include "events/mvApplicationEvent.h"
#include "layers/mvBackgroundLayer.h"
#include "layers/mvPlotLayer.h"
#include "mvSeries.h"
#include "mvImgui.h"
#include "mvGraph.h"

namespace Marvel {

	class mvPlot : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Plot)
		MV_NORETURN_VALUE()

		mvPlot(const std::string& parent, const std::string& name, int width, int height)
			: mvAppItem(parent, name), m_width(width), m_height(height)
		{

			m_graph = new marvel::mvImGuiGraph();

			// create data for the plot
			std::vector<marvel::mvPoint> dataset1;
			dataset1.emplace_back(0.0, 0.0);
			dataset1.emplace_back(1.0, 1.0);
			dataset1.emplace_back(2.0, 1.0);

			std::vector<marvel::mvPoint> dataset2;
			dataset2.emplace_back(-6.0, 3.0);
			dataset2.emplace_back(0.0, -3.0);
			dataset2.emplace_back(3.0, 0.0);

			// create a line series
			marvel::mvSeries* lineseries1 = new marvel::mvLineSeries(m_graph, "Series1", dataset1, mvGREEN);
			marvel::mvSeries* lineseries2 = new marvel::mvLineSeries(m_graph, "Series2", dataset2, mvRED);

			// add series
			m_graph->addSeries(lineseries1);
			m_graph->addSeries(lineseries2);

			//// (optional) add an input poller for additional interactions (more worked needed here)
			m_graph->addInputPoller(new marvel::mvImguiInput());

			m_graph->OnEvent(marvel::mvWindowResizeEvent(m_width, m_height));
		}

		virtual void draw() override
		{


			int m_cposx = (int)ImGui::GetCursorScreenPos().x;
			int m_cposy = (int)ImGui::GetCursorScreenPos().y;
			//int m_width = ImGui::GetWindowWidth()*0.5;
			//int m_height = ImGui::GetWindowHeight()*0.5;

			// set starting point
			m_graph->setStart(m_cposx, m_cposy);

			if(m_width != m_graph->getWidth() || m_height != m_graph->getHeight())
				m_graph->OnEvent(marvel::mvWindowResizeEvent(m_width, m_height));

			// must be called since imgui updates every frame
			m_graph->OnEvent(marvel::mvAppRenderEvent());

		}

	private:

		marvel::mvImGuiGraph* m_graph;
		int m_width;
		int m_height;

	};

}