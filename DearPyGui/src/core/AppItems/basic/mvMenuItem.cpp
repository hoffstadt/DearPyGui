#include "mvMenuItem.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvMenuItem::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Adds a menu item to an existing menu. Menu items act similar to selectables.", { "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_CALLBACK |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_FILTER |
			MV_PARSER_ARG_DROP_CALLBACK |
			MV_PARSER_ARG_DRAG_CALLBACK |
			MV_PARSER_ARG_PAYLOAD_TYPE |
			MV_PARSER_ARG_TRACKED |
			MV_PARSER_ARG_ENABLED)
		);

		parser.addArg<mvPyDataType::Bool>("default_value", mvArgType::KEYWORD_ARG, "False");

		parser.addArg<mvPyDataType::String>("shortcut", mvArgType::KEYWORD_ARG, "''", "Displays text on the menu item. Typically used to show a shortcut key command.");

		parser.addArg<mvPyDataType::Bool>("check", mvArgType::KEYWORD_ARG, "False", "Displays a checkmark on the menu item when it is selected.");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvMenuItem::mvMenuItem(mvUUID uuid)
		: mvAppItem(uuid) 
	{
	}

	void mvMenuItem::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id(_uuid);

		// This is ugly and goes against our style system but its the only widget that ImGui chooses to push the disable color for us
		// so we have to map our text disable color to the system text disable color, or we can create a new constant which goes agains our 
		// constants. 
		ImGui::PushStyleColor(ImGuiCol_TextDisabled, ImGui::GetStyleColorVec4(ImGuiCol_Text));

		// create menu item and see if its selected
		if (ImGui::MenuItem(_internalLabel.c_str(), _shortcut.c_str(), _check ? _value.get() : nullptr, _enabled))
		{
			mvApp::GetApp()->getCallbackRegistry().addCallback(_callback, _uuid, nullptr, _user_data);
		}

		ImGui::PopStyleColor();

	}

	void mvMenuItem::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "shortcut")) _shortcut = ToString(item);
		if (PyObject* item = PyDict_GetItemString(dict, "check")) _check = ToBool(item);

}

	void mvMenuItem::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "shortcut", mvPyObject(ToPyString(_shortcut)));
		PyDict_SetItemString(dict, "check", mvPyObject(ToPyBool(_check)));
	}

	PyObject* mvMenuItem::getPyValue()
	{
		return ToPyBool(*_value);
	}

	void mvMenuItem::setPyValue(PyObject* value)
	{
		*_value = ToBool(value);
	}

	void mvMenuItem::setDataSource(mvUUID dataSource)
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
		_value = *static_cast<std::shared_ptr<bool>*>(item->getValue());
	}
}
