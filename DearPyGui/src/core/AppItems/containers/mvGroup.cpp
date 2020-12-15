#include "mvGroup.h"
#include "mvInput.h"
#include "mvPythonTranslator.h"
#include "mvGlobalIntepreterLock.h"

namespace Marvel {

	mvGroup::mvGroup(const std::string& name)
		: mvAppItem(name)
	{
		m_description.container = true;
	}

	void mvGroup::draw()
	{
		auto styleManager = m_styleManager.getScopedStyleManager();

		if (m_width != 0)
			ImGui::PushItemWidth((float)m_width);

		ImGui::BeginGroup();

		for (auto item : m_children)
		{
			if (m_width != 0)
				item->setWidth(m_width);

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

			if (m_horizontal)
				ImGui::SameLine(0.0, m_hspacing);

			item->getState().update();
		}

		if (m_width != 0)
			ImGui::PopItemWidth();

		ImGui::EndGroup();

		if (!m_tip.empty() && ImGui::IsItemHovered())
			ImGui::SetTooltip("%s", m_tip.c_str());

	}

	void mvGroup::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;
		if (PyObject* item = PyDict_GetItemString(dict, "horizontal")) m_horizontal = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "horizontal_spacing")) m_hspacing = ToFloat(item);
	}

	void mvGroup::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;
		PyDict_SetItemString(dict, "horizontal", ToPyBool(m_horizontal));
		PyDict_SetItemString(dict, "horizontal_spacing", ToPyFloat(m_hspacing));
	}

}