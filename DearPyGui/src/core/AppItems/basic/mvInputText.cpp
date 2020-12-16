#include "mvInputText.h"
#include "mvDataStorage.h"
#include <misc/cpp/imgui_stdlib.h>
#include <utility>
#include "mvPythonTranslator.h"
#include "mvGlobalIntepreterLock.h"

namespace Marvel {

	void mvInputText::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_input_text", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "default_value", "", "''"},
			{mvPythonDataType::String, "hint", "", "''"},
			{mvPythonDataType::Bool, "multiline", "", "False"},
			{mvPythonDataType::Bool, "no_spaces", "Filter out spaces, tabs", "False"},
			{mvPythonDataType::Bool, "uppercase", "", "False"},
			{mvPythonDataType::Bool, "decimal", "Allow 0123456789.+-*/", "False"},
			{mvPythonDataType::Bool, "hexadecimal", "Allow 0123456789ABCDEFabcdef", "False"},
			{mvPythonDataType::Bool, "readonly", "", "False"},
			{mvPythonDataType::Bool, "password", "Password mode, display all characters as '*'", "False"},
			{mvPythonDataType::Bool, "scientific", "Allow 0123456789.+-*/eE (Scientific notation input)", "False"},
			{mvPythonDataType::Callable, "callback", "Registers a callback", "None"},
			{mvPythonDataType::Object, "callback_data", "Callback data", "None"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip", "''"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::String, "source", "", "''"},
			{mvPythonDataType::Bool, "enabled", "Display grayed out text so selectable cannot be selected", "True"},
			{mvPythonDataType::Integer, "width","", "0"},
			{mvPythonDataType::Integer, "height","", "0"},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter", "False"},
			{mvPythonDataType::String, "label", "", "''"},
			{mvPythonDataType::String, "popup", "", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
		}, "Adds input for text values.", "None", "Adding Widgets") });
	}

	mvInputText::mvInputText(const std::string& name, const std::string& default_value, const std::string& dataSource)
		: 
		mvStringPtrBase(name, default_value, dataSource)
	{
		m_description.disableAllowed = true;
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

	void mvInputText::draw()
	{
		auto styleManager = m_styleManager.getScopedStyleManager();
		ScopedID id;

		if (!m_enabled)
		{
			ImVec4 disabled_color = ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
			disabled_color.w = 0.392f;
			styleManager.addColorStyle(ImGuiCol_FrameBg, disabled_color);
			styleManager.addColorStyle(ImGuiCol_FrameBgHovered, disabled_color);
			styleManager.addColorStyle(ImGuiCol_FrameBgActive, disabled_color);
			styleManager.addColorStyle(ImGuiCol_Text, ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled)));
		}

		if (m_multiline)
			m_hint = "";

		if (m_hint.empty())
		{
			if (m_multiline)
			{
				if (ImGui::InputTextMultiline(m_label.c_str(), m_value, ImVec2((float)m_width, (float)m_height), m_flags))
					mvApp::GetApp()->getCallbackRegistry().addCallback(m_callback, m_name, m_callbackData);
			}
			else
			{
				if (ImGui::InputText(m_label.c_str(), m_value, m_flags))
					mvApp::GetApp()->getCallbackRegistry().addCallback(m_callback, m_name, m_callbackData);
			}
		}

		else
		{
			if (ImGui::InputTextWithHint(m_label.c_str(), m_hint.c_str(), m_value, m_flags))
				mvApp::GetApp()->getCallbackRegistry().addCallback(m_callback, m_name, m_callbackData);
		}

	}

	void mvInputText::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;
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
	}

	void mvInputText::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;
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
	}

	PyObject* add_input_text(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* default_value = "";
		const char* hint = "";
		int multiline = 0;
		int no_spaces = false;
		int uppercase = false;
		int decimal = false;
		int hexadecimal = false;
		int readonly = false;
		int password = false;
		int scientific = false;
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		int width = 0;
		int height = 0;
		const char* before = "";
		const char* parent = "";
		const char* source = "";
		int enabled = true;
		int on_enter = false;
		const char* label = "";
		const char* popup = "";
		int show = true;

		//int flags = 0;

		if (!(*mvApp::GetApp()->getParsers())["add_input_text"].parse(args, kwargs, __FUNCTION__,
			&name, &default_value, &hint, &multiline, &no_spaces,
			&uppercase, &decimal, &hexadecimal, &readonly, &password, &scientific, &callback,
			&callback_data, &tip, &parent, &before, &source, &enabled, &width, &height, &on_enter,
			&label, &popup, &show))
			return ToPyBool(false);


		auto item = CreateRef<mvInputText>(name, default_value, source);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);


		return ToPyBool(mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before));
	}

}