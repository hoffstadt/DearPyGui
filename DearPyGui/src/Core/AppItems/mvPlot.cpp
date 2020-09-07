#include "mvPlot.h"
#define PY_SSIZE_T_CLEAN
#include <Python.h>

namespace Marvel {

	mvPlot::mvPlot(const std::string& parent, const std::string& name, std::string  xname,
		std::string yname, int width, int height, ImPlotFlags flags,
		ImPlotAxisFlags xflags, ImPlotAxisFlags yflags, PyObject* queryCallback)
		: mvAppItem(parent, name), m_xaxisName(std::move(xname)), m_yaxisName(std::move(yname)),
		m_flags(flags), m_xflags(xflags), m_yflags(yflags), m_queryCallback(queryCallback)
	{
		m_width = width;
		m_height = height;
	}

	void mvPlot::addSeries(mvSeries* series)
	{

		if (m_series.empty())
		{
			if (!m_setXLimits)
			{
				m_xlimits.x = series->m_minX;
				m_xlimits.y = series->m_maxX;
			}

			if (!m_setYLimits)
			{
				m_ylimits.x = series->m_minY;
				m_ylimits.y = series->m_maxY;
			}
		}
		else
		{
			if (series->m_minX < m_xlimits.x && !m_setXLimits) m_xlimits.x = series->m_minX;
			if (series->m_minY < m_ylimits.x && !m_setYLimits) m_ylimits.x = series->m_minY;
			if (series->m_maxX > m_xlimits.y && !m_setXLimits) m_xlimits.y = series->m_maxX;
			if (series->m_maxY > m_ylimits.y && !m_setYLimits) m_ylimits.y = series->m_maxY;
		}

		m_series.push_back(series);

		m_dirty = true;
	}

	void mvPlot::updateSeries(mvSeries* series)
	{

		// check if series exist
		bool exists = false;
		for (auto item : m_series)
		{
			if (item->getName() == series->getName())
			{
				exists = true;
				break;
			}
		}

		if (exists)
		{
			auto oldSeries = m_series;
			m_series.clear();

			for (auto item : oldSeries)
			{
				if (item->getName() == series->getName())
				{
					delete item;
					item = nullptr;
					addSeries(series);
					continue;
				}

				m_series.push_back(item);
			}

			return;
		}

		addSeries(series);
	}

	void mvPlot::deleteSeries(const std::string& name)
	{

		// check if series exist
		bool exists = false;
		for (auto item : m_series)
		{
			if (item->getName() == name)
			{
				exists = true;
				break;
			}
		}

		if (exists)
		{
			auto oldSeries = m_series;
			m_series.clear();

			for (auto item : oldSeries)
			{
				if (item->getName() == name)
				{
					delete item;
					item = nullptr;
					continue;
				}

				m_series.push_back(item);
			}

		}

	}

	void mvPlot::SetColorMap(ImPlotColormap colormap)
	{
		m_colormap = colormap;
	}

	void mvPlot::resetXTicks()
	{
		m_xlabels.clear();
		m_xclabels.clear();
		m_xlabelLocations.clear();
	}

	void mvPlot::resetYTicks()
	{
		m_ylabels.clear();
		m_yclabels.clear();
		m_ylabelLocations.clear();
	}

	void mvPlot::setXTicks(const std::vector<std::string>& labels, const std::vector<double>& locations)
	{
		m_xlabels = labels;
		m_xlabelLocations = locations;

		for (const auto& item : m_xlabels)
			m_xclabels.push_back(item.data());
	}

	void mvPlot::setYTicks(const std::vector<std::string>& labels, const std::vector<double>& locations)
	{
		m_ylabels = labels;
		m_ylabelLocations = locations;

		for (const auto& item : m_ylabels)
			m_yclabels.push_back(item.data());
	}

	void mvPlot::clear()
	{
		for (auto& series : m_series)
		{
			delete series;
			series = nullptr;
		}

		m_series.clear();

	}

	void mvPlot::draw()
	{
		ImGui::PushID(m_colormap);

		if (m_setXLimits || m_dirty)
			ImPlot::SetNextPlotLimitsX(m_xlimits.x, m_xlimits.y, ImGuiCond_Always);

		if (m_setYLimits || m_dirty)
			ImPlot::SetNextPlotLimitsY(m_ylimits.x, m_ylimits.y, ImGuiCond_Always);

		// resets automatic sizing when new data is added
		if (m_dirty) m_dirty = false;

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
			m_xflags, m_yflags))
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

			if (m_queryCallback != nullptr && m_queried)
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

	void mvPlot::setXLimits(float x_min, float x_max)
	{
		m_setXLimits = true;
		m_xlimits = ImVec2(x_min, x_max);
	}

	void mvPlot::setYLimits(float y_min, float y_max)
	{
		m_setYLimits = true;
		m_ylimits = ImVec2(y_min, y_max);
	}

	void mvPlot::setXLimitsAuto()
	{
		m_setXLimits = false;
	}

	void mvPlot::setYLimitsAuto()
	{
		m_setYLimits = false;
	}

	bool mvPlot::isPlotQueried() const
	{
		return m_queried;
	}

	float* mvPlot::getPlotQueryArea()
	{
		return m_queryArea;
	}

	void mvAreaSeries::drawPolygon()
	{

		ImPlotLimits limits = ImPlot::GetPlotLimits();

		std::vector<ImVec2> points;
		for (unsigned i = 0; i < m_xs.size(); i++)
		{
			float x = m_xs[i] > limits.X.Max ? limits.X.Max : m_xs[i];
			x = m_xs[i] < limits.X.Min ? limits.X.Min : x;

			float y = m_ys[i] > limits.Y.Max ? limits.Y.Max : m_ys[i];
			y = m_ys[i] < limits.Y.Min ? limits.Y.Min : y;
			auto p = ImPlot::PlotToPixels({ x, y });
			points.push_back(p);
		}

		if (m_fill.specified)
		{
			int i;
			int y;
			int miny, maxy;
			int x1, y1;
			int x2, y2;
			int ind1, ind2;
			int ints;
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
						ind1 = i - 1;
						ind2 = i;
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
					ImGui::GetWindowDrawList()->AddLine({ (float)polyints[i], (float)y }, { (float)polyints[i + 1], (float)y}, m_fill, 1.0f);
			}
			delete[] polyints;
		}

	}

}