#include <algorithm>
#include "mvPlot.h"
#include "mvApp.h"
#include "PythonUtilities/mvPythonTranslator.h"
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "core/mvInput.h"
#include "Registries/mvValueStorage.h"

namespace Marvel {

	mvSeries::mvSeries(std::string name, const std::vector<const std::vector<float>*> data)
		:
		m_name(name)
	{

		for (const auto* list : data)
			m_data.push_back(*list);

		if (!m_data[0].empty())
		{
			m_maxX = m_data[0][0];
			m_minX = m_data[0][0];
		}

		if (m_data.size() > 1)
		{
			if (!m_data[1].empty())
			{
				m_maxY = m_data[0][1];
				m_minY = m_data[0][1];
			}
		}

		for (const auto& x : m_data[0])
		{
			if (x > m_maxX) m_maxX = x;
			if (x < m_minX) m_minX = x;
			
		}

		if (m_data.size() > 1)
		{
			for (const auto& y : m_data[1])
			{
				if (y > m_maxY) m_maxY = y;
				if (y < m_minY) m_minY = y;
			}
		}

	}

	mvSeries::mvSeries(std::string name, const ImPlotPoint& boundsMin, const ImPlotPoint& boundsMax)
		: m_name(std::move(name))
	{
		m_maxX = (float)boundsMax.x;
		m_maxY = (float)boundsMax.y;
		m_minX = (float)boundsMin.x;
		m_minY = (float)boundsMin.y;
	}

	mvPlot::mvPlot(const std::string& name, PyObject* queryCallback)
		: mvAppItem(name), m_queryCallback(queryCallback)
	{
		m_width = -1;
		m_height = -1;
	}

	void mvPlot::addDragPoint(const std::string& name, bool show_label, const mvColor& color, float radius, PyObject* callback, double* dummyValue, const std::string& source)
	{
		float* value = mvValueStorage::AddFloat2Value(source, { (float)dummyValue[0], (float)dummyValue[1] });

		m_dragPoints.push_back({ name, value, show_label, color, radius, callback, value[0], value[1], source});
	}

	void mvPlot::updateDragPoint(const std::string& name, bool show_label, const mvColor& color, float radius, PyObject* callback, double* dummyValue, const std::string& source)
	{
		// check if drag point exist
		bool exists = false;
		for (auto& item : m_dragPoints)
		{
			if (item.name == name)
			{
				exists = true;
				item.name = name;
				if (item.source != source)
				{
					mvValueStorage::DecrementRef(source.empty() ? name : source);
					item.value = mvValueStorage::AddFloat2Value(source.empty() ? name : source, { (float)dummyValue[0], (float)dummyValue[1] });
				}
				item.show_label = show_label;
				item.color = color;
				item.radius = radius;
				item.callback = callback;
				item.dummyx = dummyValue[0];
				item.dummyy = dummyValue[1];
				item.source = source;
				break;
			}
		}

		if (!exists)
			addDragPoint(name, show_label, color, radius, callback, dummyValue, source.empty() ? name : source);
	}

	void mvPlot::addDragLine(const std::string& name, bool show_label, const mvColor& color, float thickness, bool y_line, PyObject* callback, double dummyValue, const std::string& source)
	{
		float* value = mvValueStorage::AddFloatValue(source, (float)dummyValue);

		m_dragLines.push_back({ name, value, show_label, color, thickness, y_line, callback, *value, source});
	}

	void mvPlot::updateDragLine(const std::string& name, bool show_label, const mvColor& color, float thickness, bool y_line, PyObject* callback, double dummyValue, const std::string& source)
	{

		// check if drag line exist
		bool exists = false;
		for (auto& item : m_dragLines)
		{
			if (item.name == name)
			{
				exists = true;
				item.name = name;
				if (item.source != source)
				{
					mvValueStorage::DecrementRef(source.empty() ? name : source);
					item.value = mvValueStorage::AddFloatValue(source.empty() ? name : source, (float)dummyValue);
				}
				item.show_label = show_label;
				item.color = color;
				item.thickness = thickness;
				item.y_line = y_line;
				item.callback = callback;
				item.dummyValue = dummyValue;
				item.source = source;
				break;
			}
		}

		if (!exists)
			addDragLine(name, show_label, color, thickness, y_line, callback, dummyValue, source.empty() ? name : source);
	}

	void mvPlot::deleteDragPoint(const std::string& name)
	{
		// check if annotations exist
		bool exists = false;
		for (auto item : m_dragPoints)
		{
			if (item.name == name)
				exists = true;
		}

		if (exists)
		{
			auto oldDragPoints = m_dragPoints;
			m_dragPoints.clear();

			for (auto item : oldDragPoints)
			{
				if (item.name == name)
				{
					mvValueStorage::DecrementRef(item.source);
					continue;
				}

				m_dragPoints.push_back(item);
			}

		}
	}

