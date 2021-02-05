#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	PyObject* add_radio_button(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvRadioButton : public mvIntPtrBase
	{
		MV_APPITEM_TYPE(mvAppItemType::RadioButtons, mvRadioButton, "add_radio_button")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::RadioButtons, mvThemeCol_RadioButton_Text			, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::RadioButtons, mvThemeCol_RadioButton_Bg				, 7L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::RadioButtons, mvThemeCol_RadioButton_BgHovered		, 8L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::RadioButtons, mvThemeCol_RadioButton_BgActive		, 18L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::RadioButtons, mvThemeCol_RadioButton_Border			, 5L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::RadioButtons, mvThemeCol_RadioButton_BorderShadow	, 6L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_RadioButton_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_RadioButton_Bg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_RadioButton_BgHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_RadioButton_BgActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_RadioButton_Border),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_RadioButton_BorderShadow),
		MV_END_COLOR_CONSTANTS

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
