#include "mvTab.h"
#include "mvTabBar.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {
	void mvTab::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String);
		mvAppItem::AddCommonArgs(parser);
		parser.removeArg("source");
		parser.removeArg("width");
		parser.removeArg("height");
		parser.removeArg("callback");
		parser.removeArg("callback_data");
		parser.removeArg("enabled");

		parser.addArg<mvPyDataType::Bool>("closable", mvArgType::KEYWORD_ARG, "False", "creates a button on the tab that can hide the tab");
		parser.addArg<mvPyDataType::Bool>("no_reorder", mvArgType::KEYWORD_ARG, "False", "Disable reordering this tab or having another tab cross over this tab");
		parser.addArg<mvPyDataType::Bool>("leading", mvArgType::KEYWORD_ARG, "False", "Enforce the tab position to the left of the tab bar (after the tab list popup button)");
		parser.addArg<mvPyDataType::Bool>("trailing", mvArgType::KEYWORD_ARG, "False", "Enforce the tab position to the right of the tab bar (before the scrolling buttons)");
		parser.addArg<mvPyDataType::Bool>("no_tooltip", mvArgType::KEYWORD_ARG, "False", "Disable tooltip for the given tab");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}
	mvTab::mvTab(const std::string& name)
		: 
		mvBoolPtrBase(name)
	{
	}

	void mvTab::addFlag(ImGuiTabItemFlags flag)
	{
		m_flags |= flag;
	}

	void mvTab::removeFlag(ImGuiTabItemFlags flag)
	{
		m_flags &= ~flag;
	}

	void mvTab::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id;
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);

		// cast parent to mvTabBar
		auto parent = (mvTabBar*)m_parentPtr;

		// check if this is first tab
		if (parent->getSpecificValue().empty())
		{
			// set mvTabBar value to the first tab name
			parent->setValue(m_name);
			*m_value = true;
		}

		// create tab item and see if it is selected
		if (ImGui::BeginTabItem(m_label.c_str(), m_closable ? &m_show : nullptr, m_flags))
		{

			// set other tab's value false
			for (auto child : parent->m_children[1])
			{
				if (child->getType() == mvAppItemType::mvTab)
					*((mvTab*)child.get())->m_value = false;
			}

			// set current tab value true
			*m_value = true;

			// run call back if it exists
			if (parent->getSpecificValue() != m_name)
				mvApp::GetApp()->getCallbackRegistry().addCallback(parent->getCallback(), m_name, parent->getCallbackData());

			parent->setValue(m_name);

			//we do this so that the children dont get the theme
			scope.cleanup();

			for (auto& item : m_children[1])
			{
				// skip item if it's not shown
				if (!item->m_show)
					continue;

				// set item width
				if (item->m_width != 0)
					ImGui::SetNextItemWidth((float)item->m_width);

				if (item->m_focusNextFrame)
				{
					ImGui::SetKeyboardFocusHere();
					item->m_focusNextFrame = false;
				}

				if (item->m_dirtyPos)
				{
					ImGui::SetCursorPos(item->getState().getItemPos());
					item->m_dirtyPos = false;
				}
				item->getState().setPos({ ImGui::GetCursorPosX(), ImGui::GetCursorPosY() });

				item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

				item->getState().update();
			}

			ImGui::EndTabItem();
		}

	}

	void mvTab::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		if (PyObject* item = PyDict_GetItemString(dict, "closable")) m_closable = ToBool(item);

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

	void mvTab::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		PyDict_SetItemString(dict, "closable", ToPyBool(m_closable));

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