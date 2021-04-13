#pragma once

#include "mvTypeBases.h"
#include <vector>

namespace Marvel {

	MV_REGISTER_WIDGET(mvTable, MV_ITEM_DESC_CONTAINER, StorageValueTypes::None, 1);
	class mvTable : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvTable, add_table)

		MV_CREATE_CONSTANT(mvTable_SizingFixedFit,	8192L, 0L);
		MV_CREATE_CONSTANT(mvTable_SizingFixedSame,	16384L, 0L);
		MV_CREATE_CONSTANT(mvTable_SizingStretchProp,	24576L, 0L);
		MV_CREATE_CONSTANT(mvTable_SizingStretchSame,	32768L, 0L);

		MV_CREATE_CONSTANT(mvThemeCol_Table_HeaderBg		, ImGuiCol_TableHeaderBg	, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Table_BorderStrong	, ImGuiCol_TableBorderStrong, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Table_BorderLight		, ImGuiCol_TableBorderLight	, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Table_BorderHovered	, ImGuiCol_SeparatorHovered	, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Table_BorderActive	, ImGuiCol_SeparatorActive	, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Table_RowBg			, ImGuiCol_TableRowBg		, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Table_RowBgAlt		, ImGuiCol_TableRowBgAlt	, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_Table_CellPaddingX, ImGuiStyleVar_CellPadding, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Table_CellPaddingY, ImGuiStyleVar_CellPadding, 1L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
			MV_ADD_GENERAL_CONSTANT(mvTable_SizingFixedFit),
			MV_ADD_GENERAL_CONSTANT(mvTable_SizingFixedSame),
			MV_ADD_GENERAL_CONSTANT(mvTable_SizingStretchProp),
			MV_ADD_GENERAL_CONSTANT(mvTable_SizingStretchSame)
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Table_HeaderBg,		mvColor( 48,  48,  51, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Table_BorderStrong,	mvColor( 79,  79,  89, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Table_BorderLight,	mvColor( 59,  59,  64, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Table_BorderHovered, mvColor( 26, 102, 191, 199)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Table_BorderActive,	mvColor( 26, 102, 191, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Table_RowBg,			mvColor(  0,   0,   0,   0)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Table_RowBgAlt,		mvColor(255, 255, 255,  15))
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT_F(mvThemeStyle_Table_CellPaddingX, 4, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_Table_CellPaddingY, 2, 20),
		MV_END_STYLE_CONSTANTS

	public:

		mvTable(const std::string& name);

		void draw(ImDrawList* drawlist, float x, float y) override;

		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

		void onChildAdd(mvRef<mvAppItem> item) override;
		void onChildRemoved(mvRef<mvAppItem> item) override;
		void onChildrenRemoved() override;

	private:

		int  m_columns = 0;
		int  m_inner_width = 0;
		ImGuiTableFlags m_flags = 0;
		bool m_tableHeader = true;
		
	};

}
