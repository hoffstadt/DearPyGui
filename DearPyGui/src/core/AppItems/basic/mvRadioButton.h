#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	struct mvRadioButtonConfig : public mvAppItemConfig
	{
		std::vector<std::string> items;
		int default_value = 0;
		bool horizontal = false;
	};

	MV_REGISTER_WIDGET(mvRadioButton);
	class mvRadioButton : public mvIntPtrBase
	{

		MV_APPITEM_TYPE(mvAppItemType::mvRadioButton, add_radio_button)

		MV_CREATE_CONSTANT(mvThemeCol_RadioButton_Text,             0L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_RadioButton_Bg,               7L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_RadioButton_BgHovered,        8L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_RadioButton_BgActive,        18L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_RadioButton_Border,           5L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_RadioButton_BorderShadow,     6L, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_RadioButton_BorderSize,    12L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_RadioButton_PaddingX,      10L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_RadioButton_PaddingY,      10L, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_RadioButton_InnerSpacingX, 14L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_RadioButton_InnerSpacingY, 14L, 1L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_ADD_CONSTANT(mvThemeCol_RadioButton_Text,			mvColor(255, 255, 255, 255), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_RadioButton_Bg,				mvColor( 41,  74, 122, 138), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_RadioButton_BgHovered,		mvColor( 66, 150, 250, 102), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_RadioButton_BgActive,		mvColor( 66, 150, 250, 171), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_RadioButton_Border,			mvColor(110, 110, 128, 128), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_RadioButton_BorderShadow,	mvColor(  0,   0,   0,   0), mvColor(128, 128, 128, 63)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT(mvThemeStyle_RadioButton_BorderSize,    0,  1),
			MV_ADD_CONSTANT(mvThemeStyle_RadioButton_PaddingX,      4, 20),
			MV_ADD_CONSTANT(mvThemeStyle_RadioButton_PaddingY,      3, 20),
			MV_ADD_CONSTANT(mvThemeStyle_RadioButton_InnerSpacingX, 4, 20),
			MV_ADD_CONSTANT(mvThemeStyle_RadioButton_InnerSpacingY, 4, 20),
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvRadioButton(const std::string& name, int default_value, const std::string& dataSource);

		void draw(ImDrawList* drawlist, float x, float y)               override;

		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

		// cpp interface
		void updateConfig(mvAppItemConfig* config) override;
		mvAppItemConfig* getConfig() override;

	private:

		std::vector<std::string> m_itemnames;
		bool                     m_horizontal = false;
		mvRadioButtonConfig		 m_config;
	};

}
