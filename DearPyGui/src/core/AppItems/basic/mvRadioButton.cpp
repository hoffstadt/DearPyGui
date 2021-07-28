#include <utility>
#include "mvRadioButton.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
namespace Marvel {

	void mvRadioButton::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Adds a set of radio buttons. If items keyword is empty, nothing will be shown.", { "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SOURCE |
			MV_PARSER_ARG_CALLBACK |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_ENABLED |
			MV_PARSER_ARG_FILTER |
			MV_PARSER_ARG_DROP_CALLBACK |
			MV_PARSER_ARG_DRAG_CALLBACK |
			MV_PARSER_ARG_PAYLOAD_TYPE |
			MV_PARSER_ARG_TRACKED |
			MV_PARSER_ARG_POS)
		);

		parser.addArg<mvPyDataType::StringList>("items", mvArgType::POSITIONAL_ARG, "()", "A tuple of items to be shown as radio options. Can consist of any combination of types.");

		parser.addArg<mvPyDataType::String>("default_value", mvArgType::KEYWORD_ARG, "''");

		parser.addArg<mvPyDataType::Bool>("horizontal", mvArgType::KEYWORD_ARG, "False", "Displays the radio options horizontally.");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvRadioButton::mvRadioButton(mvUUID uuid)
		: mvStringPtrBase(uuid)
	{
	}

	void mvRadioButton::setPyValue(PyObject* value)
	{
		*_value = ToString(value);
		updateIndex();
	}

	void mvRadioButton::updateIndex()
	{
		_index = 0;
		_disabledindex = 0;

		int index = 0;
		for (const auto& name : _itemnames)
		{
			if (name == *_value)
			{
				_index = index;
				_disabledindex = index;
				break;
			}
			index++;
		}
	}

	void mvRadioButton::draw(ImDrawList* drawlist, float x, float y)
	{

		ImGui::BeginGroup();

		ScopedID id(_uuid);

		if (!_enabled)
		{
			_disabled_value = *_value;
			_disabledindex = _index;
		}

		for (size_t i = 0; i < _itemnames.size(); i++)
		{
			if (_horizontal && i != 0)
				ImGui::SameLine();

			if (ImGui::RadioButton(_itemnames[i].c_str(), _enabled ? &_index : &_disabledindex, (int)i))
			{
				*_value = _itemnames[_index];
				_disabled_value = _itemnames[_index];
				auto value = *_value;
				mvApp::GetApp()->getCallbackRegistry().submitCallback([=]() {
					mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), _uuid, ToPyString(value), _user_data);
					});
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
				_itemnames = ToStringVect(item);
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
			_itemnames = ToStringVect(item);
			updateIndex();
		}
		if (PyObject* item = PyDict_GetItemString(dict, "horizontal")) _horizontal = ToBool(item);
	}

	void mvRadioButton::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		PyDict_SetItemString(dict, "items", ToPyList(_itemnames));
		PyDict_SetItemString(dict, "horizontal", ToPyBool(_horizontal));
	}

}