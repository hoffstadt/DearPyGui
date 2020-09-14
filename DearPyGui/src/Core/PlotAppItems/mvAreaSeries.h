#pragma once
#include "mvPlot.h"

namespace Marvel {

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
			auto item = ImPlot::RegisterOrGetItem(m_name.c_str());
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