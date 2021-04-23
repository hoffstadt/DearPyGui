#include "mvMenu.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvMenu::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String);
		mvAppItem::AddCommonArgs(parser);
		parser.removeArg("source");
		parser.removeArg("width");
		parser.removeArg("height");
		parser.removeArg("callback");
		parser.removeArg("callback_data");
		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvMenu::mvMenu(const std::string& name)
			: mvBoolPtrBase(name)
	{
	}

	void mvMenu::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id;
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);

		// create menu and see if its selected
		if (ImGui::BeginMenu(m_label.c_str(), m_enabled))
		{

			// set other menus's value false on same level
			for (auto sibling : m_parentPtr->m_children[1])
			{
				// ensure sibling
				if (sibling->getType() == mvAppItemType::mvMenu)
					*((mvMenu*)sibling.get())->m_value = false;
			}

			// set current menu value true
			*m_value = true;

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

				if (item->m_focusNextFrame)
				{
					ImGui::SetKeyboardFocusHere();
					item->m_focusNextFrame = false;
				}

				if (item->m_dirtyPos)
				{
					ImGui::SetCursorPos(item->getState().getItemPos());
					item->m_dirtyPos = false;
				}
				item->getState().setPos({ ImGui::GetCursorPosX(), ImGui::GetCursorPosY() });

				item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

				item->getState().update();
			}

			registerWindowFocusing();

			ImGui::EndMenu();
		}

	}

	void mvMenu::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		if (PyObject* item = PyDict_GetItemString(dict, "enabled")) m_enabled = ToBool(item);

	}

	void mvMenu::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		PyDict_SetItemString(dict, "enabled", ToPyBool(m_enabled));
	}

}