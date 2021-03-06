#include "mvColorPicker.h"
#include "mvApp.h"
#include <array>
#include "mvItemRegistry.h"
#include "mvThemeManager.h"

namespace Marvel {

	void mvColorPicker3::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		parsers->insert({ "add_color_picker3", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value", "", "(0, 0, 0, 255)"},
			{mvPythonDataType::Callable, "callback", "Registers a callback", "None"},
			{mvPythonDataType::Object, "callback_data", "Callback data", "None"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::String, "source", "", "''"},
			{mvPythonDataType::Integer, "width","", "0"},
			{mvPythonDataType::Integer, "height", "", "0"},
			{mvPythonDataType::String, "label", "", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::Bool, "no_alpha", "ignore Alpha component", "False"},
			{mvPythonDataType::Bool, "no_small_preview", "disable colored square preview next to the inputs. (e.g. to show only the inputs). This only displays if the side preview is not shown.", "False"},
			{mvPythonDataType::Bool, "no_inputs", "disable inputs sliders/text widgets (e.g. to show only the small preview colored square)", "False"},
			{mvPythonDataType::Bool, "no_tooltip", "disable tooltip when hovering the preview.", "False"},
			{mvPythonDataType::Bool, "no_label", "disable display of inline text label", "False"},
			{mvPythonDataType::Bool, "no_side_preview", "disable bigger color preview on right side of the picker, use small colored square preview instead , unless small preview is also hidden", "False"},
			{mvPythonDataType::Bool, "alpha_bar", "show vertical alpha bar/gradient in picker.", "False"},
			{mvPythonDataType::Bool, "alpha_preview", "display preview as a transparent color over a checkerboard, instead of opaque.", "False"},
			{mvPythonDataType::Bool, "alpha_preview_half", "display half opaque / half checkerboard, instead of opaque.", "False"},
			{mvPythonDataType::Bool, "display_rgb", "override _display_ type among RGB/HSV/Hex.", "False"},
			{mvPythonDataType::Bool, "display_hsv", "override _display_ type among RGB/HSV/Hex.", "False"},
			{mvPythonDataType::Bool, "display_hex", "override _display_ type among RGB/HSV/Hex.", "False"},
			{mvPythonDataType::Bool, "uint8", "display values formatted as 0..255", "False"},
			{mvPythonDataType::Bool, "floats", "display values formatted as 0.0f..1.0f floats instead of 0..255 integers.", "False"},
			{mvPythonDataType::Bool, "picker_hue_bar", "bar for Hue, rectangle for Sat/Value.", "False"},
			{mvPythonDataType::Bool, "picker_hue_wheel", "wheel for Hue, triangle for Sat/Value.", "False"},
			{mvPythonDataType::Bool, "input_rgb", "input and output data in RGB format.", "False"},
			{mvPythonDataType::Bool, "input_hsv", "input and output data in HSV format.", "False"},
		}, "Adds an rgb color picking widget. Click and draging the color square will copy the color to be applied on any other color widget. Right Click allows the style of the color picker to be changed.", "None", "Adding Widgets") });
	}

	void mvColorPicker4::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		parsers->insert({ "add_color_picker4", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value", "", "(0, 0, 0, 255)"},
			{mvPythonDataType::Callable, "callback", "Registers a callback", "None"},
			{mvPythonDataType::Object, "callback_data", "Callback data", "None"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::String, "source", "", "''"},
			{mvPythonDataType::Integer, "width", "", "0"},
			{mvPythonDataType::Integer, "height", "", "0"},
			{mvPythonDataType::String, "label", "", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::Bool, "no_alpha", "ignore Alpha component", "False"},
			{mvPythonDataType::Bool, "no_small_preview", "disable colored square preview next to the inputs. (e.g. to show only the inputs). This only displays if the side preview is not shown.", "False"},
			{mvPythonDataType::Bool, "no_inputs", "disable inputs sliders/text widgets (e.g. to show only the small preview colored square)", "False"},
			{mvPythonDataType::Bool, "no_tooltip", "disable tooltip when hovering the preview.", "False"},
			{mvPythonDataType::Bool, "no_label", "disable display of inline text label", "False"},
			{mvPythonDataType::Bool, "no_side_preview", "disable bigger color preview on right side of the picker, use small colored square preview instead , unless small preview is also hidden", "False"},
			{mvPythonDataType::Bool, "alpha_bar", "show vertical alpha bar/gradient in picker.", "False"},
			{mvPythonDataType::Bool, "alpha_preview", "display preview as a transparent color over a checkerboard, instead of opaque.", "False"},
			{mvPythonDataType::Bool, "alpha_preview_half", "display half opaque / half checkerboard, instead of opaque.", "False"},
			{mvPythonDataType::Bool, "display_rgb", "override _display_ type among RGB/HSV/Hex.", "False"},
			{mvPythonDataType::Bool, "display_hsv", "override _display_ type among RGB/HSV/Hex.", "False"},
			{mvPythonDataType::Bool, "display_hex", "override _display_ type among RGB/HSV/Hex.", "False"},
			{mvPythonDataType::Bool, "uint8", "display values formatted as 0..255", "False"},
			{mvPythonDataType::Bool, "floats", "display values formatted as 0.0f..1.0f floats instead of 0..255 integers.", "False"},
			{mvPythonDataType::Bool, "picker_hue_bar", "bar for Hue, rectangle for Sat/Value.", "False"},
			{mvPythonDataType::Bool, "picker_hue_wheel", "wheel for Hue, triangle for Sat/Value.", "False"},
			{mvPythonDataType::Bool, "input_rgb", "input and output data in RGB format.", "False"},
			{mvPythonDataType::Bool, "input_hsv", "input and output data in HSV format.", "False"},
		}, "Adds an rgba color picking widget. Click and draging the color square will copy the color to be applied on any other color widget. Right Click allows the style of the color picker to be changed", "None", "Adding Widgets") });
	}

	mvColorPicker3::mvColorPicker3(const std::string& name, float* color, const std::string& dataSource)
		: 
		mvColorPtrBase(name, color)
	{
		m_config = {};
	}

	mvColorPicker3::mvColorPicker3(const std::string& name, const mvColorPickerConfig& config)
		:
		mvColorPtrBase(name, config.default_value.data()),
		m_config(config)
	{
		m_config.name = name;
		updateConfig(&m_config);
	}

	void mvColorPicker3::updateConfig(mvAppItemConfig* config)
	{
		auto aconfig = (mvColorPickerConfig*)config;

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

	mvAppItemConfig* mvColorPicker3::getConfig()
	{
		return &m_config;
	}

	void mvColorPicker3::draw()
	{
		ScopedID id;
		mvImGuiThemeScope scope(this);

		if (ImGui::ColorPicker3(m_label.c_str(), m_value->data(), m_flags))
			mvApp::GetApp()->getCallbackRegistry().addCallback(m_core_config.callback, m_core_config.name, m_core_config.callback_data);

	}

	mvColorPicker4::mvColorPicker4(const std::string& name, float* color, const std::string& dataSource)
		: 
		mvColorPtrBase(name, color)
	{
		m_config = {};
	}

	mvColorPicker4::mvColorPicker4(const std::string& name, const mvColorPickerConfig& config)
		:
		mvColorPtrBase(name, config.default_value.data()),
		m_config(config)
	{
		m_config.name = name;
		updateConfig(&m_config);
	}

	void mvColorPicker4::updateConfig(mvAppItemConfig* config)
	{
		auto aconfig = (mvColorPickerConfig*)config;

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

	mvAppItemConfig* mvColorPicker4::getConfig()
	{
		return &m_config;
	}

	void mvColorPicker4::draw()
	{
		ScopedID id;
		mvImGuiThemeScope scope(this);

		if (ImGui::ColorPicker4(m_label.c_str(), m_value->data(), m_flags))
			mvApp::GetApp()->getCallbackRegistry().addCallback(m_core_config.callback, m_core_config.name, m_core_config.callback_data);

	}

#ifdef MV_CPP
	void add_color_picker3(const std::string& name, const mvColorPickerConfig& config)
	{
		auto item = CreateRef<mvColorPicker3>(name, config);
		mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, config.parent.c_str(), config.before.c_str());
	}

	void add_color_picker4(const std::string& name, const mvColorPickerConfig& config)
	{
		auto item = CreateRef<mvColorPicker4>(name, config);
		mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, config.parent.c_str(), config.before.c_str());
	}
