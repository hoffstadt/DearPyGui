#include "mvColorPicker.h"
#include "mvApp.h"
#include <array>
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvColorPicker::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		mvPythonParser parser(mvPyDataType::String);
		mvAppItem::AddCommonArgs(parser);

		parser.addArg<mvPyDataType::IntList>("default_value", mvArgType::POSITIONAL_ARG, "(0, 0, 0, 255)");
		parser.addArg<mvPyDataType::Bool>("no_alpha", mvArgType::KEYWORD_ARG, "False", "ignore Alpha component");
		parser.addArg<mvPyDataType::Bool>("no_picker", mvArgType::KEYWORD_ARG, "False", "disable picker when clicking on colored square.");
		parser.addArg<mvPyDataType::Bool>("no_side_preview", mvArgType::KEYWORD_ARG, "False", "disable bigger color preview on right side of the picker, use small colored square preview instead , unless small preview is also hidden");
		parser.addArg<mvPyDataType::Bool>("no_small_preview", mvArgType::KEYWORD_ARG, "False", "disable colored square preview next to the inputs. (e.g. to show only the inputs). This only displays if the side preview is not shown.");
		parser.addArg<mvPyDataType::Bool>("no_inputs", mvArgType::KEYWORD_ARG, "False", "disable inputs sliders/text widgets (e.g. to show only the small preview colored square)");
		parser.addArg<mvPyDataType::Bool>("no_tooltip", mvArgType::KEYWORD_ARG, "False", "disable tooltip when hovering the preview.");
		parser.addArg<mvPyDataType::Bool>("no_label", mvArgType::KEYWORD_ARG, "False", "disable display of inline text label");
		parser.addArg<mvPyDataType::Bool>("alpha_bar", mvArgType::KEYWORD_ARG, "False", "show vertical alpha bar/gradient in picker.");
		parser.addArg<mvPyDataType::Bool>("alpha_preview", mvArgType::KEYWORD_ARG, "False", "display preview as a transparent color over a checkerboard, instead of opaque.");
		parser.addArg<mvPyDataType::Bool>("alpha_preview_half", mvArgType::KEYWORD_ARG, "False", "display half opaque / half checkerboard, instead of opaque.");
		parser.addArg<mvPyDataType::Bool>("display_rgb", mvArgType::KEYWORD_ARG, "False", "override _display_ type among RGB/HSV/Hex.");
		parser.addArg<mvPyDataType::Bool>("display_hsv", mvArgType::KEYWORD_ARG, "False", "override _display_ type among RGB/HSV/Hex.");
		parser.addArg<mvPyDataType::Bool>("display_hex", mvArgType::KEYWORD_ARG, "False", "override _display_ type among RGB/HSV/Hex.");
		parser.addArg<mvPyDataType::Bool>("uint8", mvArgType::KEYWORD_ARG, "False", "display values formatted as 0..255");
		parser.addArg<mvPyDataType::Bool>("floats", mvArgType::KEYWORD_ARG, "False", "display values formatted as 0.0f..1.0f floats instead of 0..255 integers.");
		parser.addArg<mvPyDataType::Bool>("input_rgb", mvArgType::KEYWORD_ARG, "False", "input and output data in RGB format.");
		parser.addArg<mvPyDataType::Bool>("input_hsv", mvArgType::KEYWORD_ARG, "False", "input and output data in HSV format.");
		parser.addArg<mvPyDataType::Bool>("picker_hue_bar", mvArgType::KEYWORD_ARG, "False", "bar for Hue, rectangle for Sat/Value");
		parser.addArg<mvPyDataType::Bool>("picker_hue_wheel", mvArgType::KEYWORD_ARG, "False", "wheel for Hue, triangle for Sat/Value.");
		
		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvColorPicker::mvColorPicker(const std::string& name)
		: 
		mvColorPtrBase(name)
	{
	}

	void mvColorPicker::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id;
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);

		if (!m_enabled) std::copy(m_value->data(), m_value->data() + 4, m_disabled_value);

		if (m_3component)
		{
			if (ImGui::ColorPicker3(m_label.c_str(), m_enabled ? m_value->data() : &m_disabled_value[0], m_flags))
				mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callback_data);
		}
		else
		{
			if (ImGui::ColorPicker4(m_label.c_str(), m_enabled ? m_value->data() : &m_disabled_value[0], m_flags))
				mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callback_data);
		}

	}

	void mvColorPicker::handleSpecificKeywordArgs(PyObject* dict)
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

	void mvColorPicker::getSpecificConfiguration(PyObject* dict)
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

}