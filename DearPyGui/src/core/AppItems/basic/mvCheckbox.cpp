#include <utility>
#include "mvCheckbox.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvCheckbox::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ s_parser, mvPythonParser({
			{mvPythonDataType::Optional},
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
		: 
		mvBoolPtrBase(name, default_value)
	{
		m_description.disableAllowed = true;
	}

	mvCheckbox::mvCheckbox(const std::string& name, const mvCheckboxConfig& config)
		: 
		mvBoolPtrBase(name, config.default_value), 
		m_config(config)
	{
		m_description.disableAllowed = true;

		m_config.name = name;
		updateConfig(&m_config);
	}

	void mvCheckbox::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id;
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);

		if (!m_core_config.enabled) m_disabled_value = *m_value;

		if (ImGui::Checkbox(m_label.c_str(), m_core_config.enabled ? m_value.get() : &m_disabled_value))
			mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_core_config.name, m_core_config.callback_data);

	}

	void mvCheckbox::updateConfig(mvAppItemConfig* config)
	{
		auto aconfig = (mvCheckboxConfig*)config;

		m_core_config.width = config->width;
		m_core_config.height = config->height;
		m_core_config.label = config->label;
		m_core_config.show = config->show;
		m_core_config.callback = config->callback;
		m_core_config.callback_data = config->callback_data;
		m_core_config.enabled = config->enabled;

		m_config.source = aconfig->source;

		if (config != &m_config)
			m_config = *aconfig;
	}

	mvAppItemConfig* mvCheckbox::getConfig()
	{
		return &m_config;
	}

	PyObject* mvCheckbox::add_checkbox(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		std::string sname = std::string(std::string("$$DPG_") + s_internal_id + std::to_string(i));
		const char* name = sname.c_str();
		int default_value = 0;
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* before = "";
		const char* parent = "";
		const char* source = "";
		const char* label = "";
		int show = true;
		int enabled = true;

		if (!(mvApp::GetApp()->getParsers())["add_checkbox"].parse(args, kwargs, __FUNCTION__, &name,
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

		return ToPyString(name);
	}
}