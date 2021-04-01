#include <utility>
#include "mvSelectable.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvSelectable::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ s_command, mvPythonParser({
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "default_value", "", "False"},
			{mvPythonDataType::Callable, "callback", "Registers a callback", "None"},
			{mvPythonDataType::Object, "callback_data", "Callback data", "None"},
			{mvPythonDataType::Integer, "width","", "0"},
			{mvPythonDataType::Integer, "height", "", "0"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::String, "source", "", "''"},
			{mvPythonDataType::Bool, "enabled", "Display grayed out text so selectable cannot be selected", "True"},
			{mvPythonDataType::String, "label", "", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::Bool, "span_columns", "span all columns", "False"},
		}, "Adds a selectable.", "None", "Adding Widgets") });
	}

	mvSelectable::mvSelectable(const std::string& name, bool default_value, const std::string& dataSource)
		: mvBoolPtrBase(name, default_value)
	{
		m_description.disableAllowed = true;
	}

	void mvSelectable::setEnabled(bool value)
	{
		if (value == m_enabled)
			return;

		if (value)
			m_flags &= ~ImGuiSelectableFlags_Disabled;

		else
			m_flags |= ImGuiSelectableFlags_Disabled;

		m_enabled = value;
	}

	void mvSelectable::draw(ImDrawList* drawlist, float x, float y)
	{

		ScopedID id;
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);

		if (ImGui::Selectable(m_label.c_str(), m_value.get(), m_flags, ImVec2((float)m_width, (float)m_height)))
			mvApp::GetApp()->getCallbackRegistry().addCallback(m_callback, m_name, m_callback_data);

	}

	void mvSelectable::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 

		// helper for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags, bool flip)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		// window flags
		flagop("span_columns", ImGuiSelectableFlags_SpanAllColumns, m_flags, false);

	}

	void mvSelectable::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags, bool flip)
		{
			PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
		};

		// window flags
		checkbitset("span_columns", ImGuiSelectableFlags_SpanAllColumns, m_flags, false);
	}

	PyObject* mvSelectable::add_selectable(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		std::string sname = std::string(std::string("$$DPG_") + s_internal_id + std::to_string(i));
		const char* name = sname.c_str();
		int default_value = false;
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		int width = 0;
		int height = 0;
		const char* before = "";
		const char* parent = "";
		const char* source = "";
		int enabled = true;
		const char* label = "";
		int show = true;
		int span_columns = false;

		//ImGuiSelectableFlags flags = ImGuiSelectableFlags_None;

		if (!(mvApp::GetApp()->getParsers())["add_selectable"].parse(args, kwargs, __FUNCTION__, &name,
			&default_value, &callback, &callback_data, &width, &height, &before, &parent, &source, &enabled,
			&label, &show, &span_columns))
			return ToPyBool(false);

		auto item = CreateRef<mvSelectable>(name, default_value, source);
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