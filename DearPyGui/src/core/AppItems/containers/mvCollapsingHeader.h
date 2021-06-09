#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvCollapsingHeader, MV_ITEM_DESC_CONTAINER, StorageValueTypes::Bool, 1);
	class mvCollapsingHeader : public mvBoolPtrBase
	{
	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvCollapsingHeader, add_collapsing_header)

		MV_CREATE_CONSTANT(mvThemeCol_CollapsingHeader_Text, ImGuiCol_Text, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_CollapsingHeader_Bg, ImGuiCol_Header, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_CollapsingHeader_BgHovered, ImGuiCol_HeaderHovered, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_CollapsingHeader_BgActive, ImGuiCol_HeaderActive, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_CollapsingHeader_Border, ImGuiCol_Border, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_CollapsingHeader_BorderShadow, ImGuiCol_BorderShadow, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_CollapsingHeader_BorderSize, ImGuiStyleVar_FrameBorderSize, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_CollapsingHeader_Rounding, ImGuiStyleVar_FrameRounding, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_CollapsingHeader_PaddingX, ImGuiStyleVar_FramePadding, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_CollapsingHeader_PaddingY, ImGuiStyleVar_FramePadding, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_CollapsingHeader_ItemSpacingX, ImGuiStyleVar_ItemSpacing, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_CollapsingHeader_ItemSpacingY, ImGuiStyleVar_ItemSpacing, 1L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_CollapsingHeader_Text, mvImGuiCol_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_CollapsingHeader_Bg, mvImGuiCol_Header),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_CollapsingHeader_BgHovered, mvImGuiCol_HeaderHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_CollapsingHeader_BgActive, mvImGuiCol_HeaderActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_CollapsingHeader_Border, mvImGuiCol_Border),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_CollapsingHeader_BorderShadow, mvImGuiCol_BorderShadow),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT_F(mvThemeStyle_CollapsingHeader_BorderSize, 0,  1),
			MV_ADD_CONSTANT_F(mvThemeStyle_CollapsingHeader_Rounding, 0, 12),
			MV_ADD_CONSTANT_F(mvThemeStyle_CollapsingHeader_PaddingX, 4, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_CollapsingHeader_PaddingY, 3, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_CollapsingHeader_ItemSpacingX, 8, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_CollapsingHeader_ItemSpacingY, 4, 20),
		MV_END_STYLE_CONSTANTS

	public:

		mvCollapsingHeader(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y)               override;

		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:

		ImGuiTreeNodeFlags m_flags = ImGuiTreeNodeFlags_None;
		bool               m_closable = false;

	};

}