#include "mvGroup.h"
#include "mvInput.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvGroup::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String);
		mvAppItem::AddCommonArgs(parser);
		parser.removeArg("source");
		parser.removeArg("height");
		parser.removeArg("label");
		parser.removeArg("callback");
		parser.removeArg("callback_data");
		parser.removeArg("enabled");

		parser.addArg<mvPyDataType::Bool>("horizontal", mvArgType::KEYWORD_ARG, "False");
		parser.addArg<mvPyDataType::Float>("horizontal_spacing", mvArgType::KEYWORD_ARG, "-1");


		parser.finalize();

		parsers->insert({ s_command, parser });
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

			if (item->m_focusNextFrame)
			{
				ImGui::SetKeyboardFocusHere();
				item->m_focusNextFrame = false;
			}

			item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

			if (m_horizontal)
				ImGui::SameLine(0.0, m_hspacing);

			item->getState().update();
		}

		if (m_width != 0)
			ImGui::PopItemWidth();

		ImGui::EndGroup();
	}

	void mvGroup::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		if (PyObject* item = PyDict_GetItemString(dict, "horizontal")) m_horizontal = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "horizontal_spacing")) m_hspacing = ToFloat(item);
	}

	void mvGroup::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		PyDict_SetItemString(dict, "horizontal", ToPyBool(m_horizontal));
		PyDict_SetItemString(dict, "horizontal_spacing", ToPyFloat(m_hspacing));
	}

}