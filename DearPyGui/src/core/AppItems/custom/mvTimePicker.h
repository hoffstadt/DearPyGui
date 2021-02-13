#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	PyObject* add_time_picker(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvTimePicker : public mvTimePtrBase
	{

		MV_APPITEM_TYPE(mvAppItemType::TimePicker, mvTimePicker, "add_time_picker")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::TimePicker, mvThemeCol_TimePicker_Time					,  0L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TimePicker, mvThemeCol_TimePicker_TimeHovered			, 22L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TimePicker, mvThemeCol_TimePicker_DropDown				, 24L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TimePicker, mvThemeCol_TimePicker_DropDownHovered		, 25L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TimePicker, mvThemeCol_TimePicker_DropDownActive		, 26L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TimePicker, mvThemeCol_TimePicker_DropDownBg			,  4L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TimePicker, mvThemeCol_TimePicker_Scrollbar				, 14L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TimePicker, mvThemeCol_TimePicker_ScrollbarGrab			, 15L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TimePicker, mvThemeCol_TimePicker_ScrollbarGrabHovered	, 16L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TimePicker, mvThemeCol_TimePicker_ScrollbarGrabActive	, 17L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TimePicker, mvThemeCol_TimePicker_Border				,  5L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TimePicker, mvThemeCol_TimePicker_BorderShadow			,  6L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TimePicker, mvThemeStyle_TimePicker_BorderSize			, 12L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TimePicker, mvThemeStyle_TimePicker_Rounding			, 11L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TimePicker, mvThemeStyle_TimePicker_PaddingX			, 10L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TimePicker, mvThemeStyle_TimePicker_PaddingY			, 10L, 1L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TimePicker, mvThemeStyle_TimePicker_TextAlignX			, 22L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TimePicker, mvThemeStyle_TimePicker_TextAlignY			, 22L, 1L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TimePicker, mvThemeStyle_TimePicker_DropPaddingX		,  1L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TimePicker, mvThemeStyle_TimePicker_DropPaddingY		,  1L, 1L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TimePicker, mvThemeStyle_TimePicker_DropBorderSize		,  9L, 0L)
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TimePicker, mvThemeStyle_TimePicker_DropItemSpacingX	, 13L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TimePicker, mvThemeStyle_TimePicker_DropItemSpacingY	, 13L, 1L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TimePicker, mvThemeStyle_TimePicker_DropTextAlignX		, 23L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TimePicker, mvThemeStyle_TimePicker_DropTextAlignY		, 23L, 1L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TimePicker_Time),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TimePicker_TimeHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TimePicker_DropDown),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TimePicker_DropDownHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TimePicker_DropDownActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TimePicker_DropDownBg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TimePicker_Scrollbar),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TimePicker_ScrollbarGrab),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TimePicker_ScrollbarGrabHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TimePicker_ScrollbarGrabActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TimePicker_Border),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TimePicker_BorderShadow),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_TimePicker_BorderSize			, 0,  1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_TimePicker_Rounding			, 0, 12),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_TimePicker_PaddingX			, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_TimePicker_PaddingY			, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_TimePicker_TextAlignX			, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_TimePicker_TextAlignY			, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_TimePicker_DropPaddingX		, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_TimePicker_DropPaddingY		, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_TimePicker_DropBorderSize		, 0,  1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_TimePicker_DropItemSpacingX	, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_TimePicker_DropItemSpacingY	, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_TimePicker_DropTextAlignX		, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_TimePicker_DropTextAlignY		, 0, 20),
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvTimePicker(const std::string& name, tm default_value);

		void draw              ()               override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

	private:

		bool m_hour24 = false;

	};

}