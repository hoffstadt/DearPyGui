#pragma once

#include "mvTypeBases.h"
#include <vector>

namespace Marvel {

	MV_REGISTER_WIDGET(mvTable);
	class mvTable : public mvAppItem
	{
		MV_APPITEM_TYPE(mvAppItemType::mvTable, add_table)

		MV_CREATE_THEME_CONSTANT(mvTable_SizingFixedFit,			8192L, 0L);
		MV_CREATE_THEME_CONSTANT(mvTable_SizingFixedSame,			16384L, 0L);
		MV_CREATE_THEME_CONSTANT(mvTable_SizingStretchProp,			24576L, 0L);
		MV_CREATE_THEME_CONSTANT(mvTable_SizingStretchSame,			32768L, 0L);

		MV_CREATE_THEME_CONSTANT(mvThemeCol_Table_HeaderBg,			44L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Table_BorderStrong,		45L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Table_BorderLight,		46L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Table_BorderHovered,	28L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Table_BorderActive,		29L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Table_RowBg,			47L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Table_RowBgAlt,			48L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Table_CellPaddingX,	16L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Table_CellPaddingY,	16L, 1L);

		MV_START_GENERAL_CONSTANTS
			MV_CREATE_CONSTANT_SINGLE(mvTable_SizingFixedFit),
			MV_CREATE_CONSTANT_SINGLE(mvTable_SizingFixedSame),
			MV_CREATE_CONSTANT_SINGLE(mvTable_SizingStretchProp),
			MV_CREATE_CONSTANT_SINGLE(mvTable_SizingStretchSame)
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
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Table_CellPaddingX, 4, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Table_CellPaddingY, 2, 20),
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvTable(const std::string& name, int inner_width);

		void draw() override;

		void incrementColumns();
		void decrementColumns();
		const std::string& getLastColumnAdded() const;
		void setLastColumnAdded(const std::string& name);

		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:

		int  m_columns = 0;
		int  m_inner_width = 0;
		ImGuiTableFlags m_flags = 0;
		bool m_tableHeader = true;
		
		// this keeps track of the last column so
		// addAfter() can use this to place
		// columns correctly.
		std::string m_lastColumnAdded;
	};

}
