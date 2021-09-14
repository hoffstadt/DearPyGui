#include "mvInputText.h"
#include <misc/cpp/imgui_stdlib.h>
#include <utility>
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"

namespace Marvel {

	void mvInputText::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		std::vector<mvPythonDataElement> args;

		AddCommonArgs(args,(CommonParserArgs)(
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

		args.push_back({ mvPyDataType::String, "default_value", mvArgType::KEYWORD_ARG, "''" });
		args.push_back({ mvPyDataType::String, "hint", mvArgType::KEYWORD_ARG, "''", "Displayed only when value is empty string. Will reappear if input value is set to empty string. Will not show if default value is anything other than default empty string." });
		args.push_back({ mvPyDataType::Bool, "multiline", mvArgType::KEYWORD_ARG, "False", "Allows for multiline text input." });
		args.push_back({ mvPyDataType::Bool, "no_spaces", mvArgType::KEYWORD_ARG, "False", "Filter out spaces and tabs." });
		args.push_back({ mvPyDataType::Bool, "uppercase", mvArgType::KEYWORD_ARG, "False", "Automatically make all inputs uppercase." });
		args.push_back({ mvPyDataType::Bool, "tab_input", mvArgType::KEYWORD_ARG, "False", "Allows tabs to be input instead of changing widget focus." });
		args.push_back({ mvPyDataType::Bool, "decimal", mvArgType::KEYWORD_ARG, "False", "Only allow 0123456789.+-*/" });
		args.push_back({ mvPyDataType::Bool, "hexadecimal", mvArgType::KEYWORD_ARG, "False", "Only allow 0123456789ABCDEFabcdef" });
		args.push_back({ mvPyDataType::Bool, "readonly", mvArgType::KEYWORD_ARG, "False", "Activates read only mode." });
		args.push_back({ mvPyDataType::Bool, "password", mvArgType::KEYWORD_ARG, "False", "Password mode, display all characters as '*'." });
		args.push_back({ mvPyDataType::Bool, "scientific", mvArgType::KEYWORD_ARG, "False", "Only allow 0123456789.+-*/eE (Scientific notation input)" });
		args.push_back({ mvPyDataType::Bool, "on_enter", mvArgType::KEYWORD_ARG, "False", "Only runs callback on enter key press." });

		mvPythonParserSetup setup;
		setup.about = "Adds input for text.";
		setup.category = { "Widgets" };
		setup.returnType = mvPyDataType::UUID;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers->insert({ s_command, parser });
	}

	mvInputText::mvInputText(mvUUID uuid)
		: 
		mvAppItem(uuid)
	{
	}

	void mvInputText::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvInputText*>(item);
		if (_source != 0) _value = titem->_value;
		_disabled_value = titem->_disabled_value;
		_hint = titem->_hint;
		_multiline = titem->_multiline;
		_flags = titem->_flags;
		_stor_flags = titem->_stor_flags;
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

		mvAppItem* item = GetItem((*mvApp::GetApp()->itemRegistry), dataSource);
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

	void mvInputText::draw(ImDrawList* drawlist, float x, float y)
	{

		//-----------------------------------------------------------------------------
		// pre draw
		//-----------------------------------------------------------------------------

		// show/hide
		if (!_show)
			return;

		// focusing
		if (_focusNextFrame)
		{
			ImGui::SetKeyboardFocusHere();
			_focusNextFrame = false;
		}

		// cache old cursor position
		ImVec2 previousCursorPos = ImGui::GetCursorPos();

		// set cursor position if user set
		if (_dirtyPos)
			ImGui::SetCursorPos(_state.pos);

		// update widget's position state
		_state.pos = { ImGui::GetCursorPosX(), ImGui::GetCursorPosY() };

		// set item width
		if (_width != 0)
			ImGui::SetNextItemWidth((float)_width);

		// set indent
		if (_indent > 0.0f)
			ImGui::Indent(_indent);

		// push font if a font object is attached
		if (_font)
		{
			ImFont* fontptr = static_cast<mvFont*>(_font.get())->getFontPtr();
			ImGui::PushFont(fontptr);
		}

		// themes
		if (auto classTheme = getClassThemeComponent())
			static_cast<mvThemeComponent*>(classTheme.get())->draw(nullptr, 0.0f, 0.0f);

		if (_theme)
		{
			static_cast<mvTheme*>(_theme.get())->setSpecificEnabled(_enabled);
			static_cast<mvTheme*>(_theme.get())->setSpecificType((int)getType());
			static_cast<mvTheme*>(_theme.get())->draw(nullptr, 0.0f, 0.0f);
		}

		//-----------------------------------------------------------------------------
		// draw
		//-----------------------------------------------------------------------------
		{

			ScopedID id(_uuid);

			bool activated = false;

			if (_multiline)
				_hint = "";

			if (_hint.empty())
			{
				if (_multiline)
					activated = ImGui::InputTextMultiline(_internalLabel.c_str(), _value.get(), ImVec2((float)_width, (float)_height), _flags);
				else
					activated = ImGui::InputText(_internalLabel.c_str(), _value.get(), _flags);
			}
			else
				activated = ImGui::InputTextWithHint(_internalLabel.c_str(), _hint.c_str(), _value.get(), _flags);

			if (activated)
			{
				auto value = *_value;
				if(_alias.empty())
					mvApp::GetApp()->getCallbackRegistry().submitCallback([=]() {
						mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), _uuid, ToPyString(value), _user_data);
						});
				else
					mvApp::GetApp()->getCallbackRegistry().submitCallback([=]() {
					mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), _alias, ToPyString(value), _user_data);
						});
			}

		}

		//-----------------------------------------------------------------------------
		// update state
		//-----------------------------------------------------------------------------
		UpdateAppItemState(_state);

		//-----------------------------------------------------------------------------
		// post draw
		//-----------------------------------------------------------------------------

		// set cursor position to cached position
		if (_dirtyPos)
			ImGui::SetCursorPos(previousCursorPos);

		if (_indent > 0.0f)
			ImGui::Unindent(_indent);

		// pop font off stack
		if (_font)
			ImGui::PopFont();

		// handle popping themes
		if (auto classTheme = getClassThemeComponent())
			static_cast<mvThemeComponent*>(classTheme.get())->customAction();

		if (_theme)
		{
			static_cast<mvTheme*>(_theme.get())->setSpecificEnabled(_enabled);
			static_cast<mvTheme*>(_theme.get())->setSpecificType((int)getType());
			static_cast<mvTheme*>(_theme.get())->customAction();
		}

		if (_handlerRegistry)
			_handlerRegistry->customAction(&_state);

		// handle drag & drop payloads
		for (auto& item : _children[3])
			item->draw(nullptr, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

		// handle drag & drop if used
		if (_dropCallback)
		{
			ScopedID id(_uuid);
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(_payloadType.c_str()))
				{
					auto payloadActual = static_cast<const mvDragPayload*>(payload->Data);
					if (_alias.empty())
						mvApp::GetApp()->getCallbackRegistry().addCallback(_dropCallback,_uuid, payloadActual->getDragData(), nullptr);
					else
						mvApp::GetApp()->getCallbackRegistry().addCallback(_dropCallback,_alias, payloadActual->getDragData(), nullptr);
				}

				ImGui::EndDragDropTarget();
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
		 
		PyDict_SetItemString(dict, "hint", mvPyObject(ToPyString(_hint)));
		PyDict_SetItemString(dict, "multline", mvPyObject(ToPyBool(_multiline)));

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, mvPyObject(ToPyBool(flags & flag)));
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