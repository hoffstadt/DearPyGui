#include "mvPlot.h"
#define PY_SSIZE_T_CLEAN
#include <Python.h>

namespace Marvel {

	mvSeries::mvSeries(std::string  name, const std::vector<mvVec2>& points)
		: m_name(std::move(name))
	{
		if (!points.empty())
		{
			m_maxX = points[0].x;
			m_minX = points[0].x;
			m_maxY = points[0].y;
			m_minY = points[0].y;
		}

		for (auto& point : points)
		{
			if (point.x > m_maxX) m_maxX = point.x;
			if (point.y > m_maxY) m_maxY = point.y;
			if (point.x < m_minX) m_minX = point.x;
			if (point.y < m_minY) m_minY = point.y;
			m_xs.push_back(point.x);
			m_ys.push_back(point.y);
		}

	}

	mvPlot::mvPlot(const std::string& parent, const std::string& name, std::string  xname,
		std::string yname, ImPlotFlags flags,
		ImPlotAxisFlags xflags, ImPlotAxisFlags yflags, PyObject* queryCallback)
		: mvAppItem(parent, name), m_xaxisName(std::move(xname)), m_yaxisName(std::move(yname)),
		m_flags(flags), m_xflags(xflags), m_yflags(yflags), m_queryCallback(queryCallback)
	{
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
		m_dirty = true;
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
			ImPlot::PushColormap(m_colormap);

			for (auto series : m_series)
				series->draw();

			ImPlot::PopColormap();

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
}