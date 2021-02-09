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