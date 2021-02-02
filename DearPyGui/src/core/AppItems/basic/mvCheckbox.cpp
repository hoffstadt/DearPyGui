#include <utility>
#include "mvCheckbox.h"
#include "mvApp.h"
#include "mvValueStorage.h"

namespace Marvel {

	void mvCheckbox::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_checkbox", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "default_value", "", "False"},
			{mvPythonDataType::Callable, "callback", "Registers a callback", "None"},
			{mvPythonDataType::Object, "callback_data", "Callback data", "None"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::String, "source", "Overrides 'name' as value storage key", "''"},
			{mvPythonDataType::String, "label", "Overrides 'name' as label", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::Bool, "enabled", "", "True"}
		}, "Adds a checkbox widget.", "None", "Adding Widgets") });
	}

	mvCheckbox::mvCheckbox(const std::string& name, bool default_value, const std::string& dataSource)
		: mvBoolPtrBase(name, default_value, dataSource)
	{
		m_description.disableAllowed = true;
	}

	void mvCheckbox::draw()
	{
		auto styleManager = m_styleManager.getScopedStyleManager();
		ScopedID id;
		mvImGuiThemeScope scope(this);

		if (!m_core_config.enabled)
		{
			ImVec4 disabled_color = ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
			disabled_color.w = 0.392f;
			styleManager.addColorStyle(ImGuiCol_CheckMark, disabled_color);
			styleManager.addColorStyle(ImGuiCol_FrameBg, disabled_color);
			styleManager.addColorStyle(ImGuiCol_FrameBgHovered, disabled_color);
			styleManager.addColorStyle(ImGuiCol_FrameBgActive, disabled_color);
			styleManager.addColorStyle(ImGuiCol_Text, ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled)));
			m_disabled_value = *m_value;
		}

		if (ImGui::Checkbox(m_label.c_str(), m_core_config.enabled ? m_value : &m_disabled_value))
			mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_core_config.name, m_core_config.callback_data);

	}

#ifndef MV_CPP
	PyObject* add_checkbox(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int default_value = 0;
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* before = "";
		const char* parent = "";
		const char* source = "";
		const char* label = "";
		int show = true;
		int enabled = true;

		if (!(*mvApp::GetApp()->getParsers())["add_checkbox"].parse(args, kwargs, __FUNCTION__, &name,
			&default_value, &callback, &callback_data, &parent, &before, &source,
			&label, &show, &enabled))
			return ToPyBool(false);

		auto item = CreateRef<mvCheckbox>(name, default_value, source);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before);

		return GetPyNone();
	}
#endif
}