#include "mvColorEdit.h"
#include "mvApp.h"
#include <array>
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvColorEdit3::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		parsers->insert({ s_command, mvPythonParser({
			{mvPythonDataType::Optional},
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
			{mvPythonDataType::Bool, "enabled", "", "True"},
			{mvPythonDataType::Bool, "no_alpha", "ignore Alpha component", "False"},
			{mvPythonDataType::Bool, "no_picker", "disable picker when clicking on colored square.", "False"},
			{mvPythonDataType::Bool, "no_options", " disable toggling options menu when right-clicking on inputs/small preview.", "False"},
			{mvPythonDataType::Bool, "no_small_preview", "disable colored square preview next to the inputs. For color pickers this only shows if the side preview is not shown.", "False"},
			{mvPythonDataType::Bool, "no_inputs", "disable inputs sliders/text widgets (e.g. to show only the small preview colored square)", "False"},
			{mvPythonDataType::Bool, "no_tooltip", "disable tooltip when hovering the preview.", "False"},
			{mvPythonDataType::Bool, "no_label", "disable display of inline text label", "False"},
			{mvPythonDataType::Bool, "no_drag_drop", "disable display of inline text label", "False"},
			{mvPythonDataType::Bool, "alpha_bar", "show vertical alpha bar/gradient in picker.", "False"},
			{mvPythonDataType::Bool, "alpha_preview", "display preview as a transparent color over a checkerboard, instead of opaque.", "False"},
			{mvPythonDataType::Bool, "alpha_preview_half", "display half opaque / half checkerboard, instead of opaque.", "False"},
			{mvPythonDataType::Bool, "display_rgb", "override _display_ type among RGB/HSV/Hex.", "False"},
			{mvPythonDataType::Bool, "display_hsv", "override _display_ type among RGB/HSV/Hex.", "False"},
			{mvPythonDataType::Bool, "display_hex", "override _display_ type among RGB/HSV/Hex.", "False"},
			{mvPythonDataType::Bool, "uint8", "display values formatted as 0..255", "False"},
			{mvPythonDataType::Bool, "floats", "display values formatted as 0.0f..1.0f floats instead of 0..255 integers.", "False"},
			{mvPythonDataType::Bool, "input_rgb", "input and output data in RGB format.", "False"},
			{mvPythonDataType::Bool, "input_hsv", "input and output data in HSV format.", "False"},

		}, "Adds an rgb color editing widget. Click and draging the color square will copy the color to be applied on any other color widget.", "None", "Adding Widgets") });
	}

	void mvColorEdit4::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ s_command, mvPythonParser({
			{mvPythonDataType::Optional},
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
			{mvPythonDataType::Bool, "enabled", "", "True"},
			{mvPythonDataType::Bool, "no_alpha", "ignore Alpha component", "False"},
			{mvPythonDataType::Bool, "no_picker", "disable picker when clicking on colored square.", "False"},
			{mvPythonDataType::Bool, "no_options", " disable toggling options menu when right-clicking on inputs/small preview.", "False"},
			{mvPythonDataType::Bool, "no_small_preview", "disable colored square preview next to the inputs. (e.g. to show only the inputs). This only displays if the side preview is not shown.", "False"},
			{mvPythonDataType::Bool, "no_inputs", "disable inputs sliders/text widgets (e.g. to show only the small preview colored square)", "False"},
			{mvPythonDataType::Bool, "no_tooltip", "disable tooltip when hovering the preview.", "False"},
			{mvPythonDataType::Bool, "no_label", "disable display of inline text label", "False"},
			{mvPythonDataType::Bool, "no_drag_drop", "disable display of inline text label", "False"},
			{mvPythonDataType::Bool, "alpha_bar", "show vertical alpha bar/gradient in picker.", "False"},
			{mvPythonDataType::Bool, "alpha_preview", "display preview as a transparent color over a checkerboard, instead of opaque.", "False"},
			{mvPythonDataType::Bool, "alpha_preview_half", "display half opaque / half checkerboard, instead of opaque.", "False"},
			{mvPythonDataType::Bool, "display_rgb", "override _display_ type among RGB/HSV/Hex.", "False"},
			{mvPythonDataType::Bool, "display_hsv", "override _display_ type among RGB/HSV/Hex.", "False"},
			{mvPythonDataType::Bool, "display_hex", "override _display_ type among RGB/HSV/Hex.", "False"},
			{mvPythonDataType::Bool, "uint8", "display values formatted as 0..255", "False"},
			{mvPythonDataType::Bool, "floats", "display values formatted as 0.0f..1.0f floats instead of 0..255 integers.", "False"},
			{mvPythonDataType::Bool, "input_rgb", "input and output data in RGB format.", "False"},
			{mvPythonDataType::Bool, "input_hsv", "input and output data in HSV format.", "False"},

		}, "Adds an rgba color editing widget. Click and draging the color square will copy the color to be applied on any other color widget.", "None", "Adding Widgets") });
	}

	mvColorEdit3::mvColorEdit3(const std::string& name)
		: 
		mvColorPtrBase(name)
	{
	}

	void mvColorEdit3::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id;
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);

		if (!m_enabled) std::copy(m_value->data(), m_value->data() + 3, m_disabled_value);

		if (ImGui::ColorEdit3(m_label.c_str(), m_enabled ? m_value->data() : &m_disabled_value[0], m_flags))
			mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callback_data);
	}

	mvColorEdit4::mvColorEdit4(const std::string& name)
		: 
		mvColorPtrBase(name)
	{
	}

	void mvColorEdit4::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id;
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);

		if (!m_enabled) std::copy(m_value->data(), m_value->data() + 4, m_disabled_value);

		if (ImGui::ColorEdit4(m_label.c_str(), m_enabled ? m_value->data() : &m_disabled_value[0], m_flags))
			mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callback_data);

	}

	void mvColorEdit3::setExtraConfigDict(PyObject* dict)
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
		flagop("no_picker", ImGuiColorEditFlags_NoPicker, m_flags);
		flagop("no_options", ImGuiColorEditFlags_NoOptions, m_flags);
		flagop("no_small_preview", ImGuiColorEditFlags_NoSmallPreview, m_flags);
		flagop("no_inputs", ImGuiColorEditFlags_NoInputs, m_flags);
		flagop("no_tooltip", ImGuiColorEditFlags_NoTooltip, m_flags);
		flagop("no_label", ImGuiColorEditFlags_NoLabel, m_flags);
		flagop("no_drag_drop", ImGuiColorEditFlags_NoDragDrop, m_flags);
		flagop("alpha_bar", ImGuiColorEditFlags_AlphaBar, m_flags);

		static std::vector<std::string> AlphaPreviewKeywords{ "alpha_preview", "alpha_preview_half" };
		static std::vector<int> AlphaPreviewFlags{ ImGuiColorEditFlags_AlphaPreview, ImGuiColorEditFlags_AlphaPreviewHalf };

		static std::vector<std::string> DisplayValueTypeKeywords{ "uint8", "floats" };
		static std::vector<int> DisplayValueTypeFlags{ ImGuiColorEditFlags_Uint8, ImGuiColorEditFlags_Float };

		std::vector<std::string> DisplayTypeKeywords{ "display_rgb", "display_hsv", "display_hex" };
		std::vector<int> DisplayTypeFlags{ ImGuiColorEditFlags_DisplayRGB, ImGuiColorEditFlags_DisplayHSV, ImGuiColorEditFlags_DisplayHex };

		static std::vector<std::string> IOTypeKeywords{ "input_rgb", "input_hsv" };
		static std::vector<int> IOTypeFlags{ ImGuiColorEditFlags_InputRGB, ImGuiColorEditFlags_InputHSV };

		conflictingflagop(AlphaPreviewKeywords, AlphaPreviewFlags, m_flags);
		conflictingflagop(DisplayValueTypeKeywords, DisplayValueTypeFlags, m_flags);
		conflictingflagop(DisplayTypeKeywords, DisplayTypeFlags, m_flags);
		conflictingflagop(IOTypeKeywords, IOTypeFlags, m_flags);

	}

	void mvColorEdit3::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
		};

		checkbitset("no_alpha", ImGuiColorEditFlags_NoAlpha, m_flags);
		checkbitset("no_picker", ImGuiColorEditFlags_NoPicker, m_flags);
		checkbitset("no_options", ImGuiColorEditFlags_NoOptions, m_flags);
		checkbitset("no_small_preview", ImGuiColorEditFlags_NoSmallPreview, m_flags);
		checkbitset("no_inputs", ImGuiColorEditFlags_NoInputs, m_flags);
		checkbitset("no_tooltip", ImGuiColorEditFlags_NoTooltip, m_flags);
		checkbitset("no_label", ImGuiColorEditFlags_NoLabel, m_flags);
		checkbitset("no_drag_drop", ImGuiColorEditFlags_NoDragDrop, m_flags);
		checkbitset("alpha_bar", ImGuiColorEditFlags_AlphaBar, m_flags);
		checkbitset("alpha_preview", ImGuiColorEditFlags_AlphaPreview, m_flags);
		checkbitset("alpha_preview_half", ImGuiColorEditFlags_AlphaPreviewHalf, m_flags);
		checkbitset("display_rgb", ImGuiColorEditFlags_DisplayRGB, m_flags);
		checkbitset("display_hsv", ImGuiColorEditFlags_DisplayHSV, m_flags);
		checkbitset("display_hex", ImGuiColorEditFlags_DisplayHex, m_flags);
		checkbitset("unit8", ImGuiColorEditFlags_Uint8, m_flags);
		checkbitset("floats", ImGuiColorEditFlags_Float, m_flags);
		checkbitset("input_rgb", ImGuiColorEditFlags_InputRGB, m_flags);
		checkbitset("input_hsv", ImGuiColorEditFlags_InputHSV, m_flags);
	}

	void mvColorEdit4::setExtraConfigDict(PyObject* dict)
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
		flagop("no_picker", ImGuiColorEditFlags_NoPicker, m_flags);
		flagop("no_options", ImGuiColorEditFlags_NoOptions, m_flags);
		flagop("no_small_preview", ImGuiColorEditFlags_NoSmallPreview, m_flags);
		flagop("no_inputs", ImGuiColorEditFlags_NoInputs, m_flags);
		flagop("no_tooltip", ImGuiColorEditFlags_NoTooltip, m_flags);
		flagop("no_label", ImGuiColorEditFlags_NoLabel, m_flags);
		flagop("no_side_preview", ImGuiColorEditFlags_NoSidePreview, m_flags);
		flagop("no_drag_drop", ImGuiColorEditFlags_NoDragDrop, m_flags);
		flagop("alpha_bar", ImGuiColorEditFlags_AlphaBar, m_flags);

		static std::vector<std::string> AlphaPreviewKeywords{ "alpha_preview", "alpha_preview_half" };
		static std::vector<int> AlphaPreviewFlags{ ImGuiColorEditFlags_AlphaPreview, ImGuiColorEditFlags_AlphaPreviewHalf };

		static std::vector<std::string> DisplayValueTypeKeywords{ "uint8", "floats" };
		static std::vector<int> DisplayValueTypeFlags{ ImGuiColorEditFlags_Uint8, ImGuiColorEditFlags_Float };

		std::vector<std::string> DisplayTypeKeywords{ "display_rgb", "display_hsv", "display_hex" };
		std::vector<int> DisplayTypeFlags{ ImGuiColorEditFlags_DisplayRGB, ImGuiColorEditFlags_DisplayHSV, ImGuiColorEditFlags_DisplayHex };

		static std::vector<std::string> IOTypeKeywords{ "input_rgb", "input_hsv" };
		static std::vector<int> IOTypeFlags{ ImGuiColorEditFlags_InputRGB, ImGuiColorEditFlags_InputHSV };

		conflictingflagop(AlphaPreviewKeywords, AlphaPreviewFlags, m_flags);
		conflictingflagop(DisplayValueTypeKeywords, DisplayValueTypeFlags, m_flags);
		conflictingflagop(DisplayTypeKeywords, DisplayTypeFlags, m_flags);
		conflictingflagop(IOTypeKeywords, IOTypeFlags, m_flags);

	}

	void mvColorEdit4::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
		};

		checkbitset("no_alpha", ImGuiColorEditFlags_NoAlpha, m_flags);
		checkbitset("no_picker", ImGuiColorEditFlags_NoPicker, m_flags);
		checkbitset("no_options", ImGuiColorEditFlags_NoOptions, m_flags);
		checkbitset("no_small_preview", ImGuiColorEditFlags_NoSmallPreview, m_flags);
		checkbitset("no_inputs", ImGuiColorEditFlags_NoInputs, m_flags);
		checkbitset("no_tooltip", ImGuiColorEditFlags_NoTooltip, m_flags);
		checkbitset("no_label", ImGuiColorEditFlags_NoLabel, m_flags);
		checkbitset("no_drag_drop", ImGuiColorEditFlags_NoDragDrop, m_flags);
		checkbitset("alpha_bar", ImGuiColorEditFlags_AlphaBar, m_flags);
		checkbitset("alpha_preview", ImGuiColorEditFlags_AlphaPreview, m_flags);
		checkbitset("alpha_preview_half", ImGuiColorEditFlags_AlphaPreviewHalf, m_flags);
		checkbitset("display_rgb", ImGuiColorEditFlags_DisplayRGB, m_flags);
		checkbitset("display_hsv", ImGuiColorEditFlags_DisplayHSV, m_flags);
		checkbitset("display_hex", ImGuiColorEditFlags_DisplayHex, m_flags);
		checkbitset("unit8", ImGuiColorEditFlags_Uint8, m_flags);
		checkbitset("floats", ImGuiColorEditFlags_Float, m_flags);
		checkbitset("input_rgb", ImGuiColorEditFlags_InputRGB, m_flags);
		checkbitset("input_hsv", ImGuiColorEditFlags_InputHSV, m_flags);
	}

}
