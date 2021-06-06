#include <algorithm>
#include "mvPlot.h"
#include "mvPlotLegend.h"
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
#include "mvPlotYAxis.h"
#include "containers/mvDragPayload.h"

namespace Marvel {

	void mvPlot::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		{
			mvPythonParser parser(mvPyDataType::String, "Undocumented function", { "Plotting", "Widgets" });
			mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
				MV_PARSER_ARG_ID |
				MV_PARSER_ARG_WIDTH |
				MV_PARSER_ARG_HEIGHT |
				MV_PARSER_ARG_INDENT |
				MV_PARSER_ARG_PARENT |
				MV_PARSER_ARG_BEFORE |
				MV_PARSER_ARG_LABEL |
				MV_PARSER_ARG_SHOW |
				MV_PARSER_ARG_DROP_CALLBACK |
				MV_PARSER_ARG_DRAG_CALLBACK |
				MV_PARSER_ARG_PAYLOAD_TYPE |
				MV_PARSER_ARG_FILTER |
				MV_PARSER_ARG_TRACKED |
				MV_PARSER_ARG_POS)
			);

			parser.addArg<mvPyDataType::Callable>("query_callback", mvArgType::KEYWORD_ARG, "None", "Callback ran when plot is queried");

			parser.addArg<mvPyDataType::String>("x_axis_name", mvArgType::KEYWORD_ARG, "''");
			// plot flags
			parser.addArg<mvPyDataType::Bool>("no_title", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("no_menus", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("no_box_select", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("no_mouse_pos", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("no_highlight", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("no_child", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("query", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("crosshairs", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("anti_aliased", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("equal_aspects", mvArgType::KEYWORD_ARG, "False");

			// x axis flags
			parser.addArg<mvPyDataType::Bool>("xaxis_no_gridlines", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("xaxis_no_tick_marks", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("xaxis_no_tick_labels", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("xaxis_log_scale", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("xaxis_time", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("xaxis_invert", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("xaxis_lock_min", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("xaxis_lock_max", mvArgType::KEYWORD_ARG, "False");

			parser.finalize();
			parsers->insert({ s_command, parser });
		}

		{
			mvPythonParser parser(mvPyDataType::Bool, "Undocumented function", { "Plotting", "Widgets" });
			parser.addArg<mvPyDataType::String>("plot");
			parser.finalize();
			parsers->insert({ "is_plot_queried", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::FloatList, "Undocumented function", { "Plotting", "Widgets" });
			parser.addArg<mvPyDataType::String>("plot");
			parser.finalize();
			parsers->insert({ "get_plot_query_area", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::FloatList, "Undocumented function", { "Plotting", "Widgets" });
			parser.addArg<mvPyDataType::String>("plot");
			parser.finalize();
			parsers->insert({ "get_plot_xlimits", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Undocumented function", { "Plotting", "Widgets" });
			parser.addArg<mvPyDataType::String>("plot");
			parser.addArg<mvPyDataType::Float>("xmin");
			parser.addArg<mvPyDataType::Float>("xmax");
			parser.finalize();
			parsers->insert({ "set_plot_xlimits", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Undocumented function", { "Plotting", "Widgets" });
			parser.addArg<mvPyDataType::String>("plot");
			parser.finalize();
			parsers->insert({ "set_plot_xlimits_auto", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Undocumented function", { "Plotting", "Widgets" });
			parser.addArg<mvPyDataType::String>("plot");
			parser.finalize();
			parsers->insert({ "reset_xticks", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Undocumented function", { "Plotting", "Widgets" });
			parser.addArg<mvPyDataType::String>("plot");
			parser.addArg<mvPyDataType::Object>("label_pairs");
			parser.finalize();
			parsers->insert({ "set_xticks", parser });
		}

	}

	static bool CheckList(const char* plot, PyObject* list)
	{
		if (!PyList_Check(list))
		{
			mvThrowPythonError(1000, std::string(plot) + " add area series requires a list of floats.");
			return false;
		}
		return true;
	}

	static bool CheckIfPlotOk(const char* name, mvAppItem* plot)
	{
		if (plot == nullptr)
		{
			mvThrowPythonError(1000, std::string(name) + " plot does not exist.");
			return false;
		}

		if (plot->getType() != mvAppItemType::mvPlot)
		{
			mvThrowPythonError(1000, std::string(name) + " is not a plot.");
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
				mvThrowPythonError(1000, std::string(name) + " data list must be the same size.");
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
		if (item->getType() == mvAppItemType::mvPlotLegend)
			m_flags &= ~ImPlotFlags_NoLegend;

		if (item->getType() == mvAppItemType::mvPlotYAxis)
		{
			updateFlags();
			updateAxesNames();
		}
	}

	void mvPlot::onChildRemoved(mvRef<mvAppItem> item)
	{

		if (item->getType() == mvAppItemType::mvPlotLegend)
			m_flags |= ImPlotFlags_NoLegend;
	}

	void mvPlot::updateBounds()
	{
		bool first = true;
		for (auto& axis : m_children[1])
		{
			for (auto& series : axis->m_children[1])
			{
				mvSeriesBase* child = static_cast<mvSeriesBase*>(series.get());
				if (child->doesSeriesContributeToBounds())
				{
					const auto& x_maxMin = child->getMaxMin(0);
					if (first && !m_setXLimits)
					{
						m_xlimits.x = x_maxMin.second;
						m_xlimits.y = x_maxMin.first;
						first = false;
					}
					else if (!m_setXLimits)
					{
						if (x_maxMin.second < m_xlimits.x) m_xlimits.x = x_maxMin.second;
						if (x_maxMin.first > m_xlimits.y) m_xlimits.y = x_maxMin.first;
					}
				}
			}
		}

		m_dirty = true;
	}

	void mvPlot::updateFlags()
	{
		for (int i = 0; i < m_children[1].size(); i++)
		{
			auto child = static_cast<mvPlotYAxis*>(m_children[1][i].get());
			switch (i)
			{
			case(0):
				m_yflags = child->getFlags();
				break;

			case(1):
				m_y1flags = child->getFlags();
				if (child->isShown())
					addFlag(ImPlotFlags_YAxis2);
				else
					removeFlag(ImPlotFlags_YAxis2);
				break;

			case(2):
				m_y2flags = child->getFlags();
				if (child->isShown())
					addFlag(ImPlotFlags_YAxis3);
				else
					removeFlag(ImPlotFlags_YAxis3);
				break;

			default:
				m_yflags = child->getFlags();
				break;
			}
		}

	}

	void mvPlot::updateAxesNames()
	{
		m_y1axisName = "";
		m_y2axisName = "";
		m_y3axisName = "";

		for (int i = 0; i < m_children[1].size(); i++)
		{
			auto axis = m_children[1][i].get();
			switch (i)
			{
			case(0):
				m_y1axisName = axis->m_specificedlabel;
				break;

			case(1):
				m_y2axisName = axis->m_specificedlabel;
				break;

			case(2):
				m_y3axisName = axis->m_specificedlabel;
				break;

			default:
				m_y1axisName = axis->m_specificedlabel;
				break;
			}
		}

	}

	bool mvPlot::canChildBeAdded(mvAppItemType type)
	{
		if (type == mvAppItemType::mvPlotLegend) return true;
		if (type == mvAppItemType::mvPlotYAxis) return true;

		if (type == mvAppItemType::mvDragPoint) return true;
		if (type == mvAppItemType::mvDragLine) return true;
		if (type == mvAppItemType::mvAnnotation) return true;

		if (type == mvAppItemType::mvDrawLine) return true;
		if (type == mvAppItemType::mvDrawArrow) return true;
		if (type == mvAppItemType::mvDrawTriangle) return true;
		if (type == mvAppItemType::mvDrawCircle) return true;
		if (type == mvAppItemType::mvDrawEllipse) return true;
		if (type == mvAppItemType::mvDrawBezierCubic) return true;
		if (type == mvAppItemType::mvDrawBezierQuadratic) return true;
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

	void mvPlot::setXTicks(const std::vector<std::string>& labels, const std::vector<double>& locations)
	{
		m_xlabels = labels;
		m_xlabelLocations = locations;

		for (const auto& item : m_xlabels)
			m_xclabels.push_back(item.data());
	}

	void mvPlot::draw(ImDrawList* drawlist, float x, float y)
	{

		ImGui::PushID(m_colormap);

		if (m_setXLimits || m_dirty)
			ImPlot::SetNextPlotLimitsX(m_xlimits.x, m_xlimits.y, ImGuiCond_Always);

		// resets automatic sizing when new data is added
		if (m_dirty) 
			m_dirty = false;

		if (!m_xlabels.empty())
		{
			// TODO: Checks
			ImPlot::SetNextPlotTicksX(m_xlabelLocations.data(), (int)m_xlabels.size(), m_xclabels.data());
		}

		// gives axes change to make changes to ticks, limits, etc.
		for (auto& item : m_children[1])
		{
			// skip item if it's not shown
			if (!item->m_show)
				continue;
			item->customAction();
		}

		mvImPlotThemeScope scope(this);
		mvFontScope fscope(this);

		if (ImPlot::BeginPlot(m_label.c_str(), m_xaxisName.empty() ? nullptr : m_xaxisName.c_str(), m_y1axisName.empty() ? nullptr : m_y1axisName.c_str(),
			ImVec2((float)m_width, (float)m_height), m_flags,
			m_xflags, m_yflags, m_y1flags, m_y2flags, m_y2axisName.empty() ? nullptr : m_y2axisName.c_str(), m_y3axisName.empty() ? nullptr : m_y3axisName.c_str()))
		{
			//ImPlot::PushPlotClipRect();
			ImPlot::PushColormap(m_colormap);

			for (auto& item : m_children[0])
			{
				// skip item if it's not shown
				if (!item->m_show)
					continue;
				item->draw(drawlist, ImPlot::GetPlotPos().x, ImPlot::GetPlotPos().y);

				item->postDraw();
			}

			for (auto& item : m_children[1])
			{
				// skip item if it's not shown
				if (!item->m_show)
					continue;
				item->draw(drawlist, ImPlot::GetPlotPos().x, ImPlot::GetPlotPos().y);

				item->postDraw();
			}

			for (auto& item : m_children[2])
			{
				// skip item if it's not shown
				if (!item->m_show)
					continue;
				
				item->draw(ImPlot::GetPlotDrawList(), ImPlot::GetPlotPos().x, ImPlot::GetPlotPos().y);
				
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
					mvApp::GetApp()->getCallbackRegistry().addCallback(m_queryCallback, m_name, area, nullptr);
					});
			}

			if (ImPlot::IsPlotHovered())
				mvInput::setPlotMousePosition((float)ImPlot::GetPlotMousePos().x, (float)ImPlot::GetPlotMousePos().y);

			m_xlimits_actual.x = (float)ImPlot::GetPlotLimits().X.Min;
			m_xlimits_actual.y = (float)ImPlot::GetPlotLimits().X.Max;

			// todo: resolve clipping
			//ImPlot::PopPlotClipRect();

			if (m_dropCallback)
			{
				ScopedID id;
				if (ImPlot::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(m_payloadType.c_str()))
					{
						auto payloadActual = static_cast<const mvDragPayload*>(payload->Data);
						mvApp::GetApp()->getCallbackRegistry().addCallback(getDropCallback(), m_name, payloadActual->getDragData(), nullptr);
					}

					ImPlot::EndDragDropTarget();
				}
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

	bool mvPlot::isPlotQueried() const
	{
		return m_queried;
	}

	float* mvPlot::getPlotQueryArea()
	{
		return m_queryArea;
	}

	void mvPlot::addFlag(ImPlotFlags flag)
	{
		m_flags |= flag;
	}

	void mvPlot::removeFlag(ImPlotFlags flag)
	{
		m_flags &= ~flag;
	}

	void mvPlot::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "x_axis_name"))m_xaxisName = ToString(item);

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

	}

	void mvPlot::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "x_axis_name", ToPyString(m_xaxisName));

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
		};

		// plot flags
		checkbitset("no_title",             ImPlotFlags_NoTitle,          m_flags);
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
	}

	void mvPlot::postDraw()
	{

		m_state.update();

		// event handlers
		for (auto& item : m_children[3])
		{
			if (!item->preDraw())
				continue;

			item->draw(nullptr, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
		}

		// drag drop
		for (auto& item : m_children[4])
		{
			if (!item->preDraw())
				continue;

			item->draw(nullptr, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
		}
	}


	PyObject* mvPlot::reset_xticks(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;

		if (!(mvApp::GetApp()->getParsers())["reset_xticks"].parse(args, kwargs, __FUNCTION__, &plot))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			mvThrowPythonError(1000, message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			mvThrowPythonError(1000, message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());

		graph->resetXTicks();

		return GetPyNone();
	}

	PyObject* mvPlot::set_xticks(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		PyObject* label_pairs;

		if (!(mvApp::GetApp()->getParsers())["set_xticks"].parse(args, kwargs, __FUNCTION__, &plot, &label_pairs))
			return GetPyNone();

		auto mlabel_pairs = ToVectPairStringFloat(label_pairs);

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			mvThrowPythonError(1000, message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			mvThrowPythonError(1000, message + " is not a plot.");
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

	PyObject* mvPlot::set_plot_xlimits_auto(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;

		if (!(mvApp::GetApp()->getParsers())["set_plot_xlimits_auto"].parse(args, kwargs, __FUNCTION__, &plot))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			mvThrowPythonError(1000, message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			mvThrowPythonError(1000, message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());

		//graph->setXLimitsAuto();

		return GetPyNone();
	}

	PyObject* mvPlot::set_plot_xlimits(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		float xmin;
		float xmax;

		if (!(mvApp::GetApp()->getParsers())["set_plot_xlimits"].parse(args, kwargs, __FUNCTION__, &plot, &xmin, &xmax))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			mvThrowPythonError(1000, message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			mvThrowPythonError(1000, message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());

		graph->setXLimits(xmin, xmax);

		return GetPyNone();
	}

	PyObject* mvPlot::is_plot_queried(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;

		if (!(mvApp::GetApp()->getParsers())["is_plot_queried"].parse(args, kwargs, __FUNCTION__, &plot))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			mvThrowPythonError(1000, message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			mvThrowPythonError(1000, message + " is not a plot.");
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

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			mvThrowPythonError(1000, message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			mvThrowPythonError(1000, message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());

		const ImVec2& lim = graph->getXLimits();
		return ToPyPair(lim.x, lim.y);
	}

	PyObject* mvPlot::get_plot_query_area(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;

		if (!(mvApp::GetApp()->getParsers())["get_plot_query_area"].parse(args, kwargs, __FUNCTION__, &plot))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			mvThrowPythonError(1000, message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			mvThrowPythonError(1000, message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());

		float* result = graph->getPlotQueryArea();
		return Py_BuildValue("(ffff)", result[0], result[1], result[2], result[3]);
	}

}
