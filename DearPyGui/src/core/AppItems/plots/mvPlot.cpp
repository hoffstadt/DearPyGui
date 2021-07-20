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
		m_label = "Plot###" + std::to_string(m_uuid);
		m_width = -1;
		m_height = -1;
	}

	void mvPlot::onChildAdd(mvRef<mvAppItem> item)
	{
		if (item->getType() == mvAppItemType::mvPlotLegend)
			m_flags &= ~ImPlotFlags_NoLegend;

		if (item->getType() == mvAppItemType::mvPlotAxis)
		{
			updateFlags();
			updateAxesNames();
		}
	}

	void mvPlot::onChildRemoved(mvRef<mvAppItem> item)
	{

		if (item->getType() == mvAppItemType::mvPlotLegend)
			m_flags |= ImPlotFlags_NoLegend;

		if (item->getType() == mvAppItemType::mvPlotAxis)
			updateFlags();
	}

	void mvPlot::updateFlags()
	{
		for (size_t i = 0; i < m_children[1].size(); i++)
		{
			auto child = static_cast<mvPlotAxis*>(m_children[1][i].get());
			switch (i)
			{
			case(0):
				m_xflags = child->getFlags();
				break;

			case(1):
				m_yflags = child->getFlags();
				break;

			case(2):
				m_y1flags = child->getFlags();
				if (child->isShown())
					addFlag(ImPlotFlags_YAxis2);
				else
					removeFlag(ImPlotFlags_YAxis2);
				break;

			case(3):
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
		m_xaxisName = "";
		m_y1axisName = "";
		m_y2axisName = "";
		m_y3axisName = "";

		for (size_t i = 0; i < m_children[1].size(); i++)
		{
			auto axis = m_children[1][i].get();
			switch (i)
			{
			case(0):
				m_xaxisName = axis->m_specificedlabel;
				break;

			case(1):
				m_y1axisName = axis->m_specificedlabel;
				break;

			case(2):
				m_y2axisName = axis->m_specificedlabel;
				break;

			case(3):
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
		m_dirty = true;			{
			m_colormap = colormap;
			m_dirty = true;
		}
	}

	void mvPlot::draw(ImDrawList* drawlist, float x, float y)
	{

		//ImGui::PushID(m_colormap);

		// custom input mapping
		ImPlot::GetInputMap().PanButton = m_pan_button;
		ImPlot::GetInputMap().FitButton = m_fit_button;
		ImPlot::GetInputMap().ContextMenuButton = m_context_menu_button;
		ImPlot::GetInputMap().BoxSelectButton = m_box_select_button;
		ImPlot::GetInputMap().BoxSelectCancelButton = m_box_select_cancel_button;
		ImPlot::GetInputMap().QueryButton = m_query_button;
		ImPlot::GetInputMap().QueryToggleMod = m_query_toggle_mod;
		ImPlot::GetInputMap().HorizontalMod = m_horizontal_mod;
		ImPlot::GetInputMap().VerticalMod = m_vertical_mod;
		if(m_pan_mod != -1) ImPlot::GetInputMap().PanMod = m_pan_mod;
		if (m_box_select_mod != -1) ImPlot::GetInputMap().BoxSelectMod = m_box_select_mod;
		if (m_query_mod != -1) ImPlot::GetInputMap().QueryMod = m_query_mod;

		// gives axes change to make changes to ticks, limits, etc.
		for (auto& item : m_children[1])
		{
			// skip item if it's not shown
			if (!item->m_show)
				continue;
			item->customAction();
		}

		if (m_fitDirty)
		{
			ImPlot::FitNextPlotAxes(m_axisfitDirty[0], m_axisfitDirty[1], m_axisfitDirty[2], m_axisfitDirty[3]);
			m_fitDirty = false;
			m_axisfitDirty[0] = false;
			m_axisfitDirty[1] = false;
			m_axisfitDirty[2] = false;
			m_axisfitDirty[3] = false;
		}

		if (ImPlot::BeginPlot(m_label.c_str(), 
			m_xaxisName.empty() ? nullptr : m_xaxisName.c_str(), 
			m_y1axisName.empty() ? nullptr : m_y1axisName.c_str(),
			ImVec2((float)m_width, (float)m_height), 
			m_flags, m_xflags, m_yflags, m_y1flags, m_y2flags, 
			m_y2axisName.empty() ? nullptr : m_y2axisName.c_str(), 
			m_y3axisName.empty() ? nullptr : m_y3axisName.c_str()))
		{
			ImPlot::PushPlotClipRect();
			ImPlot::PushColormap(m_colormap);

			// legend, drag point and lines
			for (auto& item : m_children[0])
			{
				// skip item if it's not shown
				if (!item->m_show)
					continue;
				item->draw(drawlist, ImPlot::GetPlotPos().x, ImPlot::GetPlotPos().y);

				item->postDraw();
			}

			// axes
			for (auto& item : m_children[1])
			{
				// skip item if it's not shown
				if (!item->m_show)
					continue;
				item->draw(drawlist, ImPlot::GetPlotPos().x, ImPlot::GetPlotPos().y);

				item->postDraw();
			}

			// drawings
			for (auto& item : m_children[2])
			{
				// skip item if it's not shown
				if (!item->m_show)
					continue;
				
				//item->draw(ImPlot::GetPlotDrawList(), ImPlot::GetPlotPos().x, ImPlot::GetPlotPos().y);
				item->draw(ImPlot::GetPlotDrawList(), 0.0f, 0.0f);
				
				item->getState().update();
			}
			

			ImPlot::PopColormap();

			m_queried = ImPlot::IsPlotQueried();

			if (m_queried)
			{
				ImPlotLimits area = ImPlot::GetPlotQuery();
				m_queryArea[0] = area.X.Min;
				m_queryArea[1] = area.X.Max;
				m_queryArea[2] = area.Y.Min;
				m_queryArea[3] = area.Y.Max;
			}

			if (m_callback != nullptr && m_queried)
			{
				mvApp::GetApp()->getCallbackRegistry().submitCallback([=]() {
					PyObject* area = PyTuple_New(4);
					PyTuple_SetItem(area, 0, PyFloat_FromDouble(m_queryArea[0]));
					PyTuple_SetItem(area, 1, PyFloat_FromDouble(m_queryArea[1]));
					PyTuple_SetItem(area, 2, PyFloat_FromDouble(m_queryArea[2]));
					PyTuple_SetItem(area, 3, PyFloat_FromDouble(m_queryArea[3]));
					mvApp::GetApp()->getCallbackRegistry().addCallback(m_callback, m_uuid, area, m_user_data);
					});
			}

			if (ImPlot::IsPlotHovered())
				mvInput::setPlotMousePosition((float)ImPlot::GetPlotMousePos().x, (float)ImPlot::GetPlotMousePos().y);

			// todo: resolve clipping
			ImPlot::PopPlotClipRect();

			if (m_dropCallback)
			{
				ScopedID id(m_uuid);
				if (ImPlot::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(m_payloadType.c_str()))
					{
						auto payloadActual = static_cast<const mvDragPayload*>(payload->Data);
						mvApp::GetApp()->getCallbackRegistry().addCallback(getDropCallback(), m_uuid, payloadActual->getDragData(), nullptr);
					}

					ImPlot::EndDragDropTarget();
				}
			}

			// update state
			auto context = ImPlot::GetCurrentContext();
			m_flags = context->CurrentPlot->Flags;

			registerWindowFocusing();

			ImPlot::EndPlot();
			
		}

		ImPlot::GetInputMap() = m_originalMap;
	}

	bool mvPlot::isPlotQueried() const
	{
		return m_queried;
	}

	double* mvPlot::getPlotQueryArea()
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

		// custom input mapping
		if (PyObject* item = PyDict_GetItemString(dict, "pan_button")) m_pan_button = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "pad_mod")) m_pan_mod = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "fit_button")) m_fit_button = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "context_menu_button")) m_context_menu_button = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "box_select_button")) m_box_select_button = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "box_select_mod")) m_box_select_mod = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "box_select_cancel_button")) m_box_select_cancel_button = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "query_button")) m_query_button = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "query_mod")) m_query_mod = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "query_toggle_mod")) m_query_toggle_mod = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "horizontal_mod")) m_horizontal_mod = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "vertical_mod")) m_vertical_mod = ToInt(item);

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

	}

	void mvPlot::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
	
		PyDict_SetItemString(dict, "x_axis_name", ToPyString(m_xaxisName));

		PyDict_SetItemString(dict, "pan_button", ToPyInt(m_pan_button));
		PyDict_SetItemString(dict, "pan_mod", ToPyInt(m_pan_mod));
		PyDict_SetItemString(dict, "fit_button", ToPyInt(m_fit_button));
		PyDict_SetItemString(dict, "context_menu_button", ToPyInt(m_context_menu_button));
		PyDict_SetItemString(dict, "box_select_button", ToPyInt(m_box_select_button));
		PyDict_SetItemString(dict, "box_select_mod", ToPyInt(m_box_select_mod));
		PyDict_SetItemString(dict, "box_select_cancel_button", ToPyInt(m_box_select_cancel_button));
		PyDict_SetItemString(dict, "query_button", ToPyInt(m_query_button));
		PyDict_SetItemString(dict, "query_mod", ToPyInt(m_query_mod));
		PyDict_SetItemString(dict, "query_toggle_mod", ToPyInt(m_query_toggle_mod));
		PyDict_SetItemString(dict, "horizontal_mod", ToPyInt(m_horizontal_mod));
		PyDict_SetItemString(dict, "vertical_mod", ToPyInt(m_vertical_mod));

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
	}

	void mvPlot::postDraw()
	{

		m_state.update();

		if (m_font)
		{
			ImGui::PopFont();
		}

		if (m_theme)
		{
			static_cast<mvTheme*>(m_theme.get())->customAction();
		}

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
