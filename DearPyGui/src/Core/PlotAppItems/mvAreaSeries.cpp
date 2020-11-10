#include "mvAreaSeries.h"
namespace Marvel {

	void mvAreaSeries::drawPolygon()
	{

		ImPlotLimits limits = ImPlot::GetPlotLimits();

		std::vector<ImVec2> points;
		for (unsigned i = 0; i < m_xs.size(); i++)
		{
			float x = m_xs[i] > limits.X.Max ? (float)limits.X.Max : (float)m_xs[i];
			x = m_xs[i] < limits.X.Min ? (float)limits.X.Min : x;

			float y = m_ys[i] > limits.Y.Max ? (float)limits.Y.Max : (float)m_ys[i];
			y = m_ys[i] < limits.Y.Min ? (float)limits.Y.Min : y;
			auto p = ImPlot::PlotToPixels({ x, y });
			points.push_back(p);
		}

		if (m_fill.specified)
		{
			size_t i;
			int y;
			int miny, maxy;
			int x1, y1;
			int x2, y2;
			int ind1, ind2;
			size_t ints;
			size_t n = points.size();
			int* polyints = new int[n];

			/* Determine Y maxima */
			miny = (int)points[0].y;
			maxy = (int)points[0].y;
			for (i = 1; i < n; i++)
			{
				miny = std::min(miny, (int)points[i].y);
				maxy = std::max(maxy, (int)points[i].y);
			}

			/* Draw, scanning y */
			for (y = miny; y <= maxy; y++) {
				ints = 0;
				for (i = 0; (i < n); i++) {
					if (!i)
					{
						ind1 = (int)n - 1;
						ind2 = 0;
					}
					else
					{
						ind1 = (int)i - 1;
						ind2 = (int)i;
					}
					y1 = (int)points[ind1].y;
					y2 = (int)points[ind2].y;
					if (y1 < y2)
					{
						x1 = (int)points[ind1].x;
						x2 = (int)points[ind2].x;
					}
					else if (y1 > y2)
					{
						y2 = (int)points[ind1].y;
						y1 = (int)points[ind2].y;
						x2 = (int)points[ind1].x;
						x1 = (int)points[ind2].x;
					}
					else
						continue;

					if (((y >= y1) && (y < y2)) || ((y == maxy) && (y > y1) && (y <= y2)))
						polyints[ints++] = (y - y1) * (x2 - x1) / (y2 - y1) + x1;

				}

				auto compare_int = [](const void* a, const void* b)
				{
					return (*(const int*)a) - (*(const int*)b);
				};

				qsort(polyints, ints, sizeof(int), compare_int);

				for (i = 0; i < ints; i += 2)
					ImGui::GetWindowDrawList()->AddLine({ (float)polyints[i], (float)y }, { (float)polyints[i + 1], (float)y }, m_fill, 1.0f);
			}
			delete[] polyints;
		}

	}

}
