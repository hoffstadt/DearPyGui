#include <utility>
#include "mvPlotLegend.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvLog.h"
#include "themes/mvTheme.h"
#include "mvPythonExceptions.h"
#include "mvPlot.h"
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
		ImPlot::SetLegendLocation(m_legendLocation, m_horizontal ? ImPlotOrientation_Horizontal : ImPlotOrientation_Vertical, m_outside);
	}

	bool mvPlotLegend::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvStagingContainer) return true;
		if (type == mvAppItemType::mvPlot) return true;

		mvThrowPythonError(mvErrorCode::mvIncompatibleParent, s_command,
			"Incompatible parent. Acceptable parents include: plot, staging container", this);
		MV_ITEM_REGISTRY_ERROR("Drawing item parent must be a drawing.");
		assert(false);
		return false;
	}

	void mvPlotLegend::hide()
	{
		if (auto plot = static_cast<mvPlot*>(m_parentPtr))
			plot->addFlag(ImPlotFlags_NoLegend);
		m_show = false;
	}

	void mvPlotLegend::show() 
	{
		if (auto plot = static_cast<mvPlot*>(m_parentPtr))
			plot->removeFlag(ImPlotFlags_NoLegend);
		m_show = true;
	}

	void mvPlotLegend::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "location")) m_location = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "horizontal")) m_horizontal = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "outside")) m_outside = ToBool(item);
	}

	void mvPlotLegend::postDraw()
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

		if (m_dropCallback)
		{
			if (ImPlot::BeginDragDropTargetLegend())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(m_payloadType.c_str()))
				{
					auto payloadActual = static_cast<const mvDragPayload*>(payload->Data);
					mvApp::GetApp()->getCallbackRegistry().addCallback(getDropCallback(), m_uuid, payloadActual->getDragData(), nullptr);
				}

				ImPlot::EndDragDropTarget();
			}
		}
	}

}