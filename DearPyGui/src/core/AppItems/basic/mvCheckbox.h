#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	struct mvCheckboxConfig : public mvAppItemConfig
	{
		bool default_value = false;
		std::string source = "";
	};

#ifdef MV_CPP
	void add_checkbox(const char* name, const mvCheckboxConfig& config = {});
#else
	PyObject* add_checkbox(PyObject* self, PyObject* args, PyObject* kwargs);
#endif

	class mvCheckbox : public mvBoolPtrBase
	{

		MV_APPITEM_TYPE(mvAppItemType::Checkbox, mvCheckbox, "add_checkbox")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::Checkbox, mvThemeCol_CheckBox_Text			,  0L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Checkbox, mvThemeCol_CheckBox_Bg			,  7L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Checkbox, mvThemeCol_CheckBox_BgHovered		,  8L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Checkbox, mvThemeCol_CheckBox_BgActive		, 18L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Checkbox, mvThemeCol_CheckBox_Border		,  5L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Checkbox, mvThemeCol_CheckBox_BorderShadow	,  6L, 0L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_CheckBox_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_CheckBox_Bg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_CheckBox_BgHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_CheckBox_BgActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_CheckBox_Border),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_CheckBox_BorderShadow),
		MV_END_COLOR_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvCheckbox(const std::string& name, bool default_value, const std::string& dataSource);
		mvCheckbox(const std::string& name, const mvCheckboxConfig& config);

		void draw() override;

		// cpp interface
		void updateConfig(mvAppItemConfig* config) override;
		mvAppItemConfig* getConfig() override;

	private:

		mvCheckboxConfig m_config;

	};

}
