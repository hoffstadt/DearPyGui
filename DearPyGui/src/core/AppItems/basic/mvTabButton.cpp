#include "mvTabButton.h"
#include "mvApp.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvTabButton::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		mvPythonParser parser(mvPyDataType::String);
		mvAppItem::AddCommonArgs(parser);
		parser.removeArg("width");
		parser.removeArg("height");
		parser.removeArg("source");
		parser.removeArg("enabled");

		parser.addArg<mvPyDataType::Bool>("no_reorder", mvArgType::KEYWORD, "False", "Disable reordering this tab or having another tab cross over this tab");
		parser.addArg<mvPyDataType::Bool>("leading", mvArgType::KEYWORD, "False", "Enforce the tab position to the left of the tab bar (after the tab list popup button)");
		parser.addArg<mvPyDataType::Bool>("trailing", mvArgType::KEYWORD, "False", "Enforce the tab position to the right of the tab bar (before the scrolling buttons)");
		parser.addArg<mvPyDataType::Bool>("no_tooltip", mvArgType::KEYWORD, "False", "Disable tooltip for the given tab");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvTabButton::mvTabButton(const std::string& name)
		:
		mvAppItem(name)
	{
	}

	void mvTabButton::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id;
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);

		if (ImGui::TabItemButton(m_label.c_str(), m_flags))
			mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callback_data);

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

	void mvTabButton::setExtraConfigDict(PyObject* dict)
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

	void mvTabButton::getExtraConfigDict(PyObject* dict)
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