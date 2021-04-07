#include "mvMenuItem.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvMenuItem::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ s_command, mvPythonParser({
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "default_value", "", "False"},
			{mvPythonDataType::String, "shortcut", "Adds a shortcut", "''"},
			{mvPythonDataType::Bool, "check", "Makes menu item with checkmark. Only one menu item per container can be checked at a time.", "False"},
			{mvPythonDataType::Callable, "callback", "Registers a callback", "None"},
			{mvPythonDataType::Object, "callback_data", "Callback data", "None"},
						{mvPythonDataType::String, "source", "Overrides 'name' as value storage key", "''"},
			{mvPythonDataType::String, "label", "", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::Bool, "enabled", "", "True"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
		}, "Adds a menu item to an existing menu.", "None", "Containers") });
	}

	mvMenuItem::mvMenuItem(const std::string& name)
		: mvBoolPtrBase(name) 
	{
	}

	void mvMenuItem::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id;
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);

		// This is ugly and goes against our style system but its the only widget that ImGui chooses to push teh disable color for us
		// so we have to map our text disable color to the system text disable color, or we can create a new constant which goes agains our 
		// constants. 
		ImGui::PushStyleColor(ImGuiCol_TextDisabled, ImGui::GetStyleColorVec4(ImGuiCol_Text));

		// create menu item and see if its selected
		if (ImGui::MenuItem(m_label.c_str(), m_shortcut.c_str(), m_check ? m_value.get() : nullptr, m_enabled))
		{
			mvApp::GetApp()->getCallbackRegistry().addCallback(m_callback, m_name, m_callback_data);
		}

		ImGui::PopStyleColor();

	}

	void mvMenuItem::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "shortcut")) m_shortcut = ToString(item);
		if (PyObject* item = PyDict_GetItemString(dict, "check")) m_check = ToBool(item);

}

	void mvMenuItem::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "shortcut", ToPyString(m_shortcut));
		PyDict_SetItemString(dict, "check", ToPyBool(m_check));
	}

}
