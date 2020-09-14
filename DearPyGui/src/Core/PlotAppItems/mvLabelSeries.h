#pragma once
#include "mvPlot.h"

namespace Marvel {

	class mvLabelSeries : public mvSeries
	{

	public:

		mvLabelSeries(const std::string& name, const std::vector<mvVec2>& points, int xoffset = 0, int yoffset = 0, bool vertical = false)
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

}