#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	struct mvRadioButtonConfig : public mvAppItemConfig
	{
		std::vector<std::string> items;
		int default_value = 0;
		bool horizontal = false;
	};

	PyObject* add_radio_button(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvRadioButton : public mvIntPtrBase
	{
		MV_APPITEM_TYPE(mvAppItemType::RadioButtons, mvRadioButton, "add_radio_button")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::RadioButtons, mvThemeCol_RadioButton_Text			,  0L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::RadioButtons, mvThemeCol_RadioButton_Bg				,  7L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::RadioButtons, mvThemeCol_RadioButton_BgHovered		,  8L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::RadioButtons, mvThemeCol_RadioButton_BgActive		, 18L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::RadioButtons, mvThemeCol_RadioButton_Border			,  5L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::RadioButtons, mvThemeCol_RadioButton_BorderShadow	,  6L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::RadioButtons, mvThemeStyle_RadioButton_BorderSize	, 12L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::RadioButtons, mvThemeStyle_RadioButton_PaddingX		, 10L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::RadioButtons, mvThemeStyle_RadioButton_PaddingY		, 10L, 1L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::RadioButtons, mvThemeStyle_RadioButton_InnerSpacingX, 14L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::RadioButtons, mvThemeStyle_RadioButton_InnerSpacingY, 14L, 1L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_RadioButton_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_RadioButton_Bg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_RadioButton_BgHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_RadioButton_BgActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_RadioButton_Border),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_RadioButton_BorderShadow),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_RadioButton_BorderSize	, 0,  1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_RadioButton_PaddingX		, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_RadioButton_PaddingY		, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_RadioButton_InnerSpacingX	, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_RadioButton_InnerSpacingY	, 0, 20),
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvRadioButton(const std::string& name, int default_value, const std::string& dataSource);

		void draw()               override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

	private:

		std::vector<std::string> m_itemnames;
		bool                     m_horizontal = false;

	};

}
