#include "mvColorEdit.h"
#include "mvApp.h"
#include <array>
#include "mvItemRegistry.h"

namespace Marvel {

	void mvColorEdit::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Adds an RGBA color editor. Click the small color preview will provide a color picker. Click and draging the small color preview will copy the color to be applied on any other color widget.", { "Widgets" });
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

		parser.addArg<mvPyDataType::Bool>("no_alpha", mvArgType::KEYWORD_ARG, "False", "Disable Alpha component.");
		parser.addArg<mvPyDataType::Bool>("no_picker", mvArgType::KEYWORD_ARG, "False", "Disable picker popup when color square is clicked.");
		parser.addArg<mvPyDataType::Bool>("no_options", mvArgType::KEYWORD_ARG, "False", "Disable toggling options menu when right-clicking on inputs/small preview.");
		parser.addArg<mvPyDataType::Bool>("no_small_preview", mvArgType::KEYWORD_ARG, "False", "Disable colored square preview next to the inputs. (e.g. to show only the inputs). This only displays if the side preview is not shown.");
		parser.addArg<mvPyDataType::Bool>("no_inputs", mvArgType::KEYWORD_ARG, "False", "Disable inputs sliders/text widgets. (e.g. to show only the small preview colored square)");
		parser.addArg<mvPyDataType::Bool>("no_tooltip", mvArgType::KEYWORD_ARG, "False", "Disable tooltip when hovering the preview.");
		parser.addArg<mvPyDataType::Bool>("no_label", mvArgType::KEYWORD_ARG, "False", "Disable display of inline text label.");
		parser.addArg<mvPyDataType::Bool>("no_drag_drop", mvArgType::KEYWORD_ARG, "False", "Disable ability to drag and drop small preview (color square) to apply colors.");
		parser.addArg<mvPyDataType::Bool>("alpha_bar", mvArgType::KEYWORD_ARG, "False", "Show vertical alpha bar/gradient in picker.");

		parser.addArg<mvPyDataType::Long>("alpha_preview", mvArgType::KEYWORD_ARG, "0", "mvColorEdit_AlphaPreviewNone, mvColorEdit_AlphaPreview, or mvColorEdit_AlphaPreviewHalf");
		parser.addArg<mvPyDataType::Long>("display_mode", mvArgType::KEYWORD_ARG, "1048576", "mvColorEdit_rgb, mvColorEdit_hsv, or mvColorEdit_hex");
		parser.addArg<mvPyDataType::Long>("display_type", mvArgType::KEYWORD_ARG, "8388608", "mvColorEdit_uint8 or mvColorEdit_float");
		parser.addArg<mvPyDataType::Long>("input_mode", mvArgType::KEYWORD_ARG, "134217728", "mvColorEdit_input_rgb or mvColorEdit_input_hsv");

		parser.finalize();

		parsers->insert({ s_command, parser });
	
	}

	mvColorEdit::mvColorEdit(mvUUID uuid)
		: 
		mvColorPtrBase(uuid)
	{
	}

	void mvColorEdit::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id(m_uuid);

		if (!m_enabled) std::copy(m_value->data(), m_value->data() + 4, m_disabled_value);

			if (ImGui::ColorEdit4(m_label.c_str(), m_enabled ? m_value->data() : &m_disabled_value[0], m_flags))
				mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_uuid, nullptr, m_user_data);

	}

	void mvColorEdit::handleSpecificPositionalArgs(PyObject* dict)
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

	void mvColorEdit::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		// helpers for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
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

		if (PyObject* item = PyDict_GetItemString(dict, "alpha_preview"))
		{
			long mode = ToUUID(item);

			// reset target flags
			m_flags &= ~ImGuiColorEditFlags_AlphaPreview;
			m_flags &= ~ImGuiColorEditFlags_AlphaPreviewHalf;

			switch (mode)
			{
			case mvColorEdit_AlphaPreview:
				m_flags |= ImGuiColorEditFlags_AlphaPreview;
				break;
			case mvColorEdit_AlphaPreviewHalf:
				m_flags |= ImGuiColorEditFlags_AlphaPreviewHalf;
				break;
			default:
				break;
			}
		}

		if (PyObject* item = PyDict_GetItemString(dict, "display_mode"))
		{
			long mode = ToUUID(item);

			// reset target flags
			m_flags &= ~ImGuiColorEditFlags_DisplayRGB;
			m_flags &= ~ImGuiColorEditFlags_DisplayHSV;
			m_flags &= ~ImGuiColorEditFlags_DisplayHex;

			switch (mode)
			{
			case mvColorEdit_hex:
				m_flags |= ImGuiColorEditFlags_DisplayHex;
				break;
			case mvColorEdit_hsv:
				m_flags |= ImGuiColorEditFlags_DisplayHSV;
				break;
			default:
				m_flags |= ImGuiColorEditFlags_DisplayRGB;
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
			case mvColorEdit_float:
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
			case mvColorEdit_input_hsv:
				m_flags |= ImGuiColorEditFlags_InputHSV;
				break;
			default:
				m_flags |= ImGuiColorEditFlags_InputRGB;
				break;
			}
		}

	}

	void mvColorEdit::getSpecificConfiguration(PyObject* dict)
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

		// input_mode
		if (m_flags & ImGuiColorEditFlags_InputRGB)
			PyDict_SetItemString(dict, "input_mode", ToPyUUID(mvColorEdit_input_rgb));
		else if (m_flags & ImGuiColorEditFlags_InputHSV)
			PyDict_SetItemString(dict, "input_mode", ToPyUUID(mvColorEdit_input_hsv));

		// alpha_preview
		if(m_flags & ImGuiColorEditFlags_AlphaPreview)
			PyDict_SetItemString(dict, "alpha_preview", ToPyUUID(mvColorEdit_AlphaPreview));
		else if (m_flags & ImGuiColorEditFlags_AlphaPreviewHalf)
			PyDict_SetItemString(dict, "alpha_preview", ToPyUUID(mvColorEdit_AlphaPreviewHalf));
		else
			PyDict_SetItemString(dict, "alpha_preview", ToPyUUID(mvColorEdit_AlphaPreviewNone));

		// display_mode
		if (m_flags & ImGuiColorEditFlags_DisplayHSV)
			PyDict_SetItemString(dict, "display_mode", ToPyUUID(mvColorEdit_hsv));
		else if (m_flags & ImGuiColorEditFlags_DisplayHex)
			PyDict_SetItemString(dict, "display_mode", ToPyUUID(mvColorEdit_hex));
		else
			PyDict_SetItemString(dict, "display_mode", ToPyUUID(mvColorEdit_rgb));

		// display_type
		if (m_flags & ImGuiColorEditFlags_Uint8)
			PyDict_SetItemString(dict, "display_type", ToPyUUID(mvColorEdit_uint8));
		else if (m_flags & ImGuiColorEditFlags_Float)
			PyDict_SetItemString(dict, "display_type", ToPyUUID(mvColorEdit_float));

	}

}
