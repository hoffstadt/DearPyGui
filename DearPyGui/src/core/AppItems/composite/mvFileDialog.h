#pragma once

#include "mvTypeBases.h"
#include "mvApp.h"
#include "mvItemRegistry.h"

#pragma warning(push, 0) 
#include <ImGuiFileDialog.h>
#pragma warning(pop)

namespace Marvel {

	MV_REGISTER_WIDGET(mvFileDialog, MV_ITEM_DESC_ROOT | MV_ITEM_DESC_CONTAINER, StorageValueTypes::Bool, 1);
	class mvFileDialog : public mvBoolPtrBase
	{
	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvFileDialog, add_file_dialog)

		MV_CREATE_EXTRA_COMMAND(get_file_dialog_info);

		MV_CREATE_CONSTANT(mvThemeCol_FileDialog_Text, ImGuiCol_Text, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_FileDialog_TextSelected, ImGuiCol_TextSelectedBg, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_FileDialog_TitleBg, ImGuiCol_TitleBg, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_FileDialog_TitleBgActive, ImGuiCol_TitleBgActive, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_FileDialog_TitleBgCollapsed, ImGuiCol_TitleBgCollapsed, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_FileDialog_MenuBar, ImGuiCol_MenuBarBg, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_FileDialog_Bg, ImGuiCol_WindowBg, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_FileDialog_Scrollbar, ImGuiCol_ScrollbarBg, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_FileDialog_ScrollbarGrab, ImGuiCol_ScrollbarGrab, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_FileDialog_ScrollbarGrabHovered, ImGuiCol_ScrollbarGrabHovered, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_FileDialog_ScrollbarGrabActive, ImGuiCol_ScrollbarGrabActive, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_FileDialog_ResizeBorder, ImGuiCol_SeparatorActive, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_FileDialog_ResizeGrip, ImGuiCol_ResizeGrip, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_FileDialog_ResizeGripHovered, ImGuiCol_ResizeGripHovered, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_FileDialog_ResizeGripActive, ImGuiCol_ResizeGripActive, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_FileDialog_Border, ImGuiCol_Border, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_FileDialog_BorderShadow, ImGuiCol_BorderShadow, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_FileDialog_ChildBg, ImGuiCol_ChildBg, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_FileDialog_FrameBg, ImGuiCol_FrameBg, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_FileDialog_FrameBgHovered, ImGuiCol_FrameBgHovered, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_FileDialog_FrameBgActive, ImGuiCol_FrameBgActive, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_FileDialog_Button, ImGuiCol_Button, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_FileDialog_ButtonHovered, ImGuiCol_ButtonHovered, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_FileDialog_ButtonActive, ImGuiCol_ButtonActive, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_FileDialog_Header, ImGuiCol_Header, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_FileDialog_HeaderHovered, ImGuiCol_HeaderHovered, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_FileDialog_HeaderActive, ImGuiCol_HeaderActive, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_FileDialog_Separator, ImGuiCol_Separator, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_FileDialog_BorderActive, ImGuiCol_SeparatorActive, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_FileDialog_HeaderBg, ImGuiCol_TableHeaderBg, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_FileDialog_RowBg, ImGuiCol_TableRowBg, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_FileDialog_RowBgAlt, ImGuiCol_TableRowBgAlt, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_FileDialog_BorderSize, ImGuiStyleVar_WindowBorderSize, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_FileDialog_Rounding, ImGuiStyleVar_WindowRounding, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_FileDialog_MinSizeX, ImGuiStyleVar_WindowMinSize, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_FileDialog_MinSizeY, ImGuiStyleVar_WindowMinSize, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_FileDialog_TitleAlignX, ImGuiStyleVar_WindowTitleAlign, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_FileDialog_TitleAlignY, ImGuiStyleVar_WindowTitleAlign, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_FileDialog_PaddingX, ImGuiStyleVar_WindowPadding, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_FileDialog_PaddingY, ImGuiStyleVar_WindowPadding, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_FileDialog_ItemSpacingX, ImGuiStyleVar_ItemSpacing, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_FileDialog_ItemSpacingY, ImGuiStyleVar_ItemSpacing, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_FileDialog_ScrollbarSize, ImGuiStyleVar_ScrollbarSize , 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_FileDialog_ScrollbarRounding, ImGuiStyleVar_ScrollbarRounding, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_FileDialog_ChildBgRounding, ImGuiStyleVar_ChildRounding, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_FileDialog_ChildBgBorderSize, ImGuiStyleVar_ChildBorderSize, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_FileDialog_FramePaddingX, ImGuiStyleVar_FramePadding, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_FileDialog_FramePaddingY, ImGuiStyleVar_FramePadding, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_FileDialog_FrameRounding, ImGuiStyleVar_FrameRounding, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_FileDialog_FrameBorderSize, ImGuiStyleVar_FrameBorderSize, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_FileDialog_ItemInnerSpacingX, ImGuiStyleVar_ItemInnerSpacing, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_FileDialog_ItemInnerSpacingY, ImGuiStyleVar_ItemInnerSpacing, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_FileDialog_CellPaddingX, ImGuiStyleVar_CellPadding, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_FileDialog_CellPaddingY, ImGuiStyleVar_CellPadding, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_FileDialog_ButtonTextAlignX, ImGuiStyleVar_ButtonTextAlign, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_FileDialog_ButtonTextAlignY, ImGuiStyleVar_ButtonTextAlign, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_FileDialog_SelectableTextAlignX, ImGuiStyleVar_SelectableTextAlign, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_FileDialog_SelectableTextAlignY, ImGuiStyleVar_SelectableTextAlign, 1L);

