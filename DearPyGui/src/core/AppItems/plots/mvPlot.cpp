#include <algorithm>
#include "mvPlot.h"
#include "mvApp.h"
#include "mvLog.h"
#include "mvInput.h"
#include "mvAreaSeries.h"
#include "mvBarSeries.h"
#include "mvCandleSeries.h"
#include "mvErrorSeries.h"
#include "mvHeatSeries.h"
#include "mvImageSeries.h"
#include "mvInfiniteLineSeries.h"
#include "mvLabelSeries.h"
#include "mvPieSeries.h"
#include "mvScatterSeries.h"
#include "mvShadeSeries.h"
#include "mvStairSeries.h"
#include "mvStemSeries.h"
#include "mvImPlotThemeScope.h"
#include "mvFontScope.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvPlot::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		{
			mvPythonParser parser(mvPyDataType::String);
			mvAppItem::AddCommonArgs(parser);
			parser.removeArg("source");
			parser.removeArg("callback");
			parser.removeArg("callback_data");
			parser.removeArg("enabled");

			parser.addArg<mvPyDataType::Callable>("query_callback", mvArgType::KEYWORD_ARG, "None", "Callback ran when plot is queried");

			parser.addArg<mvPyDataType::String>("x_axis_name", mvArgType::KEYWORD_ARG, "''");
			parser.addArg<mvPyDataType::String>("y_axis_name", mvArgType::KEYWORD_ARG, "''");

			// plot flags
			parser.addArg<mvPyDataType::Bool>("no_title", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("no_legend", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("no_menus", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("no_box_select", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("no_mouse_pos", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("no_highlight", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("no_child", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("query", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("crosshairs", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("anti_aliased", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("equal_aspects", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("yaxis2", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("yaxis3", mvArgType::KEYWORD_ARG, "False");

				// x axis flags
			parser.addArg<mvPyDataType::Bool>("xaxis_no_gridlines", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("xaxis_no_tick_marks", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("xaxis_no_tick_labels", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("xaxis_log_scale", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("xaxis_time", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("xaxis_invert", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("xaxis_lock_min", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("xaxis_lock_max", mvArgType::KEYWORD_ARG, "False");

				// y axis flags
			parser.addArg<mvPyDataType::Bool>("yaxis_no_gridlines", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("yaxis_no_tick_marks", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("yaxis_no_tick_labels", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("yaxis_log_scale", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("yaxis_invert", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("yaxis_lock_min", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("yaxis_lock_max", mvArgType::KEYWORD_ARG, "False");

				// y2 axis flags
			parser.addArg<mvPyDataType::Bool>("y2axis_no_gridlines", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("y2axis_no_tick_marks", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("y2axis_no_tick_labels", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("y2axis_log_scale", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("y2axis_invert", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("y2axis_lock_min", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("y2axis_lock_max", mvArgType::KEYWORD_ARG, "False");

				// y3 axis flags
			parser.addArg<mvPyDataType::Bool>("y3axis_no_gridlines", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("y3axis_no_tick_marks", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("y3axis_no_tick_labels", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("y3axis_log_scale", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("y3axis_invert", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("y3axis_lock_min", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("y3axis_lock_max", mvArgType::KEYWORD_ARG, "False");

			parser.finalize();
			parsers->insert({ s_command, parser });
		}

		{
			mvPythonParser parser(mvPyDataType::Bool);
			parser.addArg<mvPyDataType::String>("plot");
			parser.finalize();
			parsers->insert({ "is_plot_queried", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::FloatList);
			parser.addArg<mvPyDataType::String>("plot");
			parser.finalize();
			parsers->insert({ "get_plot_query_area", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::FloatList);
			parser.addArg<mvPyDataType::String>("plot");
			parser.finalize();
			parsers->insert({ "get_plot_xlimits", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::FloatList);
			parser.addArg<mvPyDataType::String>("plot");
			parser.finalize();
			parsers->insert({ "get_plot_ylimits", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None);
			parser.addArg<mvPyDataType::String>("plot");
			parser.addArg<mvPyDataType::Float>("xmin");
			parser.addArg<mvPyDataType::Float>("xmax");
			parser.finalize();
			parsers->insert({ "set_plot_xlimits", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None);
			parser.addArg<mvPyDataType::String>("plot");
			parser.addArg<mvPyDataType::Float>("ymin");
			parser.addArg<mvPyDataType::Float>("ymax");
			parser.finalize();
			parsers->insert({ "set_plot_ylimits", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None);
			parser.addArg<mvPyDataType::String>("plot");
			parser.finalize();
			parsers->insert({ "set_plot_xlimits_auto", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None);
			parser.addArg<mvPyDataType::String>("plot");
			parser.finalize();
			parsers->insert({ "set_plot_ylimits_auto", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None);
			parser.addArg<mvPyDataType::String>("plot");
			parser.finalize();
			parsers->insert({ "reset_xticks", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None);
			parser.addArg<mvPyDataType::String>("plot");
			parser.finalize();
			parsers->insert({ "reset_yticks", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None);
			parser.addArg<mvPyDataType::String>("plot");
			parser.addArg<mvPyDataType::Object>("label_pairs");
			parser.finalize();
			parsers->insert({ "set_xticks", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None);
			parser.addArg<mvPyDataType::String>("plot");
			parser.addArg<mvPyDataType::Object>("label_pairs");
			parser.finalize();
			parsers->insert({ "set_yticks", parser });
		}


	}

	static bool CheckList(const char* plot, PyObject* list)
	{
		if (!PyList_Check(list))
		{
			ThrowPythonException(std::string(plot) + " add area series requires a list of floats.");
			return false;
		}
		return true;
	}

	static bool CheckIfPlotOk(const char* name, mvAppItem* plot)
	{
		if (plot == nullptr)
		{
			ThrowPythonException(std::string(name) + " plot does not exist.");
			return false;
		}

		if (plot->getType() != mvAppItemType::mvPlot)
		{
			ThrowPythonException(std::string(name) + " is not a plot.");
			return false;
		}
		return true;
	}

	static bool Check2ArraySizes(const char* name, const std::vector<float>* first, const std::vector<float>* second)
	{
		if (second == nullptr)
			return true;

		return first->size() == second->size();
	}

	static bool CheckArraySizes(const char* name, const std::vector<const std::vector<float>*>& arrays)
	{
		for (size_t i = 0; i < arrays.size() - 1; i++)
		{
			if (!Check2ArraySizes(name, arrays[i], arrays[i + 1]))
			{
				ThrowPythonException(std::string(name) + " data list must be the same size.");
				return false;
			}
		}
		return true;
	}

	mvPlot::mvPlot(const std::string& name)
		: mvAppItem(name)
	{
		m_width = -1;
		m_height = -1;
	}

	void mvPlot::onChildAdd(mvRef<mvAppItem> item)
	{
		if (static_cast<mvSeriesBase*>(item.get())->doesSeriesContributeToBounds())
			updateBounds();
	}

	void mvPlot::updateBounds()
	{
		bool first = true;
		bool first_0 = true;
		bool first_1 = true;
		bool first_2 = true;
		for (auto& series : m_children[1])
		{
			mvSeriesBase* child = static_cast<mvSeriesBase*>(series.get());
			if (child->doesSeriesContributeToBounds())
			{
				const auto& x_maxMin = child->getMaxMin(0);
				const auto& y_maxMin = child->getMaxMin(1);
				ImPlotYAxis axis = child->getAxis();

				if (first && !m_setXLimits)
				{
					m_xlimits.x = x_maxMin.second;
					m_xlimits.y = x_maxMin.first;
					first = false;
				}
				else if(!m_setXLimits)
				{
					if (x_maxMin.second < m_xlimits.x) m_xlimits.x = x_maxMin.second;
					if (x_maxMin.first > m_xlimits.y) m_xlimits.y = x_maxMin.first;
				}

				switch (axis)
				{
				case ImPlotYAxis_1:
				{
					if (first_0 && !m_setYLimits)
					{
						m_ylimits.x = y_maxMin.second;
						m_ylimits.y = y_maxMin.first;
						first_0 = false;
					}
					else if(!m_setYLimits)
					{
						if (y_maxMin.second < m_ylimits.x) m_ylimits.x = y_maxMin.second;
						if (y_maxMin.first > m_ylimits.y) m_ylimits.y = y_maxMin.first;
					}
					break;
				}
				case ImPlotYAxis_2:
				{
					if (first_1 && !m_setY2Limits)
					{
						m_y2limits.x = y_maxMin.second;
						m_y2limits.y = y_maxMin.first;
						first_1 = false;
					}
					else if(!m_setY2Limits)
					{
						if (y_maxMin.second < m_y2limits.x) m_y2limits.x = y_maxMin.second;
						if (y_maxMin.first > m_y2limits.y) m_y2limits.y = y_maxMin.first;
					}
					break;
				}
				case ImPlotYAxis_3:
				{
					if (first_2 && !m_setY3Limits)
					{
						m_y3limits.x = y_maxMin.second;
						m_y3limits.y = y_maxMin.first;
						first_2 = false;
					}
					else if(!m_setY3Limits)
					{
						if (y_maxMin.second < m_y3limits.x) m_y3limits.x = y_maxMin.second;
						if (y_maxMin.first > m_y3limits.y) m_y3limits.y = y_maxMin.first;
					}
					break;
				}
				default:
					break;
				}
				

			}
		}

		m_dirty = true;
	}

	bool mvPlot::canChildBeAdded(mvAppItemType type)
	{
		if (type == mvAppItemType::mvDragPoint) return true;
		if (type == mvAppItemType::mvDragLine) return true;
		if (type == mvAppItemType::mvAnnotation) return true;
		if (type == mvAppItemType::mvLineSeries) return true;
		if (type == mvAppItemType::mvVLineSeries) return true;
		if (type == mvAppItemType::mvHLineSeries) return true;
		if (type == mvAppItemType::mvScatterSeries) return true;
		if (type == mvAppItemType::mvStemSeries) return true;
		if (type == mvAppItemType::mvStairSeries) return true;
		if (type == mvAppItemType::mvBarSeries) return true;
		if (type == mvAppItemType::mvErrorSeries) return true;
		if (type == mvAppItemType::mvHeatSeries) return true;
		if (type == mvAppItemType::mvImageSeries) return true;
		if (type == mvAppItemType::mvPieSeries) return true;
		if (type == mvAppItemType::mvShadeSeries) return true;
		if (type == mvAppItemType::mvLabelSeries) return true;
		if (type == mvAppItemType::mvCandleSeries) return true;
		if (type == mvAppItemType::mvAreaSeries) return true;

		if (type == mvAppItemType::mvDrawLine) return true;
		if (type == mvAppItemType::mvDrawArrow) return true;
		if (type == mvAppItemType::mvDrawTriangle) return true;
		if (type == mvAppItemType::mvDrawCircle) return true;
		if (type == mvAppItemType::mvDrawBezierCurve) return true;
		if (type == mvAppItemType::mvDrawQuad) return true;
		if (type == mvAppItemType::mvDrawRect) return true;
		if (type == mvAppItemType::mvDrawText) return true;
		if (type == mvAppItemType::mvDrawPolygon) return true;
		if (type == mvAppItemType::mvDrawPolyline) return true;
		if (type == mvAppItemType::mvDrawImage) return true;

		mvThrowPythonError(1000, "Plot children must be compatible.");
		MV_ITEM_REGISTRY_ERROR("Plot children must be compatible.");
		assert(false);

		return false;
	}

	void mvPlot::SetColorMap(ImPlotColormap colormap)
	{
		if (colormap < ImPlot::GetColormapCount())
		m_dirty = true;			{
			m_colormap = colormap;
			m_dirty = true;
		}
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

	void mvPlot::draw(ImDrawList* drawlist, float x, float y)
	{

		ImGui::PushID(m_colormap);

		if (m_setXLimits || m_dirty)
			ImPlot::SetNextPlotLimitsX(m_xlimits.x, m_xlimits.y, ImGuiCond_Always);

		if (m_setYLimits || m_dirty)
			ImPlot::SetNextPlotLimitsY(m_ylimits.x, m_ylimits.y, ImGuiCond_Always);

		if (m_setY2Limits || m_dirty)
			ImPlot::SetNextPlotLimitsY(m_y2limits.x, m_y2limits.y, ImGuiCond_Always, ImPlotYAxis_2);

		if (m_setY3Limits || m_dirty)
			ImPlot::SetNextPlotLimitsY(m_y3limits.x, m_y3limits.y, ImGuiCond_Always, ImPlotYAxis_3);

		// resets automatic sizing when new data is added
		if (m_dirty) 
			m_dirty = false;

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

		mvImPlotThemeScope scope(this);
		mvFontScope fscope(this);

		if (ImPlot::BeginPlot(m_label.c_str(), m_xaxisName.empty() ? nullptr : m_xaxisName.c_str(), m_yaxisName.empty() ? nullptr : m_yaxisName.c_str(),
			ImVec2((float)m_width, (float)m_height), m_flags,
			m_xflags, m_yflags, m_y2flags, m_y3flags))
		{
			ImPlot::PushColormap(m_colormap);

			for (auto item : m_children[0])
			{
				// skip item if it's not shown
				if (!item->m_show)
					continue;
				ImPlot::PushPlotClipRect();
				item->draw(drawlist, ImPlot::GetPlotPos().x, ImPlot::GetPlotPos().y);
				ImPlot::PopPlotClipRect();
				item->getState().update();
			}

			for (auto item : m_children[1])
			{
				// skip item if it's not shown
				if (!item->m_show)
					continue;
				ImPlot::PushPlotClipRect();
				item->draw(drawlist, ImPlot::GetPlotPos().x, ImPlot::GetPlotPos().y);
				ImPlot::PopPlotClipRect();

				item->getState().update();
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
				mvApp::GetApp()->getCallbackRegistry().submitCallback([=]() {
					PyObject* area = PyTuple_New(4);
					PyTuple_SetItem(area, 0, PyFloat_FromDouble(m_queryArea[0]));
					PyTuple_SetItem(area, 1, PyFloat_FromDouble(m_queryArea[1]));
					PyTuple_SetItem(area, 2, PyFloat_FromDouble(m_queryArea[2]));
					PyTuple_SetItem(area, 3, PyFloat_FromDouble(m_queryArea[3]));
					mvApp::GetApp()->getCallbackRegistry().addCallback(m_queryCallback, m_name, area);
					});
			}


			if (ImPlot::IsPlotHovered())
				mvInput::setPlotMousePosition((float)ImPlot::GetPlotMousePos().x, (float)ImPlot::GetPlotMousePos().y);

			m_xlimits_actual.x = (float)ImPlot::GetPlotLimits().X.Min;
			m_xlimits_actual.y = (float)ImPlot::GetPlotLimits().X.Max;
			m_ylimits_actual.x = (float)ImPlot::GetPlotLimits().Y.Min;
			m_ylimits_actual.y = (float)ImPlot::GetPlotLimits().Y.Max;
			m_y2limits_actual.x = (float)ImPlot::GetPlotLimits(ImPlotYAxis_2).Y.Min;
			m_y2limits_actual.y = (float)ImPlot::GetPlotLimits(ImPlotYAxis_2).Y.Max;
			m_y3limits_actual.x = (float)ImPlot::GetPlotLimits(ImPlotYAxis_3).Y.Min;
			m_y3limits_actual.y = (float)ImPlot::GetPlotLimits(ImPlotYAxis_3).Y.Max;

			// shouldnt we just draw them when we draw widgets
			//ImPlot::PushPlotClipRect();
			//auto topleft = ImPlot::GetPlotPos();
			//m_drawList->draw(ImPlot::GetPlotDrawList(), topleft.x, topleft.y);
			//ImPlot::PopPlotClipRect();

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

	void mvPlot::setY2Limits(float y_min, float y_max)
	{
		m_setY2Limits = true;
		m_y2limits = ImVec2(y_min, y_max);
	}

	void mvPlot::setY3Limits(float y_min, float y_max)
	{
		m_setY3Limits = true;
		m_y3limits = ImVec2(y_min, y_max);
	}

	void mvPlot::setXLimitsAuto()
	{
		m_setXLimits = false;
	}

	void mvPlot::setYLimitsAuto()
	{
		m_setYLimits = false;
	}

	void mvPlot::setY2LimitsAuto()
	{
		m_setY2Limits = false;
	}

	void mvPlot::setY3LimitsAuto()
	{
		m_setY3Limits = false;
	}

	bool mvPlot::isPlotQueried() const
	{
		return m_queried;
	}

	float* mvPlot::getPlotQueryArea()
	{
		return m_queryArea;
	}

	void mvPlot::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "x_axis_name"))m_xaxisName = ToString(item);
		if (PyObject* item = PyDict_GetItemString(dict, "y_axis_name")) m_yaxisName = ToString(item);

		if (PyObject* item = PyDict_GetItemString(dict, "query_callback"))
		{
			if (m_queryCallback)
				Py_XDECREF(m_queryCallback);
			Py_XINCREF(item);
			m_queryCallback = item;
		}

		// helper for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		// plot flags
		flagop("no_title",             ImPlotFlags_NoTitle,          m_flags);
		flagop("no_legend",            ImPlotFlags_NoLegend,         m_flags);
		flagop("no_menus",             ImPlotFlags_NoMenus,          m_flags);
		flagop("no_box_select",        ImPlotFlags_NoBoxSelect,      m_flags);
		flagop("no_mouse_pos",         ImPlotFlags_NoMousePos,       m_flags);
		flagop("no_highlight",         ImPlotFlags_NoHighlight,      m_flags);
		flagop("no_child",             ImPlotFlags_NoChild,          m_flags);
		flagop("query",                ImPlotFlags_Query,            m_flags);
		flagop("crosshairs",           ImPlotFlags_Crosshairs,       m_flags);
		flagop("anti_aliased",         ImPlotFlags_AntiAliased,      m_flags);
		flagop("equal_aspects",        ImPlotFlags_Equal,            m_flags);
		flagop("yaxis2",               ImPlotFlags_YAxis2,           m_flags);
		flagop("yaxis3",               ImPlotFlags_YAxis3,           m_flags);

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

		// y2 axis flags
		flagop("y2axis_no_gridlines", ImPlotAxisFlags_NoGridLines,    m_y2flags);
		flagop("y2axis_no_tick_marks", ImPlotAxisFlags_NoTickMarks,   m_y2flags);
		flagop("y2axis_no_tick_labels", ImPlotAxisFlags_NoTickLabels, m_y2flags);
		flagop("y2axis_log_scale", ImPlotAxisFlags_LogScale,          m_y2flags);
		flagop("y2axis_invert", ImPlotAxisFlags_Invert,               m_y2flags);
		flagop("y2axis_lock_min", ImPlotAxisFlags_LockMin,            m_y2flags);
		flagop("y2axis_lock_max", ImPlotAxisFlags_LockMax,            m_y2flags);

		// y3 axis flags
		flagop("y3axis_no_gridlines", ImPlotAxisFlags_NoGridLines,    m_y3flags);
		flagop("y3axis_no_tick_marks", ImPlotAxisFlags_NoTickMarks,   m_y3flags);
		flagop("y3axis_no_tick_labels", ImPlotAxisFlags_NoTickLabels, m_y3flags);
		flagop("y3axis_log_scale", ImPlotAxisFlags_LogScale,          m_y3flags);
		flagop("y3axis_invert", ImPlotAxisFlags_Invert,               m_y3flags);
		flagop("y3axis_lock_min", ImPlotAxisFlags_LockMin,            m_y3flags);
		flagop("y3axis_lock_max", ImPlotAxisFlags_LockMax,            m_y3flags);

	}

	void mvPlot::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "x_axis_name", ToPyString(m_xaxisName));
		PyDict_SetItemString(dict, "y_axis_name", ToPyString(m_yaxisName));

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
		};

		// plot flags
		checkbitset("no_title",             ImPlotFlags_NoTitle,          m_flags);
		checkbitset("no_legend",            ImPlotFlags_NoLegend,         m_flags);
		checkbitset("no_menus",             ImPlotFlags_NoMenus,          m_flags);
		checkbitset("no_box_select",        ImPlotFlags_NoBoxSelect,      m_flags);
		checkbitset("no_mouse_pos",         ImPlotFlags_NoMousePos,       m_flags);
		checkbitset("no_highlight",         ImPlotFlags_NoHighlight,      m_flags);
		checkbitset("no_child",             ImPlotFlags_NoChild,          m_flags);
		checkbitset("query",                ImPlotFlags_Query,            m_flags);
		checkbitset("crosshairs",           ImPlotFlags_Crosshairs,       m_flags);
		checkbitset("anti_aliased",         ImPlotFlags_AntiAliased,      m_flags);
		checkbitset("equal_aspects",        ImPlotFlags_Equal,            m_flags);
		checkbitset("yaxis2",               ImPlotFlags_YAxis2,           m_flags);
		checkbitset("yaxis3",               ImPlotFlags_YAxis3,           m_flags);

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

		// y2 axis flags
		checkbitset("y2axis_no_gridlines",   ImPlotAxisFlags_NoGridLines,  m_y2flags);
		checkbitset("y2axis_no_tick_marks",  ImPlotAxisFlags_NoTickMarks,  m_y2flags);
		checkbitset("y2axis_no_tick_labels", ImPlotAxisFlags_NoTickLabels, m_y2flags);
		checkbitset("y2axis_log_scale",      ImPlotAxisFlags_LogScale,     m_y2flags);
		checkbitset("y2axis_invert",         ImPlotAxisFlags_Invert,       m_y2flags);
		checkbitset("y2axis_lock_min",       ImPlotAxisFlags_LockMin,      m_y2flags);
		checkbitset("y2axis_lock_max",       ImPlotAxisFlags_LockMax,      m_y2flags);

		// y3 axis flags
		checkbitset("y3axis_no_gridlines",   ImPlotAxisFlags_NoGridLines,  m_y3flags);
		checkbitset("y3axis_no_tick_marks",  ImPlotAxisFlags_NoTickMarks,  m_y3flags);
		checkbitset("y3axis_no_tick_labels", ImPlotAxisFlags_NoTickLabels, m_y3flags);
		checkbitset("y3axis_log_scale",      ImPlotAxisFlags_LogScale,     m_y3flags);
		checkbitset("y3axis_invert",         ImPlotAxisFlags_Invert,       m_y3flags);
		checkbitset("y3axis_lock_min",       ImPlotAxisFlags_LockMin,      m_y3flags);
		checkbitset("y3axis_lock_max",       ImPlotAxisFlags_LockMax,      m_y3flags);
	}

	PyObject* mvPlot::add_plot(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		std::string name = std::string(std::string("$$DPG_ns") + s_internal_id + std::to_string(i));
		auto [parent, before] = mvAppItem::GetNameFromArgs(name, args, kwargs);
		auto item = CreateRef<mvPlot>(name);
		item->checkArgs(args, kwargs);
		item->handleSpecificPositionalArgs(args);
		item->handleKeywordArgs(kwargs);
		mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent.c_str(), before.c_str());
		return ToPyString(name);
	}

	PyObject* mvPlot::reset_xticks(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;

		if (!(mvApp::GetApp()->getParsers())["reset_xticks"].parse(args, kwargs, __FUNCTION__, &plot))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());

		graph->resetXTicks();

		return GetPyNone();
	}

	PyObject* mvPlot::reset_yticks(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;

		if (!(mvApp::GetApp()->getParsers())["reset_yticks"].parse(args, kwargs, __FUNCTION__, &plot))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());

		graph->resetYTicks();

		return GetPyNone();
	}

	PyObject* mvPlot::set_xticks(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		PyObject* label_pairs;

		if (!(mvApp::GetApp()->getParsers())["set_xticks"].parse(args, kwargs, __FUNCTION__, &plot, &label_pairs))
			return GetPyNone();

		auto mlabel_pairs = ToVectPairStringFloat(label_pairs);

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());

		std::vector<std::string> labels;
		std::vector<double> locations;

		for (const auto& item : mlabel_pairs)
		{
			labels.emplace_back(item.first.c_str());
			locations.emplace_back((double)item.second);
		}
		graph->setXTicks(labels, locations);


		return GetPyNone();
	}

	PyObject* mvPlot::set_yticks(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		PyObject* label_pairs;

		if (!(mvApp::GetApp()->getParsers())["set_yticks"].parse(args, kwargs, __FUNCTION__, &plot, &label_pairs))
			return GetPyNone();

		auto mlabel_pairs = ToVectPairStringFloat(label_pairs);

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());

		std::vector<std::string> labels;
		std::vector<double> locations;

		for (const auto& item : mlabel_pairs)
		{
			labels.emplace_back(item.first.c_str());
			locations.emplace_back((double)item.second);
		}
		graph->setYTicks(labels, locations);

		return GetPyNone();
	}

	PyObject* mvPlot::set_plot_xlimits_auto(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;

		if (!(mvApp::GetApp()->getParsers())["set_plot_xlimits_auto"].parse(args, kwargs, __FUNCTION__, &plot))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());

		graph->setXLimitsAuto();

		return GetPyNone();
	}

	PyObject* mvPlot::set_plot_ylimits_auto(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;

		if (!(mvApp::GetApp()->getParsers())["set_plot_ylimits_auto"].parse(args, kwargs, __FUNCTION__, &plot))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());

		graph->setYLimitsAuto();

		return GetPyNone();
	}

	PyObject* mvPlot::set_plot_xlimits(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		float xmin;
		float xmax;

		if (!(mvApp::GetApp()->getParsers())["set_plot_xlimits"].parse(args, kwargs, __FUNCTION__, &plot, &xmin, &xmax))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());

		graph->setXLimits(xmin, xmax);

		return GetPyNone();
	}

	PyObject* mvPlot::set_plot_ylimits(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		float ymin;
		float ymax;

		if (!(mvApp::GetApp()->getParsers())["set_plot_ylimits"].parse(args, kwargs, __FUNCTION__, &plot, &ymin, &ymax))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());

		graph->setYLimits(ymin, ymax);

		return GetPyNone();
	}

	PyObject* mvPlot::is_plot_queried(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;

		if (!(mvApp::GetApp()->getParsers())["is_plot_queried"].parse(args, kwargs, __FUNCTION__, &plot))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());

		return ToPyBool(graph->isPlotQueried());
	}

	PyObject* mvPlot::get_plot_xlimits(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;

		if (!(mvApp::GetApp()->getParsers())["get_plot_xlimits"].parse(args, kwargs, __FUNCTION__, &plot))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());

		const ImVec2& lim = graph->getXLimits();
		return ToPyPair(lim.x, lim.y);
	}

	PyObject* mvPlot::get_plot_ylimits(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;

		if (!(mvApp::GetApp()->getParsers())["get_plot_ylimits"].parse(args, kwargs, __FUNCTION__, &plot))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());

		const ImVec2& lim = graph->getYLimits();
		return ToPyPair(lim.x, lim.y);
	}

	PyObject* mvPlot::get_plot_query_area(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;

		if (!(mvApp::GetApp()->getParsers())["get_plot_query_area"].parse(args, kwargs, __FUNCTION__, &plot))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());

		float* result = graph->getPlotQueryArea();
		return Py_BuildValue("(ffff)", result[0], result[1], result[2], result[3]);
	}

}
