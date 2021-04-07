#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvCollapsingHeader, MV_ITEM_DESC_CONTAINER, StorageValueTypes::Bool, 1);
	class mvCollapsingHeader : public mvBoolPtrBase
	{
	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION2(mvAppItemType::mvCollapsingHeader, add_collapsing_header)

		MV_CREATE_CONSTANT(mvThemeCol_CollapsingHeader_Text		    , ImGuiCol_Text			, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_CollapsingHeader_Bg			, ImGuiCol_Header		, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_CollapsingHeader_BgHovered	, ImGuiCol_HeaderHovered, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_CollapsingHeader_BgActive		, ImGuiCol_HeaderActive	, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_CollapsingHeader_Border		, ImGuiCol_Border		, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_CollapsingHeader_BorderShadow	, ImGuiCol_BorderShadow	, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_CollapsingHeader_BorderSize		, ImGuiStyleVar_FrameBorderSize	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_CollapsingHeader_Rounding	    , ImGuiStyleVar_FrameRounding	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_CollapsingHeader_PaddingX	    , ImGuiStyleVar_FramePadding	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_CollapsingHeader_PaddingY	    , ImGuiStyleVar_FramePadding	, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_CollapsingHeader_ItemSpacingX	, ImGuiStyleVar_ItemSpacing		, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_CollapsingHeader_ItemSpacingY	, ImGuiStyleVar_ItemSpacing		, 1L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_CollapsingHeader_Text,			mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_CollapsingHeader_Bg,				mvColor( 66, 150, 250, 79)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_CollapsingHeader_BgHovered,		mvColor( 66, 150, 250, 204)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_CollapsingHeader_BgActive,		mvColor( 66, 150, 250, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_CollapsingHeader_Border,			mvColor(110, 110, 128, 128)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_CollapsingHeader_BorderShadow,	mvColor(  0,   0,   0,   0)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT(mvThemeStyle_CollapsingHeader_BorderSize	, 0,  1),
			MV_ADD_CONSTANT(mvThemeStyle_CollapsingHeader_Rounding		, 0, 12),
			MV_ADD_CONSTANT(mvThemeStyle_CollapsingHeader_PaddingX		, 4, 20),
			MV_ADD_CONSTANT(mvThemeStyle_CollapsingHeader_PaddingY		, 3, 20),
			MV_ADD_CONSTANT(mvThemeStyle_CollapsingHeader_ItemSpacingX	, 8, 20),
			MV_ADD_CONSTANT(mvThemeStyle_CollapsingHeader_ItemSpacingY	, 4, 20),
		MV_END_STYLE_CONSTANTS

	public:

		mvCollapsingHeader(const std::string& name);

		void draw(ImDrawList* drawlist, float x, float y)               override;

		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:

		ImGuiTreeNodeFlags m_flags = ImGuiTreeNodeFlags_None;
		bool               m_closable = false;

	};

}