		MV_START_EXTRA_COMMANDS
			MV_ADD_EXTRA_COMMAND(get_file_dialog_info);
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_FileDialog_Text, mvImGuiCol_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_FileDialog_TextSelected, mvImGuiCol_TextSelectedBg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_FileDialog_TitleBg, mvImGuiCol_TitleBg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_FileDialog_TitleBgActive, mvImGuiCol_TitleBgActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_FileDialog_TitleBgCollapsed, mvImGuiCol_TitleBgCollapsed),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_FileDialog_MenuBar, mvImGuiCol_MenuBarBg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_FileDialog_Bg, mvImGuiCol_WindowBg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_FileDialog_Scrollbar, mvImGuiCol_ScrollbarBg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_FileDialog_ScrollbarGrab, mvImGuiCol_ScrollbarGrab),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_FileDialog_ScrollbarGrabHovered, mvImGuiCol_ScrollbarGrabHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_FileDialog_ScrollbarGrabActive, mvImGuiCol_ScrollbarGrabActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_FileDialog_ResizeBorder, mvImGuiCol_SeparatorActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_FileDialog_ResizeGrip, mvImGuiCol_ResizeGrip),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_FileDialog_ResizeGripHovered, mvImGuiCol_ResizeGripHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_FileDialog_ResizeGripActive, mvImGuiCol_ResizeGripActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_FileDialog_Border, mvImGuiCol_Border),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_FileDialog_BorderShadow, mvImGuiCol_BorderShadow),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_FileDialog_ChildBg, mvImGuiCol_ChildBg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_FileDialog_FrameBg, mvImGuiCol_FrameBg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_FileDialog_FrameBgHovered, mvImGuiCol_FrameBgHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_FileDialog_FrameBgActive, mvImGuiCol_FrameBgActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_FileDialog_Button, mvImGuiCol_Button),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_FileDialog_ButtonHovered, mvImGuiCol_ButtonHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_FileDialog_ButtonActive, mvImGuiCol_ButtonActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_FileDialog_Header, mvImGuiCol_Header),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_FileDialog_HeaderHovered, mvImGuiCol_HeaderHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_FileDialog_HeaderActive, mvImGuiCol_HeaderActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_FileDialog_Separator, mvImGuiCol_Separator),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_FileDialog_BorderActive, mvImGuiCol_SeparatorActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_FileDialog_HeaderBg, mvImGuiCol_TableHeaderBg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_FileDialog_RowBg, mvImGuiCol_TableRowBg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_FileDialog_RowBgAlt, mvImGuiCol_TableRowBgAlt),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT_F(mvThemeStyle_FileDialog_BorderSize, 1, 1),
			MV_ADD_CONSTANT_F(mvThemeStyle_FileDialog_Rounding, 0, 12),
			MV_ADD_CONSTANT_F(mvThemeStyle_FileDialog_MinSizeX, 32, 50),
			MV_ADD_CONSTANT_F(mvThemeStyle_FileDialog_MinSizeY, 32, 50),
			MV_ADD_CONSTANT_F(mvThemeStyle_FileDialog_TitleAlignX, 0.5, 1),
			MV_ADD_CONSTANT_F(mvThemeStyle_FileDialog_TitleAlignY, 0.5, 1),
			MV_ADD_CONSTANT_F(mvThemeStyle_FileDialog_PaddingX, 8, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_FileDialog_PaddingY, 8, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_FileDialog_ItemSpacingX, 8, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_FileDialog_ItemSpacingY, 4, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_FileDialog_ScrollbarSize, 16, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_FileDialog_ScrollbarRounding, 0, 12),
			MV_ADD_CONSTANT_F(mvThemeStyle_FileDialog_ChildBgRounding, 0, 12),
			MV_ADD_CONSTANT_F(mvThemeStyle_FileDialog_ChildBgBorderSize, 1, 1),
			MV_ADD_CONSTANT_F(mvThemeStyle_FileDialog_FramePaddingX, 4, 12),
			MV_ADD_CONSTANT_F(mvThemeStyle_FileDialog_FramePaddingY, 3, 12),
			MV_ADD_CONSTANT_F(mvThemeStyle_FileDialog_FrameRounding, 0, 12),
			MV_ADD_CONSTANT_F(mvThemeStyle_FileDialog_FrameBorderSize, 0, 1),
			MV_ADD_CONSTANT_F(mvThemeStyle_FileDialog_ItemInnerSpacingX, 4, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_FileDialog_ItemInnerSpacingY, 4, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_FileDialog_CellPaddingX, 4, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_FileDialog_CellPaddingY, 2, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_FileDialog_ButtonTextAlignX, 0.5, 1),
			MV_ADD_CONSTANT_F(mvThemeStyle_FileDialog_ButtonTextAlignY, 0.5, 1),
			MV_ADD_CONSTANT_F(mvThemeStyle_FileDialog_SelectableTextAlignX, 0, 1),
			MV_ADD_CONSTANT_F(mvThemeStyle_FileDialog_SelectableTextAlignY, 0, 1),
		MV_END_STYLE_CONSTANTS

	public:

		mvFileDialog(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		void drawPanel();

		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

		ImGuiFileDialog& getDialog();
		void             markDirty() { m_dirtySettings = true; }
		PyObject* getInfoDict();
		bool      getContinueValue() const { return *m_value; }

	private:

		ImGuiFileDialog m_instance;
		bool            m_dirtySettings = true;

		std::string m_filters;
		std::string m_defaultPath;
		std::string m_defaultFilename = ".";
		int         m_fileCount = 0;
		bool        m_modal = false;
		bool        m_directory = false;
	};

}