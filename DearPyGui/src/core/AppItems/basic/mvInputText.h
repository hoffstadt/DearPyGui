#pragma once

#include "mvApp.h"
#include "mvTypeBases.h"

namespace Marvel {

	PyObject* add_input_text(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvInputText : public mvStringPtrBase
	{
		MV_APPITEM_TYPE(mvAppItemType::InputText, mvInputText, "add_input_text")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::InputText, mvThemeCol_InputText_Text			,  0L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::InputText, mvThemeCol_InputText_TextHighlight	, 44L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::InputText, mvThemeCol_InputText_Bg				,  7L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::InputText, mvThemeCol_InputText_Hint			,  1L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::InputText, mvThemeCol_InputText_Border			,  5L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::InputText, mvThemeCol_InputText_BorderShadow	,  6L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::InputText, mvThemeStyle_InputText_Rounding		, 11L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::InputText, mvThemeStyle_InputText_BorderSize	, 12L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::InputText, mvThemeStyle_InputText_PaddingX		, 10L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::InputText, mvThemeStyle_InputText_PaddingY		, 10L, 1L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::InputText, mvThemeStyle_InputText_InnerSpacingX	, 14L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::InputText, mvThemeStyle_InputText_InnerSpacingY	, 14L, 1L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputText_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputText_TextHighlight),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputText_Bg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputText_Hint),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputText_Border),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputText_BorderShadow),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeStyle_InputText_Rounding),
			MV_CREATE_CONSTANT_PAIR(mvThemeStyle_InputText_BorderSize),
			MV_CREATE_CONSTANT_PAIR(mvThemeStyle_InputText_PaddingX),
			MV_CREATE_CONSTANT_PAIR(mvThemeStyle_InputText_PaddingY),
			MV_CREATE_CONSTANT_PAIR(mvThemeStyle_InputText_InnerSpacingX),
			MV_CREATE_CONSTANT_PAIR(mvThemeStyle_InputText_InnerSpacingY),
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

	private:

		std::string         m_hint;
		bool                m_multiline = false;
		ImGuiInputTextFlags m_flags = 0;
		ImGuiInputTextFlags m_stor_flags = 0;

	};

}