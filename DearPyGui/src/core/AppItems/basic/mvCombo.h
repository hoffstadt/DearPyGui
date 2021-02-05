#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	PyObject* add_combo(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvCombo : public mvStringPtrBase
	{
		MV_APPITEM_TYPE(mvAppItemType::Combo, mvCombo, "add_combo")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::Combo, mvThemeCol_Combo_Text                ,  0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Combo, mvThemeCol_Combo_Selected            , 24L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Combo, mvThemeCol_Combo_Hovered             , 25L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Combo, mvThemeCol_Combo_Active              , 26L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Combo, mvThemeCol_Combo_Bg                  ,  7L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Combo, mvThemeCol_Combo_BgHovered           ,  8L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Combo, mvThemeCol_Combo_DropBg              ,  4L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Combo, mvThemeCol_Combo_DropButtonBg        , 21L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Combo, mvThemeCol_Combo_DropButtonHovered   , 22L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Combo, mvThemeCol_Combo_Scrollbar           , 14L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Combo, mvThemeCol_Combo_ScrollbarGrab       , 15L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Combo, mvThemeCol_Combo_ScrollbarGrabHovered, 16L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Combo, mvThemeCol_Combo_ScrollbarGrabActive , 17L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Combo, mvThemeCol_Combo_Border              ,  5L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Combo, mvThemeCol_Combo_BorderShadow        ,  6L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Combo_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Combo_Selected),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Combo_Hovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Combo_Active),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Combo_Bg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Combo_BgHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Combo_DropBg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Combo_DropButtonBg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Combo_DropButtonHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Combo_Scrollbar),           
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Combo_ScrollbarGrab),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Combo_ScrollbarGrabHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Combo_ScrollbarGrabActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Combo_Border),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Combo_BorderShadow),
		MV_END_COLOR_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvCombo(const std::string& name, const std::string& default_value, const std::string& dataSource);

		void draw()               override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

	private:
		ImGuiComboFlags m_flags = ImGuiComboFlags_None;
		std::vector<std::string> m_items;
	};

}
