#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	PyObject* add_date_picker(PyObject* self, PyObject* args, PyObject* kwargs);

	MV_REGISTER_WIDGET(mvDatePicker);
	class mvDatePicker : public mvTimePtrBase
	{

		MV_APPITEM_TYPE(mvAppItemType::mvDatePicker, "add_date_picker")

		MV_CREATE_THEME_CONSTANT(mvThemeCol_DatePicker_Date			,  0L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_DatePicker_DateDisabled	,  1L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_DatePicker_DateHovered	, 22L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_DatePicker_DateActive	, 23L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_DatePicker_Border		,  5L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_DatePicker_BorderShadow	,  6L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_DatePicker_Rounding	, 11L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_DatePicker_BorderSize	, 12L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_DatePicker_PaddingX	, 10L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_DatePicker_PaddingY	, 10L, 1L);


		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_DatePicker_Date,			mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_DatePicker_DateDisabled, mvColor(128, 128, 128, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_DatePicker_DateHovered,	mvColor( 66, 150, 250, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_DatePicker_DateActive,	mvColor( 15, 135, 250, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_DatePicker_Border,		mvColor(110, 110, 128, 128)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_DatePicker_BorderShadow, mvColor(  0,   0,   0,   0)),
		MV_END_COLOR_CONSTANTS

			MV_START_STYLE_CONSTANTS
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DatePicker_Rounding	, 0, 12),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DatePicker_BorderSize	, 0,  1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DatePicker_PaddingX	, 4, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DatePicker_PaddingY	, 3, 20),
			MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvDatePicker(const std::string& name, tm default_value);

		void draw              ()               override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

	private:

		int m_level = 0;

	};

}