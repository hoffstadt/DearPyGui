#include <utility>
#include "mvPlotAxis.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvLog.h"
#include "themes/mvTheme.h"
#include "mvPythonExceptions.h"
#include "mvPlot.h"
#include "containers/mvDragPayload.h"

namespace Marvel {

	void mvPlotAxis::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		{
			mvPythonParser parser(mvPyDataType::UUID, "Adds a plot legend to a plot.", { "Plotting", "Widgets" });
			mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
				MV_PARSER_ARG_ID |
				MV_PARSER_ARG_PARENT |
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
			mvPythonParser parser(mvPyDataType::FloatList, "Gets the specified axis limits.", { "Plotting", "Widgets" });
			parser.addArg<mvPyDataType::UUID>("axis");
			parser.finalize();
			parsers->insert({ "get_axis_limits", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Sets limits on the axis for pan and zoom.", { "Plotting", "Widgets" });
			parser.addArg<mvPyDataType::UUID>("axis");
			parser.addArg<mvPyDataType::Float>("ymin");
			parser.addArg<mvPyDataType::Float>("ymax");
			parser.finalize();
			parsers->insert({ "set_axis_limits", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Removes all limits on specified axis.", { "Plotting", "Widgets" });
			parser.addArg<mvPyDataType::UUID>("axis");
			parser.finalize();
			parsers->insert({ "set_axis_limits_auto", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Sets the axis boundries max and min in the data series currently on the plot.", { "Plotting", "Widgets" });
			parser.addArg<mvPyDataType::UUID>("axis");
			parser.finalize();
			parsers->insert({ "fit_axis_data", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Removes the and manually set axis ticks and applys the default auto axis ticks.", { "Plotting", "Widgets" });
			parser.addArg<mvPyDataType::UUID>("axis");
			parser.finalize();
			parsers->insert({ "reset_axis_ticks", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Replaces axis ticks with 'label_pairs' argument", { "Plotting", "Widgets" });
			parser.addArg<mvPyDataType::UUID>("axis");
			parser.addArg<mvPyDataType::Object>("label_pairs", mvArgType::REQUIRED_ARG, "...", "Tuples of label and value in the form '((label, axis_value), (label, axis_value), ...)'");
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
		if (_setLimits || _dirty)
		{
			switch (_location)
			{
			case(0):
				ImPlot::SetNextPlotLimitsX(_limits.x, _limits.y, ImGuiCond_Always);
				break;

			case(1):
				ImPlot::SetNextPlotLimitsY(_limits.x, _limits.y, ImGuiCond_Always);
				break;

			case(2):
				ImPlot::SetNextPlotLimitsY(_limits.x, _limits.y, ImGuiCond_Always, ImPlotYAxis_2);
				break;

			case(3):
				ImPlot::SetNextPlotLimitsY(_limits.x, _limits.y, ImGuiCond_Always, ImPlotYAxis_3);
				break;

			default:
				ImPlot::SetNextPlotLimitsY(_limits.x, _limits.y, ImGuiCond_Always);
				break;
			}

			_dirty = false;
			
		}

		if (!_labels.empty())
		{
			// TODO: Checks
			if(_location == 0)
				ImPlot::SetNextPlotTicksX(_labelLocations.data(), (int)_labels.size(), _clabels.data());
			else
				ImPlot::SetNextPlotTicksY(_labelLocations.data(), (int)_labels.size(), _clabels.data());
		}
	}

	void mvPlotAxis::draw(ImDrawList* drawlist, float x, float y)
	{

		// todo: add check
		if(_axis != 0)
			ImPlot::SetPlotYAxis(_location - 1);

		for (auto& item : _children[1])
			item->draw(drawlist, ImPlot::GetPlotPos().x, ImPlot::GetPlotPos().y);

		// x axis
		if (_axis == 0)
		{
			_limits_actual.x = (float)ImPlot::GetPlotLimits(_location).X.Min;
			_limits_actual.y = (float)ImPlot::GetPlotLimits(_location).X.Max;
		}

		// y axis
		else
		{
			_limits_actual.x = (float)ImPlot::GetPlotLimits(_location -1).Y.Min;
			_limits_actual.y = (float)ImPlot::GetPlotLimits(_location -1).Y.Max;
		}

		//auto context = ImPlot::GetCurrentContext();
		//_flags = context->CurrentPlot->CurrentYAxis.Flags

	}

	void mvPlotAxis::fitAxisData()
	{
		static_cast<mvPlot*>(_parentPtr)->_fitDirty = true;
		static_cast<mvPlot*>(_parentPtr)->_axisfitDirty[_location] = true;
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
		flagop("no_gridlines", ImPlotAxisFlags_NoGridLines, _flags);
		flagop("no_tick_marks", ImPlotAxisFlags_NoTickMarks, _flags);
		flagop("no_tick_labels", ImPlotAxisFlags_NoTickLabels, _flags);
		flagop("log_scale", ImPlotAxisFlags_LogScale, _flags);
		flagop("invert", ImPlotAxisFlags_Invert, _flags);
		flagop("lock_min", ImPlotAxisFlags_LockMin, _flags);
		flagop("lock_max", ImPlotAxisFlags_LockMax, _flags);
		flagop("time", ImPlotAxisFlags_Time, _flags);

		
		if (_parentPtr)
		{
			static_cast<mvPlot*>(_parentPtr)->updateFlags();
			static_cast<mvPlot*>(_parentPtr)->updateAxesNames();
		}

		if (wasShownLastFrameReset())
		{
			if (auto plot = static_cast<mvPlot*>(_parentPtr))
				plot->removeFlag(ImPlotFlags_NoLegend);
			_show = true;
		}

		if (wasHiddenLastFrameReset())
		{
			if (auto plot = static_cast<mvPlot*>(_parentPtr))
				plot->addFlag(ImPlotFlags_NoLegend);
			_show = false;
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
				_axis = ToInt(item);
				if (_axis > 1)
					_axis = 1;
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
		_setLimits = true;
		_limits = ImVec2(y_min, y_max);
	}

	void mvPlotAxis::setLimitsAuto()
	{
		_setLimits = false;
	}

	void mvPlotAxis::onChildAdd(mvRef<mvAppItem> item)
	{
	}

	void mvPlotAxis::onChildRemoved(mvRef<mvAppItem> item)
	{
	}

	void mvPlotAxis::resetYTicks()
	{
		_labels.clear();
		_clabels.clear();
		_labelLocations.clear();
	}

	void mvPlotAxis::setYTicks(const std::vector<std::string>& labels, const std::vector<double>& locations)
	{
		_labels = labels;
		_labelLocations = locations;

		for (const auto& item : _labels)
			_clabels.push_back(item.data());
	}

	void mvPlotAxis::postDraw()
	{

		_state.update();

		if (_font)
		{
			ImGui::PopFont();
		}

		if (_theme)
		{
			static_cast<mvTheme*>(_theme.get())->customAction();
		}

		if (_dropCallback)
		{
			ScopedID id(_uuid);
			if (_location == 0 && ImPlot::BeginDragDropTargetX())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(_payloadType.c_str()))
				{
					auto payloadActual = static_cast<const mvDragPayload*>(payload->Data);
					mvApp::GetApp()->getCallbackRegistry().addCallback(getDropCallback(), _uuid, payloadActual->getDragData(), nullptr);
				}

				ImPlot::EndDragDropTarget();
			}
			else if (ImPlot::BeginDragDropTargetY(_location-1))
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(_payloadType.c_str()))
				{
					auto payloadActual = static_cast<const mvDragPayload*>(payload->Data);
					mvApp::GetApp()->getCallbackRegistry().addCallback(getDropCallback(), _uuid, payloadActual->getDragData(), nullptr);
				}

				ImPlot::EndDragDropTarget();
			}
		}
	}

	PyObject* mvPlotAxis::set_axis_ticks(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* plotraw;
		PyObject* label_pairs;

		if (!(mvApp::GetApp()->getParsers())["set_axis_ticks"].parse(args, kwargs, __FUNCTION__, &plotraw, &label_pairs))
			return GetPyNone();

		auto mlabel_pairs = ToVectPairStringFloat(label_pairs);

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID plot = mvAppItem::GetIDFromPyObject(plotraw);

		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "set_axis_ticks",
				"Item not found: " + std::to_string(plot), nullptr);
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlotAxis)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "set_axis_ticks",
				"Incompatible type. Expected types include: mvPlotAxis", aplot);
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
		PyObject* axisraw;
		float ymin;
		float ymax;

		if (!(mvApp::GetApp()->getParsers())["set_axis_limits"].parse(args, kwargs, __FUNCTION__, &axisraw, &ymin, &ymax))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID axis = mvAppItem::GetIDFromPyObject(axisraw);

		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(axis);
		if (aplot == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "set_axis_limits",
				"Item not found: " + std::to_string(axis), nullptr);
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlotAxis)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "set_axis_limits",
				"Incompatible type. Expected types include: mvPlotAxis", aplot);
			return GetPyNone();
		}

		mvPlotAxis* graph = static_cast<mvPlotAxis*>(aplot);

		graph->setLimits(ymin, ymax);

		return GetPyNone();
	}

	PyObject* mvPlotAxis::set_axis_limits_auto(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* axisraw;

		if (!(mvApp::GetApp()->getParsers())["set_axis_limits_auto"].parse(args, kwargs, __FUNCTION__, &axisraw))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID axis = mvAppItem::GetIDFromPyObject(axisraw);

		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(axis);
		if (aplot == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "set_axis_limits",
				"Item not found: " + std::to_string(axis), nullptr);
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlotAxis)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "set_axis_limits",
				"Incompatible type. Expected types include: mvPlotAxis", aplot);
			return GetPyNone();
		}

		mvPlotAxis* graph = static_cast<mvPlotAxis*>(aplot);

		graph->setLimitsAuto();

		return GetPyNone();
	}

	PyObject* mvPlotAxis::fit_axis_data(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* axisraw;

		if (!(mvApp::GetApp()->getParsers())["fit_axis_data"].parse(args, kwargs, __FUNCTION__, &axisraw))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID axis = mvAppItem::GetIDFromPyObject(axisraw);

		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(axis);
		if (aplot == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "fit_axis_data",
				"Item not found: " + std::to_string(axis), nullptr);
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlotAxis)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "fit_axis_data",
				"Incompatible type. Expected types include: mvPlotAxis", aplot);
			return GetPyNone();
		}

		mvPlotAxis* graph = static_cast<mvPlotAxis*>(aplot);

		graph->fitAxisData();

		return GetPyNone();
	}

	PyObject* mvPlotAxis::get_axis_limits(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* plotraw;

		if (!(mvApp::GetApp()->getParsers())["get_axis_limits"].parse(args, kwargs, __FUNCTION__, &plotraw))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID plot = mvAppItem::GetIDFromPyObject(plotraw);

		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "get_axis_limits",
				"Item not found: " + std::to_string(plot), nullptr);
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlotAxis)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "get_axis_limits",
				"Incompatible type. Expected types include: mvPlotAxis", aplot);
			return GetPyNone();
		}

		mvPlotAxis* graph = static_cast<mvPlotAxis*>(aplot);

		const ImVec2& lim = graph->getYLimits();
		return ToPyPair(lim.x, lim.y);
	}

	PyObject* mvPlotAxis::reset_axis_ticks(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* plotraw;

		if (!(mvApp::GetApp()->getParsers())["reset_axis_ticks"].parse(args, kwargs, __FUNCTION__, &plotraw))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID plot = mvAppItem::GetIDFromPyObject(plotraw);

		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "reset_axis_ticks",
				"Item not found: " + std::to_string(plot), nullptr);
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlotAxis)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "reset_axis_ticks",
				"Incompatible type. Expected types include: mvPlotAxis", aplot);
			return GetPyNone();
		}

		mvPlotAxis* graph = static_cast<mvPlotAxis*>(aplot);

		graph->resetYTicks();

		return GetPyNone();
	}

}