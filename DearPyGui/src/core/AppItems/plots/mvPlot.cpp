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
#include "mvPlotAxis.h"
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
				MV_PARSER_ARG_CALLBACK |
				MV_PARSER_ARG_USER_DATA |
				MV_PARSER_ARG_DROP_CALLBACK |
				MV_PARSER_ARG_DRAG_CALLBACK |
				MV_PARSER_ARG_PAYLOAD_TYPE |
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
		for (int i = 0; i < m_children[1].size(); i++)
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

		for (int i = 0; i < m_children[1].size(); i++)
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

	void mvPlot::draw(ImDrawList* drawlist, float x, float y)
	{

		ImGui::PushID(m_colormap);

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
					mvApp::GetApp()->getCallbackRegistry().addCallback(m_callback, m_name, area, m_user_data);
					});
			}

			if (ImPlot::IsPlotHovered())
				mvInput::setPlotMousePosition((float)ImPlot::GetPlotMousePos().x, (float)ImPlot::GetPlotMousePos().y);

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

		mvPlot* graph = static_cast<mvPlot*>(aplot);

		return ToPyBool(graph->isPlotQueried());
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

		mvPlot* graph = static_cast<mvPlot*>(aplot);

		double* result = graph->getPlotQueryArea();
		return Py_BuildValue("(dddd)", result[0], result[1], result[2], result[3]);
	}

}
