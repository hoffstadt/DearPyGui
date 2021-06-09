#include <utility>
#include "mvRadioButton.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvRadioButton::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Undocumented function", { "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SOURCE |
			MV_PARSER_ARG_CALLBACK |
			MV_PARSER_ARG_USER_DATA |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_ENABLED |
			MV_PARSER_ARG_FILTER |
			MV_PARSER_ARG_DROP_CALLBACK |
			MV_PARSER_ARG_DRAG_CALLBACK |
			MV_PARSER_ARG_PAYLOAD_TYPE |
			MV_PARSER_ARG_TRACKED |
			MV_PARSER_ARG_POS)
		);

		parser.addArg<mvPyDataType::Integer>("items", mvArgType::POSITIONAL_ARG, "()");

		parser.addArg<mvPyDataType::String>("default_value", mvArgType::KEYWORD_ARG, "''");

		parser.addArg<mvPyDataType::Bool>("horizontal", mvArgType::KEYWORD_ARG, "False");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvRadioButton::mvRadioButton(mvUUID uuid)
		: mvStringPtrBase(uuid)
	{
	}

	void mvRadioButton::setPyValue(PyObject* value)
	{
		*m_value = ToString(value);
		updateIndex();
	}

	void mvRadioButton::updateIndex()
	{
		m_index = 0;
		m_disabledindex = 0;

		int index = 0;
		for (const auto& name : m_itemnames)
		{
			if (name == *m_value)
			{
				m_index = index;
				m_disabledindex = index;
				break;
			}
			index++;
		}
	}

	void mvRadioButton::draw(ImDrawList* drawlist, float x, float y)
	{

		ImGui::BeginGroup();

		ScopedID id(m_uuid);
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);

		if (!m_enabled)
		{
			m_disabled_value = *m_value;
			m_disabledindex = m_index;
		}

		for (size_t i = 0; i < m_itemnames.size(); i++)
		{
			if (m_horizontal && i != 0)
				ImGui::SameLine();

			if (ImGui::RadioButton(m_itemnames[i].c_str(), m_enabled ? &m_index : &m_disabledindex, (int)i))
			{
				*m_value = m_itemnames[m_index];
				m_disabled_value = m_itemnames[m_index];
				mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_uuid, nullptr, m_user_data);
			}

		}

		ImGui::EndGroup();
	}

	void mvRadioButton::handleSpecificPositionalArgs(PyObject* dict)
	{
		if (!mvApp::GetApp()->getParsers()[s_command].verifyPositionalArguments(dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				m_itemnames = ToStringVect(item);
				break;

			default:
				break;
			}
		}
	}

	void mvRadioButton::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		if (PyObject* item = PyDict_GetItemString(dict, "items"))
		{
			m_itemnames = ToStringVect(item);
			updateIndex();
		}
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