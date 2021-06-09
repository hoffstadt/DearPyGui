#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvColorButton, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 1);
	class mvColorButton : public mvColorPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvColorButton, add_color_button)

		MV_CREATE_CONSTANT(mvThemeCol_ColorButton_Text, ImGuiCol_Text, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorButton_Separator, ImGuiCol_Separator, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorButton_PopupBg, ImGuiCol_PopupBg, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorButton_Border, ImGuiCol_Border, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorButton_BorderShadow, ImGuiCol_BorderShadow, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_ColorButton_Rounding, ImGuiStyleVar_FrameRounding	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorButton_BorderSize, ImGuiStyleVar_FrameBorderSize	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorButton_PopupRounding, ImGuiStyleVar_PopupRounding	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorButton_PopupBorderSize, ImGuiStyleVar_PopupBorderSize	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorButton_PopupPaddingX, ImGuiStyleVar_WindowPadding	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorButton_PopupPaddingY, ImGuiStyleVar_WindowPadding	, 1L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_ADD_CONSTANT(mvThemeCol_ColorButton_Text, mvImGuiCol_Text, mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_ColorButton_Separator, mvImGuiCol_Separator, mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_ColorButton_PopupBg, mvImGuiCol_PopupBg, mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_ColorButton_Border, mvImGuiCol_Border, mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_ColorButton_BorderShadow, mvImGuiCol_BorderShadow, mvColor(128, 128, 128, 63)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT_F(mvThemeStyle_ColorButton_Rounding		, 0, 12),
			MV_ADD_CONSTANT_F(mvThemeStyle_ColorButton_BorderSize		, 0,  1),
			MV_ADD_CONSTANT_F(mvThemeStyle_ColorButton_PopupRounding	, 0, 12),
			MV_ADD_CONSTANT_F(mvThemeStyle_ColorButton_PopupBorderSize, 1,  1),
			MV_ADD_CONSTANT_F(mvThemeStyle_ColorButton_PopupPaddingX	, 8, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_ColorButton_PopupPaddingY	, 8, 20),
		MV_END_STYLE_CONSTANTS

	public:

		mvColorButton(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;

		void handleSpecificPositionalArgs(PyObject* dict) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:

		ImGuiColorEditFlags m_flags = ImGuiColorEditFlags_None;
		bool                m_no_alpha = false;
		bool                m_no_border = false;
		bool                m_no_drag_drop = false;

	};

}
