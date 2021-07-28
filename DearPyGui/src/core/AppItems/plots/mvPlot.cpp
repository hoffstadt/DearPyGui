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
#include "mvPythonExceptions.h"
#include "mvPlotAxis.h"
#include "themes/mvTheme.h"
#include "containers/mvDragPayload.h"

namespace Marvel {

	void mvPlot::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		{
			mvPythonParser parser(mvPyDataType::UUID, "Adds a plot which is used to hold series, and can be drawn to with draw commands.", { "Plotting", "Containers", "Widgets" }, true);
			mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
				MV_PARSER_ARG_ID |
				MV_PARSER_ARG_WIDTH |
				MV_PARSER_ARG_HEIGHT |
				MV_PARSER_ARG_INDENT |
				MV_PARSER_ARG_PARENT |
				MV_PARSER_ARG_BEFORE |
				MV_PARSER_ARG_SHOW |
				MV_PARSER_ARG_CALLBACK |
				MV_PARSER_ARG_DROP_CALLBACK |
				MV_PARSER_ARG_DRAG_CALLBACK |
				MV_PARSER_ARG_PAYLOAD_TYPE |
				MV_PARSER_ARG_SEARCH_DELAY |
				MV_PARSER_ARG_FILTER |
				MV_PARSER_ARG_TRACKED |
				MV_PARSER_ARG_POS)
			);

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

			// key modifiers
			parser.addArg<mvPyDataType::Integer>("pan_button", mvArgType::KEYWORD_ARG, "internal_dpg.mvMouseButton_Left", "enables panning when held");
			parser.addArg<mvPyDataType::Integer>("pan_mod", mvArgType::KEYWORD_ARG, "-1", "optional modifier that must be held for panning");
			parser.addArg<mvPyDataType::Integer>("fit_button", mvArgType::KEYWORD_ARG, "internal_dpg.mvMouseButton_Left", "fits visible data when double clicked");
			parser.addArg<mvPyDataType::Integer>("context_menu_button", mvArgType::KEYWORD_ARG, "internal_dpg.mvMouseButton_Right", "opens plot context menu (if enabled) when clicked");
			parser.addArg<mvPyDataType::Integer>("box_select_button", mvArgType::KEYWORD_ARG, "internal_dpg.mvMouseButton_Right", "begins box selection when pressed and confirms selection when released");
			parser.addArg<mvPyDataType::Integer>("box_select_mod", mvArgType::KEYWORD_ARG, "-1", "begins box selection when pressed and confirms selection when released");
			parser.addArg<mvPyDataType::Integer>("box_select_cancel_button", mvArgType::KEYWORD_ARG, "internal_dpg.mvMouseButton_Left", "cancels active box selection when pressed");
			parser.addArg<mvPyDataType::Integer>("query_button", mvArgType::KEYWORD_ARG, "internal_dpg.mvMouseButton_Middle", "begins query selection when pressed and end query selection when released");
			parser.addArg<mvPyDataType::Integer>("query_mod", mvArgType::KEYWORD_ARG, "-1", "optional modifier that must be held for query selection");
			parser.addArg<mvPyDataType::Integer>("query_toggle_mod", mvArgType::KEYWORD_ARG, "internal_dpg.mvKey_Control", "when held, active box selections turn into queries");
			parser.addArg<mvPyDataType::Integer>("horizontal_mod", mvArgType::KEYWORD_ARG, "internal_dpg.mvKey_Alt", "expands active box selection/query horizontally to plot edge when held");
			parser.addArg<mvPyDataType::Integer>("vertical_mod", mvArgType::KEYWORD_ARG, "internal_dpg.mvKey_Shift", "expands active box selection/query vertically to plot edge when held");

			parser.finalize();
			parsers->insert({ s_command, parser });
		}

		{
			mvPythonParser parser(mvPyDataType::Bool, "Returns true if the plot is currently being queried. (Requires plot 'query' kwarg to be enabled)", { "Plotting", "Widgets" });
			parser.addArg<mvPyDataType::UUID>("plot");
			parser.finalize();
			parsers->insert({ "is_plot_queried", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::FloatList, "Returns the last/current query area of the plot. (Requires plot 'query' kwarg to be enabled)", { "Plotting", "Widgets" });
			parser.addArg<mvPyDataType::UUID>("plot");
			parser.finalize();
			parsers->insert({ "get_plot_query_area", parser });
		}

	}

	mvPlot::mvPlot(mvUUID uuid)
		: mvAppItem(uuid)
	{
		_label = "Plot###" + std::to_string(_uuid);
		_width = -1;
		_height = -1;
	}

	void mvPlot::onChildAdd(mvRef<mvAppItem> item)
	{
		if (item->getType() == mvAppItemType::mvPlotLegend)
			_flags &= ~ImPlotFlags_NoLegend;

		if (item->getType() == mvAppItemType::mvPlotAxis)
		{
			updateFlags();
			updateAxesNames();
		}
	}

	void mvPlot::onChildRemoved(mvRef<mvAppItem> item)
	{

		if (item->getType() == mvAppItemType::mvPlotLegend)
			_flags |= ImPlotFlags_NoLegend;

		if (item->getType() == mvAppItemType::mvPlotAxis)
			updateFlags();
	}

	void mvPlot::updateFlags()
	{
		for (size_t i = 0; i < _children[1].size(); i++)
		{
			auto child = static_cast<mvPlotAxis*>(_children[1][i].get());
			switch (i)
			{
			case(0):
				_xflags = child->getFlags();
				break;

			case(1):
				_yflags = child->getFlags();
				break;

			case(2):
				_y1flags = child->getFlags();
				if (child->isShown())
					addFlag(ImPlotFlags_YAxis2);
				else
					removeFlag(ImPlotFlags_YAxis2);
				break;

			case(3):
				_y2flags = child->getFlags();
				if (child->isShown())
					addFlag(ImPlotFlags_YAxis3);
				else
					removeFlag(ImPlotFlags_YAxis3);
				break;

			default:
				_yflags = child->getFlags();
				break;
			}
		}

	}

	void mvPlot::updateAxesNames()
	{
		_xaxisName = "";
		_y1axisName = "";
		_y2axisName = "";
		_y3axisName = "";

		for (size_t i = 0; i < _children[1].size(); i++)
		{
			auto axis = _children[1][i].get();
			switch (i)
			{
			case(0):
				_xaxisName = axis->getSpecifiedLabel();
				break;

			case(1):
				_y1axisName = axis->getSpecifiedLabel();
				break;

			case(2):
				_y2axisName = axis->getSpecifiedLabel();
				break;

			case(3):
				_y3axisName = axis->getSpecifiedLabel();
				break;

			default:
				_y1axisName = axis->getSpecifiedLabel();
				break;
			}
		}

	}

	bool mvPlot::canChildBeAdded(mvAppItemType type)
	{
		if (type == mvAppItemType::mvPlotLegend) return true;
		if (type == mvAppItemType::mvPlotAxis) return true;

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
		if (type == mvAppItemType::mvDrawLayer) return true;

		if (type == mvAppItemType::mvActivatedHandler) return true;
		if (type == mvAppItemType::mvActiveHandler) return true;
		if (type == mvAppItemType::mvClickedHandler) return true;
		if (type == mvAppItemType::mvDeactivatedAfterEditHandler) return true;
		if (type == mvAppItemType::mvDeactivatedHandler) return true;
		if (type == mvAppItemType::mvEditedHandler) return true;
		if (type == mvAppItemType::mvFocusHandler) return true;
		if (type == mvAppItemType::mvHoverHandler) return true;
		if (type == mvAppItemType::mvResizeHandler) return true;
		if (type == mvAppItemType::mvToggledOpenHandler) return true;
		if (type == mvAppItemType::mvVisibleHandler) return true;

		mvThrowPythonError(mvErrorCode::mvIncompatibleChild, s_command,
			"Incompatible child. Acceptable children include: mvDraw*, mvDragPoint, mvDragLine, mvAnnotation, mvPlotLegend, mvPlotAxis", this);

		MV_ITEM_REGISTRY_ERROR("Plot children must be compatible.");
		assert(false);

		return false;
	}

	void mvPlot::SetColorMap(ImPlotColormap colormap)
	{
		if (colormap < ImPlot::GetColormapCount())
		_dirty = true;			{
			_colormap = colormap;
			_dirty = true;
		}
	}

	void mvPlot::draw(ImDrawList* drawlist, float x, float y)
	{

		//ImGui::PushID(_colormap);

		// custom input mapping
		ImPlot::GetInputMap().PanButton = _pan_button;
		ImPlot::GetInputMap().FitButton = _fit_button;
		ImPlot::GetInputMap().ContextMenuButton = _context_menu_button;
		ImPlot::GetInputMap().BoxSelectButton = _box_select_button;
		ImPlot::GetInputMap().BoxSelectCancelButton = _box_select_cancel_button;
		ImPlot::GetInputMap().QueryButton = _query_button;
		ImPlot::GetInputMap().QueryToggleMod = _query_toggle_mod;
		ImPlot::GetInputMap().HorizontalMod = _horizontal_mod;
		ImPlot::GetInputMap().VerticalMod = _vertical_mod;
		if(_pan_mod != -1) ImPlot::GetInputMap().PanMod = _pan_mod;
		if (_box_select_mod != -1) ImPlot::GetInputMap().BoxSelectMod = _box_select_mod;
		if (_query_mod != -1) ImPlot::GetInputMap().QueryMod = _query_mod;

		// gives axes change to make changes to ticks, limits, etc.
		for (auto& item : _children[1])
		{
			// skip item if it's not shown
			if (!item->isShown())
				continue;
			item->customAction();
		}

		if (_fitDirty)
		{
			ImPlot::FitNextPlotAxes(_axisfitDirty[0], _axisfitDirty[1], _axisfitDirty[2], _axisfitDirty[3]);
			_fitDirty = false;
			_axisfitDirty[0] = false;
			_axisfitDirty[1] = false;
			_axisfitDirty[2] = false;
			_axisfitDirty[3] = false;
		}

		if (ImPlot::BeginPlot(_label.c_str(), 
			_xaxisName.empty() ? nullptr : _xaxisName.c_str(), 
			_y1axisName.empty() ? nullptr : _y1axisName.c_str(),
			ImVec2((float)_width, (float)_height), 
			_flags, _xflags, _yflags, _y1flags, _y2flags, 
			_y2axisName.empty() ? nullptr : _y2axisName.c_str(), 
			_y3axisName.empty() ? nullptr : _y3axisName.c_str()))
		{
			
			ImPlot::PushColormap(_colormap);

			// legend, drag point and lines
			for (auto& item : _children[0])
			{
				// skip item if it's not shown
				if (!item->isShown())
					continue;
				item->draw(drawlist, ImPlot::GetPlotPos().x, ImPlot::GetPlotPos().y);

				item->postDraw();
			}

			// axes
			for (auto& item : _children[1])
			{
				// skip item if it's not shown
				if (!item->isShown())
					continue;
				item->draw(drawlist, ImPlot::GetPlotPos().x, ImPlot::GetPlotPos().y);

				item->postDraw();
			}

			ImPlot::PushPlotClipRect();
			// drawings
			for (auto& item : _children[2])
			{
				// skip item if it's not shown
				if (!item->isShown())
					continue;
				
				//item->draw(ImPlot::GetPlotDrawList(), ImPlot::GetPlotPos().x, ImPlot::GetPlotPos().y);
				item->draw(ImPlot::GetPlotDrawList(), 0.0f, 0.0f);
				
				item->getState().update();
			}

			ImPlot::PopPlotClipRect();
			

			ImPlot::PopColormap();

			_queried = ImPlot::IsPlotQueried();

			if (_queried)
			{
				ImPlotLimits area = ImPlot::GetPlotQuery();
				_queryArea[0] = area.X.Min;
				_queryArea[1] = area.X.Max;
				_queryArea[2] = area.Y.Min;
				_queryArea[3] = area.Y.Max;
			}

			if (_callback != nullptr && _queried)
			{
				mvApp::GetApp()->getCallbackRegistry().submitCallback([=]() {
					PyObject* area = PyTuple_New(4);
					PyTuple_SetItem(area, 0, PyFloat_FromDouble(_queryArea[0]));
					PyTuple_SetItem(area, 1, PyFloat_FromDouble(_queryArea[1]));
					PyTuple_SetItem(area, 2, PyFloat_FromDouble(_queryArea[2]));
					PyTuple_SetItem(area, 3, PyFloat_FromDouble(_queryArea[3]));
					mvApp::GetApp()->getCallbackRegistry().addCallback(_callback, _uuid, area, _user_data);
					});
			}

			if (ImPlot::IsPlotHovered())
				mvInput::setPlotMousePosition((float)ImPlot::GetPlotMousePos().x, (float)ImPlot::GetPlotMousePos().y);

			// todo: resolve clipping
			

			if (_dropCallback)
			{
				ScopedID id(_uuid);
				if (ImPlot::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(_payloadType.c_str()))
					{
						auto payloadActual = static_cast<const mvDragPayload*>(payload->Data);
						mvApp::GetApp()->getCallbackRegistry().addCallback(getDropCallback(), _uuid, payloadActual->getDragData(), nullptr);
					}

					ImPlot::EndDragDropTarget();
				}
			}

			// update state
			auto context = ImPlot::GetCurrentContext();
			_flags = context->CurrentPlot->Flags;

			registerWindowFocusing();

			ImPlot::EndPlot();
			
		}

		ImPlot::GetInputMap() = _originalMap;
	}

	bool mvPlot::isPlotQueried() const
	{
		return _queried;
	}

	double* mvPlot::getPlotQueryArea()
	{
		return _queryArea;
	}

	void mvPlot::addFlag(ImPlotFlags flag)
	{
		_flags |= flag;
	}

	void mvPlot::removeFlag(ImPlotFlags flag)
	{
		_flags &= ~flag;
	}

	void mvPlot::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "x_axis_name"))_xaxisName = ToString(item);

		// custom input mapping
		if (PyObject* item = PyDict_GetItemString(dict, "pan_button")) _pan_button = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "pad_mod")) _pan_mod = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "fit_button")) _fit_button = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "context_menu_button")) _context_menu_button = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "box_select_button")) _box_select_button = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "box_select_mod")) _box_select_mod = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "box_select_cancel_button")) _box_select_cancel_button = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "query_button")) _query_button = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "query_mod")) _query_mod = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "query_toggle_mod")) _query_toggle_mod = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "horizontal_mod")) _horizontal_mod = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "vertical_mod")) _vertical_mod = ToInt(item);

		// helper for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		// plot flags
		flagop("no_title",             ImPlotFlags_NoTitle,          _flags);
		flagop("no_menus",             ImPlotFlags_NoMenus,          _flags);
		flagop("no_box_select",        ImPlotFlags_NoBoxSelect,      _flags);
		flagop("no_mouse_pos",         ImPlotFlags_NoMousePos,       _flags);
		flagop("no_highlight",         ImPlotFlags_NoHighlight,      _flags);
		flagop("no_child",             ImPlotFlags_NoChild,          _flags);
		flagop("query",                ImPlotFlags_Query,            _flags);
		flagop("crosshairs",           ImPlotFlags_Crosshairs,       _flags);
		flagop("anti_aliased",         ImPlotFlags_AntiAliased,      _flags);
		flagop("equal_aspects",        ImPlotFlags_Equal,            _flags);

	}

	void mvPlot::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
	
		PyDict_SetItemString(dict, "x_axis_name", ToPyString(_xaxisName));

		PyDict_SetItemString(dict, "pan_button", ToPyInt(_pan_button));
		PyDict_SetItemString(dict, "pan_mod", ToPyInt(_pan_mod));
		PyDict_SetItemString(dict, "fit_button", ToPyInt(_fit_button));
		PyDict_SetItemString(dict, "context_menu_button", ToPyInt(_context_menu_button));
		PyDict_SetItemString(dict, "box_select_button", ToPyInt(_box_select_button));
		PyDict_SetItemString(dict, "box_select_mod", ToPyInt(_box_select_mod));
		PyDict_SetItemString(dict, "box_select_cancel_button", ToPyInt(_box_select_cancel_button));
		PyDict_SetItemString(dict, "query_button", ToPyInt(_query_button));
		PyDict_SetItemString(dict, "query_mod", ToPyInt(_query_mod));
		PyDict_SetItemString(dict, "query_toggle_mod", ToPyInt(_query_toggle_mod));
		PyDict_SetItemString(dict, "horizontal_mod", ToPyInt(_horizontal_mod));
		PyDict_SetItemString(dict, "vertical_mod", ToPyInt(_vertical_mod));

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
		};

		// plot flags
		checkbitset("no_title",             ImPlotFlags_NoTitle,          _flags);
		checkbitset("no_menus",             ImPlotFlags_NoMenus,          _flags);
		checkbitset("no_box_select",        ImPlotFlags_NoBoxSelect,      _flags);
		checkbitset("no_mouse_pos",         ImPlotFlags_NoMousePos,       _flags);
		checkbitset("no_highlight",         ImPlotFlags_NoHighlight,      _flags);
		checkbitset("no_child",             ImPlotFlags_NoChild,          _flags);
		checkbitset("query",                ImPlotFlags_Query,            _flags);
		checkbitset("crosshairs",           ImPlotFlags_Crosshairs,       _flags);
		checkbitset("anti_aliased",         ImPlotFlags_AntiAliased,      _flags);
		checkbitset("equal_aspects",        ImPlotFlags_Equal,            _flags);
	}

	void mvPlot::postDraw()
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

		// event handlers
		for (auto& item : _children[3])
		{
			if (!item->preDraw())
				continue;

			item->draw(nullptr, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
		}

		// drag drop
		for (auto& item : _children[4])
		{
			if (!item->preDraw())
				continue;

			item->draw(nullptr, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
		}
	}

	PyObject* mvPlot::is_plot_queried(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvUUID plot;

		if (!(mvApp::GetApp()->getParsers())["is_plot_queried"].parse(args, kwargs, __FUNCTION__, &plot))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "is_plot_queried",
				"Item not found: " + std::to_string(plot), nullptr);
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "is_plot_queried",
				"Incompatible type. Expected types include: mvPlot", aplot);
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot);

		return ToPyBool(graph->isPlotQueried());
	}

	PyObject* mvPlot::get_plot_query_area(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvUUID plot;

		if (!(mvApp::GetApp()->getParsers())["get_plot_query_area"].parse(args, kwargs, __FUNCTION__, &plot))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "get_plot_query_area",
				"Item not found: " + std::to_string(plot), nullptr);
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "is_plot_queried",
				"Incompatible type. Expected types include: mvPlot", aplot);
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot);

		double* result = graph->getPlotQueryArea();
		return Py_BuildValue("(dddd)", result[0], result[1], result[2], result[3]);
	}

}
