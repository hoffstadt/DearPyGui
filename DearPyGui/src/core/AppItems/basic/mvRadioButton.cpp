#include <utility>
#include "mvRadioButton.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {
	void mvRadioButton::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String);
		mvAppItem::AddCommonArgs(parser);
		parser.removeArg("width");
		parser.removeArg("height");
		parser.removeArg("label");

		parser.addArg<mvPyDataType::Integer>("items", mvArgType::POSITIONAL_ARG, "()");

		parser.addArg<mvPyDataType::Integer>("default_value", mvArgType::KEYWORD_ARG, "0");

		parser.addArg<mvPyDataType::Bool>("horizontal", mvArgType::KEYWORD_ARG, "False");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvRadioButton::mvRadioButton(const std::string& name)
		: mvIntPtrBase(name)
	{
	}

	void mvRadioButton::draw(ImDrawList* drawlist, float x, float y)
	{

		ImGui::BeginGroup();

		ScopedID id;
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);

		if (!m_enabled) m_disabled_value = *m_value;

		for (size_t i = 0; i < m_itemnames.size(); i++)
		{
			if (m_horizontal && i != 0)
				ImGui::SameLine();

			if (ImGui::RadioButton((m_itemnames[i] + "##" + m_name).c_str(), m_enabled ? m_value.get() : &m_disabled_value, (int)i))
				mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callback_data);
		}

		ImGui::EndGroup();
	}

	void mvRadioButton::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		if (PyObject* item = PyDict_GetItemString(dict, "items")) m_itemnames = ToStringVect(item);
		if (PyObject* item = PyDict_GetItemString(dict, "horizontal")) m_horizontal = ToBool(item);
	}

	void mvRadioButton::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		PyDict_SetItemString(dict, "items", ToPyList(m_itemnames));
		PyDict_SetItemString(dict, "horizontal", ToPyBool(m_horizontal));
	}

}