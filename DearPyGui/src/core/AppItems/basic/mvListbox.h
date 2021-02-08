#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	PyObject* add_listbox(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvListbox : public mvIntPtrBase
	{

		MV_APPITEM_TYPE(mvAppItemType::Listbox, mvListbox, "add_listbox")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::Listbox, mvThemeCol_Listbox_Text                 ,  0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Listbox, mvThemeCol_Listbox_Selected             , 24L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Listbox, mvThemeCol_Listbox_Hovered              , 25L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Listbox, mvThemeCol_Listbox_Active               , 26L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Listbox, mvThemeCol_Listbox_Bg                   ,  7L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Listbox, mvThemeCol_Listbox_Border               ,  5L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Listbox, mvThemeCol_Listbox_Scrollbar            , 14L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Listbox, mvThemeCol_Listbox_ScrollbarGrab        , 15L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Listbox, mvThemeCol_Listbox_ScrollbarGrabHovered , 16L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Listbox, mvThemeCol_Listbox_ScrollbarGrabActive  , 17L);


		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Listbox_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Listbox_Selected),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Listbox_Hovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Listbox_Active),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Listbox_Bg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Listbox_Border),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Listbox_Scrollbar),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Listbox_ScrollbarGrab),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Listbox_ScrollbarGrabHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Listbox_ScrollbarGrabActive),
		MV_END_COLOR_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvListbox(const std::string& name, int default_value, const std::string& dataSource);

		void draw()               override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

	private:

		std::vector<std::string> m_names;
		int                      m_itemsHeight = 3; // number of items to show (default -1)
		std::vector<const char*> m_charNames;

	};

}
