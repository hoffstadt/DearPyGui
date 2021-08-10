#include <utility>
#include "mvPlotLegend.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvLog.h"
#include "themes/mvTheme.h"
#include "mvPythonExceptions.h"
#include "mvPlot.h"
#include "mvSubPlots.h"
#include "containers/mvDragPayload.h"

namespace Marvel {

	void mvPlotLegend::InsertParser(std::map<std::string, mvPythonParser>* parsers)
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

		parser.addArg<mvPyDataType::Integer>("location", mvArgType::KEYWORD_ARG, "5", "location, mvPlot_Location_*");
		parser.addArg<mvPyDataType::Bool>("horizontal", mvArgType::KEYWORD_ARG, "False");
		parser.addArg<mvPyDataType::Bool>("outside", mvArgType::KEYWORD_ARG, "False");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvPlotLegend::mvPlotLegend(mvUUID uuid)
		: 
		mvAppItem(uuid)
	{
	}

	void mvPlotLegend::draw(ImDrawList* drawlist, float x, float y)
	{
		ImPlot::SetLegendLocation(_legendLocation, _horizontal ? ImPlotOrientation_Horizontal : ImPlotOrientation_Vertical, _outside);
	}

	void mvPlotLegend::hide()
	{
		if (auto plot = static_cast<mvPlot*>(_parentPtr))
			plot->addFlag(ImPlotFlags_NoLegend);
		else if (auto plot = static_cast<mvSubPlots*>(_parentPtr))
			plot->addFlag(ImPlotSubplotFlags_NoLegend);
		_show = false;
	}

	void mvPlotLegend::show() 
	{
		if (auto plot = static_cast<mvPlot*>(_parentPtr))
			plot->removeFlag(ImPlotFlags_NoLegend);
		else if (auto plot = static_cast<mvSubPlots*>(_parentPtr))
			plot->removeFlag(ImPlotSubplotFlags_NoLegend);
		_show = true;
	}

	void mvPlotLegend::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "location")) _location = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "horizontal")) _horizontal = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "outside")) _outside = ToBool(item);
	}

	void mvPlotLegend::postDraw()
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
			if (ImPlot::BeginDragDropTargetLegend())
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

}