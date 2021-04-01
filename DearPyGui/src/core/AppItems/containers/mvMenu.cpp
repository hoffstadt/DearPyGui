#include "mvMenu.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvMenu::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ s_parser, mvPythonParser({
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "label", "", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Bool, "enabled", "", "True"},
		}, "Adds a menu to an existing menu bar. Must be followed by a call to end.", "None", "Containers") });
	}

	mvMenu::mvMenu(const std::string& name)
			: mvBoolPtrBase(name, false)
	{
		m_description.container = true;
	}

	void mvMenu::draw()
	{
		ScopedID id;
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);

		// create menu and see if its selected
		if (ImGui::BeginMenu(m_label.c_str(), m_core_config.enabled))
		{

			// set other menus's value false on same level
			for (auto sibling : m_parent->m_children1)
			{
				// ensure sibling
				if (sibling->getType() == mvAppItemType::mvMenu)
					*((mvMenu*)sibling.get())->m_value = false;
			}

			// set current menu value true
			*m_value = true;

			//we do this so that the children dont get the theme
			scope.cleanup();

			for (auto& item : m_children1)
			{
				// skip item if it's not shown
				if (!item->m_core_config.show)
					continue;

				// set item width
				if (item->m_core_config.width != 0)
					ImGui::SetNextItemWidth((float)item->m_core_config.width);

				item->draw();

				item->getState().update();
			}

			registerWindowFocusing();

			ImGui::EndMenu();
		}

	}

	void mvMenu::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		if (PyObject* item = PyDict_GetItemString(dict, "enabled")) m_core_config.enabled = ToBool(item);

	}

	void mvMenu::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		PyDict_SetItemString(dict, "enabled", ToPyBool(m_core_config.enabled));
	}


	PyObject* mvMenu::add_menu(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		std::string sname = std::string(std::string("$$DPG_") + s_internal_id + std::to_string(i));
		const char* name = sname.c_str();
		const char* label = "";
		int show = true;
		const char* parent = "";
		const char* before = "";
		int enabled = true;

		if (!(mvApp::GetApp()->getParsers())["add_menu"].parse(args, kwargs, __FUNCTION__, &name,
			&label, &show, &parent, &before, &enabled))
			return ToPyBool(false);

		auto item = CreateRef<mvMenu>(name);

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