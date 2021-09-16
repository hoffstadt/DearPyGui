#include "mvDrawlist.h"
#include "mvContext.h"
#include "mvItemRegistry.h"
#include "mvViewport.h"
#include "mvAppItemCommons.h"
#include "mvLog.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvDrawlist::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		std::vector<mvPythonDataElement> args;

		AddCommonArgs(args,(CommonParserArgs)(
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

		mvPythonParserSetup setup;
		setup.about = "Adds a drawing canvas.";
		setup.category = { "Drawlist", "Widgets" };
		setup.returnType = mvPyDataType::UUID;
		setup.createContextManager = true;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers->insert({ s_command, parser });
	}

	mvDrawlist::mvDrawlist(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	void mvDrawlist::draw(ImDrawList* drawlist, float x, float y)
	{

		if (!_show)
			return;

		_startx = (float)ImGui::GetCursorScreenPos().x;
		_starty = (float)ImGui::GetCursorScreenPos().y;

		ImDrawList* internal_drawlist = ImGui::GetWindowDrawList();

		ImGui::PushClipRect({ _startx, _starty }, { _startx + (float)_width, _starty + (float)_height }, true);

		for (auto& item : _children[2])
		{
			// skip item if it's not shown
			if (!item->_show)
				continue;

			item->draw(internal_drawlist, _startx, _starty);

			UpdateAppItemState(item->_state);
		}

		ImGui::PopClipRect();

		if (ImGui::InvisibleButton(_internalLabel.c_str(), ImVec2((float)_width, (float)_height), ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight | ImGuiButtonFlags_MouseButtonMiddle))
		{
			if(_alias.empty())
				GContext->callbackRegistry->addCallback(getCallback(false), _uuid, nullptr, _user_data);
			else
				GContext->callbackRegistry->addCallback(getCallback(false), _alias, nullptr, _user_data);
		}

		UpdateAppItemState(_state);

		if (_handlerRegistry)
			_handlerRegistry->customAction(&_state);

		if (ImGui::IsItemHovered())
		{
			ImVec2 mousepos = ImGui::GetMousePos();
			GContext->input.mouseDrawingPos.x = mousepos.x - _startx;
			GContext->input.mouseDrawingPos.y = mousepos.y - _starty;
		}
	}

}
