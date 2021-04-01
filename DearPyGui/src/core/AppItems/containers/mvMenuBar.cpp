#include "mvMenuBar.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "containers/mvWindowAppItem.h"
#include "containers/mvChild.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvMenuBar::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ s_command, mvPythonParser({
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
		}, "Adds a menu bar to a window. Must be followed by a call to end.", "None", "Containers") });
	}

	mvMenuBar::mvMenuBar(const std::string& name)
			: mvBoolPtrBase(name, true)
		{
			m_description.container = true;

			// TODO use code below to set item height when font and scale systems are done
			//float FontSize = ImGui::GetFontSize(); // = Base Font Size * Current Window Scale
			//ImGuiStyle currentStyle = ImGui::GetStyle(); // = Padding for the Top and Bottom
			//m_height = int(currentStyle.FramePadding.y * 2 + FontSize);
			m_core_config.height = 21;
		}

	void mvMenuBar::draw(ImDrawList* drawlist, float x, float y)
	{
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);

		if (ImGui::BeginMenuBar())
		{
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

				item->draw(drawlist, x, y);

				item->getState().update();
			}
			ImGui::EndMenuBar();
		}
	}

	PyObject* mvMenuBar::add_menu_bar(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		std::string sname = std::string(std::string("$$DPG_") + s_internal_id + std::to_string(i));
		const char* name = sname.c_str();
		int show = true;
		const char* parent = "";
		const char* before = "";

		if (!(mvApp::GetApp()->getParsers())["add_menu_bar"].parse(args, kwargs, __FUNCTION__, &name,
			&show, &parent, &before))
			return ToPyBool(false);

		auto parentItem = mvApp::GetApp()->getItemRegistry().topParent();

		if (parentItem == nullptr)
		{
			ThrowPythonException("Menubar requires a window to be on the parent stack.");
			return ToPyBool(false);
		}

		else if (parentItem->getType() == mvAppItemType::mvWindowAppItem)
		{
			auto window = static_cast<mvWindowAppItem*>(parentItem.get());
			window->addFlag(ImGuiWindowFlags_MenuBar);
			window->addMenuBar();

			auto item = CreateRef<mvMenuBar>(name);

			item->checkConfigDict(kwargs);
			item->setConfigDict(kwargs);
			item->setExtraConfigDict(kwargs);

			if (mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before))
			{
				mvApp::GetApp()->getItemRegistry().pushParent(item);
				if (!show)
					item->hide();

			}

			return GetPyNone();
		}

		else if (parentItem->getType() == mvAppItemType::mvChild)
		{
			auto child = static_cast<mvChild*>(parentItem.get());
			child->addFlag(ImGuiWindowFlags_MenuBar);

			auto item = CreateRef<mvMenuBar>(name);

			item->checkConfigDict(kwargs);
			item->setConfigDict(kwargs);
			item->setExtraConfigDict(kwargs);

			if (mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before))
			{
				mvApp::GetApp()->getItemRegistry().pushParent(item);
				if (!show)
					item->hide();

			}

			return GetPyNone();
		}

		return ToPyString(name);
	}

}