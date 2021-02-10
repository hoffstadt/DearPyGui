#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	PyObject* add_child(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvChild : public mvBoolPtrBase
	{

		MV_APPITEM_TYPE(mvAppItemType::Child, mvChild, "add_child")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::Child, mvThemeCol_Child_Bg						,  3L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Child, mvThemeCol_Child_MenuBar					, 13L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Child, mvThemeCol_Child_Border					,  5L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Child, mvThemeCol_Child_Scrollbar				, 14L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Child, mvThemeCol_Child_ScrollbarGrab			, 15L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Child, mvThemeCol_Child_ScrollbarGrabHovered	, 16L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Child, mvThemeCol_Child_ScrollbarGrabActive		, 17L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Child, mvThemeStyle_Child_BorderSize				,  7L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Child, mvThemeStyle_Child_Rounding				,  6L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Child, mvThemeStyle_Child_PaddingX				,  1L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Child, mvThemeStyle_Child_PaddingY				,  1L, 1L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Child, mvThemeStyle_Child_ItemSpacingX			, 13L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Child, mvThemeStyle_Child_ItemSpacingY			, 13L, 1L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Child, mvThemeStyle_Child_ScrollbarSize			, 17L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Child, mvThemeStyle_Child_ScrollbarRounding		, 18L, 0L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Child_Bg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Child_MenuBar),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Child_Border),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Child_Scrollbar),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Child_ScrollbarGrab),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Child_ScrollbarGrabHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Child_ScrollbarGrabActive),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Child_BorderSize			, 0, 1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Child_Rounding			, 0, 12),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Child_PaddingX			, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Child_PaddingY			, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Child_ItemSpacingX		, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Child_ItemSpacingY		, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Child_ScrollbarSize		, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Child_ScrollbarRounding	, 0, 12),
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvChild(const std::string& name);

		void draw()               override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

		void addFlag           (ImGuiWindowFlags flag);
		void removeFlag        (ImGuiWindowFlags flag);

	private:

		bool             m_border     = true;
		bool             m_autosize_x = false;
		bool             m_autosize_y = false;
		ImGuiWindowFlags m_windowflags = ImGuiWindowFlags_NoSavedSettings;

	};

}