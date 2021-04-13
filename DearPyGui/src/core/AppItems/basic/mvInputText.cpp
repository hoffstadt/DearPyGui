#include "mvInputText.h"
#include <misc/cpp/imgui_stdlib.h>
#include <utility>
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvInputText::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String);
		mvAppItem::AddCommonArgs(parser);

		parser.addArg<mvPyDataType::String>("default_value", mvArgType::KEYWORD, "''");
		parser.addArg<mvPyDataType::String>("hint", mvArgType::KEYWORD, "''");

		parser.addArg<mvPyDataType::Bool>("multiline", mvArgType::KEYWORD, "False");
		parser.addArg<mvPyDataType::Bool>("no_spaces", mvArgType::KEYWORD, "False", "Filter out spaces, tabs");
		parser.addArg<mvPyDataType::Bool>("uppercase", mvArgType::KEYWORD, "False");
		parser.addArg<mvPyDataType::Bool>("tab_input", mvArgType::KEYWORD, "False", "Allows tabs to be input instead of changing widget focus");
		parser.addArg<mvPyDataType::Bool>("decimal", mvArgType::KEYWORD, "False", "Allow 0123456789.+-*/");
		parser.addArg<mvPyDataType::Bool>("hexadecimal", mvArgType::KEYWORD, "False", "Allow 0123456789ABCDEFabcdef");
		parser.addArg<mvPyDataType::Bool>("readonly", mvArgType::KEYWORD, "False");
		parser.addArg<mvPyDataType::Bool>("password", mvArgType::KEYWORD, "False", "Password mode, display all characters as '*'");
		parser.addArg<mvPyDataType::Bool>("scientific", mvArgType::KEYWORD, "False", "Allow 0123456789.+-*/eE (Scientific notation input)");
		parser.addArg<mvPyDataType::Bool>("on_enter", mvArgType::KEYWORD, "False", "Only runs callback on enter");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvInputText::mvInputText(const std::string& name)
		: 
		mvStringPtrBase(name)
	{
	}

	void mvInputText::setEnabled(bool value)
	{
		if (value)
			m_flags = m_stor_flags;

		else
		{
			m_stor_flags = m_flags;
			m_flags |= ImGuiInputTextFlags_ReadOnly;
			m_flags &= ~ImGuiInputTextFlags_EnterReturnsTrue;
		}

		m_enabled = value;
	}

	void mvInputText::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id;
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);

		if (m_multiline)
			m_hint = "";

		if (m_hint.empty())
		{
			if (m_multiline)
			{
				if (ImGui::InputTextMultiline(m_label.c_str(), m_value.get(), ImVec2((float)m_width, (float)m_height), m_flags))
					mvApp::GetApp()->getCallbackRegistry().addCallback(m_callback, m_name, m_callback_data);
			}
			else
			{
				if (ImGui::InputText(m_label.c_str(), m_value.get(), m_flags))
					mvApp::GetApp()->getCallbackRegistry().addCallback(m_callback, m_name, m_callback_data);
			}
		}

		else
		{
			if (ImGui::InputTextWithHint(m_label.c_str(), m_hint.c_str(), m_value.get(), m_flags))
				mvApp::GetApp()->getCallbackRegistry().addCallback(m_callback, m_name, m_callback_data);
		}

	}

	void mvInputText::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		if (PyObject* item = PyDict_GetItemString(dict, "hint")) m_hint = ToString(item);
		if (PyObject* item = PyDict_GetItemString(dict, "multiline")) m_multiline = ToBool(item);

		// helper for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		// flags
		flagop("no_spaces", ImGuiInputTextFlags_CharsNoBlank, m_flags);
		flagop("uppercase", ImGuiInputTextFlags_CharsUppercase, m_flags);
		flagop("decimal", ImGuiInputTextFlags_CharsDecimal, m_flags);
		flagop("hexadecimal", ImGuiInputTextFlags_CharsHexadecimal, m_flags);
		flagop("readonly", ImGuiInputTextFlags_ReadOnly, m_flags);
		flagop("password", ImGuiInputTextFlags_Password, m_flags);
		flagop("on_enter", ImGuiInputTextFlags_EnterReturnsTrue, m_flags);
		flagop("scientific", ImGuiInputTextFlags_CharsScientific, m_flags);
		flagop("tab_input", ImGuiInputTextFlags_AllowTabInput, m_flags);
	}

	void mvInputText::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		PyDict_SetItemString(dict, "hint", ToPyString(m_hint));
		PyDict_SetItemString(dict, "multline", ToPyBool(m_multiline));

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
		};

		// window flags
		checkbitset("no_spaces", ImGuiInputTextFlags_CharsNoBlank, m_flags);
		checkbitset("uppercase", ImGuiInputTextFlags_CharsUppercase, m_flags);
		checkbitset("decimal", ImGuiInputTextFlags_CharsDecimal, m_flags);
		checkbitset("hexadecimal", ImGuiInputTextFlags_CharsHexadecimal, m_flags);
		checkbitset("readonly", ImGuiInputTextFlags_ReadOnly, m_flags);
		checkbitset("password", ImGuiInputTextFlags_Password, m_flags);
		checkbitset("on_enter", ImGuiInputTextFlags_EnterReturnsTrue, m_flags);
		checkbitset("scientific", ImGuiInputTextFlags_CharsScientific, m_flags);
		checkbitset("tab_input", ImGuiInputTextFlags_AllowTabInput, m_flags);
	}

}