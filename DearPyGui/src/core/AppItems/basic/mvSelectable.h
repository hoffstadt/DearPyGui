#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	struct mvSelectableConfig : public mvAppItemConfig
	{
		bool default_value = false;
		bool span_columns = false;
	};

	PyObject* add_selectable(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvSelectable : public mvBoolPtrBase
	{
		MV_APPITEM_TYPE(mvAppItemType::Selectable, mvSelectable, "add_selectable")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::Selectable, mvThemeCol_Selectable_Text			,  0L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Selectable, mvThemeCol_Selectable_Bg			, 24L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Selectable, mvThemeCol_Selectable_BgHovered		, 25L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Selectable, mvThemeCol_Selectable_BgActive		, 26L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Selectable, mvThemeStyle_Selectable_TextAlignX	, 23L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Selectable, mvThemeStyle_Selectable_TextAlignY	, 23L, 0L);


		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Selectable_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Selectable_Bg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Selectable_BgHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Selectable_BgActive),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
				MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Selectable_TextAlignX, 0, 20),
				MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Selectable_TextAlignY, 0, 20),
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvSelectable(const std::string& name, bool default_value, const std::string& dataSource);

		void setEnabled(bool value)     override;
		void draw()               override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

		// cpp interface
		void updateConfig(mvAppItemConfig* config) override;
		mvAppItemConfig* getConfig() override;

	private:
		ImGuiSelectableFlags m_flags = ImGuiSelectableFlags_None;
		mvSelectableConfig   m_config;
	};

}
