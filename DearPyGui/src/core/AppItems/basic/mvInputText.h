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

	PyObject* add_input_text(PyObject* self, PyObject* args, PyObject* kwargs);

	MV_REGISTER_WIDGET(mvInputText);
	class mvInputText : public mvStringPtrBase
	{
		MV_APPITEM_TYPE(mvAppItemType::mvInputText, "add_input_text")

		MV_CREATE_THEME_CONSTANT(mvThemeCol_InputText_Text			,  0L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_InputText_TextHighlight	, 44L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_InputText_Bg				,  7L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_InputText_Hint			,  1L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_InputText_Border			,  5L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_InputText_BorderShadow	,  6L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputText_Rounding		, 11L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputText_BorderSize	, 12L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputText_PaddingX		, 10L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputText_PaddingY		, 10L, 1L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputText_InnerSpacingX	, 14L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_InputText_InnerSpacingY	, 14L, 1L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputText_Text,			mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputText_TextHighlight,	mvColor( 66, 150, 250,  89)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputText_Bg,			mvColor( 41,  74, 122, 138)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputText_Hint,			mvColor(128, 128, 128, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputText_Border,		mvColor(110, 110, 128, 128)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputText_BorderShadow,	mvColor(  0,   0,   0,   0)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputText_Rounding		, 0, 12),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputText_BorderSize		, 0,  1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputText_PaddingX		, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputText_PaddingY		, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputText_InnerSpacingX	, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_InputText_InnerSpacingY	, 0, 20),
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvInputText(const std::string& name, const std::string& default_value, const std::string& dataSource);

		void setEnabled        (bool value)     override;
		void draw              ()               override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

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