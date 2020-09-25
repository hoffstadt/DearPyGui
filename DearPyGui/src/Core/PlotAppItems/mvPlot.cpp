#include "mvPlot.h"
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "Core/mvInput.h"

namespace Marvel {

	mvSeries::mvSeries(std::string name, const ImPlotPoint& boundsMin, const ImPlotPoint& boundsMax)
		: m_name(std::move(name))
	{
		m_maxX = boundsMax.x;
		m_maxY = boundsMax.y;
		m_minX = boundsMin.x;
		m_minY = boundsMin.y;
	}

	mvSeries::mvSeries(std::string name, const std::vector<mvVec2>& points)
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

	mvSeries::mvSeries(std::string name, const std::vector<mvVec4>& points)
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
			m_extra1.push_back(point.z);
			m_extra2.push_back(point.w);
		}

	}

	mvSeries::mvSeries(std::string name, const std::vector<std::vector<float>>& points)
		: m_name(std::move(name))
	{

		m_maxX = 1;
		m_minX = 0;
		m_maxY = 1;
		m_minY = 0;
		m_extra3 = points;
	}

	mvPlot::mvPlot(const std::string& name, PyObject* queryCallback)
		: mvAppItem(name), m_queryCallback(queryCallback)
	{
		m_width = -1;
		m_height = -1;
	}

	void mvPlot::addAnnotation(const std::string& name, double x, double y, float xoffset, float yoffset, const mvColor& color, const std::string& text, bool clamped)
	{
		m_annotations.push_back({ name, x, y, { xoffset, yoffset }, color, text, clamped });
	}

	void mvPlot::updateAnnotation(const std::string& name, double x, double y, float xoffset, float yoffset, const mvColor& color, const std::string& text, bool clamped)
	{
		if (name.empty())
		{
			addAnnotation(name, x, y, xoffset, yoffset, color, text, clamped);
			return;
		}

		// check if annotation exist
		bool exists = false;
		for (auto& item : m_annotations)
		{
			if (item.name == name)
			{
				exists = true;
				item.name = name;
				item.x = x;
				item.y = y;
				item.pix_offset.x = xoffset;
				item.pix_offset.y = yoffset;
				item.color = color;
				item.text = text;
				item.clamped = clamped;
			}
		}

		if(!exists)
			addAnnotation(name, x, y, xoffset, yoffset, color, text, clamped);
	}

	void mvPlot::deleteAnnotation(const std::string& name)
	{
		// check if annotations exist
		bool exists = false;
		for (auto item : m_annotations)
		{
			if (item.name == name)
				exists = true;
		}

		if (exists)
		{
			auto oldAnnotations = m_annotations;
			m_annotations.clear();

			for (auto item : oldAnnotations)
			{
				if (item.name == name)
					continue;

				m_annotations.push_back(item);
			}

		}
	}

	void mvPlot::addSeries(mvSeries* series, bool updateBounds)
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

		if(updateBounds)
			m_dirty = true;
	}

	void mvPlot::updateSeries(mvSeries* series, bool updateBounds)
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
					addSeries(series, updateBounds);
					continue;
				}

				m_series.push_back(item);
			}

			return;
		}

		addSeries(series, updateBounds);
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
		m_annotations.clear();

	}

	void mvPlot::draw()
	{
		if (m_colormapscale)
		{
			ImPlot::ShowColormapScale(m_scale_min, m_scale_max, m_scale_height);
			ImGui::SameLine();
		}

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
			m_xflags, m_yflags, m_y2flags, m_y3flags))
		{
			ImPlot::PushColormap(m_colormap);

			// series
			for (auto series : m_series)
				series->draw();

			// annotations
			if (m_showAnnotations)
			{
				for (const auto& annotation : m_annotations)
				{
					if (annotation.clamped)
						ImPlot::AnnotateClamped(annotation.x, annotation.y, annotation.pix_offset, annotation.color.toVec4(), annotation.text.c_str());
					else
						ImPlot::Annotate(annotation.x, annotation.y, annotation.pix_offset, annotation.color.toVec4(), annotation.text.c_str());
				}
			}

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


			if (ImPlot::IsPlotHovered())
				mvInput::setPlotMousePosition(ImPlot::GetPlotMousePos().x, ImPlot::GetPlotMousePos().y);

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

	void mvPlot::setExtraConfigDict(PyObject* dict)
	{
		mvGlobalIntepreterLock gil;

		if (PyObject* item = PyDict_GetItemString(dict, "xAxisName")) m_xaxisName = ToString(item);
		if (PyObject* item = PyDict_GetItemString(dict, "yAxisName")) m_yaxisName = ToString(item);
		if (PyObject* item = PyDict_GetItemString(dict, "show_color_scale")) m_colormapscale = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "show_annotations")) m_showAnnotations = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "scale_min")) m_scale_min = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "scale_max")) m_scale_max = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "scale_height")) m_scale_height = ToInt(item);

		// helper for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		// plot flags
		flagop("no_legend",            ImPlotFlags_NoLegend,         m_flags);
		flagop("no_menus",             ImPlotFlags_NoMenus,          m_flags);
		flagop("no_box_select",        ImPlotFlags_NoBoxSelect,      m_flags);
		flagop("no_mouse_pos",         ImPlotFlags_NoMousePos,       m_flags);
		flagop("no_highlight",         ImPlotFlags_NoHighlight,      m_flags);
		flagop("no_child",             ImPlotFlags_NoChild,          m_flags);
		flagop("query",                ImPlotFlags_Query,            m_flags);
		flagop("crosshairs",           ImPlotFlags_Crosshairs,       m_flags);
		flagop("anti_aliased",         ImPlotFlags_AntiAliased,      m_flags);

		// x axis flags
		flagop("xaxis_no_gridlines",   ImPlotAxisFlags_NoGridLines,  m_xflags);
		flagop("xaxis_no_tick_marks",  ImPlotAxisFlags_NoTickMarks,  m_xflags);
		flagop("xaxis_no_tick_labels", ImPlotAxisFlags_NoTickLabels, m_xflags);
		flagop("xaxis_log_scale",      ImPlotAxisFlags_LogScale,     m_xflags);
		flagop("xaxis_time",           ImPlotAxisFlags_Time,         m_xflags);
		flagop("xaxis_invert",         ImPlotAxisFlags_Invert,       m_xflags);
		flagop("xaxis_lock_min",       ImPlotAxisFlags_LockMin,      m_xflags);
		flagop("xaxis_lock_max",       ImPlotAxisFlags_LockMax,      m_xflags);

		// y axis flags
		flagop("yaxis_no_gridlines",   ImPlotAxisFlags_NoGridLines,  m_yflags);
		flagop("yaxis_no_tick_marks",  ImPlotAxisFlags_NoTickMarks,  m_yflags);
		flagop("yaxis_no_tick_labels", ImPlotAxisFlags_NoTickLabels, m_yflags);
		flagop("yaxis_log_scale",      ImPlotAxisFlags_LogScale,     m_yflags);
		flagop("yaxis_invert",         ImPlotAxisFlags_Invert,       m_yflags);
		flagop("yaxis_lock_min",       ImPlotAxisFlags_LockMin,      m_yflags);
		flagop("yaxis_lock_max",       ImPlotAxisFlags_LockMax,      m_yflags);

	}

	void mvPlot::getExtraConfigDict(PyObject* dict)
	{
		mvGlobalIntepreterLock gil;

		PyDict_SetItemString(dict, "xAxisName", ToPyString(m_xaxisName));
		PyDict_SetItemString(dict, "yAxisName", ToPyString(m_yaxisName));
		PyDict_SetItemString(dict, "show_color_scale", ToPyBool(m_colormapscale));
		PyDict_SetItemString(dict, "show_annotations", ToPyBool(m_showAnnotations));
		PyDict_SetItemString(dict, "scale_min", ToPyFloat(m_scale_min));
		PyDict_SetItemString(dict, "scale_max", ToPyFloat(m_scale_max));
		PyDict_SetItemString(dict, "scale_height", ToPyFloat(m_scale_height));

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
		};

		// plot flags
		checkbitset("no_legend",            ImPlotFlags_NoLegend,         m_flags);
		checkbitset("no_menus",             ImPlotFlags_NoMenus,          m_flags);
		checkbitset("no_box_select",        ImPlotFlags_NoBoxSelect,      m_flags);
		checkbitset("no_mouse_pos",         ImPlotFlags_NoMousePos,       m_flags);
		checkbitset("no_highlight",         ImPlotFlags_NoHighlight,      m_flags);
		checkbitset("no_child",             ImPlotFlags_NoChild,          m_flags);
		checkbitset("query",                ImPlotFlags_Query,            m_flags);
		checkbitset("crosshairs",           ImPlotFlags_Crosshairs,       m_flags);
		checkbitset("anti_aliased",         ImPlotFlags_AntiAliased,      m_flags);

		// x axis flags
		checkbitset("xaxis_no_gridlines",   ImPlotAxisFlags_NoGridLines,  m_xflags);
		checkbitset("xaxis_no_tick_marks",  ImPlotAxisFlags_NoTickMarks,  m_xflags);
		checkbitset("xaxis_no_tick_labels", ImPlotAxisFlags_NoTickLabels, m_xflags);
		checkbitset("xaxis_log_scale",      ImPlotAxisFlags_LogScale,     m_xflags);
		checkbitset("xaxis_time",           ImPlotAxisFlags_Time,         m_xflags);
		checkbitset("xaxis_invert",         ImPlotAxisFlags_Invert,       m_xflags);
		checkbitset("xaxis_lock_min",       ImPlotAxisFlags_LockMin,      m_xflags);
		checkbitset("xaxis_lock_max",       ImPlotAxisFlags_LockMax,      m_xflags);

		// y axis flags
		checkbitset("yaxis_no_gridlines",   ImPlotAxisFlags_NoGridLines,  m_yflags);
		checkbitset("yaxis_no_tick_marks",  ImPlotAxisFlags_NoTickMarks,  m_yflags);
		checkbitset("yaxis_no_tick_labels", ImPlotAxisFlags_NoTickLabels, m_yflags);
		checkbitset("yaxis_log_scale",      ImPlotAxisFlags_LogScale,     m_yflags);
		checkbitset("yaxis_invert",         ImPlotAxisFlags_Invert,       m_yflags);
		checkbitset("yaxis_lock_min",       ImPlotAxisFlags_LockMin,      m_yflags);
		checkbitset("yaxis_lock_max",       ImPlotAxisFlags_LockMax,      m_yflags);
	}
}