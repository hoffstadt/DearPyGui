#include "mvGroup.h"
#include "mvInput.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvGroup::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ s_command, mvPythonParser({
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Integer, "width","", "0"},
			{mvPythonDataType::Bool, "horizontal", "", "False"},
			{mvPythonDataType::Float, "horizontal_spacing","", "-1"},
		}, "Creates a group that other widgets can belong to. The group allows item commands to be issued for all of its members.\
				Must be closed with the end command."
		, "None", "Containers") });
	}

	mvGroup::mvGroup(const std::string& name)
		: mvAppItem(name)
	{
	}

	void mvGroup::draw(ImDrawList* drawlist, float x, float y)
	{
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);

		if (m_width != 0)
			ImGui::PushItemWidth((float)m_width);

		ImGui::BeginGroup();

		//we do this so that the children dont get the theme
		scope.cleanup();

		for (auto& item : m_children[1])
		{
			if (m_width != 0)
				item->setWidth(m_width);

			// skip item if it's not shown
			if (!item->m_show)
				continue;

			// set item width
			if (item->m_width != 0)
				ImGui::SetNextItemWidth((float)item->m_width);

			item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

			if (m_horizontal)
				ImGui::SameLine(0.0, m_hspacing);

			item->getState().update();
		}

		if (m_width != 0)
			ImGui::PopItemWidth();

		ImGui::EndGroup();
	}

	void mvGroup::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		if (PyObject* item = PyDict_GetItemString(dict, "horizontal")) m_horizontal = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "horizontal_spacing")) m_hspacing = ToFloat(item);
	}

	void mvGroup::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		PyDict_SetItemString(dict, "horizontal", ToPyBool(m_horizontal));
		PyDict_SetItemString(dict, "horizontal_spacing", ToPyFloat(m_hspacing));
	}

}