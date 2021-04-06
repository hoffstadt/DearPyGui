#include "mvTooltip.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvTooltip::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ s_command, mvPythonParser({
			{mvPythonDataType::String, "tipparent", "Sets the item's tool tip to be the same as the named item's tool tip"},
			{mvPythonDataType::Optional},
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
		m_show = true;
		
	}

	void mvTooltip::draw(ImDrawList* drawlist, float x, float y)
	{
		if (ImGui::IsItemHovered())
		{
			mvImGuiThemeScope scope(this);
			mvFontScope fscope(this);
			ImGui::BeginTooltip();

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

				item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

				item->getState().update();
			}

			ImGui::EndTooltip();
		}

	}

	PyObject* mvTooltip::add_tooltip(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* tipparent;
		static int i = 0; i++;
		std::string sname = std::string(std::string("$$DPG_") + s_internal_id + std::to_string(i));
		const char* name = sname.c_str();
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

		return ToPyString(name);

	}

}