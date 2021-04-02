#include <utility>
#include "mvRadioButton.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {
	void mvRadioButton::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ s_command, mvPythonParser({
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::StringList, "items", "", "()"},
			{mvPythonDataType::Integer, "default_value", "", "0"},
			{mvPythonDataType::Callable, "callback", "Registers a callback", "None"},
			{mvPythonDataType::Object, "callback_data", "Callback data", "None"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::String, "source", "", "''"},
			{mvPythonDataType::Bool, "enabled", "Display grayed out text so selectable cannot be selected", "True"},
			{mvPythonDataType::Bool, "horizontal", "", "False"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
		}, "Adds a set of radio buttons. If items is empty, nothing will be shown.", "None", "Adding Widgets") });
	}

	mvRadioButton::mvRadioButton(const std::string& name, int default_value, const std::string& dataSource)
		: mvIntPtrBase(name, default_value)
	{
	}

	void mvRadioButton::draw(ImDrawList* drawlist, float x, float y)
	{

		ImGui::BeginGroup();

		ScopedID id;
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);

		if (!m_enabled) m_disabled_value = *m_value;

		for (size_t i = 0; i < m_itemnames.size(); i++)
		{
			if (m_horizontal && i != 0)
				ImGui::SameLine();

			if (ImGui::RadioButton((m_itemnames[i] + "##" + m_name).c_str(), m_enabled ? m_value.get() : &m_disabled_value, (int)i))
				mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callback_data);
		}

		ImGui::EndGroup();
	}

	void mvRadioButton::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		if (PyObject* item = PyDict_GetItemString(dict, "items")) m_itemnames = ToStringVect(item);
		if (PyObject* item = PyDict_GetItemString(dict, "horizontal")) m_horizontal = ToBool(item);
	}

	void mvRadioButton::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		PyDict_SetItemString(dict, "items", ToPyList(m_itemnames));
		PyDict_SetItemString(dict, "horizontal", ToPyBool(m_horizontal));
	}

	PyObject* mvRadioButton::add_radio_button(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		std::string sname = std::string(std::string("$$DPG_") + s_internal_id + std::to_string(i));
		const char* name = sname.c_str();
		PyObject* items;
		int default_value = 0;
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* before = "";
		const char* parent = "";
		const char* source = "";
		int enabled = true;
		int horizontal = false;
		int show = true;

		if (!(mvApp::GetApp()->getParsers())["add_radio_button"].parse(args, kwargs, __FUNCTION__, &name, &items,
			&default_value, &callback, &callback_data, &parent, &before, &source, &enabled, &horizontal, &show))
			return ToPyBool(false);

		auto item = CreateRef<mvRadioButton>(name, default_value, source);
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

		return ToPyString(name);
	}

}