#include <utility>
#include "mvPlotAxis.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"
#include "mvLog.h"
#include "mvPythonExceptions.h"
#include "mvImPlotThemeScope.h"
#include "mvFontScope.h"
#include "mvPlot.h"
#include "containers/mvDragPayload.h"

namespace Marvel {

	void mvPlotAxis::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		{
			mvPythonParser parser(mvPyDataType::String, "Undocumented function", { "Widgets" });
			mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
				MV_PARSER_ARG_ID |
				MV_PARSER_ARG_PARENT |
				MV_PARSER_ARG_LABEL |
				MV_PARSER_ARG_DROP_CALLBACK |
				MV_PARSER_ARG_DRAG_CALLBACK |
				MV_PARSER_ARG_PAYLOAD_TYPE |
				MV_PARSER_ARG_SHOW)
			);

			parser.addArg<mvPyDataType::Integer>("axis");
			parser.addArg<mvPyDataType::Bool>("no_gridlines", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("no_tick_marks", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("no_tick_labels", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("log_scale", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("invert", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("lock_min", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("lock_max", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("time", mvArgType::KEYWORD_ARG, "False");


			parser.finalize();

			parsers->insert({ s_command, parser });
		}

		{
			mvPythonParser parser(mvPyDataType::FloatList, "Undocumented function", { "Plotting", "Widgets" });
			parser.addArg<mvPyDataType::String>("axis");
			parser.finalize();
			parsers->insert({ "get_axis_limits", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Undocumented function", { "Plotting", "Widgets" });
			parser.addArg<mvPyDataType::String>("axis");
			parser.addArg<mvPyDataType::Float>("ymin");
			parser.addArg<mvPyDataType::Float>("ymax");
			parser.finalize();
			parsers->insert({ "set_axis_limits", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Undocumented function", { "Plotting", "Widgets" });
			parser.addArg<mvPyDataType::String>("axis");
			parser.finalize();
			parsers->insert({ "reset_axis_ticks", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Undocumented function", { "Plotting", "Widgets" });
			parser.addArg<mvPyDataType::String>("axis");
			parser.addArg<mvPyDataType::Object>("label_pairs");
			parser.finalize();
			parsers->insert({ "set_axis_ticks", parser });
		}
	}

	mvPlotAxis::mvPlotAxis(mvUUID uuid)
		: 
		mvAppItem(uuid)
	{
	}

	void mvPlotAxis::customAction()
	{
		if (m_setLimits || m_dirty)
		{
			switch (m_location)
			{
			case(0):
				ImPlot::SetNextPlotLimitsX(m_limits.x, m_limits.y, ImGuiCond_Always);
				break;

			case(1):
				ImPlot::SetNextPlotLimitsY(m_limits.x, m_limits.y, ImGuiCond_Always);
				break;

			case(2):
				ImPlot::SetNextPlotLimitsY(m_limits.x, m_limits.y, ImGuiCond_Always, ImPlotYAxis_2);
				break;

			case(3):
				ImPlot::SetNextPlotLimitsY(m_limits.x, m_limits.y, ImGuiCond_Always, ImPlotYAxis_3);
				break;

			default:
				ImPlot::SetNextPlotLimitsY(m_limits.x, m_limits.y, ImGuiCond_Always);
				break;
			}

			m_dirty = false;
			
		}

		if (!m_labels.empty())
		{
			// TODO: Checks
			if(m_location == 0)
				ImPlot::SetNextPlotTicksX(m_labelLocations.data(), (int)m_labels.size(), m_clabels.data());
			else
				ImPlot::SetNextPlotTicksY(m_labelLocations.data(), (int)m_labels.size(), m_clabels.data());
		}
	}

	void mvPlotAxis::draw(ImDrawList* drawlist, float x, float y)
	{
		mvImPlotThemeScope scope(this);
		mvFontScope fscope(this);

		// todo: add check
		if(m_axis != 0)
			ImPlot::SetPlotYAxis(m_location - 1);

		for (auto& item : m_children[1])
		{
			// skip item if it's not shown
			if (!item->m_show)
				continue;
			item->draw(drawlist, ImPlot::GetPlotPos().x, ImPlot::GetPlotPos().y);

			item->getState().update();
		}

		// x axis
		if (m_axis == 0)
		{
			//m_limits_actual.x = (float)ImPlot::GetPlotLimits(m_location).X.Min;
			//m_limits_actual.y = (float)ImPlot::GetPlotLimits(m_location).X.Max;
		}

		// y axis
		else
		{
			m_limits_actual.x = (float)ImPlot::GetPlotLimits(m_location -1).Y.Min;
			m_limits_actual.y = (float)ImPlot::GetPlotLimits(m_location -1).Y.Max;
		}

	}

	bool mvPlotAxis::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvStagingContainer) return true;
		if (type == mvAppItemType::mvPlot) return true;

		mvThrowPythonError(1000, "Drawing item parent must be a drawing.");
		MV_ITEM_REGISTRY_ERROR("Drawing item parent must be a drawing.");
		assert(false);
		return false;
	}

	void mvPlotAxis::hide()
	{
		if (auto plot = static_cast<mvPlot*>(m_parentPtr))
			plot->addFlag(ImPlotFlags_NoLegend);
		m_show = false;
	}

	void mvPlotAxis::show()
	{
		if (auto plot = static_cast<mvPlot*>(m_parentPtr))
			plot->removeFlag(ImPlotFlags_NoLegend);
		m_show = true;
	}

	bool mvPlotAxis::canChildBeAdded(mvAppItemType type)
	{

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
		if (type == mvAppItemType::mvHistogramSeries) return true;
		if (type == mvAppItemType::mv2dHistogramSeries) return true;

		mvThrowPythonError(1000, "Plot children must be compatible.");
		MV_ITEM_REGISTRY_ERROR("Plot children must be compatible.");
		assert(false);

		return false;
	}

	void mvPlotAxis::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		// helper for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		// axis flags
		flagop("no_gridlines", ImPlotAxisFlags_NoGridLines, m_flags);
		flagop("no_tick_marks", ImPlotAxisFlags_NoTickMarks, m_flags);
		flagop("no_tick_labels", ImPlotAxisFlags_NoTickLabels, m_flags);
		flagop("log_scale", ImPlotAxisFlags_LogScale, m_flags);
		flagop("invert", ImPlotAxisFlags_Invert, m_flags);
		flagop("lock_min", ImPlotAxisFlags_LockMin, m_flags);
		flagop("lock_max", ImPlotAxisFlags_LockMax, m_flags);
		flagop("time", ImPlotAxisFlags_Time, m_flags);

		
		if (m_parentPtr)
		{
			static_cast<mvPlot*>(m_parentPtr)->updateFlags();
			static_cast<mvPlot*>(m_parentPtr)->updateAxesNames();
		}
	}

	void mvPlotAxis::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!mvApp::GetApp()->getParsers()[s_command].verifyRequiredArguments(dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				m_axis = ToInt(item);
				if (m_axis > 1)
					m_axis = 1;
				break;

			default:
				break;
			}
		}
	}

	void mvPlotAxis::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
	}

	void mvPlotAxis::setLimits(float y_min, float y_max)
	{
		m_setLimits = true;
		m_limits = ImVec2(y_min, y_max);
	}

	void mvPlotAxis::onChildAdd(mvRef<mvAppItem> item)
	{
		if (static_cast<mvSeriesBase*>(item.get())->doesSeriesContributeToBounds())
			updateBounds();
	}

	void mvPlotAxis::onChildRemoved(mvRef<mvAppItem> item)
	{

	}

	void mvPlotAxis::updateBounds()
	{
		bool first = true;
		for (auto& series : m_children[1])
		{
			mvSeriesBase* child = static_cast<mvSeriesBase*>(series.get());
			if (child->doesSeriesContributeToBounds())
			{
				const auto& y_maxMin = child->getMaxMin(1);

				if (first && !m_setLimits)
				{
					m_limits.x = y_maxMin.second;
					m_limits.y = y_maxMin.first;
					first = false;
				}
				else if (!m_setLimits)
				{
					if (y_maxMin.second < m_limits.x) m_limits.x = y_maxMin.second;
					if (y_maxMin.first > m_limits.y) m_limits.y = y_maxMin.first;
				}
			}
		}

		m_dirty = true;
	}

	void mvPlotAxis::resetYTicks()
	{
		m_labels.clear();
		m_clabels.clear();
		m_labelLocations.clear();
	}

	void mvPlotAxis::setYTicks(const std::vector<std::string>& labels, const std::vector<double>& locations)
	{
		m_labels = labels;
		m_labelLocations = locations;

		for (const auto& item : m_labels)
			m_clabels.push_back(item.data());
	}

	void mvPlotAxis::postDraw()
	{

		m_state.update();

		if (m_dropCallback)
		{
			//ScopedID id;
			if (m_location == 0 && ImPlot::BeginDragDropTargetX())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(m_payloadType.c_str()))
				{
					auto payloadActual = static_cast<const mvDragPayload*>(payload->Data);
					mvApp::GetApp()->getCallbackRegistry().addCallback(getDropCallback(), m_name, payloadActual->getDragData(), nullptr);
				}

				ImPlot::EndDragDropTarget();
			}
			else if (ImPlot::BeginDragDropTargetY(m_location-1))
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(m_payloadType.c_str()))
				{
					auto payloadActual = static_cast<const mvDragPayload*>(payload->Data);
					mvApp::GetApp()->getCallbackRegistry().addCallback(getDropCallback(), m_name, payloadActual->getDragData(), nullptr);
				}

				ImPlot::EndDragDropTarget();
			}
		}
	}

	PyObject* mvPlotAxis::set_axis_ticks(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		PyObject* label_pairs;

		if (!(mvApp::GetApp()->getParsers())["set_axis_ticks"].parse(args, kwargs, __FUNCTION__, &plot, &label_pairs))
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

		if (aplot->getType() != mvAppItemType::mvPlotAxis)
		{
			std::string message = plot;
			mvThrowPythonError(1000, message + " is not a plot.");
			return GetPyNone();
		}

		mvPlotAxis* graph = static_cast<mvPlotAxis*>(aplot);

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

	PyObject* mvPlotAxis::set_axis_limits(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* axis;
		float ymin;
		float ymax;

		if (!(mvApp::GetApp()->getParsers())["set_axis_limits"].parse(args, kwargs, __FUNCTION__, &axis, &ymin, &ymax))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(axis);
		if (aplot == nullptr)
		{
			std::string message = axis;
			mvThrowPythonError(1000, message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlotAxis)
		{
			std::string message = axis;
			mvThrowPythonError(1000, message + " is not a plot.");
			return GetPyNone();
		}

		mvPlotAxis* graph = static_cast<mvPlotAxis*>(aplot);

		graph->setLimits(ymin, ymax);

		return GetPyNone();
	}

	PyObject* mvPlotAxis::get_axis_limits(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;

		if (!(mvApp::GetApp()->getParsers())["get_axis_limits"].parse(args, kwargs, __FUNCTION__, &plot))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			mvThrowPythonError(1000, message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlotAxis)
		{
			std::string message = plot;
			mvThrowPythonError(1000, message + " is not a plot.");
			return GetPyNone();
		}

		mvPlotAxis* graph = static_cast<mvPlotAxis*>(aplot);

		const ImVec2& lim = graph->getYLimits();
		return ToPyPair(lim.x, lim.y);
	}

	PyObject* mvPlotAxis::reset_axis_ticks(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;

		if (!(mvApp::GetApp()->getParsers())["reset_axis_ticks"].parse(args, kwargs, __FUNCTION__, &plot))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			mvThrowPythonError(1000, message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlotAxis)
		{
			std::string message = plot;
			mvThrowPythonError(1000, message + " is not a plot.");
			return GetPyNone();
		}

		mvPlotAxis* graph = static_cast<mvPlotAxis*>(aplot);

		graph->resetYTicks();

		return GetPyNone();
	}

}