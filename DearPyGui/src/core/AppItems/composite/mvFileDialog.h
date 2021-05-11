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

		MV_CREATE_CONSTANT(mvThemeCol_FileDialog_TitleText, ImGuiCol_Text, 0L);
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

		MV_START_EXTRA_COMMANDS
			MV_ADD_EXTRA_COMMAND(get_file_dialog_info);
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_FileDialog_TitleText, mvImGuiCol_Text),
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
		MV_END_STYLE_CONSTANTS

	public:

		mvFileDialog(const std::string& name);

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