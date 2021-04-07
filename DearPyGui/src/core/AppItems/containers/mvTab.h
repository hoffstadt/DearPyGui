#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvTab, MV_ITEM_DESC_CONTAINER, StorageValueTypes::Bool, 1);
	class mvTab : public mvBoolPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvTab, add_tab)

		MV_CREATE_CONSTANT(mvThemeCol_Tab_Text			, ImGuiCol_Text			, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Tab_Border		, ImGuiCol_Border		, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Tab_Bg			, ImGuiCol_Tab			, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Tab_BgHovered		, ImGuiCol_TabHovered	, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Tab_BgActive		, ImGuiCol_TabActive	, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Tab_PopupBg		, ImGuiCol_PopupBg		, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_Tab_Rounding		, ImGuiStyleVar_TabRounding		, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Tab_PaddingX		, ImGuiStyleVar_FramePadding	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Tab_PaddingY		, ImGuiStyleVar_FramePadding	, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Tab_InnerSpacingX	, ImGuiStyleVar_ItemInnerSpacing, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Tab_InnerSpacingY	, ImGuiStyleVar_ItemInnerSpacing, 1L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Tab_Text,		mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Tab_Bg,			mvColor( 46,  89, 148, 220)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Tab_BgHovered,	mvColor( 66, 150, 250, 204)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Tab_BgActive,	mvColor( 51, 105, 173, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Tab_Border,		mvColor(110, 110, 128, 128)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Tab_PopupBg,		mvColor( 20,  20,  20, 240)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT(mvThemeStyle_Tab_Rounding		, 4, 12),
			MV_ADD_CONSTANT(mvThemeStyle_Tab_PaddingX		, 4, 20),
			MV_ADD_CONSTANT(mvThemeStyle_Tab_PaddingY		, 3, 20),
			MV_ADD_CONSTANT(mvThemeStyle_Tab_InnerSpacingX	, 4, 20),
			MV_ADD_CONSTANT(mvThemeStyle_Tab_InnerSpacingY	, 4, 20),
		MV_END_STYLE_CONSTANTS

	public:

		mvTab(const std::string& name);

		void draw(ImDrawList* drawlist, float x, float y) override;
		void addFlag   (ImGuiTabItemFlags flag);
		void removeFlag(ImGuiTabItemFlags flag);

		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:

		bool              m_closable = false;
		ImGuiTabItemFlags m_flags = ImGuiTabItemFlags_None;

	};

}