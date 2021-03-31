#include <utility>
#include "mvListbox.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvListbox::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_listbox", mvPythonParser({
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "name", "Name of the listbox"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::StringList, "items", "", "()"},
			{mvPythonDataType::Integer, "default_value", "", "0"},
			{mvPythonDataType::Callable, "callback", "Registers a callback", "None"},
			{mvPythonDataType::Object, "callback_data", "Callback data", "None"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::String, "source", "", "''"},
			{mvPythonDataType::Bool, "enabled", "Display grayed out text so selectable cannot be selected", "True"},
			{mvPythonDataType::Integer, "width","", "0"},
			{mvPythonDataType::Integer, "num_items", "number of items to show", "3"},
			{mvPythonDataType::String, "label","", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
		}, "Adds a listbox.", "None", "Adding Widgets") });
	}

	mvListbox::mvListbox(const std::string& name, int default_value, const std::string& dataSource)
		: mvIntPtrBase(name, default_value)
	{
		m_description.disableAllowed = true;
	}

	void mvListbox::draw()
	{
		ScopedID id;
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);

		if (!m_core_config.enabled) m_disabled_value = *m_value;

		if (ImGui::ListBox(m_label.c_str(), m_core_config.enabled ? m_value.get() : &m_disabled_value, m_charNames.data(), (int)m_names.size(), m_itemsHeight))
			mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_core_config.name, m_core_config.callback_data);
	}

	void mvListbox::updateConfig(mvAppItemConfig* config)
	{
		auto aconfig = (mvListboxConfig*)config;

		m_core_config.label = config->label;
		m_core_config.width = config->width;
		m_core_config.show = config->show;
		m_core_config.enabled = config->enabled;
		m_core_config.callback = config->callback;
		m_core_config.callback_data = config->callback_data;

		m_config.source = aconfig->source;

		if (config != &m_config)
			m_config = *aconfig;
	}

	mvAppItemConfig* mvListbox::getConfig()
	{
		return &m_config;
	}

	void mvListbox::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		if (PyObject* item = PyDict_GetItemString(dict, "num_items")) m_itemsHeight = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "items"))
		{
			m_names = ToStringVect(item);
			m_charNames.clear();
			for (const std::string& item : m_names)
				m_charNames.emplace_back(item.c_str());
		}
	}

	void mvListbox::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		PyDict_SetItemString(dict, "items", ToPyList(m_names));
		PyDict_SetItemString(dict, "num_items", ToPyInt(m_itemsHeight));
	}

	PyObject* mvListbox::add_listbox(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		std::string sname = std::string(std::string("$$DPG_") + s_internal_id + std::to_string(i));
		const char* name = sname.c_str();
		PyObject* items;
		int default_value = 0;
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		int width = 0;
		int num_items = 3;
		const char* before = "";
		const char* parent = "";
		const char* source = "";
		int enabled = true;
		const char* label = "";
		int show = true;

		if (!(mvApp::GetApp()->getParsers())["add_listbox"].parse(args, kwargs, __FUNCTION__, &name, &items,
			&default_value, &callback, &callback_data, &parent, &before, &source, &enabled, &width,
			&num_items, &label, &show))
			return ToPyBool(false);

		auto item = CreateRef<mvListbox>(name, default_value, source);
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