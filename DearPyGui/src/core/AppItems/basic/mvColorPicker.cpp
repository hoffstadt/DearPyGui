#include "mvColorPicker.h"
#include "mvApp.h"
#include <array>
#include "mvItemRegistry.h"

namespace Marvel {

	void mvColorPicker::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		mvPythonParser parser(mvPyDataType::UUID, "Adds an RGB color picker. Right click the color picker for options. Click and drag the color preview to copy the color and drop on any other color widget to apply. Right Click allows the style of the color picker to be changed.", { "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_WIDTH |
			MV_PARSER_ARG_HEIGHT |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SOURCE |
			MV_PARSER_ARG_CALLBACK |
			MV_PARSER_ARG_USER_DATA |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_ENABLED |
			MV_PARSER_ARG_FILTER |
			MV_PARSER_ARG_DROP_CALLBACK |
			MV_PARSER_ARG_DRAG_CALLBACK |
			MV_PARSER_ARG_PAYLOAD_TYPE |
			MV_PARSER_ARG_TRACKED |
			MV_PARSER_ARG_POS)
		);

		parser.addArg<mvPyDataType::IntList>("default_value", mvArgType::POSITIONAL_ARG, "(0, 0, 0, 255)");
		parser.addArg<mvPyDataType::Bool>("no_alpha", mvArgType::KEYWORD_ARG, "False", "Ignore Alpha component.");
		parser.addArg<mvPyDataType::Bool>("no_side_preview", mvArgType::KEYWORD_ARG, "False", "Disable bigger color preview on right side of the picker, use small colored square preview instead , unless small preview is also hidden.");
		parser.addArg<mvPyDataType::Bool>("no_small_preview", mvArgType::KEYWORD_ARG, "False", "Disable colored square preview next to the inputs. (e.g. to show only the inputs). This only displays if the side preview is not shown.");
		parser.addArg<mvPyDataType::Bool>("no_inputs", mvArgType::KEYWORD_ARG, "False", "Disable inputs sliders/text widgets. (e.g. to show only the small preview colored square)");
		parser.addArg<mvPyDataType::Bool>("no_tooltip", mvArgType::KEYWORD_ARG, "False", "Disable tooltip when hovering the preview.");
		parser.addArg<mvPyDataType::Bool>("no_label", mvArgType::KEYWORD_ARG, "False", "Disable display of inline text label.");
		parser.addArg<mvPyDataType::Bool>("alpha_bar", mvArgType::KEYWORD_ARG, "False", "Show vertical alpha bar/gradient in picker.");
		parser.addArg<mvPyDataType::Bool>("display_rgb", mvArgType::KEYWORD_ARG, "False", "Override _display_ type among RGB/HSV/Hex.");
		parser.addArg<mvPyDataType::Bool>("display_hsv", mvArgType::KEYWORD_ARG, "False", "Override _display_ type among RGB/HSV/Hex.");
		parser.addArg<mvPyDataType::Bool>("display_hex", mvArgType::KEYWORD_ARG, "False", "Override _display_ type among RGB/HSV/Hex.");

		parser.addArg<mvPyDataType::Long>("picker_mode", mvArgType::KEYWORD_ARG, "33554432", "mvColorPicker_bar or mvColorPicker_wheel");
		parser.addArg<mvPyDataType::Long>("alpha_preview", mvArgType::KEYWORD_ARG, "0", "mvColorEdit_AlphaPreviewNone, mvColorEdit_AlphaPreview, or mvColorEdit_AlphaPreviewHalf");
		parser.addArg<mvPyDataType::Long>("display_type", mvArgType::KEYWORD_ARG, "8388608", "mvColorEdit_uint8 or mvColorEdit_float");
		parser.addArg<mvPyDataType::Long>("input_mode", mvArgType::KEYWORD_ARG, "134217728", "mvColorEdit_input_rgb or mvColorEdit_input_hsv");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvColorPicker::mvColorPicker(mvUUID uuid)
		: 
		mvColorPtrBase(uuid)
	{
	}

	void mvColorPicker::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id(m_uuid);

		if (!m_enabled) std::copy(m_value->data(), m_value->data() + 4, m_disabled_value);

