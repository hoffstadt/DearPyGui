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

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Child_Bg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Child_MenuBar),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Child_Border),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Child_Scrollbar),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Child_ScrollbarGrab),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Child_ScrollbarGrabHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Child_ScrollbarGrabActive),
		MV_END_COLOR_CONSTANTS

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