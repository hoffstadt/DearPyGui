#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvColorEdit, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Color, 1);
	class mvColorEdit : public mvColorPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvColorEdit, add_color_edit)

		MV_CREATE_CONSTANT(mvColorEdit_AlphaPreviewNone, 0);
		MV_CREATE_CONSTANT(mvColorEdit_AlphaPreview, ImGuiColorEditFlags_AlphaPreview);
		MV_CREATE_CONSTANT(mvColorEdit_AlphaPreviewHalf, ImGuiColorEditFlags_AlphaPreviewHalf);

		MV_CREATE_CONSTANT(mvColorEdit_uint8, ImGuiColorEditFlags_Uint8);
		MV_CREATE_CONSTANT(mvColorEdit_float, ImGuiColorEditFlags_Float);
			
		MV_CREATE_CONSTANT(mvColorEdit_rgb, ImGuiColorEditFlags_DisplayRGB);
		MV_CREATE_CONSTANT(mvColorEdit_hsv, ImGuiColorEditFlags_DisplayHSV);
		MV_CREATE_CONSTANT(mvColorEdit_hex, ImGuiColorEditFlags_DisplayHex);

		MV_CREATE_CONSTANT(mvColorEdit_input_rgb, ImGuiColorEditFlags_InputRGB);
		MV_CREATE_CONSTANT(mvColorEdit_input_hsv, ImGuiColorEditFlags_InputHSV);

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
			MV_ADD_CONSTANT(mvColorEdit_AlphaPreviewNone),
			MV_ADD_CONSTANT(mvColorEdit_AlphaPreview),
			MV_ADD_CONSTANT(mvColorEdit_AlphaPreviewHalf),
			MV_ADD_CONSTANT(mvColorEdit_uint8),
			MV_ADD_CONSTANT(mvColorEdit_float),
			MV_ADD_CONSTANT(mvColorEdit_rgb),
			MV_ADD_CONSTANT(mvColorEdit_hsv),
			MV_ADD_CONSTANT(mvColorEdit_hex),
			MV_ADD_CONSTANT(mvColorEdit_input_rgb),
			MV_ADD_CONSTANT(mvColorEdit_input_hsv)
		MV_END_CONSTANTS

	public:

		mvColorEdit(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;

		void handleSpecificPositionalArgs(PyObject* dict) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:

		ImGuiColorEditFlags m_flags = ImGuiColorEditFlags__OptionsDefault;


		std::array<float, 4> m_default_value = { 0.0f, 0.0f, 0.0f, 1.0f };
		bool                 m_no_alpha = false;
		bool                 m_no_picker = false;
		bool                 m_no_options = false;
		bool                 m_no_small_preview = false;
		bool                 m_no_inputs = false;
		bool                 m_no_tooltip = false;
		bool                 m_no_label = false;
		bool                 m_no_drag_drop = false;
		bool                 m_alpha_bar = false;

	};

}
