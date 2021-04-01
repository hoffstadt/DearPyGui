#include "mvGroup.h"
#include "mvInput.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvGroup::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ s_parser, mvPythonParser({
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
		m_description.container = true;
	}

	void mvGroup::draw(ImDrawList* drawlist, float x, float y)
	{
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);

		if (m_core_config.width != 0)
			ImGui::PushItemWidth((float)m_core_config.width);

		ImGui::BeginGroup();

		//we do this so that the children dont get the theme
		scope.cleanup();

		for (auto& item : m_children1)
		{
			if (m_core_config.width != 0)
				item->setWidth(m_core_config.width);

			// skip item if it's not shown
			if (!item->m_core_config.show)
				continue;

			// set item width
			if (item->m_core_config.width != 0)
				ImGui::SetNextItemWidth((float)item->m_core_config.width);

			item->draw(drawlist, x, y);

			if (m_horizontal)
				ImGui::SameLine(0.0, m_hspacing);

			item->getState().update();
		}

		if (m_core_config.width != 0)
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

	PyObject* mvGroup::add_group(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		std::string sname = std::string(std::string("$$DPG_") + s_internal_id + std::to_string(i));
		const char* name = sname.c_str();
		int show = true;
		const char* parent = "";
		const char* before = "";
		int width = 0;
		int horizontal = false;
		float horizontal_spacing = -1.0f;

		if (!(mvApp::GetApp()->getParsers())["add_group"].parse(args, kwargs, __FUNCTION__, &name,
			&show, &parent, &before, &width, &horizontal, &horizontal_spacing))
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

		}

		return ToPyString(name);
	}

}