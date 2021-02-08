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

		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorButton, mvThemeCol_ColorButton_Text		,  0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorButton, mvThemeCol_ColorButton_Separator	, 27L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorButton, mvThemeCol_ColorButton_PopupBg		,  4L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorButton, mvThemeCol_ColorButton_Border		,  5L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorButton, mvThemeCol_ColorButton_BorderShadow,  6L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorButton_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorButton_Separator),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorButton_PopupBg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorButton_Border),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorButton_BorderShadow),
		MV_END_COLOR_CONSTANTS

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

		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeCol_ColorEdit3_Text			,  0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeCol_ColorEdit3_TextHighlight	, 44L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeCol_ColorEdit3_Bg			,  7L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeCol_ColorEdit3_BgHovered		,  8L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeCol_ColorEdit3_BgActive		,  9L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeCol_ColorEdit3_PopupBg		,  4L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeCol_ColorEdit3_Border		,  5L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeCol_ColorEdit3_BorderShadow	,  6L);

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

		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeCol_ColorEdit4_Text			,  0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeCol_ColorEdit4_TextHighlight	, 44L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeCol_ColorEdit4_Bg			,  7L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeCol_ColorEdit4_BgHovered		,  8L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeCol_ColorEdit4_BgActive		,  9L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeCol_ColorEdit4_PopupBg		,  4L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeCol_ColorEdit4_Border		,  5L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeCol_ColorEdit4_BorderShadow	,  6L);

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

		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker3, mvThemeCol_ColorPicker3_Text			,  0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker3, mvThemeCol_ColorPicker3_Bg			,  7L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker3, mvThemeCol_ColorPicker3_BgHovered		,  8L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker3, mvThemeCol_ColorPicker3_BgActive		,  9L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker3, mvThemeCol_ColorPicker3_Border		,  5L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker3, mvThemeCol_ColorPicker3_BorderShadow	,  6L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorPicker3_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorPicker3_Bg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorPicker3_BgHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorPicker3_BgActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorPicker3_Border),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorPicker3_BorderShadow),
		MV_END_COLOR_CONSTANTS

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

		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker4, mvThemeCol_ColorPicker4_Text			,  0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker4, mvThemeCol_ColorPicker4_Bg			,  7L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker4, mvThemeCol_ColorPicker4_BgHovered		,  8L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker4, mvThemeCol_ColorPicker4_BgActive		,  9L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker4, mvThemeCol_ColorPicker4_Border		,  5L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker4, mvThemeCol_ColorPicker4_BorderShadow	,  6L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorPicker4_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorPicker4_Bg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorPicker4_BgHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorPicker4_BgActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorPicker4_Border),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorPicker4_BorderShadow),
		MV_END_COLOR_CONSTANTS

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
