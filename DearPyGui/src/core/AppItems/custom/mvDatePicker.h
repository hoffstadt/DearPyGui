#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	PyObject* add_date_picker(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvDatePicker : public mvTimePtrBase
	{

		MV_APPITEM_TYPE(mvAppItemType::DatePicker, mvDatePicker, "add_date_picker")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::DatePicker, mvThemeCol_DatePicker_Date			,  0L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::DatePicker, mvThemeCol_DatePicker_DateDisabled	,  1L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::DatePicker, mvThemeCol_DatePicker_DateHovered	, 22L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::DatePicker, mvThemeCol_DatePicker_DateActive	, 23L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::DatePicker, mvThemeCol_DatePicker_Border		,  5L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::DatePicker, mvThemeCol_DatePicker_BorderShadow	,  6L, 0L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_DatePicker_Date),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_DatePicker_DateDisabled),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_DatePicker_DateHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_DatePicker_DateActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_DatePicker_Border),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_DatePicker_BorderShadow),
		MV_END_COLOR_CONSTANTS

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