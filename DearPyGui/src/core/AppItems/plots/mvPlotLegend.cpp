#include <utility>
#include "mvPlotLegend.h"
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

	void mvPlotLegend::InsertParser(std::map<std::string, mvPythonParser>* parsers)
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

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvPlotLegend::mvPlotLegend(const std::string& name)
		: 
		mvAppItem(name)
	{
	}

	void mvPlotLegend::draw(ImDrawList* drawlist, float x, float y)
	{
		mvImPlotThemeScope scope(this);
		mvFontScope fscope(this);
	}

	bool mvPlotLegend::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvStagingContainer) return true;
		if (type == mvAppItemType::mvPlot) return true;

		mvThrowPythonError(1000, "Drawing item parent must be a drawing.");
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

	void mvPlotLegend::postDraw()
	{

		m_state.update();

		if (m_dropCallback)
		{
			//ScopedID id;
			if (ImPlot::BeginDragDropTargetLegend())
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

}