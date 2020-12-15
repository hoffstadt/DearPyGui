#include "mvTabBar.h"
#include "mvApp.h"
#include "mvPythonTranslator.h"
#include "mvGlobalIntepreterLock.h"

namespace Marvel {

	mvTabBar::mvTabBar(const std::string& name)
		:
		mvStringPtrBase(name, "", name)
	{
		m_description.container = true;
	}

	std::string& mvTabBar::getValue()
	{
		return *m_value;
	}

	void mvTabBar::setValue(const std::string& value)
	{
		*m_value = value;
	}

	void mvTabBar::draw()
	{
		auto styleManager = m_styleManager.getScopedStyleManager();
		ScopedID id;
		ImGui::BeginGroup();

		if (ImGui::BeginTabBar(m_label.c_str(), m_flags))
		{
			for (auto& item : m_children)
			{
				// skip item if it's not shown
				if (!item->m_show)
					continue;

				// set item width
				if (item->m_width != 0)
					ImGui::SetNextItemWidth((float)item->m_width);

				item->draw();

				// Regular Tooltip (simple)
				if (!item->m_tip.empty() && ImGui::IsItemHovered())
					ImGui::SetTooltip("%s", item->m_tip.c_str());

				item->getState().update();
			}

			ImGui::EndTabBar();
		}

		ImGui::EndGroup();
	}

	void mvTabBar::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;

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
		mvGlobalIntepreterLock gil;

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
		};

		// window flags
		checkbitset("reorderable", ImGuiTabBarFlags_Reorderable, m_flags);
	}

}