#include "mvTabButton.h"
#include "mvApp.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvTabButton::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		mvPythonParser parser(mvPyDataType::UUID, "Adds a tab button to a tab bar.", { "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_CALLBACK |
			MV_PARSER_ARG_FILTER |
			MV_PARSER_ARG_DROP_CALLBACK |
			MV_PARSER_ARG_DRAG_CALLBACK |
			MV_PARSER_ARG_PAYLOAD_TYPE |
			MV_PARSER_ARG_TRACKED |
			MV_PARSER_ARG_SHOW)
		);

		parser.addArg<mvPyDataType::Bool>("no_reorder", mvArgType::KEYWORD_ARG, "False", "Disable reordering this tab or having another tab cross over this tab.");
		parser.addArg<mvPyDataType::Bool>("leading", mvArgType::KEYWORD_ARG, "False", "Enforce the tab position to the left of the tab bar (after the tab list popup button).");
		parser.addArg<mvPyDataType::Bool>("trailing", mvArgType::KEYWORD_ARG, "False", "Enforce the tab position to the right of the tab bar (before the scrolling buttons).");
		parser.addArg<mvPyDataType::Bool>("no_tooltip", mvArgType::KEYWORD_ARG, "False", "Disable tooltip for the given tab.");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvTabButton::mvTabButton(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
	}

	void mvTabButton::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id(_uuid);

		if (ImGui::TabItemButton(_internalLabel.c_str(), _flags))
			mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), _uuid, nullptr, _user_data);

	}

	void mvTabButton::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 

		// helper for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		// window flags
		flagop("no_reorder", ImGuiTabItemFlags_NoReorder, _flags);
		flagop("leading", ImGuiTabItemFlags_Leading, _flags);
		flagop("trailing", ImGuiTabItemFlags_Trailing, _flags);
		flagop("no_tooltip", ImGuiTabItemFlags_NoTooltip, _flags);

		if (_flags & ImGuiTabItemFlags_Leading && _flags & ImGuiTabItemFlags_Trailing)
			_flags &= ~ImGuiTabItemFlags_Leading;

	}

	void mvTabButton::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
		};

		// window flags
		checkbitset("no_reorder", ImGuiTabBarFlags_Reorderable, _flags);
		checkbitset("leading", ImGuiTabItemFlags_Leading, _flags);
		checkbitset("trailing", ImGuiTabItemFlags_Trailing, _flags);
		checkbitset("no_tooltip", ImGuiTabItemFlags_NoTooltip, _flags);

	}

}