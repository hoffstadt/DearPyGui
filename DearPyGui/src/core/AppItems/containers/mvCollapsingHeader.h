#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	struct mvCollapsingHeaderConfig : public mvAppItemConfig
	{
		bool closable = false;
		bool default_open = false;
		bool open_on_double_click = false;
		bool open_on_arrow = false;
		bool leaf = false;
		bool bullet = false;
	};

	PyObject* add_collapsing_header(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvCollapsingHeader : public mvBoolPtrBase
	{

		MV_APPITEM_TYPE(mvAppItemType::CollapsingHeader, mvCollapsingHeader, "add_collapsing_header")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::CollapsingHeader, mvThemeCol_CollapsingHeader_Text			,  0L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::CollapsingHeader, mvThemeCol_CollapsingHeader_Bg			, 24L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::CollapsingHeader, mvThemeCol_CollapsingHeader_BgHovered		, 25L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::CollapsingHeader, mvThemeCol_CollapsingHeader_BgActive		, 26L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::CollapsingHeader, mvThemeCol_CollapsingHeader_Border		,  5L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::CollapsingHeader, mvThemeCol_CollapsingHeader_BorderShadow	,  6L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::CollapsingHeader, mvThemeStyle_CollapsingHeader_BorderSize	, 12L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::CollapsingHeader, mvThemeStyle_CollapsingHeader_Rounding	, 11L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::CollapsingHeader, mvThemeStyle_CollapsingHeader_PaddingX	, 10L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::CollapsingHeader, mvThemeStyle_CollapsingHeader_PaddingY	, 10L, 1L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::CollapsingHeader, mvThemeStyle_CollapsingHeader_ItemSpacingX, 13L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::CollapsingHeader, mvThemeStyle_CollapsingHeader_ItemSpacingY, 13L, 1L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_CollapsingHeader_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_CollapsingHeader_Bg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_CollapsingHeader_BgHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_CollapsingHeader_BgActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_CollapsingHeader_Border),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_CollapsingHeader_BorderShadow),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_CollapsingHeader_BorderSize	, 0, 1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_CollapsingHeader_Rounding		, 0, 12),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_CollapsingHeader_PaddingX		, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_CollapsingHeader_PaddingY		, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_CollapsingHeader_ItemSpacingX	, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_CollapsingHeader_ItemSpacingY	, 0, 20),
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvCollapsingHeader(const std::string& name);

		void draw()               override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

	private:

		ImGuiTreeNodeFlags m_flags = ImGuiTreeNodeFlags_None;
		bool               m_closable = false;

	};

}