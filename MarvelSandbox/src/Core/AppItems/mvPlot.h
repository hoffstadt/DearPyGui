#pragma once

#include "Core/AppItems/mvTypeBases.h"

#include "events/mvMouseEvent.h"
#include "events/mvApplicationEvent.h"
#include "layers/mvBackgroundLayer.h"
#include "layers/mvPlotLayer.h"
#include "mvSeries.h"
#include "Core/mvImGui.h"
#include "mvGraph.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvPlot
//
//-----------------------------------------------------------------------------

namespace Marvel {

	class mvPlot : public mvNoneItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Plot)

		mvPlot(const std::string& parent, const std::string& name, int width, int height)
			: mvNoneItemBase(parent, name), m_width(width), m_height(height)
		{

			m_graph = new marvel::mvImGuiGraph();

			//// (optional) add an input poller for additional interactions (more worked needed here)
			m_graph->addInputPoller(new marvel::mvImguiInput());

			m_graph->setSize(width, height);

		}

		virtual void draw() override
		{

			int m_cposx = (int)ImGui::GetCursorScreenPos().x;
			int m_cposy = (int)ImGui::GetCursorScreenPos().y;

			// set starting point
			m_graph->setStart(m_cposx, m_cposy);

			if (m_width != m_graph->getWidth() || m_height != m_graph->getHeight())
			{
				m_width = m_graph->getWidth();
				m_height = m_graph->getHeight();
				m_graph->OnEvent(marvel::mvWindowResizeEvent(m_width, m_height));
			}
				

			// must be called since imgui updates every frame
			m_graph->OnEvent(marvel::mvAppRenderEvent());

		}

		inline marvel::mvImGuiGraph* getGraph() { return m_graph; }
		inline void setHeight(int height) { m_height = height; }

	private:

		marvel::mvImGuiGraph* m_graph;
		int m_width;
		int m_height;

	};

}