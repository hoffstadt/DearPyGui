#include "mvTabButton.h"
#include "mvApp.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvPythonExceptions.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvTabButton::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		mvPythonParser parser(mvPyDataType::UUID, "Undocumented function", { "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_LABEL |
			MV_PARSER_ARG_CALLBACK |
			MV_PARSER_ARG_USER_DATA |
			MV_PARSER_ARG_FILTER |
			MV_PARSER_ARG_DROP_CALLBACK |
			MV_PARSER_ARG_DRAG_CALLBACK |
			MV_PARSER_ARG_PAYLOAD_TYPE |
			MV_PARSER_ARG_TRACKED |
			MV_PARSER_ARG_SHOW)
		);

		parser.addArg<mvPyDataType::Bool>("no_reorder", mvArgType::KEYWORD_ARG, "False", "Disable reordering this tab or having another tab cross over this tab");
		parser.addArg<mvPyDataType::Bool>("leading", mvArgType::KEYWORD_ARG, "False", "Enforce the tab position to the left of the tab bar (after the tab list popup button)");
		parser.addArg<mvPyDataType::Bool>("trailing", mvArgType::KEYWORD_ARG, "False", "Enforce the tab position to the right of the tab bar (before the scrolling buttons)");
		parser.addArg<mvPyDataType::Bool>("no_tooltip", mvArgType::KEYWORD_ARG, "False", "Disable tooltip for the given tab");

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
		ScopedID id(m_uuid);
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);

		if (ImGui::TabItemButton(m_label.c_str(), m_flags))
			mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_uuid, nullptr, m_user_data);

	}

	bool mvTabButton::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvTabBar)
			return true;

		mvThrowPythonError(1000, "Tab button parent must be a tab bar.");
		MV_ITEM_REGISTRY_ERROR("Tab button parent must be a tab bar.");
		assert(false);
		return false;
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
		flagop("no_reorder", ImGuiTabItemFlags_NoReorder, m_flags);
		flagop("leading", ImGuiTabItemFlags_Leading, m_flags);
		flagop("trailing", ImGuiTabItemFlags_Trailing, m_flags);
		flagop("no_tooltip", ImGuiTabItemFlags_NoTooltip, m_flags);

		if (m_flags & ImGuiTabItemFlags_Leading && m_flags & ImGuiTabItemFlags_Trailing)
			m_flags &= ~ImGuiTabItemFlags_Leading;

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
		checkbitset("no_reorder", ImGuiTabBarFlags_Reorderable, m_flags);
		checkbitset("leading", ImGuiTabItemFlags_Leading, m_flags);
		checkbitset("trailing", ImGuiTabItemFlags_Trailing, m_flags);
		checkbitset("no_tooltip", ImGuiTabItemFlags_NoTooltip, m_flags);

	}

}