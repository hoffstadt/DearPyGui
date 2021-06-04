#include <utility>
#include "mvPlotYAxis.h"
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

	void mvPlotYAxis::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		{
			mvPythonParser parser(mvPyDataType::String, "Undocumented function", { "Widgets" });
			mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
				MV_PARSER_ARG_ID |
				MV_PARSER_ARG_PARENT |
				MV_PARSER_ARG_DROP_CALLBACK |
				MV_PARSER_ARG_DRAG_CALLBACK |
				MV_PARSER_ARG_PAYLOAD_TYPE |
				MV_PARSER_ARG_SHOW)
			);

			parser.addArg<mvPyDataType::Bool>("yaxis_no_gridlines", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("yaxis_no_tick_marks", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("yaxis_no_tick_labels", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("yaxis_log_scale", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("yaxis_invert", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("yaxis_lock_min", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("yaxis_lock_max", mvArgType::KEYWORD_ARG, "False");


			parser.finalize();

			parsers->insert({ s_command, parser });
		}

		{
			mvPythonParser parser(mvPyDataType::FloatList, "Undocumented function", { "Plotting", "Widgets" });
			parser.addArg<mvPyDataType::String>("plot");
			parser.finalize();
			parsers->insert({ "get_plot_ylimits", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Undocumented function", { "Plotting", "Widgets" });
			parser.addArg<mvPyDataType::String>("plot");
			parser.addArg<mvPyDataType::Float>("ymin");
			parser.addArg<mvPyDataType::Float>("ymax");
			parser.finalize();
			parsers->insert({ "set_plot_ylimits", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Undocumented function", { "Plotting", "Widgets" });
			parser.addArg<mvPyDataType::String>("plot");
			parser.finalize();
			parsers->insert({ "reset_yticks", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Undocumented function", { "Plotting", "Widgets" });
			parser.addArg<mvPyDataType::String>("plot");
			parser.addArg<mvPyDataType::Object>("label_pairs");
			parser.finalize();
			parsers->insert({ "set_yticks", parser });
		}
	}

	mvPlotYAxis::mvPlotYAxis(const std::string& name)
		: 
		mvAppItem(name)
	{
	}

	void mvPlotYAxis::customAction()
	{
		if (m_setYLimits || m_dirty)
		{
			switch (m_location)
			{
			case(0):
				ImPlot::SetNextPlotLimitsY(m_ylimits.x, m_ylimits.y, ImGuiCond_Always);
				break;

			case(1):
				ImPlot::SetNextPlotLimitsY(m_ylimits.x, m_ylimits.y, ImGuiCond_Always, ImPlotYAxis_2);
				break;

			case(2):
				ImPlot::SetNextPlotLimitsY(m_ylimits.x, m_ylimits.y, ImGuiCond_Always, ImPlotYAxis_3);
				break;

			default:
				ImPlot::SetNextPlotLimitsY(m_ylimits.x, m_ylimits.y, ImGuiCond_Always);
				break;
			}

			m_dirty = false;
			
		}

		if (!m_ylabels.empty())
		{
			// TODO: Checks
			ImPlot::SetNextPlotTicksY(m_ylabelLocations.data(), (int)m_ylabels.size(), m_yclabels.data());
		}
	}

	void mvPlotYAxis::draw(ImDrawList* drawlist, float x, float y)
	{
		mvImPlotThemeScope scope(this);
		mvFontScope fscope(this);

		// todo: add check
		ImPlot::SetPlotYAxis(m_location);

		for (auto& item : m_children[1])
		{
			// skip item if it's not shown
			if (!item->m_show)
				continue;
			item->draw(drawlist, ImPlot::GetPlotPos().x, ImPlot::GetPlotPos().y);

			item->getState().update();
		}

		m_ylimits_actual.x = (float)ImPlot::GetPlotLimits(m_location).Y.Min;
		m_ylimits_actual.y = (float)ImPlot::GetPlotLimits(m_location).Y.Max;

	}

	bool mvPlotYAxis::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvStagingContainer) return true;
		if (type == mvAppItemType::mvPlot) return true;

		mvThrowPythonError(1000, "Drawing item parent must be a drawing.");
		MV_ITEM_REGISTRY_ERROR("Drawing item parent must be a drawing.");
		assert(false);
		return false;
	}

	void mvPlotYAxis::hide()
	{
		if (auto plot = static_cast<mvPlot*>(m_parentPtr))
			plot->addFlag(ImPlotFlags_NoLegend);
		m_show = false;
	}

	void mvPlotYAxis::show()
	{
		if (auto plot = static_cast<mvPlot*>(m_parentPtr))
			plot->removeFlag(ImPlotFlags_NoLegend);
		m_show = true;
	}

	bool mvPlotYAxis::canChildBeAdded(mvAppItemType type)
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

	void mvPlotYAxis::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		// helper for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		// y axis flags
		flagop("yaxis_no_gridlines", ImPlotAxisFlags_NoGridLines, m_flags);
		flagop("yaxis_no_tick_marks", ImPlotAxisFlags_NoTickMarks, m_flags);
		flagop("yaxis_no_tick_labels", ImPlotAxisFlags_NoTickLabels, m_flags);
		flagop("yaxis_log_scale", ImPlotAxisFlags_LogScale, m_flags);
		flagop("yaxis_invert", ImPlotAxisFlags_Invert, m_flags);
		flagop("yaxis_lock_min", ImPlotAxisFlags_LockMin, m_flags);
		flagop("yaxis_lock_max", ImPlotAxisFlags_LockMax, m_flags);

		
		if (m_parentPtr)
		{
			static_cast<mvPlot*>(m_parentPtr)->updateFlags();
			static_cast<mvPlot*>(m_parentPtr)->updateAxesNames();
		}
	}

	void mvPlotYAxis::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
	}

	void mvPlotYAxis::setYLimits(float y_min, float y_max)
	{
		m_setYLimits = true;
		m_ylimits = ImVec2(y_min, y_max);
	}

	void mvPlotYAxis::onChildAdd(mvRef<mvAppItem> item)
	{
		if (static_cast<mvSeriesBase*>(item.get())->doesSeriesContributeToBounds())
		{
			updateBounds();
			static_cast<mvPlot*>(m_parentPtr)->updateBounds();
		}

		if (item->getType() == mvAppItemType::mvPlotLegend)
			m_flags &= ~ImPlotFlags_NoLegend;
	}

	void mvPlotYAxis::onChildRemoved(mvRef<mvAppItem> item)
	{

	}

	void mvPlotYAxis::updateBounds()
	{
		bool first = true;
		for (auto& series : m_children[1])
		{
			mvSeriesBase* child = static_cast<mvSeriesBase*>(series.get());
			if (child->doesSeriesContributeToBounds())
			{
				const auto& y_maxMin = child->getMaxMin(1);

				if (first && !m_setYLimits)
				{
					m_ylimits.x = y_maxMin.second;
					m_ylimits.y = y_maxMin.first;
					first = false;
				}
				else if (!m_setYLimits)
				{
					if (y_maxMin.second < m_ylimits.x) m_ylimits.x = y_maxMin.second;
					if (y_maxMin.first > m_ylimits.y) m_ylimits.y = y_maxMin.first;
				}
			}
		}

		m_dirty = true;
	}

	void mvPlotYAxis::resetYTicks()
	{
		m_ylabels.clear();
		m_yclabels.clear();
		m_ylabelLocations.clear();
	}

	void mvPlotYAxis::setYTicks(const std::vector<std::string>& labels, const std::vector<double>& locations)
	{
		m_ylabels = labels;
		m_ylabelLocations = locations;

		for (const auto& item : m_ylabels)
			m_yclabels.push_back(item.data());
	}

	void mvPlotYAxis::postDraw()
	{

		m_state.update();

		if (m_dropCallback)
		{
			//ScopedID id;
			if (ImPlot::BeginDragDropTargetY(m_location))
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(m_payloadType.c_str()))
				{
					auto payloadActual = static_cast<const mvDragPayload*>(payload->Data);
					mvApp::GetApp()->getCallbackRegistry().addCallback(getDropCallback(), m_name, payloadActual->getDragData());
				}

				ImPlot::EndDragDropTarget();
			}
		}
	}

	PyObject* mvPlotYAxis::set_yticks(PyObject* self, PyObject* args, PyObject* kwargs)
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
			mvThrowPythonError(1000, message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlotYAxis)
		{
			std::string message = plot;
			mvThrowPythonError(1000, message + " is not a plot.");
			return GetPyNone();
		}

		mvPlotYAxis* graph = static_cast<mvPlotYAxis*>(aplot.get());

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

	PyObject* mvPlotYAxis::set_plot_ylimits(PyObject* self, PyObject* args, PyObject* kwargs)
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
			mvThrowPythonError(1000, message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlotYAxis)
		{
			std::string message = plot;
			mvThrowPythonError(1000, message + " is not a plot.");
			return GetPyNone();
		}

		mvPlotYAxis* graph = static_cast<mvPlotYAxis*>(aplot.get());

		graph->setYLimits(ymin, ymax);

		return GetPyNone();
	}

	PyObject* mvPlotYAxis::get_plot_ylimits(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;

		if (!(mvApp::GetApp()->getParsers())["get_plot_ylimits"].parse(args, kwargs, __FUNCTION__, &plot))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			mvThrowPythonError(1000, message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlotYAxis)
		{
			std::string message = plot;
			mvThrowPythonError(1000, message + " is not a plot.");
			return GetPyNone();
		}

		mvPlotYAxis* graph = static_cast<mvPlotYAxis*>(aplot.get());

		const ImVec2& lim = graph->getYLimits();
		return ToPyPair(lim.x, lim.y);
	}

	PyObject* mvPlotYAxis::reset_yticks(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;

		if (!(mvApp::GetApp()->getParsers())["reset_yticks"].parse(args, kwargs, __FUNCTION__, &plot))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			mvThrowPythonError(1000, message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlotYAxis)
		{
			std::string message = plot;
			mvThrowPythonError(1000, message + " is not a plot.");
			return GetPyNone();
		}

		mvPlotYAxis* graph = static_cast<mvPlotYAxis*>(aplot.get());

		graph->resetYTicks();

		return GetPyNone();
	}

}