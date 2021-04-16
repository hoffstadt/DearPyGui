#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvTabButton, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 1);
	class mvTabButton : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvTabButton, add_tab_button)

		MV_CREATE_CONSTANT(mvThemeCol_TabButton_Text, ImGuiCol_Text, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_TabButton_Bg, ImGuiCol_Tab, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_TabButton_BgHovered, ImGuiCol_TabHovered, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_TabButton_Border, ImGuiCol_Border, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_TabButton_PopupBg, ImGuiCol_PopupBg, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_TabButton_Rounding, ImGuiStyleVar_FrameRounding, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_TabButton_BorderSize, ImGuiStyleVar_FrameBorderSize, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_TabButton_PaddingX, ImGuiStyleVar_FramePadding, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_TabButton_PaddingY, ImGuiStyleVar_FramePadding, 1L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TabButton_Text, mvImGuiCol_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TabButton_Bg, mvImGuiCol_FrameBg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TabButton_BgHovered,	mvImGuiCol_FrameBgHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TabButton_Border, mvImGuiCol_Border),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TabButton_PopupBg, mvImGuiCol_PopupBg),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT_F(mvThemeStyle_TabButton_Rounding, 0, 12),
			MV_ADD_CONSTANT_F(mvThemeStyle_TabButton_BorderSize, 0,  1),
			MV_ADD_CONSTANT_F(mvThemeStyle_TabButton_PaddingX, 4, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_TabButton_PaddingY, 3, 20),
		MV_END_STYLE_CONSTANTS

	public:

		mvTabButton(const std::string& name);

		void draw(ImDrawList* drawlist, float x, float y) override;

		bool isParentCompatible(mvAppItemType type) override;

		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:

		ImGuiTabItemFlags m_flags = ImGuiTabItemFlags_None;

	};
}