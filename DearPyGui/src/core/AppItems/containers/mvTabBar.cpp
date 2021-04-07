#include "mvTabBar.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvTab.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvTabBar::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ s_command, mvPythonParser({
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "reorderable", "allows for moveable tabs", "False"},
			{mvPythonDataType::Callable, "callback", "Registers a callback", "None"},
			{mvPythonDataType::Object, "callback_data", "Callback data", "None"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
		}, "Adds a tab bar.", "None", "Containers") });
	}

	mvTabBar::mvTabBar(const std::string& name)
		:
		mvStringPtrBase(name)
	{
	}

	std::string& mvTabBar::getSpecificValue()
	{
		return m_uiValue;
	}

	void mvTabBar::setValue(const std::string& value)
	{
		m_uiValue = value;
	}

	void mvTabBar::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id;
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);
		ImGui::BeginGroup();

		if (ImGui::BeginTabBar(m_label.c_str(), m_flags))
		{
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

				if (*m_value == item->m_name && m_lastValue != *m_value)
					static_cast<mvTab*>(item.get())->addFlag(ImGuiTabItemFlags_SetSelected);

				item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

				if (*m_value == item->m_name)
					static_cast<mvTab*>(item.get())->removeFlag(ImGuiTabItemFlags_SetSelected);

				item->getState().update();
			}

			ImGui::EndTabBar();
		}

		ImGui::EndGroup();

		*m_value = m_uiValue;
		m_lastValue = *m_value;
	}

	void mvTabBar::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 

		// helper for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		// window flags
		flagop("reorderable", ImGuiTabBarFlags_Reorderable, m_flags);

	}

	void mvTabBar::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
		};

		// window flags
		checkbitset("reorderable", ImGuiTabBarFlags_Reorderable, m_flags);
	}

}