#include <utility>
#include "mvButton.h"
#include "mvCore.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvButton::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_button", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "small", "Small button, useful for embedding in text.", "False"},
			{mvPythonDataType::Bool, "arrow", "Arrow button, must use with direction", "False"},
			{mvPythonDataType::Integer, "direction", "A cardinal direction", "0"},
			{mvPythonDataType::Callable, "callback", "Registers a callback", "None"},
			{mvPythonDataType::Object, "callback_data", "Callback data", "None"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Integer, "width","", "0"},
			{mvPythonDataType::Integer, "height", "", "0"},
			{mvPythonDataType::String, "label", "Overrides 'name' as label", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::Bool, "enabled", "", "True"},
		}, "Adds a button.", "None", "Adding Widgets") });
	}

	mvButton::mvButton(const std::string& name)
		: 
		mvAppItem(name)
	{
		m_description.disableAllowed = true;
	}

	mvButton::mvButton(const std::string& name, const mvButtonConfig& config)
		:
		mvAppItem(name), 
		m_config(config)
	{
		m_description.disableAllowed = true;

		m_config.name = name;
		updateConfig(&m_config);
	}

	void mvButton::updateConfig(mvAppItemConfig* config)
	{
		auto aconfig = (mvButtonConfig*)config;

		m_core_config.width = config->width;
		m_core_config.height = config->height;
		m_core_config.label = config->label;
		m_core_config.show = config->show;
		m_core_config.callback = config->callback;
		m_core_config.callback_data = config->callback_data;
		m_core_config.enabled = config->enabled;
		
		m_config.arrow = aconfig->arrow;
		m_config.direction = aconfig->direction;
		m_config.small_button = aconfig->small_button;

		if (config != &m_config)
			m_config = *aconfig;
	}

	mvAppItemConfig* mvButton::getConfig()
	{
		return &m_config;
	}

	void mvButton::draw()
	{
		ScopedID id;
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);

		if (m_config.small_button)
		{
			if (ImGui::SmallButton(m_label.c_str()))
				mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_core_config.name, m_core_config.callback_data);
			return;
		}

		if (m_config.arrow)
		{
			if (ImGui::ArrowButton(m_label.c_str(), m_config.direction))
				mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_core_config.name, m_core_config.callback_data);

			return;
		}

		if (ImGui::Button(m_label.c_str(), ImVec2((float)m_core_config.width, (float)m_core_config.height)))
			mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_core_config.name, m_core_config.callback_data);

	}

#ifdef MV_CPP

	void add_button(const char* name, const mvButtonConfig& config)
	{
		auto item = CreateRef<mvButton>(name, config);

		mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, config.parent.c_str(), config.before.c_str());
	}

	void add_button(const char* name, mvCallable callable)
	{
		mvButtonConfig config;
		config.callback = callable;
		add_button(name, config);
	}

#else

	void mvButton::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "small")) m_config.small_button = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "arrow")) m_config.arrow = ToBool(item);

		if (PyObject* item = PyDict_GetItemString(dict, "direction"))
		{
			m_config.direction = ToInt(item);
			DecodelibID(m_config.direction, &m_config.direction);
		}

	}

	void mvButton::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "small", ToPyBool(m_config.small_button));
		PyDict_SetItemString(dict, "arrow", ToPyBool(m_config.arrow));
		PyDict_SetItemString(dict, "direction", ToPyInt(MV_ENCODE_CONSTANT(m_config.direction, 0)));
	}


	PyObject* add_button(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int smallb = false;
		int arrow = false;
		int direction = 4000;
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		int width = 0;
		int height = 0;
		const char* before = "";
		const char* parent = "";
		const char* label = "";
		int show = true;
		int enabled = true;

		if (!(mvApp::GetApp()->getParsers())["add_button"].parse(args, kwargs, __FUNCTION__, &name, &smallb,
			&arrow, &direction, &callback, &callback_data, &parent, &before, &width, &height,
			&label, &show, &enabled))
			return GetPyNone();

		auto item = CreateRef<mvButton>(name);
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