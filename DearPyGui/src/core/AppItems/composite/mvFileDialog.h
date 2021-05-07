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

		MV_CREATE_CONSTANT(mvThemeCol_FileDiaglog_TitleText, ImGuiCol_Text, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_FileDiaglog_TitleBg, ImGuiCol_TitleBgActive, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_FileDiaglog_Bg, ImGuiCol_PopupBg, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_FileDiaglog_ModalDimBg, ImGuiCol_ModalWindowDimBg, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_FileDiaglog_Scrollbar, ImGuiCol_ScrollbarBg, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_FileDiaglog_ScrollbarGrab, ImGuiCol_ScrollbarGrab, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_FileDiaglog_ScrollbarGrabHovered, ImGuiCol_ScrollbarGrabHovered, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_FileDiaglog_ScrollbarGrabActive, ImGuiCol_ScrollbarGrabActive, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_FileDiaglog_Border, ImGuiCol_Border, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_FileDiaglog_BorderSize, ImGuiStyleVar_PopupBorderSize, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_FileDiaglog_Rounding, ImGuiStyleVar_PopupRounding, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_FileDiaglog_PaddingX, ImGuiStyleVar_WindowPadding, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_FileDiaglog_PaddingY, ImGuiStyleVar_WindowPadding, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_FileDiaglog_ItemSpacingX, ImGuiStyleVar_ItemSpacing, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_FileDiaglog_ItemSpacingY, ImGuiStyleVar_ItemSpacing, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_FileDiaglog_ModalRounding, ImGuiStyleVar_WindowRounding, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_FileDiaglog_ModalBorderSize, ImGuiStyleVar_WindowBorderSize, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_FileDiaglog_ModalTitleAlignX, ImGuiStyleVar_WindowTitleAlign, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_FileDiaglog_ModalTitleAlignY, ImGuiStyleVar_WindowTitleAlign, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_FileDiaglog_ModalMinSizeX, ImGuiStyleVar_WindowMinSize, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_FileDiaglog_ModalMinSizeY, ImGuiStyleVar_WindowMinSize, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_FileDiaglog_ModalScrollbarSize, ImGuiStyleVar_ScrollbarSize, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_FileDiaglog_ModalScrollbarRounding, ImGuiStyleVar_ScrollbarRounding, 0L);

		MV_START_EXTRA_COMMANDS
			MV_ADD_EXTRA_COMMAND(get_file_dialog_info);
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_FileDiaglog_TitleText, mvImGuiCol_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_FileDiaglog_TitleBg, mvImGuiCol_FrameBgActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_FileDiaglog_Bg, mvImGuiCol_PopupBg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_FileDiaglog_ModalDimBg, mvImGuiCol_ModalWindowDimBg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_FileDiaglog_Scrollbar, mvImGuiCol_ScrollbarBg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_FileDiaglog_ScrollbarGrab, mvImGuiCol_ScrollbarGrab),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_FileDiaglog_ScrollbarGrabHovered, mvImGuiCol_ScrollbarGrabHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_FileDiaglog_ScrollbarGrabActive, mvImGuiCol_ScrollbarGrabActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_FileDiaglog_Border, mvImGuiCol_Border),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT_F(mvThemeStyle_FileDiaglog_BorderSize, 1, 1),
			MV_ADD_CONSTANT_F(mvThemeStyle_FileDiaglog_Rounding, 0, 12),
			MV_ADD_CONSTANT_F(mvThemeStyle_FileDiaglog_PaddingX, 8, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_FileDiaglog_PaddingY, 8, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_FileDiaglog_ItemSpacingX, 8, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_FileDiaglog_ItemSpacingY, 4, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_FileDiaglog_ModalRounding, 0, 12),
			MV_ADD_CONSTANT_F(mvThemeStyle_FileDiaglog_ModalBorderSize, 1, 1),
			MV_ADD_CONSTANT_F(mvThemeStyle_FileDiaglog_ModalTitleAlignX, 0, 1),
			MV_ADD_CONSTANT_F(mvThemeStyle_FileDiaglog_ModalTitleAlignY, 0.5, 1),
			MV_ADD_CONSTANT_F(mvThemeStyle_FileDiaglog_ModalMinSizeX, 32, 50),
			MV_ADD_CONSTANT_F(mvThemeStyle_FileDiaglog_ModalMinSizeY, 32, 50),
			MV_ADD_CONSTANT_F(mvThemeStyle_FileDiaglog_ModalScrollbarSize, 14, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_FileDiaglog_ModalScrollbarRounding, 9, 12),
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