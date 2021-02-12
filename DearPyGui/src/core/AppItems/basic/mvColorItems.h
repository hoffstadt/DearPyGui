#pragma once

#include "mvTypeBases.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvColorButton
//     * mvColorEdit3
//     * mvColorEdit4
//     * mvColorPicker3
//     * mvColorPicker4
//
//-----------------------------------------------------------------------------

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvColorButton
	//-----------------------------------------------------------------------------

	PyObject* add_color_button(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvColorButton : public mvAppItem
	{
		MV_APPITEM_TYPE(mvAppItemType::ColorButton, mvColorButton, "add_color_button")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorButton, mvThemeCol_ColorButton_Text				,  0L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorButton, mvThemeCol_ColorButton_Separator			, 27L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorButton, mvThemeCol_ColorButton_PopupBg				,  4L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorButton, mvThemeCol_ColorButton_Border				,  5L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorButton, mvThemeCol_ColorButton_BorderShadow		,  6L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorButton, mvThemeStyle_ColorButton_Rounding			, 11L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorButton, mvThemeStyle_ColorButton_BorderSize		, 12L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorButton, mvThemeStyle_ColorButton_PopupRounding		,  8L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorButton, mvThemeStyle_ColorButton_PopupBorderSize	,  9L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorButton, mvThemeStyle_ColorButton_PopupPaddingX		,  1L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorButton, mvThemeStyle_ColorButton_PopupPaddingY		,  1L, 1L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorButton_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorButton_Separator),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorButton_PopupBg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorButton_Border),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorButton_BorderShadow),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorButton_Rounding			, 0, 12),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorButton_BorderSize		, 0,  1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorButton_PopupRounding		, 0, 12),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorButton_PopupBorderSize	, 0,  1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorButton_PopupPaddingX		, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorButton_PopupPaddingY		, 0, 20),
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvColorButton(const std::string& name, const mvColor& color);

		void draw              ()               override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

	private:

		ImGuiColorEditFlags m_flags = ImGuiColorEditFlags_None;
		ImVec4 m_color;

	};

	//-----------------------------------------------------------------------------
	// mvColorEdit3
	//-----------------------------------------------------------------------------

	PyObject* add_color_edit3(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvColorEdit3 : public mvColorPtrBase
	{

		MV_APPITEM_TYPE(mvAppItemType::ColorEdit3, mvColorEdit3, "add_color_edit3")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeCol_ColorEdit3_Text					,  0L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeCol_ColorEdit3_TextHighlight			, 44L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeCol_ColorEdit3_Bg					,  7L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeCol_ColorEdit3_BgHovered				,  8L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeCol_ColorEdit3_BgActive				,  9L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeCol_ColorEdit3_PopupBg				,  4L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeCol_ColorEdit3_Border				,  5L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeCol_ColorEdit3_BorderShadow			,  6L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeStyle_ColorEdit3_Rounding			, 11L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeStyle_ColorEdit3_BorderSize			, 12L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeStyle_ColorEdit3_PopupRounding		,  8L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeStyle_ColorEdit3_PopupBorderSize		,  9L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeStyle_ColorEdit3_InnerSpacingX		, 14L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeStyle_ColorEdit3_InnerSpacingY		, 14L, 1L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeStyle_ColorEdit3_PopupPaddingX		,  1L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeStyle_ColorEdit3_PopupPaddingY		,  1L, 1L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeStyle_ColorEdit3_PaddingX			, 10L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeStyle_ColorEdit3_PaddingY			, 10L, 1L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorEdit3_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorEdit3_TextHighlight),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorEdit3_Bg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorEdit3_BgHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorEdit3_BgActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorEdit3_PopupBg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorEdit3_Border),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorEdit3_BorderShadow),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorEdit3_Rounding			, 0, 12),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorEdit3_BorderSize			, 0,  1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorEdit3_PopupRounding		, 0, 12),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorEdit3_PopupBorderSize	, 0,  1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorEdit3_InnerSpacingX		, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorEdit3_InnerSpacingY		, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorEdit3_PopupPaddingX		, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorEdit3_PopupPaddingY		, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorEdit3_PaddingX			, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorEdit3_PaddingY			, 0, 20),
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

			mvColorEdit3(const std::string& name, float* color, const std::string& dataSource);

		void draw              ()               override;


