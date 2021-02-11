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
	struct mvColorButtonConfig : public mvAppItemConfig
	{
		mvColor color;
		bool no_alpha = false;
		bool no_border = false;
		bool no_drag_drop = false;
	};




	PyObject* add_color_button(PyObject* self, PyObject* args, PyObject* kwargs);


	class mvColorButton : public mvAppItem
	{
		MV_APPITEM_TYPE(mvAppItemType::ColorButton, mvColorButton, "add_color_button")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorButton, mvThemeCol_ColorButton_Text		,  0L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorButton, mvThemeCol_ColorButton_Separator	, 27L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorButton, mvThemeCol_ColorButton_PopupBg		,  4L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorButton, mvThemeCol_ColorButton_Border		,  5L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorButton, mvThemeCol_ColorButton_BorderShadow,  6L, 0L);

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

	struct mvColorEdit3Config : public mvAppItemConfig
	{
		mvColor default_value();
		bool no_alpha = false;
		bool no_picker = false;
		bool no_options = false;
		bool no_small_preview = false;
		bool no_inputs = false;
		bool no_tooltip = false;
		bool no_label = false;
		bool no_drag_drop = false;
		bool alpha_bar = false;
		bool alpha_preview = false;
		bool alpha_preview_half = false;
		bool display_rgb = false;
		bool display_hsv = false;
		bool display_hex = false;
		bool uint8 = false;
		bool floats = false;
		bool input_rgb = false;
		bool input_hsv = false;
	};

	PyObject* add_color_edit3(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvColorEdit3 : public mvColorPtrBase
	{

		MV_APPITEM_TYPE(mvAppItemType::ColorEdit3, mvColorEdit3, "add_color_edit3")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeCol_ColorEdit3_Text			,  0L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeCol_ColorEdit3_TextHighlight	, 44L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeCol_ColorEdit3_Bg			,  7L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeCol_ColorEdit3_BgHovered		,  8L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeCol_ColorEdit3_BgActive		,  9L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeCol_ColorEdit3_PopupBg		,  4L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeCol_ColorEdit3_Border		,  5L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeCol_ColorEdit3_BorderShadow	,  6L, 0L);

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

	struct mvColorEdit4Config : mvColorEdit3Config {};


	PyObject* add_color_edit4(PyObject* self, PyObject* args, PyObject* kwargs);


	class mvColorEdit4 : public mvColorPtrBase
	{

		MV_APPITEM_TYPE(mvAppItemType::ColorEdit4, mvColorEdit4, "add_color_edit4")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeCol_ColorEdit4_Text			,  0L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeCol_ColorEdit4_TextHighlight	, 44L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeCol_ColorEdit4_Bg			,  7L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeCol_ColorEdit4_BgHovered		,  8L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeCol_ColorEdit4_BgActive		,  9L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeCol_ColorEdit4_PopupBg		,  4L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeCol_ColorEdit4_Border		,  5L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeCol_ColorEdit4_BorderShadow	,  6L, 0L);

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


	struct mvColorPicker3Config : public mvAppItemConfig
	{
		mvColor default_value();
		bool no_alpha = false;
		bool no_small_preview = false;
		bool no_inputs = false;
		bool no_tooltip = false;
		bool no_label = false;
		bool no_side_preview = false;
		bool alpha_bar = false;
		bool alpha_preview = false;
		bool alpha_preview_half = false;
		bool display_rgb = false;
		bool display_hsv = false;
		bool display_hex = false;
		bool uint8 = false;
		bool floats = false;
		bool picker_hue_bar = false;
		bool picker_hue_wheel = false;
		bool input_rgb = false;
		bool input_hsv = false;
	};


	PyObject* add_color_picker3(PyObject* self, PyObject* args, PyObject* kwargs);


	class mvColorPicker3 : public mvColorPtrBase
	{

		MV_APPITEM_TYPE(mvAppItemType::ColorPicker3, mvColorPicker3, "add_color_picker3")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker3, mvThemeCol_ColorPicker3_Text			,  0L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker3, mvThemeCol_ColorPicker3_Bg			,  7L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker3, mvThemeCol_ColorPicker3_BgHovered		,  8L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker3, mvThemeCol_ColorPicker3_BgActive		,  9L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker3, mvThemeCol_ColorPicker3_Border		,  5L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker3, mvThemeCol_ColorPicker3_BorderShadow	,  6L, 0L);

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
	struct mvColorPicker4Config : mvColorPicker3Config {};


	PyObject* add_color_picker4(PyObject* self, PyObject* args, PyObject* kwargs);


	class mvColorPicker4 : public mvColorPtrBase
	{

		MV_APPITEM_TYPE(mvAppItemType::ColorPicker4, mvColorPicker4, "add_color_picker4")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker4, mvThemeCol_ColorPicker4_Text			,  0L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker4, mvThemeCol_ColorPicker4_Bg			,  7L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker4, mvThemeCol_ColorPicker4_BgHovered		,  8L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker4, mvThemeCol_ColorPicker4_BgActive		,  9L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker4, mvThemeCol_ColorPicker4_Border		,  5L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker4, mvThemeCol_ColorPicker4_BorderShadow	,  6L, 0L);

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
