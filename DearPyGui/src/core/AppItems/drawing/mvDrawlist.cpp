#include "mvDrawlist.h"
#include "mvApp.h"
#include "mvInput.h"
#include "mvItemRegistry.h"
#include "mvViewport.h"
#include "mvAppItemCommons.h"
#include "mvLog.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvDrawlist::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "A container widget that is used to present draw items or layers. Layers and draw items should be added to this widget as children.", { "Drawlist", "Containers", "Widgets"}, true);
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_WIDTH |
			MV_PARSER_ARG_HEIGHT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_CALLBACK |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_FILTER |
			MV_PARSER_ARG_DROP_CALLBACK |
			MV_PARSER_ARG_DRAG_CALLBACK |
			MV_PARSER_ARG_PAYLOAD_TYPE |
			MV_PARSER_ARG_SEARCH_DELAY |
			MV_PARSER_ARG_TRACKED |
			MV_PARSER_ARG_POS)
		);
		parser.finalize();
		parsers->insert({ s_command, parser });
	}

	mvDrawlist::mvDrawlist(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	bool mvDrawlist::canChildBeAdded(mvAppItemType type)
	{
		if (type == mvAppItemType::mvDrawLine) return true;
		if (type == mvAppItemType::mvDrawArrow) return true;
		if (type == mvAppItemType::mvDrawTriangle) return true;
		if (type == mvAppItemType::mvDrawCircle) return true;
		if (type == mvAppItemType::mvDrawBezierCubic) return true;
		if (type == mvAppItemType::mvDrawBezierQuadratic) return true;
		if (type == mvAppItemType::mvDrawQuad) return true;
		if (type == mvAppItemType::mvDrawRect) return true;
		if (type == mvAppItemType::mvDrawText) return true;
		if (type == mvAppItemType::mvDrawPolygon) return true;
		if (type == mvAppItemType::mvDrawPolyline) return true;
		if (type == mvAppItemType::mvDrawImage) return true;
		if (type == mvAppItemType::mvDrawLayer) return true;
		if (type == mvAppItemType::mvDrawEllipse) return true;

		mvThrowPythonError(mvErrorCode::mvIncompatibleChild, s_command,
			"Incompatible child. Acceptable children include: mvDraw*", this);

		MV_ITEM_REGISTRY_ERROR("Drawing children must be draw commands only.");
		assert(false);

		return false;
	}

	void mvDrawlist::draw(ImDrawList* drawlist, float x, float y)
	{

		_startx = (float)ImGui::GetCursorScreenPos().x;
		_starty = (float)ImGui::GetCursorScreenPos().y;

		ImDrawList* internal_drawlist = ImGui::GetWindowDrawList();

		ImGui::PushClipRect({ _startx, _starty }, { _startx + (float)_width, _starty + (float)_height }, true);

		for (auto& item : _children[2])
		{
			// skip item if it's not shown
			if (!item->isShown())
				continue;

			item->draw(internal_drawlist, _startx, _starty);

			item->getState().update();
		}

		ImGui::PopClipRect();

		if (ImGui::InvisibleButton(_label.c_str(), ImVec2((float)_width, (float)_height), ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight | ImGuiButtonFlags_MouseButtonMiddle))
			mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), _uuid, nullptr, _user_data);


		if (ImGui::IsItemHovered())
		{
			ImVec2 mousepos = ImGui::GetMousePos();
			mvInput::setDrawingMousePosition((float)mousepos.x- _startx, (float)mousepos.y - _starty);
		}
	}

}