	void mvPlot::deleteDragLine(const std::string& name)
	{
		// check if annotations exist
		bool exists = false;
		for (auto item : m_dragLines)
		{
			if (item.name == name)
				exists = true;
		}

		if (exists)
		{
			auto oldDragLines = m_dragLines;
			m_dragLines.clear();

			for (auto item : oldDragLines)
			{
				if (item.name == name)
				{
					mvValueStorage::DecrementRef(item.source);
					continue;
				}

				m_dragLines.push_back(item);
			}

		}
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
			if (item->getName() == series->getName() && series->getSeriesType() == item->getSeriesType())
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
				if (item->getName() == series->getName() && series->getSeriesType() == item->getSeriesType())
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

		for (auto& line : m_dragLines)
			mvValueStorage::DecrementRef(line.source);

		for (auto& point : m_dragPoints)
			mvValueStorage::DecrementRef(point.source);

		m_series.clear();
		m_annotations.clear();
		m_dragLines.clear();
		m_dragPoints.clear();

	}

	void mvPlot::draw()
	{
		if (m_colormapscale)
		{
			ImPlot::ShowColormapScale(m_scale_min, m_scale_max, (float)m_scale_height);
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

		if (ImPlot::BeginPlot(m_name.c_str(), m_xaxisName.empty() ? nullptr : m_xaxisName.c_str(), m_yaxisName.empty() ? nullptr : m_yaxisName.c_str(),
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

			// drag lines
			if (m_showDragLines)
			{
				for (auto& line : m_dragLines)
				{
					line.dummyValue = *line.value;
					if (line.y_line)
					{
						if (ImPlot::DragLineY(line.name.c_str(), &line.dummyValue, line.show_label, line.color.toVec4(), line.thickness))
						{
							*line.value = (float)line.dummyValue;
							mvApp::GetApp()->runCallback(line.callback, line.name, nullptr);
						}
					}
					else
					{
						if (ImPlot::DragLineX(line.name.c_str(), &line.dummyValue, line.show_label, line.color.toVec4(), line.thickness))
						{
							*line.value = (float)line.dummyValue;
							mvApp::GetApp()->runCallback(line.callback, line.name, nullptr);
						}
					}
				}
			}

			// drag points
			if (m_showDragPoints)
			{
				for (auto& point : m_dragPoints)
				{
					point.dummyx = point.value[0];
					point.dummyy = point.value[1];
					if (ImPlot::DragPoint(point.name.c_str(), &point.dummyx, &point.dummyy, point.show_label, point.color.toVec4(), point.radius))
					{
						point.value[0] = (float)point.dummyx;
						point.value[1] = (float)point.dummyy;
						mvApp::GetApp()->runCallback(point.callback, point.name, nullptr);
					}
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
				mvInput::setPlotMousePosition((float)ImPlot::GetPlotMousePos().x, (float)ImPlot::GetPlotMousePos().y);

			m_xlimits_actual.x = ImPlot::GetPlotLimits().X.Min;
			m_xlimits_actual.y = ImPlot::GetPlotLimits().X.Max;
			m_ylimits_actual.x = ImPlot::GetPlotLimits().Y.Min;
			m_ylimits_actual.y = ImPlot::GetPlotLimits().Y.Max;

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
		if (dict == nullptr)
			return;

		mvGlobalIntepreterLock gil;

		if (PyObject* item = PyDict_GetItemString(dict, "x_axis_name"))m_xaxisName = ToString(item);
		if (PyObject* item = PyDict_GetItemString(dict, "y_axis_name")) m_yaxisName = ToString(item);
		if (PyObject* item = PyDict_GetItemString(dict, "show_color_scale")) m_colormapscale = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "show_annotations")) m_showAnnotations = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "show_drag_lines")) m_showDragLines = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "show_drag_points")) m_showDragPoints = ToBool(item);
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
		if (dict == nullptr)
			return;

		mvGlobalIntepreterLock gil;

		PyDict_SetItemString(dict, "x_axis_name", ToPyString(m_xaxisName));
		PyDict_SetItemString(dict, "y_axis_name", ToPyString(m_yaxisName));
		PyDict_SetItemString(dict, "show_color_scale", ToPyBool(m_colormapscale));
		PyDict_SetItemString(dict, "show_annotations", ToPyBool(m_showAnnotations));
		PyDict_SetItemString(dict, "show_drag_lines", ToPyBool(m_showDragLines));
		PyDict_SetItemString(dict, "show_drag_points", ToPyBool(m_showDragPoints));
		PyDict_SetItemString(dict, "scale_min", ToPyFloat(m_scale_min));
		PyDict_SetItemString(dict, "scale_max", ToPyFloat(m_scale_max));
		PyDict_SetItemString(dict, "scale_height", ToPyInt(m_scale_height));

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
