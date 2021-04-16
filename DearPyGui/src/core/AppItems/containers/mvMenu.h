#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvMenu, MV_ITEM_DESC_CONTAINER, StorageValueTypes::Bool, 1);
	class mvMenu : public mvBoolPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvMenu, add_menu)

		MV_CREATE_CONSTANT(mvThemeCol_Menu_Text, ImGuiCol_Text, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Menu_Bg, ImGuiCol_PopupBg, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Menu_BgHovered, ImGuiCol_HeaderHovered, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Menu_BgActive, ImGuiCol_Header, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Menu_Border, ImGuiCol_Border, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_Menu_BorderSize, ImGuiStyleVar_PopupBorderSize, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Menu_Rounding, ImGuiStyleVar_PopupRounding, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Menu_PaddingX, ImGuiStyleVar_WindowPadding, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Menu_PaddingY, ImGuiStyleVar_WindowPadding, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Menu_ItemSpacingX, ImGuiStyleVar_ItemSpacing, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Menu_ItemSpacingY, ImGuiStyleVar_ItemSpacing, 1L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Menu_Text, mvImGuiCol_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Menu_Bg, mvImGuiCol_PopupBg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Menu_BgHovered, mvImGuiCol_HeaderHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Menu_BgActive, mvImGuiCol_ButtonHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Menu_Border, mvImGuiCol_Border),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT_F(mvThemeStyle_Menu_BorderSize, 1,  1),
			MV_ADD_CONSTANT_F(mvThemeStyle_Menu_Rounding, 0, 12),
			MV_ADD_CONSTANT_F(mvThemeStyle_Menu_PaddingX, 8, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_Menu_PaddingY, 8, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_Menu_ItemSpacingX, 8, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_Menu_ItemSpacingY, 4, 20),
		MV_END_STYLE_CONSTANTS

	public:

		mvMenu(const std::string& name);

		void draw(ImDrawList* drawlist, float x, float y) override;

		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	};

}