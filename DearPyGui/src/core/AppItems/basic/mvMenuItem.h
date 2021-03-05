#pragma once

#include "mvTypeBases.h"
#include "mvApp.h"

namespace Marvel {

	struct mvMenuItemConfig : public mvAppItemConfig
	{
		std::string shortcut = "";
		bool check = false;

	};

	PyObject* add_menu_item(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvMenuItem : public mvBoolPtrBase
	{
		MV_APPITEM_TYPE(mvAppItemType::MenuItem, mvMenuItem, "add_menu_item")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::MenuItem, mvThemeCol_MenuItem_Text			,  0L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::MenuItem, mvThemeCol_MenuItem_BgHovered		, 25L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::MenuItem, ImGuiStyleVar_MenuItem_TextAlignX	, 23L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::MenuItem, ImGuiStyleVar_MenuItem_TextAlignY	, 23L, 1L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_MenuItem_Text,		mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_MenuItem_BgHovered,	mvColor( 20,  20,  20, 240)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_CREATE_CONSTANT_TUPLE(ImGuiStyleVar_MenuItem_TextAlignX, 0, 1),
			MV_CREATE_CONSTANT_TUPLE(ImGuiStyleVar_MenuItem_TextAlignY, 0, 1),
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		explicit mvMenuItem(const std::string& name);

		void draw() override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

		// cpp interface
		void updateConfig(mvAppItemConfig* config) override;
		mvAppItemConfig* getConfig() override;

	private:

		std::string m_shortcut;
		bool        m_check = false;
		mvMenuItemConfig m_config;

	};

}