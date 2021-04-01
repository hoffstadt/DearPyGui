#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvSelectable);
	class mvSelectable : public mvBoolPtrBase
	{

		MV_APPITEM_TYPE(mvAppItemType::mvSelectable, add_selectable)

		MV_CREATE_CONSTANT(mvThemeCol_Selectable_Text,		 0L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Selectable_Bg,			24L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Selectable_BgHovered,	25L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Selectable_BgActive,	26L, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_Selectable_TextAlignX,	23L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Selectable_TextAlignY,	23L, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Selectable_ItemSpacingX,	13L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Selectable_ItemSpacingY,	13L, 1L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_ADD_CONSTANT(mvThemeCol_Selectable_Text,			mvColor(255, 255, 255, 255), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_Selectable_Bg,				mvColor( 41,  74, 122, 138), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_Selectable_BgHovered,		mvColor( 66, 150, 250, 102), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_Selectable_BgActive,		mvColor( 66, 150, 250, 171), mvColor(128, 128, 128, 63)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT(mvThemeStyle_Selectable_TextAlignX,	0,  1),
			MV_ADD_CONSTANT(mvThemeStyle_Selectable_TextAlignY,	0,  1),
			MV_ADD_CONSTANT(mvThemeStyle_Selectable_ItemSpacingX,	8, 20),
			MV_ADD_CONSTANT(mvThemeStyle_Selectable_ItemSpacingY,	4, 20),
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvSelectable(const std::string& name, bool default_value, const std::string& dataSource);

		void setEnabled(bool value)     override;
		void draw(ImDrawList* drawlist, float x, float y)               override;

		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:

		ImGuiSelectableFlags m_flags = ImGuiSelectableFlags_None;
	};

}
