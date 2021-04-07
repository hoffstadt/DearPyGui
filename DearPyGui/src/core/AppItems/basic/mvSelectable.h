#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvSelectable, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Bool, 1);
	class mvSelectable : public mvBoolPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION2(mvAppItemType::mvSelectable, add_selectable)

		MV_CREATE_CONSTANT(mvThemeCol_Selectable_Text		, ImGuiCol_Text			, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Selectable_Bg			, ImGuiCol_Header		, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Selectable_BgHovered	, ImGuiCol_HeaderHovered, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Selectable_BgActive	, ImGuiCol_HeaderActive	, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_Selectable_TextAlignX	, ImGuiStyleVar_SelectableTextAlign	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Selectable_TextAlignY	, ImGuiStyleVar_SelectableTextAlign	, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Selectable_ItemSpacingX	, ImGuiStyleVar_ItemSpacing			, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Selectable_ItemSpacingY	, ImGuiStyleVar_ItemSpacing			, 1L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_ADD_CONSTANT(mvThemeCol_Selectable_Text,			mvColor(255, 255, 255, 255), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_Selectable_Bg,			mvColor( 41,  74, 122, 138), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_Selectable_BgHovered,	mvColor( 66, 150, 250, 102), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_Selectable_BgActive,		mvColor( 66, 150, 250, 171), mvColor(128, 128, 128, 63)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT(mvThemeStyle_Selectable_TextAlignX		, 0,  1),
			MV_ADD_CONSTANT(mvThemeStyle_Selectable_TextAlignY		, 0,  1),
			MV_ADD_CONSTANT(mvThemeStyle_Selectable_ItemSpacingX	, 8, 20),
			MV_ADD_CONSTANT(mvThemeStyle_Selectable_ItemSpacingY	, 4, 20),
		MV_END_STYLE_CONSTANTS

	public:

		mvSelectable(const std::string& name);

		void setEnabled(bool value)     override;
		void draw(ImDrawList* drawlist, float x, float y)               override;

		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:

		ImGuiSelectableFlags m_flags = ImGuiSelectableFlags_None;
	};

}