#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP
	
	private:

		ImGuiColorEditFlags m_flags = ImGuiColorEditFlags_None;

	};

	//-----------------------------------------------------------------------------
	// mvColorEdit4
	//-----------------------------------------------------------------------------

	PyObject* add_color_edit4(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvColorEdit4 : public mvColorPtrBase
	{

		MV_APPITEM_TYPE(mvAppItemType::ColorEdit4, mvColorEdit4, "add_color_edit4")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeCol_ColorEdit4_Text					,  0L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeCol_ColorEdit4_TextHighlight			, 44L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeCol_ColorEdit4_Bg					,  7L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeCol_ColorEdit4_BgHovered				,  8L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeCol_ColorEdit4_BgActive				,  9L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeCol_ColorEdit4_PopupBg				,  4L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeCol_ColorEdit4_Border				,  5L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeCol_ColorEdit4_BorderShadow			,  6L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeStyle_ColorEdit4_Rounding			, 11L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeStyle_ColorEdit4_BorderSize			, 12L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeStyle_ColorEdit4_PopupRounding		,  8L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeStyle_ColorEdit4_PopupBorderSize		,  9L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeStyle_ColorEdit4_InnerSpacingX		, 14L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeStyle_ColorEdit4_InnerSpacingY		, 14L, 1L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeStyle_ColorEdit4_PopupPaddingX		,  1L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeStyle_ColorEdit4_PopupPaddingY		,  1L, 1L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeStyle_ColorEdit4_PaddingX			, 10L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeStyle_ColorEdit4_PaddingY			, 10L, 1L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorEdit4_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorEdit4_TextHighlight),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorEdit4_Bg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorEdit4_BgHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorEdit4_BgActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorEdit4_PopupBg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorEdit4_Border),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorEdit4_BorderShadow),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorEdit4_Rounding			, 0, 12),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorEdit4_BorderSize			, 0,  1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorEdit4_PopupRounding		, 0, 12),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorEdit4_PopupBorderSize	, 0,  1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorEdit4_InnerSpacingX		, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorEdit4_InnerSpacingY		, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorEdit4_PopupPaddingX		, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorEdit4_PopupPaddingY		, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorEdit4_PaddingX			, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorEdit4_PaddingY			, 0, 20),
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvColorEdit4(const std::string& name, float* color, const std::string& dataSource);

		void draw              ()               override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

	private:

		ImGuiColorEditFlags m_flags = ImGuiColorEditFlags_None;

	};

	//-----------------------------------------------------------------------------
	// mvColorPicker3
	//-----------------------------------------------------------------------------

	PyObject* add_color_picker3(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvColorPicker3 : public mvColorPtrBase
	{

		MV_APPITEM_TYPE(mvAppItemType::ColorPicker3, mvColorPicker3, "add_color_picker3")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker3, mvThemeCol_ColorPicker3_Text				,  0L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker3, mvThemeCol_ColorPicker3_Bg				,  7L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker3, mvThemeCol_ColorPicker3_BgHovered			,  8L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker3, mvThemeCol_ColorPicker3_BgActive			,  9L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker3, mvThemeCol_ColorPicker3_Border			,  5L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker3, mvThemeCol_ColorPicker3_BorderShadow		,  6L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker3, mvThemeStyle_ColorPicker3_Rounding		, 11L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker3, mvThemeStyle_ColorPicker3_BorderSize		, 12L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker3, mvThemeStyle_ColorPicker3_PopupRounding	, 8L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker3, mvThemeStyle_ColorPicker3_PopupBorderSize	, 9L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker3, mvThemeStyle_ColorPicker3_InnerSpacingX	, 14L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker3, mvThemeStyle_ColorPicker3_InnerSpacingY	, 14L, 1L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker3, mvThemeStyle_ColorPicker3_PopupPaddingX	, 1L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker3, mvThemeStyle_ColorPicker3_PopupPaddingY	, 1L, 1L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker3, mvThemeStyle_ColorPicker3_PaddingX		, 10L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker3, mvThemeStyle_ColorPicker3_PaddingY		, 10L, 1L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorPicker3_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorPicker3_Bg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorPicker3_BgHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorPicker3_BgActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorPicker3_Border),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorPicker3_BorderShadow),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorPicker3_Rounding			, 0, 12),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorPicker3_BorderSize		, 0,  1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorPicker3_PopupRounding	, 0, 12),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorPicker3_PopupBorderSize	, 0,  1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorPicker3_InnerSpacingX	, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorPicker3_InnerSpacingY	, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorPicker3_PopupPaddingX	, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorPicker3_PopupPaddingY	, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorPicker3_PaddingX			, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorPicker3_PaddingY			, 0, 20),
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvColorPicker3(const std::string& name, float* color, const std::string& dataSource);

		void draw              ()               override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

	private:

		ImGuiColorEditFlags m_flags = ImGuiColorEditFlags_None;

	};

	//-----------------------------------------------------------------------------
	// mvColorPicker4
	//-----------------------------------------------------------------------------

	PyObject* add_color_picker4(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvColorPicker4 : public mvColorPtrBase
	{

		MV_APPITEM_TYPE(mvAppItemType::ColorPicker4, mvColorPicker4, "add_color_picker4")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker4, mvThemeCol_ColorPicker4_Text				,  0L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker4, mvThemeCol_ColorPicker4_Bg				,  7L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker4, mvThemeCol_ColorPicker4_BgHovered			,  8L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker4, mvThemeCol_ColorPicker4_BgActive			,  9L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker4, mvThemeCol_ColorPicker4_Border			,  5L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker4, mvThemeCol_ColorPicker4_BorderShadow		,  6L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker4, mvThemeStyle_ColorPicker4_Rounding		, 11L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker4, mvThemeStyle_ColorPicker4_BorderSize		, 12L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker4, mvThemeStyle_ColorPicker4_PopupRounding	,  8L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker4, mvThemeStyle_ColorPicker4_PopupBorderSize	,  9L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker4, mvThemeStyle_ColorPicker4_InnerSpacingX	, 14L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker4, mvThemeStyle_ColorPicker4_InnerSpacingY	, 14L, 1L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker4, mvThemeStyle_ColorPicker4_PopupPaddingX	,  1L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker4, mvThemeStyle_ColorPicker4_PopupPaddingY	,  1L, 1L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker4, mvThemeStyle_ColorPicker4_PaddingX		, 10L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker4, mvThemeStyle_ColorPicker4_PaddingY		, 10L, 1L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorPicker4_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorPicker4_Bg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorPicker4_BgHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorPicker4_BgActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorPicker4_Border),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorPicker4_BorderShadow),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorPicker4_Rounding			, 0, 12),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorPicker4_BorderSize		, 0,  1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorPicker4_PopupRounding	, 0, 12),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorPicker4_PopupBorderSize	, 0,  1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorPicker4_InnerSpacingX	, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorPicker4_InnerSpacingY	, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorPicker4_PopupPaddingX	, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorPicker4_PopupPaddingY	, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorPicker4_PaddingX			, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorPicker4_PaddingY			, 0, 20),
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvColorPicker4(const std::string& name, float* color, const std::string& dataSource);

		void draw              ()               override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

	private:

		ImGuiColorEditFlags m_flags = ImGuiColorEditFlags_None;

	};

}
