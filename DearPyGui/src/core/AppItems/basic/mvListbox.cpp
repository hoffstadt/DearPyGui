#include <utility>
#include "mvListbox.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvListbox::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String);
		mvAppItem::AddCommonArgs(parser);
		parser.removeArg("height");

		parser.addArg<mvPyDataType::StringList>("items", mvArgType::POSITIONAL_ARG, "()");

		parser.addArg<mvPyDataType::Integer>("default_value", mvArgType::KEYWORD_ARG, "0");
		parser.addArg<mvPyDataType::Integer>("num_items", mvArgType::KEYWORD_ARG, "3", "number of items to show");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvListbox::mvListbox(const std::string& name)
		: mvIntPtrBase(name)
	{
	}

	void mvListbox::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id;
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);

		if (!m_enabled) m_disabled_value = *m_value;

		if (ImGui::ListBox(m_label.c_str(), m_enabled ? m_value.get() : &m_disabled_value, m_charNames.data(), (int)m_names.size(), m_itemsHeight))
			mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callback_data);
	}

	void mvListbox::handleSpecificPositionalArgs(PyObject* dict)
	{
		if (!mvApp::GetApp()->getParsers()[s_command].verifyPositionalArguments(dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				m_names = ToStringVect(item);
				m_charNames.clear();
				for (const std::string& item : m_names)
					m_charNames.emplace_back(item.c_str());
				break;

			default:
				break;
			}
		}
	}

	void mvListbox::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		if (PyObject* item = PyDict_GetItemString(dict, "num_items")) m_itemsHeight = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "items"))
		{
			m_names = ToStringVect(item);
			m_charNames.clear();
			for (const std::string& item : m_names)
				m_charNames.emplace_back(item.c_str());
		}
	}

	void mvListbox::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		PyDict_SetItemString(dict, "items", ToPyList(m_names));
		PyDict_SetItemString(dict, "num_items", ToPyInt(m_itemsHeight));
	}

}