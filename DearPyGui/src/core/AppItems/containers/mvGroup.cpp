#include "mvGroup.h"
#include "mvInput.h"
#include "mvPythonTranslator.h"
#include "mvGlobalIntepreterLock.h"
#include "mvApp.h"

namespace Marvel {

	void mvGroup::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_group", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip", "''"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Integer, "width","", "0"},
			{mvPythonDataType::Bool, "horizontal", "", "False"},
			{mvPythonDataType::Float, "horizontal_spacing","", "-1"},
			{mvPythonDataType::String, "popup", "", "''"},
		}, "Creates a group that other widgets can belong to. The group allows item commands to be issued for all of its members.\
				Must be closed with the end command."
		, "None", "Containers") });
	}

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

		for (auto& item : m_children)
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

	PyObject* add_group(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int show = true;
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		int width = 0;
		int horizontal = false;
		float horizontal_spacing = -1.0f;
		const char* popup = "";

		if (!(*mvApp::GetApp()->getParsers())["add_group"].parse(args, kwargs, __FUNCTION__, &name,
			&show, &tip, &parent, &before, &width, &horizontal, &horizontal_spacing, &popup))
			return ToPyBool(false);

		auto item = CreateRef<mvGroup>(name);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		if (mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before))
		{
			mvApp::GetApp()->getItemRegistry().pushParent(item);
			if (!show)
				item->hide();

			return ToPyBool(true);
		}
		return ToPyBool(false);
	}

}