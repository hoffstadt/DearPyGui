#include "mvTooltip.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvTooltip::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_tooltip", mvPythonParser({
			{mvPythonDataType::String, "tipparent", "Sets the item's tool tip to be the same as the named item's tool tip"},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Bool, "show","Attempt to render", "True"},
		}, "Adds an advanced tool tip for an item. This command must come immediately after the item the tip is for.",
			"None", "Containers") });
	}

	mvTooltip::mvTooltip(const std::string& name)
		: 
		mvBoolPtrBase(name, false)
	{

		// has to be showed that way it can check for hovering
		// otherwise it will never show
		m_core_config.show = true;
		m_description.container = true;
		m_description.addAfterRequired = true;
		
	}

	void mvTooltip::draw()
	{
		if (ImGui::IsItemHovered())
		{
			mvImGuiThemeScope scope(this);
			mvFontScope fscope(this);
			ImGui::BeginTooltip();

			//we do this so that the children dont get the theme
			scope.cleanup();

			for (auto& item : m_children)
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

			ImGui::EndTooltip();
		}

	}

#ifndef MV_CPP

	PyObject* add_tooltip(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* tipparent;
		const char* name;
		const char* parent = "";
		const char* before = "";
		int show = true;

		if (!(mvApp::GetApp()->getParsers())["add_tooltip"].parse(args, kwargs, __FUNCTION__, &tipparent,
			&name, &parent, &before, &show))
			return ToPyBool(false);

		auto item = CreateRef<mvTooltip>(name);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		if (mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, tipparent, before))
		{
			mvApp::GetApp()->getItemRegistry().pushParent(item);
			if (!show)
				item->hide();

		}

		return GetPyNone();

	}

#endif
}