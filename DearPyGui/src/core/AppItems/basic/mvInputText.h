#pragma once

#include "mvApp.h"
#include "mvTypeBases.h"

namespace Marvel {

	struct mvInputTextConfig : public mvAppItemConfig
	{
		std::string default_value = "";
		std::string hint = "";
		bool multiline = false;
		bool no_spaces = false;
		bool uppercase = false;
		bool tab_input = false;
		bool decimal = false;
		bool hexadecimal = false;
		bool readonly = false;
		bool password = false;
		bool scientific = false;
		bool on_enter = false;
	};

	

	MV_REGISTER_WIDGET(mvInputText);
	class mvInputText : public mvStringPtrBase
	{

		MV_APPITEM_TYPE(mvAppItemType::mvInputText, add_input_text)

		MV_CREATE_CONSTANT(mvThemeCol_InputText_Text,             0L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_InputText_TextHighlight,   44L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_InputText_Bg,               7L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_InputText_Hint,             1L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_InputText_Border,           5L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_InputText_BorderShadow,     6L, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_InputText_Rounding,      11L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_InputText_BorderSize,    12L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_InputText_PaddingX,      10L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_InputText_PaddingY,      10L, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_InputText_InnerSpacingX, 14L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_InputText_InnerSpacingY, 14L, 1L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_ADD_CONSTANT(mvThemeCol_InputText_Text,			mvColor(255, 255, 255, 255), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_InputText_TextHighlight,mvColor( 66, 150, 250,  89), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_InputText_Bg,			mvColor( 41,  74, 122, 138), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_InputText_Hint,			mvColor(128, 128, 128, 255), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_InputText_Border,		mvColor(110, 110, 128, 128), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_InputText_BorderShadow,	mvColor(  0,   0,   0,   0), mvColor(128, 128, 128, 63)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT(mvThemeStyle_InputText_Rounding		, 0, 12),
			MV_ADD_CONSTANT(mvThemeStyle_InputText_BorderSize		, 0,  1),
			MV_ADD_CONSTANT(mvThemeStyle_InputText_PaddingX		, 4, 20),
			MV_ADD_CONSTANT(mvThemeStyle_InputText_PaddingY		, 3, 20),
			MV_ADD_CONSTANT(mvThemeStyle_InputText_InnerSpacingX	, 4, 20),
			MV_ADD_CONSTANT(mvThemeStyle_InputText_InnerSpacingY	, 4, 20),
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvInputText(const std::string& name, const std::string& default_value, const std::string& dataSource);

		void setEnabled        (bool value)     override;
		void draw(ImDrawList* drawlist, float x, float y) override;

		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

		// cpp interface
		void updateConfig(mvAppItemConfig* config) override;
		mvAppItemConfig* getConfig() override;

	private:

		std::string         m_hint;
		bool                m_multiline = false;
		ImGuiInputTextFlags m_flags = 0;
		ImGuiInputTextFlags m_stor_flags = 0;
		mvInputTextConfig m_config;

	};

}