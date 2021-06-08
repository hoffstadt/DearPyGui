#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvPopup, MV_ITEM_DESC_CONTAINER | MV_ITEM_DESC_AFTER, StorageValueTypes::Bool, 1);
	class mvPopup : public mvBoolPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvPopup, add_popup)

		MV_CREATE_EXTRA_COMMAND(close_popup);

		MV_CREATE_CONSTANT(mvThemeCol_Popup_TitleText, ImGuiCol_Text, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Popup_TitleBg, ImGuiCol_TitleBgActive, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Popup_Bg, ImGuiCol_PopupBg, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Popup_ModalDimBg, ImGuiCol_ModalWindowDimBg, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Popup_Scrollbar, ImGuiCol_ScrollbarBg, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Popup_ScrollbarGrab, ImGuiCol_ScrollbarGrab, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Popup_ScrollbarGrabHovered, ImGuiCol_ScrollbarGrabHovered, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Popup_ScrollbarGrabActive, ImGuiCol_ScrollbarGrabActive, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Popup_Border, ImGuiCol_Border, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_Popup_BorderSize, ImGuiStyleVar_PopupBorderSize, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Popup_Rounding, ImGuiStyleVar_PopupRounding, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Popup_PaddingX, ImGuiStyleVar_WindowPadding, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Popup_PaddingY, ImGuiStyleVar_WindowPadding, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Popup_ItemSpacingX, ImGuiStyleVar_ItemSpacing, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Popup_ItemSpacingY, ImGuiStyleVar_ItemSpacing, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Popup_ModalRounding, ImGuiStyleVar_WindowRounding, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Popup_ModalBorderSize, ImGuiStyleVar_WindowBorderSize, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Popup_ModalTitleAlignX, ImGuiStyleVar_WindowTitleAlign, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Popup_ModalTitleAlignY, ImGuiStyleVar_WindowTitleAlign, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Popup_ModalMinSizeX, ImGuiStyleVar_WindowMinSize, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Popup_ModalMinSizeY, ImGuiStyleVar_WindowMinSize, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Popup_ModalScrollbarSize, ImGuiStyleVar_ScrollbarSize, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Popup_ModalScrollbarRounding, ImGuiStyleVar_ScrollbarRounding, 0L);

		MV_START_EXTRA_COMMANDS
			MV_ADD_EXTRA_COMMAND(close_popup);
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Popup_TitleText, mvImGuiCol_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Popup_TitleBg, mvImGuiCol_FrameBgActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Popup_Bg, mvImGuiCol_PopupBg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Popup_ModalDimBg, mvImGuiCol_ModalWindowDimBg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Popup_Scrollbar, mvImGuiCol_ScrollbarBg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Popup_ScrollbarGrab, mvImGuiCol_ScrollbarGrab),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Popup_ScrollbarGrabHovered, mvImGuiCol_ScrollbarGrabHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Popup_ScrollbarGrabActive, mvImGuiCol_ScrollbarGrabActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Popup_Border, mvImGuiCol_Border),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT_F(mvThemeStyle_Popup_BorderSize, 1, 1),
			MV_ADD_CONSTANT_F(mvThemeStyle_Popup_Rounding, 0, 12),
			MV_ADD_CONSTANT_F(mvThemeStyle_Popup_PaddingX, 8, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_Popup_PaddingY, 8, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_Popup_ItemSpacingX, 8, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_Popup_ItemSpacingY, 4, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_Popup_ModalRounding, 0, 12),
			MV_ADD_CONSTANT_F(mvThemeStyle_Popup_ModalBorderSize, 1, 1),
			MV_ADD_CONSTANT_F(mvThemeStyle_Popup_ModalTitleAlignX, 0, 1),
			MV_ADD_CONSTANT_F(mvThemeStyle_Popup_ModalTitleAlignY, 0.5, 1),
			MV_ADD_CONSTANT_F(mvThemeStyle_Popup_ModalMinSizeX, 32, 50),
			MV_ADD_CONSTANT_F(mvThemeStyle_Popup_ModalMinSizeY, 32, 50),
			MV_ADD_CONSTANT_F(mvThemeStyle_Popup_ModalScrollbarSize,14, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_Popup_ModalScrollbarRounding, 9, 12),
		MV_END_STYLE_CONSTANTS

	public:

		mvPopup(mvUUID uuid);

		void closePopup();

		void draw(ImDrawList* drawlist, float x, float y) override;

		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:

		bool       m_modal = false;
		int        m_button = 1;
		mvAppItem* m_parentAddress = nullptr;
		bool       m_close = false;

	};

}