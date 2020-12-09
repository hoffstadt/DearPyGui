#pragma once

#include "mvPlot.h"

namespace Marvel {

	class mvAreaSeries : public mvSeries
	{

	public:

		mvAreaSeries(const std::string& name, const std::vector<float>* x,
			const std::vector<float>* y, mvColor color, mvColor fill, ImPlotYAxis_ axis)
			: 
			mvSeries(name, { x, y }, axis),
			m_color(color), 
			m_fill(fill)
		{
		}

		mvSeriesType getSeriesType() override { return mvSeriesType::Area; }

		void draw() override
		{

			ImPlot::PushPlotClipRect();
			auto item = ImPlot::RegisterOrGetItem(m_name.c_str());
			if (item->Show)
				drawPolygon();
			ImPlot::PopPlotClipRect();

		}

		void drawPolygon();

	private:

		mvColor m_color = MV_DEFAULT_COLOR;
		mvColor m_fill = MV_DEFAULT_COLOR;
	};

}