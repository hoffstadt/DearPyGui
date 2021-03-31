#include "mvColorButton.h"
#include "mvApp.h"
#include <array>
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvColorButton::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_color_button", mvPythonParser({
			{mvPythonDataType::FloatList, "color"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "name"},		
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Callable, "callback", "Registers a callback", "None"},
			{mvPythonDataType::Object, "callback_data", "Callback data", "None"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Integer, "width","", "0"},
			{mvPythonDataType::Integer, "height", "", "0"},
			{mvPythonDataType::Bool, "show", "Attemp to render", "True"},
			{mvPythonDataType::Bool, "no_alpha", "ignore Alpha component", "False"},
			{mvPythonDataType::Bool, "no_border", "disable border (which is enforced by default)", "False"},
			{mvPythonDataType::Bool, "no_drag_drop", "disable display of inline text label", "False"},
			{mvPythonDataType::Bool, "enabled", "", "True"},
		}, "Adds a color button.", "None", "Adding Widgets") });
	}

	mvColorButton::mvColorButton(const std::string& name, const mvColor& color)
		: 
		mvAppItem(name),
		m_color(color.toVec4())
	{
		m_description.disableAllowed = true;
		m_config.color = color;
	}

	mvColorButton::mvColorButton(const std::string& name, const mvColorButtonConfig& config)
		: 
		mvAppItem(name),
		m_color(config.color.toVec4()),
		m_config(config)
	{
		m_description.disableAllowed = true;
		m_config.name = name;
		updateConfig(&m_config);
	}

	void mvColorButton::draw()
	{
		ScopedID id;
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);

		if (ImGui::ColorButton(m_label.c_str(), m_color, m_flags, ImVec2((float)m_core_config.width, (float)m_core_config.height)))
			mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_core_config.name, m_core_config.callback_data);

	}

	void mvColorButton::updateConfig(mvAppItemConfig* config)
	{
		auto aconfig = (mvColorButtonConfig*)config;

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

	mvAppItemConfig* mvColorButton::getConfig()
	{
		return &m_config;
	}

	void mvColorButton::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "color")) m_color = ToColor(item).toVec4();

		// helpers for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};
		flagop("no_alpha", ImGuiColorEditFlags_NoAlpha, m_flags);
		flagop("no_border", ImGuiColorEditFlags_NoBorder, m_flags);
		flagop("no_drag_drop", ImGuiColorEditFlags_NoDragDrop, m_flags);
	}

	void mvColorButton::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "color", ToPyColor({ (int)m_color.x * 255, (int)m_color.y * 255, (int)m_color.z * 255, (int)m_color.w * 255 }));

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
		};

		checkbitset("no_alpha", ImGuiColorEditFlags_NoAlpha, m_flags);
		checkbitset("no_border", ImGuiColorEditFlags_NoBorder, m_flags);
		checkbitset("no_drag_drop", ImGuiColorEditFlags_NoDragDrop, m_flags);
	}

	PyObject* mvColorButton::add_color_button(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		std::string sname = std::string(std::string("$$DPG_") + s_internal_id + std::to_string(i));
		const char* name = sname.c_str();
		PyObject* color;
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* parent = "";
		const char* before = "";
		int width = 0;
		int height = 0;
		int show = true;
		int no_alpha = false;
		int no_border = false;
		int no_drag_drop = false;
		int enabled = true;

		if (!(mvApp::GetApp()->getParsers())["add_color_button"].parse(args, kwargs, __FUNCTION__,
			&color, &name, &callback, &callback_data, &parent, &before, &width, &height,
			&show, &no_alpha, &no_border, &no_drag_drop, &enabled))
			return ToPyBool(false);

		auto item = CreateRef<mvColorButton>(name, ToColor(color));
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