#include <utility>
#include "mvCombo.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvCombo::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_combo", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::StringList, "items", "", "()"},
			{mvPythonDataType::String, "default_value", "", "''"},
			{mvPythonDataType::Callable, "callback", "Registers a callback", "None"},
			{mvPythonDataType::Object, "callback_data", "Callback data", "None"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::String, "source", "Overrides 'name' as value storage key", "''"},
			{mvPythonDataType::Bool, "enabled", "Display grayed out text so selectable cannot be selected", "True"},
			{mvPythonDataType::Integer, "width","", "0"},
			{mvPythonDataType::String, "label", "Overrides 'name' as label", "''"},
			{mvPythonDataType::Bool, "show","Attemp to render", "True"},
			{mvPythonDataType::Bool, "popup_align_left","Align the popup toward the left by default", "False"},
			{mvPythonDataType::Bool, "height_small","Max ~4 items visible", "False"},
			{mvPythonDataType::Bool, "height_regular","Max ~8 items visible (default)", "False"},
			{mvPythonDataType::Bool, "height_large","Max ~20 items visible", "False"},
			{mvPythonDataType::Bool, "height_largest","As many items visible as possible", "False"},
			{mvPythonDataType::Bool, "no_arrow_button","Display on the preview box without the square arrow button", "False"},
			{mvPythonDataType::Bool, "no_preview","Display only a square arrow button", "False"},
		}, "Adds a combo.", "None", "Adding Widgets") });
	}

	mvCombo::mvCombo(const std::string& name, const std::string& default_value, const std::string& dataSource)
		: 
		mvStringPtrBase(name, default_value)
	{
		m_description.disableAllowed = true;
	}

	mvCombo::mvCombo(const std::string& name, const mvComboConfig& config)
		:
		mvStringPtrBase(name, config.default_value),
		m_config(config)
	{
		m_description.disableAllowed = true;

		m_config.name = name;
		updateConfig(&m_config);
	}

	void mvCombo::draw()
	{

		ScopedID id;
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);

		static std::vector<std::string> disabled_items{};

		// The second parameter is the label previewed before opening the combo.
		if (ImGui::BeginCombo(m_label.c_str(), m_value->c_str(), m_flags)) 
		{
			for (const auto& name : m_core_config.enabled ? m_config.items : disabled_items)
			{
				bool is_selected = (*m_value == name);
				if (ImGui::Selectable((name).c_str(), is_selected))
				{
					if (m_core_config.enabled) { *m_value = name; }
					mvApp::GetApp()->getCallbackRegistry().addCallback(m_core_config.callback, m_core_config.name, m_core_config.callback_data);

				}

				// Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
				if (is_selected)
					ImGui::SetItemDefaultFocus();   
			}

			ImGui::EndCombo();
		}

	}

	void mvCombo::updateConfig(mvAppItemConfig* config)
	{
		auto aconfig = (mvComboConfig*)config;

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

	mvAppItemConfig* mvCombo::getConfig()
	{
		return &m_config;
	}

#ifdef MV_CPP
	void add_combo(const char* name, const mvComboConfig& config)
	{
		auto item = CreateRef<mvCombo>(name, config);

		mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, config.parent.c_str(), config.before.c_str());
	}

	void add_combo(const char* name, const std::vector<std::string>& items, mvCallable callable)
	{
		mvComboConfig config;
		config.items = items;
		config.callback = callable;
		add_combo(name, config);
	}
#else

	void mvCombo::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "items")) m_config.items = ToStringVect(item);

		// helpers for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		auto conflictingflagop = [dict](const char* const keywords[4], const int flags[4], int& mflags)
		{

			for (size_t i = 0; i < 4; i++)
			{
				if (PyObject* item = PyDict_GetItemString(dict, keywords[i]))
				{
					//turning all conflicting flags false
					for (int i = 0; i < 4; i++) mflags &= ~flags[i];
					//writing only the first conflicting flag
					ToBool(item) ? mflags |= flags[i] : mflags &= ~flags[i];
					break;
				}
			}

		};

		flagop("popup_align_left", ImGuiComboFlags_PopupAlignLeft, m_flags);
		flagop("no_arrow_button", ImGuiComboFlags_NoArrowButton, m_flags);
		flagop("no_preview", ImGuiComboFlags_NoPreview, m_flags);

		constexpr static const char* HeightKeywords[4]{
			"height_small",
			"height_regular",
			"height_large",
			"height_largest" };

		constexpr static const int HeightFlags[4]{
			ImGuiComboFlags_HeightSmall,
			ImGuiComboFlags_HeightRegular,
			ImGuiComboFlags_HeightLarge,
			ImGuiComboFlags_HeightLargest };

		conflictingflagop(HeightKeywords, HeightFlags, m_flags);

	}

	void mvCombo::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "items", ToPyList(m_config.items));

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
		};
		checkbitset("popup_align_left", ImGuiComboFlags_PopupAlignLeft, m_flags);
		checkbitset("height_small", ImGuiComboFlags_HeightSmall, m_flags);
		checkbitset("height_regular", ImGuiComboFlags_HeightRegular, m_flags);
		checkbitset("height_large", ImGuiComboFlags_HeightLarge, m_flags);
		checkbitset("height_largest", ImGuiComboFlags_HeightLargest, m_flags);
		checkbitset("no_arrow_button", ImGuiComboFlags_NoArrowButton, m_flags);
		checkbitset("no_preview", ImGuiComboFlags_NoPreview, m_flags);
	}

	PyObject* add_combo(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* default_value = "";
		PyObject* items;
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		int width = 0;
		const char* before = "";
		const char* parent = "";
		const char* source = "";
		int enabled = true;
		const char* label = "";
		int show = true;
		int popup_align_left = false;
		int height_small = false;
		int height_regular = false;
		int height_large = false;
		int height_largest = false;
		int no_arrow_button = false;
		int no_preview = false;


		if (!(mvApp::GetApp()->getParsers())["add_combo"].parse(args, kwargs, __FUNCTION__, &name, &items,
			&default_value, &callback, &callback_data, &parent, &before, &source, &enabled, &width,
			&label, &show, &popup_align_left, &height_small, &height_regular, &height_large,
			&height_largest, &no_arrow_button, &no_preview))
			return ToPyBool(false);

		auto item = CreateRef<mvCombo>(name, default_value, source);
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