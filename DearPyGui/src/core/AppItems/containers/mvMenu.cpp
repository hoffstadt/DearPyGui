#include "mvMenu.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvMenu::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String, "Undocumented function", { "Containers", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_LABEL |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_FILTER |
			MV_PARSER_ARG_DROP_CALLBACK |
			MV_PARSER_ARG_DRAG_CALLBACK |
			MV_PARSER_ARG_PAYLOAD_TYPE |
			MV_PARSER_ARG_TRACKED |
			MV_PARSER_ARG_ENABLED)
		);
		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvMenu::mvMenu(mvUUID uuid)
			: mvBoolPtrBase(uuid)
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
			for (auto& sibling : m_parentPtr->m_children[1])
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
				if (!item->preDraw())
					continue;

				item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

				item->postDraw();
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