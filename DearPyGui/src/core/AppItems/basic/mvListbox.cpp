#include <utility>
#include "mvListbox.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvListbox::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Adds a listbox. If height is not large enought to show all items a scroll bar will appear.", { "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_WIDTH |
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

		parser.addArg<mvPyDataType::StringList>("items", mvArgType::POSITIONAL_ARG, "()", "A tuple of items to be shown in the listbox. Can consist of any combination of types.");

		parser.addArg<mvPyDataType::String>("default_value", mvArgType::KEYWORD_ARG, "''");
		parser.addArg<mvPyDataType::Integer>("num_items", mvArgType::KEYWORD_ARG, "3", "Expands the height of the listbox to show specified number of items.");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvListbox::mvListbox(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	void mvListbox::setPyValue(PyObject* value)
	{
		*_value = ToString(value);
		updateIndex();
	}
	
	PyObject* mvListbox::getPyValue()
	{
		return ToPyString(*_value);
	}

	void mvListbox::setDataSource(mvUUID dataSource)
	{
		if (dataSource == _source) return;
		_source = dataSource;

		mvAppItem* item = mvApp::GetApp()->getItemRegistry().getItem(dataSource);
		if (!item)
		{
			mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
				"Source item not found: " + std::to_string(dataSource), this);
			return;
		}
		if (item->getValueType() != getValueType())
		{
			mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
				"Values types do not match: " + std::to_string(dataSource), this);
			return;
		}
		_value = *static_cast<std::shared_ptr<std::string>*>(item->getValue());
	}

	void mvListbox::updateIndex()
	{
		_index = 0;
		_disabledindex = 0;

		int index = 0;
		for (const auto& name : _names)
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

	void mvListbox::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id(_uuid);

		if (!_enabled)
		{
			_disabled_value = *_value;
			_disabledindex = _index;
		}

		if (ImGui::ListBox(_internalLabel.c_str(), _enabled ? &_index : &_disabledindex, _charNames.data(), (int)_names.size(), _itemsHeight))
		{
			*_value = _names[_index];
			_disabled_value = _names[_index];
			auto value = *_value;
			mvApp::GetApp()->getCallbackRegistry().submitCallback([=]() {
				mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), _uuid, ToPyString(value), _user_data);
				});
		}
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
				_names = ToStringVect(item);
				_charNames.clear();
				for (const std::string& item : _names)
					_charNames.emplace_back(item.c_str());
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
		 
		if (PyObject* item = PyDict_GetItemString(dict, "num_items")) _itemsHeight = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "items"))
		{
			_names = ToStringVect(item);
			_charNames.clear();
			for (const std::string& item : _names)
				_charNames.emplace_back(item.c_str());
			updateIndex();
		}
	}

	void mvListbox::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		PyDict_SetItemString(dict, "items", ToPyList(_names));
		PyDict_SetItemString(dict, "num_items", ToPyInt(_itemsHeight));
	}

}