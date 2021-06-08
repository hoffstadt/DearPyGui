#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvTimePicker, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Time, 1);
	class mvTimePicker : public mvTimePtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvTimePicker, add_time_picker)

		MV_CREATE_CONSTANT(mvThemeCol_TimePicker_Time, ImGuiCol_Text, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_TimePicker_TimeHovered, ImGuiCol_ButtonHovered, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_TimePicker_DropSelected, ImGuiCol_Header, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_TimePicker_DropHovered, ImGuiCol_HeaderHovered, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_TimePicker_DropActive, ImGuiCol_HeaderActive, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_TimePicker_DropBg, ImGuiCol_PopupBg, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_TimePicker_Scrollbar, ImGuiCol_ScrollbarBg, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_TimePicker_ScrollbarGrab, ImGuiCol_ScrollbarGrab, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_TimePicker_ScrollbarGrabHovered, ImGuiCol_ScrollbarGrabHovered, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_TimePicker_ScrollbarGrabActive, ImGuiCol_ScrollbarGrabActive, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_TimePicker_Border, ImGuiCol_Border, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_TimePicker_BorderShadow, ImGuiCol_BorderShadow, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_TimePicker_BorderSize, ImGuiStyleVar_FrameBorderSize, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_TimePicker_Rounding, ImGuiStyleVar_FrameRounding, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_TimePicker_PaddingX, ImGuiStyleVar_FramePadding, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_TimePicker_PaddingY, ImGuiStyleVar_FramePadding, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_TimePicker_TextAlignX, ImGuiStyleVar_ButtonTextAlign, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_TimePicker_TextAlignY, ImGuiStyleVar_ButtonTextAlign, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_TimePicker_DropPaddingX, ImGuiStyleVar_WindowPadding, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_TimePicker_DropPaddingY, ImGuiStyleVar_WindowPadding, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_TimePicker_DropBorderSize, ImGuiStyleVar_PopupBorderSize, 0L)
		MV_CREATE_CONSTANT(mvThemeStyle_TimePicker_DropItemSpacingX, ImGuiStyleVar_ItemSpacing, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_TimePicker_DropItemSpacingY, ImGuiStyleVar_ItemSpacing, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_TimePicker_DropTextAlignX, ImGuiStyleVar_SelectableTextAlign, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_TimePicker_DropTextAlignY, ImGuiStyleVar_SelectableTextAlign, 1L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TimePicker_Time, mvImGuiCol_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TimePicker_TimeHovered, mvImGuiCol_ButtonHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TimePicker_DropSelected, mvImGuiCol_Header),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TimePicker_DropHovered, mvImGuiCol_FrameBgHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TimePicker_DropActive, mvImGuiCol_FrameBgActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TimePicker_DropBg, mvImGuiCol_PopupBg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TimePicker_Scrollbar, mvImGuiCol_ScrollbarBg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TimePicker_ScrollbarGrab, mvImGuiCol_ScrollbarGrab),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TimePicker_ScrollbarGrabHovered, mvImGuiCol_ScrollbarGrabHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TimePicker_ScrollbarGrabActive,	mvImGuiCol_ScrollbarGrabActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TimePicker_Border, mvImGuiCol_Border),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TimePicker_BorderShadow, mvImGuiCol_BorderShadow),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT_F(mvThemeStyle_TimePicker_BorderSize, 0,  1),
			MV_ADD_CONSTANT_F(mvThemeStyle_TimePicker_Rounding, 0, 12),
			MV_ADD_CONSTANT_F(mvThemeStyle_TimePicker_PaddingX, 8, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_TimePicker_PaddingY, 4, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_TimePicker_TextAlignX, 0,  1),
			MV_ADD_CONSTANT_F(mvThemeStyle_TimePicker_TextAlignY, 0,  1),
			MV_ADD_CONSTANT_F(mvThemeStyle_TimePicker_DropPaddingX, 8, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_TimePicker_DropPaddingY, 4, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_TimePicker_DropBorderSize, 1,  1),
			MV_ADD_CONSTANT_F(mvThemeStyle_TimePicker_DropItemSpacingX, 8, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_TimePicker_DropItemSpacingY, 4, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_TimePicker_DropTextAlignX, 0,  1),
			MV_ADD_CONSTANT_F(mvThemeStyle_TimePicker_DropTextAlignY, 0,  1),
		MV_END_STYLE_CONSTANTS

	public:

		mvTimePicker(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y)override;

		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:

		bool m_hour24 = false;

	};

}