		if (ImGui::ColorPicker4(m_label.c_str(), m_enabled ? m_value->data() : &m_disabled_value[0], m_flags))
			mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_uuid, nullptr, m_user_data);

	}

	void mvColorPicker::handleSpecificPositionalArgs(PyObject* dict)
	{
		if (!mvApp::GetApp()->getParsers()[s_command].verifyPositionalArguments(dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				setPyValue(item);
				break;

			default:
				break;
			}
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

		if (PyObject* item = PyDict_GetItemString(dict, "picker_mode"))
		{
			long mode = ToUUID(item);

			// reset target flags
			m_flags &= ~ImGuiColorEditFlags_PickerHueBar;
			m_flags &= ~ImGuiColorEditFlags_PickerHueWheel;

			switch (mode)
			{
			case ImGuiColorEditFlags_PickerHueWheel:
				m_flags |= ImGuiColorEditFlags_PickerHueWheel;
				break;
			default:
				m_flags |= ImGuiColorEditFlags_PickerHueBar;
				break;
			}
		}

		if (PyObject* item = PyDict_GetItemString(dict, "alpha_preview"))
		{
			long mode = ToUUID(item);

			// reset target flags
			m_flags &= ~ImGuiColorEditFlags_AlphaPreview;
			m_flags &= ~ImGuiColorEditFlags_AlphaPreviewHalf;

			switch (mode)
			{
			case ImGuiColorEditFlags_AlphaPreview:
				m_flags |= ImGuiColorEditFlags_AlphaPreview;
				break;
			case ImGuiColorEditFlags_AlphaPreviewHalf:
				m_flags |= ImGuiColorEditFlags_AlphaPreviewHalf;
				break;
			default:
				break;
			}
		}

		if (PyObject* item = PyDict_GetItemString(dict, "display_type"))
		{
			long mode = ToUUID(item);

			// reset target flags
			m_flags &= ~ImGuiColorEditFlags_Uint8;
			m_flags &= ~ImGuiColorEditFlags_Float;

			switch (mode)
			{
			case ImGuiColorEditFlags_Float:
				m_flags |= ImGuiColorEditFlags_Float;
				break;
			default:
				m_flags |= ImGuiColorEditFlags_Uint8;
				break;
			}
		}

		if (PyObject* item = PyDict_GetItemString(dict, "input_mode"))
		{
			long mode = ToUUID(item);

			// reset target flags
			m_flags &= ~ImGuiColorEditFlags_InputRGB;
			m_flags &= ~ImGuiColorEditFlags_InputHSV;

			switch (mode)
			{
			case ImGuiColorEditFlags_InputHSV:
				m_flags |= ImGuiColorEditFlags_InputHSV;
				break;
			default:
				m_flags |= ImGuiColorEditFlags_InputRGB;
				break;
			}
		}

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
		checkbitset("display_rgb", ImGuiColorEditFlags_DisplayRGB, m_flags);
		checkbitset("display_hsv", ImGuiColorEditFlags_DisplayHSV, m_flags);
		checkbitset("display_hex", ImGuiColorEditFlags_DisplayHex, m_flags);

		// input_mode
		if (m_flags & ImGuiColorEditFlags_InputRGB)
			PyDict_SetItemString(dict, "input_mode", ToPyUUID(ImGuiColorEditFlags_InputRGB));
		else if (m_flags & ImGuiColorEditFlags_InputHSV)
			PyDict_SetItemString(dict, "input_mode", ToPyUUID(ImGuiColorEditFlags_InputHSV));

		// alpha_preview
		if (m_flags & ImGuiColorEditFlags_AlphaPreview)
			PyDict_SetItemString(dict, "alpha_preview", ToPyUUID(ImGuiColorEditFlags_AlphaPreview));
		else if (m_flags & ImGuiColorEditFlags_AlphaPreviewHalf)
			PyDict_SetItemString(dict, "alpha_preview", ToPyUUID(ImGuiColorEditFlags_AlphaPreviewHalf));
		else
			PyDict_SetItemString(dict, "alpha_preview", ToPyUUID(0));

		// display_type
		if (m_flags & ImGuiColorEditFlags_Uint8)
			PyDict_SetItemString(dict, "display_type", ToPyUUID(ImGuiColorEditFlags_Uint8));
		else if (m_flags & ImGuiColorEditFlags_Float)
			PyDict_SetItemString(dict, "display_type", ToPyUUID(ImGuiColorEditFlags_Float));

		// picker_mode
		if (m_flags & ImGuiColorEditFlags_PickerHueWheel)
			PyDict_SetItemString(dict, "picker_mode", ToPyUUID(ImGuiColorEditFlags_PickerHueWheel));
		else if (m_flags & ImGuiColorEditFlags_PickerHueBar)
			PyDict_SetItemString(dict, "picker_mode", ToPyUUID(ImGuiColorEditFlags_PickerHueBar));
	}

}