#else

	void mvColorPicker3::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		// helpers for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		auto conflictingflagop = [dict](const std::vector<std::string>& keywords, std::vector<int> flags, int& mflags)
		{

			for (size_t i = 0; i < keywords.size(); i++)
			{
				if (PyObject* item = PyDict_GetItemString(dict, keywords[i].c_str()))
				{
					//turning all conflicting flags false
					for (const auto& flag : flags) mflags &= ~flag;
					//writing only the first conflicting flag
					ToBool(item) ? mflags |= flags[i] : mflags &= ~flags[i];
					break;
				}
			}

		};

		flagop("no_alpha", ImGuiColorEditFlags_NoAlpha, m_flags);
		flagop("no_small_preview", ImGuiColorEditFlags_NoSmallPreview, m_flags);
		flagop("no_inputs", ImGuiColorEditFlags_NoInputs, m_flags);
		flagop("no_tooltip", ImGuiColorEditFlags_NoTooltip, m_flags);
		flagop("no_label", ImGuiColorEditFlags_NoLabel, m_flags);
		flagop("no_side_preview", ImGuiColorEditFlags_NoSidePreview, m_flags);
		flagop("alpha_bar", ImGuiColorEditFlags_AlphaBar, m_flags);
		flagop("display_rgb", ImGuiColorEditFlags_DisplayRGB, m_flags);
		flagop("display_hsv", ImGuiColorEditFlags_DisplayHSV, m_flags);
		flagop("display_hex", ImGuiColorEditFlags_DisplayHex, m_flags);

		static std::vector<std::string> AlphaPreviewKeywords{ "alpha_preview", "alpha_preview_half" };
		static std::vector<int> AlphaPreviewFlags{ ImGuiColorEditFlags_AlphaPreview, ImGuiColorEditFlags_AlphaPreviewHalf };

		static std::vector<std::string> DisplayValueTypeKeywords{ "uint8", "floats" };
		static std::vector<int> DisplayValueTypeFlags{ ImGuiColorEditFlags_Uint8, ImGuiColorEditFlags_Float };

		static std::vector<std::string> PickerTypeKeywords{ "picker_hue_bar", "picker_hue_wheel" };
		static std::vector<int> PickerTypeFlags{ ImGuiColorEditFlags_PickerHueBar, ImGuiColorEditFlags_PickerHueWheel };

		static std::vector<std::string> IOTypeKeywords{ "input_rgb", "input_hsv" };
		static std::vector<int> IOTypeFlags{ ImGuiColorEditFlags_InputRGB, ImGuiColorEditFlags_InputHSV };

		conflictingflagop(AlphaPreviewKeywords, AlphaPreviewFlags, m_flags);
		conflictingflagop(DisplayValueTypeKeywords, DisplayValueTypeFlags, m_flags);
		conflictingflagop(PickerTypeKeywords, PickerTypeFlags, m_flags);
		conflictingflagop(IOTypeKeywords, IOTypeFlags, m_flags);

	}

	void mvColorPicker3::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
		};

		checkbitset("no_alpha", ImGuiColorEditFlags_NoAlpha, m_flags);
		checkbitset("no_small_preview", ImGuiColorEditFlags_NoSmallPreview, m_flags);
		checkbitset("no_inputs", ImGuiColorEditFlags_NoInputs, m_flags);
		checkbitset("no_tooltip", ImGuiColorEditFlags_NoTooltip, m_flags);
		checkbitset("no_label", ImGuiColorEditFlags_NoLabel, m_flags);
		checkbitset("no_side_preview", ImGuiColorEditFlags_NoSidePreview, m_flags);
		checkbitset("alpha_bar", ImGuiColorEditFlags_AlphaBar, m_flags);
		checkbitset("alpha_preview", ImGuiColorEditFlags_AlphaPreview, m_flags);
		checkbitset("alpha_preview_half", ImGuiColorEditFlags_AlphaPreviewHalf, m_flags);
		checkbitset("display_rgb", ImGuiColorEditFlags_DisplayRGB, m_flags);
		checkbitset("display_hsv", ImGuiColorEditFlags_DisplayHSV, m_flags);
		checkbitset("display_hex", ImGuiColorEditFlags_DisplayHex, m_flags);
		checkbitset("unit8", ImGuiColorEditFlags_Uint8, m_flags);
		checkbitset("floats", ImGuiColorEditFlags_Float, m_flags);
		checkbitset("picker_hue_bar", ImGuiColorEditFlags_PickerHueBar, m_flags);
		checkbitset("picker_hue_wheel", ImGuiColorEditFlags_PickerHueWheel, m_flags);
		checkbitset("input_rgb", ImGuiColorEditFlags_InputRGB, m_flags);
		checkbitset("input_hsv", ImGuiColorEditFlags_InputHSV, m_flags);
	}

	PyObject* add_color_picker3(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(255));
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		int width = 0;
		int height = 0;
		const char* before = "";
		const char* parent = "";
		const char* source = "";
		const char* label = "";
		int show = true;
		int no_alpha = false;
		int no_small_preview = false;
		int no_inputs = false;
		int no_tooltip = false;
		int no_label = false;
		int no_side_preview = false;
		int alpha_bar = false;
		int alpha_preview = false;
		int alpha_preview_half = false;
		int display_rgb = false;
		int display_hsv = false;
		int display_hex = false;
		int uint8 = false;
		int floats = false;
		int picker_hue_bar = false;
		int picker_hue_wheel = false;
		int input_rgb = false;
		int input_hsv = false;


		if (!(mvApp::GetApp()->getParsers())["add_color_picker3"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&callback, &callback_data, &parent, &before, &source, &width, &height, &label, &show,
			&no_alpha, &no_small_preview, &no_inputs, &no_tooltip, &no_label, &no_side_preview, &alpha_bar,
			&alpha_preview, &alpha_preview_half, &display_rgb, &display_hsv, &display_hex, &uint8, &floats, &picker_hue_bar,
			&picker_hue_wheel, &input_rgb, &input_hsv))
			return ToPyBool(false);

		auto color = ToColor(default_value);
		ImVec4 v = color.toVec4();
		auto item = CreateRef<mvColorPicker3>(name, &v.x, source);
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

	void mvColorPicker4::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		// helpers for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		auto conflictingflagop = [dict](const std::vector<std::string>& keywords, std::vector<int> flags, int& mflags)
		{

			for (size_t i = 0; i < keywords.size(); i++)
			{
				if (PyObject* item = PyDict_GetItemString(dict, keywords[i].c_str()))
				{
					//turning all conflicting flags false
					for (const auto& flag : flags) mflags &= ~flag;
					//writing only the first conflicting flag
					ToBool(item) ? mflags |= flags[i] : mflags &= ~flags[i];
					break;
				}
			}

		};

		flagop("no_alpha", ImGuiColorEditFlags_NoAlpha, m_flags);
		flagop("no_small_preview", ImGuiColorEditFlags_NoSmallPreview, m_flags);
		flagop("no_inputs", ImGuiColorEditFlags_NoInputs, m_flags);
		flagop("no_tooltip", ImGuiColorEditFlags_NoTooltip, m_flags);
		flagop("no_label", ImGuiColorEditFlags_NoLabel, m_flags);
		flagop("no_side_preview", ImGuiColorEditFlags_NoSidePreview, m_flags);
		flagop("alpha_bar", ImGuiColorEditFlags_AlphaBar, m_flags);
		flagop("display_rgb", ImGuiColorEditFlags_DisplayRGB, m_flags);
		flagop("display_hsv", ImGuiColorEditFlags_DisplayHSV, m_flags);
		flagop("display_hex", ImGuiColorEditFlags_DisplayHex, m_flags);

		static std::vector<std::string> AlphaPreviewKeywords{ "alpha_preview", "alpha_preview_half" };
		static std::vector<int> AlphaPreviewFlags{ ImGuiColorEditFlags_AlphaPreview, ImGuiColorEditFlags_AlphaPreviewHalf };

		static std::vector<std::string> DisplayValueTypeKeywords{ "uint8", "floats" };
		static std::vector<int> DisplayValueTypeFlags{ ImGuiColorEditFlags_Uint8, ImGuiColorEditFlags_Float };

		static std::vector<std::string> PickerTypeKeywords{ "picker_hue_bar", "picker_hue_wheel" };
		static std::vector<int> PickerTypeFlags{ ImGuiColorEditFlags_PickerHueBar, ImGuiColorEditFlags_PickerHueWheel };

		static std::vector<std::string> IOTypeKeywords{ "input_rgb", "input_hsv" };
		static std::vector<int> IOTypeFlags{ ImGuiColorEditFlags_InputRGB, ImGuiColorEditFlags_InputHSV };

		conflictingflagop(AlphaPreviewKeywords, AlphaPreviewFlags, m_flags);
		conflictingflagop(DisplayValueTypeKeywords, DisplayValueTypeFlags, m_flags);
		conflictingflagop(PickerTypeKeywords, PickerTypeFlags, m_flags);
		conflictingflagop(IOTypeKeywords, IOTypeFlags, m_flags);

	}

	void mvColorPicker4::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
		};

		checkbitset("no_alpha", ImGuiColorEditFlags_NoAlpha, m_flags);
		checkbitset("no_small_preview", ImGuiColorEditFlags_NoSmallPreview, m_flags);
		checkbitset("no_inputs", ImGuiColorEditFlags_NoInputs, m_flags);
		checkbitset("no_tooltip", ImGuiColorEditFlags_NoTooltip, m_flags);
		checkbitset("no_label", ImGuiColorEditFlags_NoLabel, m_flags);
		checkbitset("no_side_preview", ImGuiColorEditFlags_NoSidePreview, m_flags);
		checkbitset("alpha_bar", ImGuiColorEditFlags_AlphaBar, m_flags);
		checkbitset("alpha_preview", ImGuiColorEditFlags_AlphaPreview, m_flags);
		checkbitset("alpha_preview_half", ImGuiColorEditFlags_AlphaPreviewHalf, m_flags);
		checkbitset("display_rgb", ImGuiColorEditFlags_DisplayRGB, m_flags);
		checkbitset("display_hsv", ImGuiColorEditFlags_DisplayHSV, m_flags);
		checkbitset("display_hex", ImGuiColorEditFlags_DisplayHex, m_flags);
		checkbitset("unit8", ImGuiColorEditFlags_Uint8, m_flags);
		checkbitset("floats", ImGuiColorEditFlags_Float, m_flags);
		checkbitset("picker_hue_bar", ImGuiColorEditFlags_PickerHueBar, m_flags);
		checkbitset("picker_hue_wheel", ImGuiColorEditFlags_PickerHueWheel, m_flags);
		checkbitset("input_rgb", ImGuiColorEditFlags_InputRGB, m_flags);
		checkbitset("input_hsv", ImGuiColorEditFlags_InputHSV, m_flags);
	}

	PyObject* add_color_picker4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 3, PyLong_FromLong(255));
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		int width = 0;
		int height = 0;
		const char* before = "";
		const char* parent = "";
		const char* source = "";
		const char* label = "";
		int show = true;
		int no_alpha = false;
		int no_small_preview = false;
		int no_inputs = false;
		int no_tooltip = false;
		int no_label = false;
		int no_side_preview = false;
		int alpha_bar = false;
		int alpha_preview = false;
		int alpha_preview_half = false;
		int display_rgb = false;
		int display_hsv = false;
		int display_hex = false;
		int uint8 = false;
		int floats = false;
		int picker_hue_bar = false;
		int picker_hue_wheel = false;
		int input_rgb = false;
		int input_hsv = false;

		if (!(mvApp::GetApp()->getParsers())["add_color_picker4"].parse(args, kwargs, __FUNCTION__, &name,
			&default_value, &callback, &callback_data, &parent, &before, &source, &width, &height,
			&label, &show, &no_alpha, &no_small_preview, &no_inputs, &no_tooltip, &no_label, &no_side_preview, &alpha_bar,
			&alpha_preview, &alpha_preview_half, &display_rgb, &display_hsv, &display_hex, &uint8, &floats, &picker_hue_bar,
			&picker_hue_wheel, &input_rgb, &input_hsv))
			return ToPyBool(false);

		auto color = ToColor(default_value);
		ImVec4 v = color.toVec4();
		auto item = CreateRef<mvColorPicker4>(name, &v.x, source);
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

#endif // !MV_CPP
}