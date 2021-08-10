#include "mvInputText.h"
#include <misc/cpp/imgui_stdlib.h>
#include <utility>
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvInputText::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Adds input for text.", { "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_WIDTH |
			MV_PARSER_ARG_HEIGHT |
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

		parser.addArg<mvPyDataType::String>("default_value", mvArgType::KEYWORD_ARG, "''");
		parser.addArg<mvPyDataType::String>("hint", mvArgType::KEYWORD_ARG, "''", "Displayed only when value is empty string. Will reappear if input value is set to empty string. Will not show if default value is anything other than default empty string.");

		parser.addArg<mvPyDataType::Bool>("multiline", mvArgType::KEYWORD_ARG, "False", "Allows for multiline text input.");
		parser.addArg<mvPyDataType::Bool>("no_spaces", mvArgType::KEYWORD_ARG, "False", "Filter out spaces and tabs.");
		parser.addArg<mvPyDataType::Bool>("uppercase", mvArgType::KEYWORD_ARG, "False", "Automatically make all inputs uppercase.");
		parser.addArg<mvPyDataType::Bool>("tab_input", mvArgType::KEYWORD_ARG, "False", "Allows tabs to be input instead of changing widget focus.");
		parser.addArg<mvPyDataType::Bool>("decimal", mvArgType::KEYWORD_ARG, "False", "Only allow 0123456789.+-*/");
		parser.addArg<mvPyDataType::Bool>("hexadecimal", mvArgType::KEYWORD_ARG, "False", "Only allow 0123456789ABCDEFabcdef");
		parser.addArg<mvPyDataType::Bool>("readonly", mvArgType::KEYWORD_ARG, "False", "Activates read only mode.");
		parser.addArg<mvPyDataType::Bool>("password", mvArgType::KEYWORD_ARG, "False", "Password mode, display all characters as '*'.");
		parser.addArg<mvPyDataType::Bool>("scientific", mvArgType::KEYWORD_ARG, "False", "Only allow 0123456789.+-*/eE (Scientific notation input)");
		parser.addArg<mvPyDataType::Bool>("on_enter", mvArgType::KEYWORD_ARG, "False", "Only runs callback on enter key press.");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvInputText::mvInputText(mvUUID uuid)
		: 
		mvAppItem(uuid)
	{
	}

	PyObject* mvInputText::getPyValue()
	{
		return ToPyString(*_value);
	}

	void mvInputText::setPyValue(PyObject* value)
	{
		*_value = ToString(value);
	}

	void mvInputText::setDataSource(mvUUID dataSource)
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
		_value = std::get<std::shared_ptr<std::string>>(item->getValue());
	}

	void mvInputText::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id(_uuid);

		if (_multiline)
			_hint = "";

		if (_hint.empty())
		{
			if (_multiline)
			{
				if (ImGui::InputTextMultiline(_internalLabel.c_str(), _value.get(), ImVec2((float)_width, (float)_height), _flags))
				{
					auto value = *_value;
					mvApp::GetApp()->getCallbackRegistry().submitCallback([=]() {
						mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), _uuid, ToPyString(value), _user_data);
						});
				}
			}
			else
			{
				if (ImGui::InputText(_internalLabel.c_str(), _value.get(), _flags))
				{
					auto value = *_value;
					mvApp::GetApp()->getCallbackRegistry().submitCallback([=]() {
						mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), _uuid, ToPyString(value), _user_data);
						});
				}
			}
		}

		else
		{
			if (ImGui::InputTextWithHint(_internalLabel.c_str(), _hint.c_str(), _value.get(), _flags))
			{
				auto value = *_value;
				mvApp::GetApp()->getCallbackRegistry().submitCallback([=]() {
					mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), _uuid, ToPyString(value), _user_data);
					});
			}
		}

	}

	void mvInputText::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		if (PyObject* item = PyDict_GetItemString(dict, "hint")) _hint = ToString(item);
		if (PyObject* item = PyDict_GetItemString(dict, "multiline")) _multiline = ToBool(item);

		// helper for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		// flags
		flagop("no_spaces", ImGuiInputTextFlags_CharsNoBlank, _flags);
		flagop("uppercase", ImGuiInputTextFlags_CharsUppercase, _flags);
		flagop("decimal", ImGuiInputTextFlags_CharsDecimal, _flags);
		flagop("hexadecimal", ImGuiInputTextFlags_CharsHexadecimal, _flags);
		flagop("readonly", ImGuiInputTextFlags_ReadOnly, _flags);
		flagop("password", ImGuiInputTextFlags_Password, _flags);
		flagop("on_enter", ImGuiInputTextFlags_EnterReturnsTrue, _flags);
		flagop("scientific", ImGuiInputTextFlags_CharsScientific, _flags);
		flagop("tab_input", ImGuiInputTextFlags_AllowTabInput, _flags);

		if (wasEnabledLastFrameReset())
			_flags = _stor_flags;

		if (wasDisabledLastFrameReset())
		{
			_stor_flags = _flags;
			_flags |= ImGuiInputTextFlags_ReadOnly;
			_flags &= ~ImGuiInputTextFlags_EnterReturnsTrue;
		}
	}

	void mvInputText::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		PyDict_SetItemString(dict, "hint", ToPyString(_hint));
		PyDict_SetItemString(dict, "multline", ToPyBool(_multiline));

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
		};

		// window flags
		checkbitset("no_spaces", ImGuiInputTextFlags_CharsNoBlank, _flags);
		checkbitset("uppercase", ImGuiInputTextFlags_CharsUppercase, _flags);
		checkbitset("decimal", ImGuiInputTextFlags_CharsDecimal, _flags);
		checkbitset("hexadecimal", ImGuiInputTextFlags_CharsHexadecimal, _flags);
		checkbitset("readonly", ImGuiInputTextFlags_ReadOnly, _flags);
		checkbitset("password", ImGuiInputTextFlags_Password, _flags);
		checkbitset("on_enter", ImGuiInputTextFlags_EnterReturnsTrue, _flags);
		checkbitset("scientific", ImGuiInputTextFlags_CharsScientific, _flags);
		checkbitset("tab_input", ImGuiInputTextFlags_AllowTabInput, _flags);
	}

}