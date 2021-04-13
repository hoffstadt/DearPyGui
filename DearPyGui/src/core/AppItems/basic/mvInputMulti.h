#pragma once

#include "mvTypeBases.h"
#include "mvApp.h"
#include "mvModule_Core.h"
#include <string>

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvInputIntMulti
//     * mvInputFloatMulti
//
//-----------------------------------------------------------------------------

namespace Marvel {
	  
	//-----------------------------------------------------------------------------
	// mvInputIntMulti
	//-----------------------------------------------------------------------------
	MV_REGISTER_WIDGET(mvInputIntMulti, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Int4, 1);
	class mvInputIntMulti : public mvInt4PtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvInputIntMulti, add_input_intx)

		MV_CREATE_CONSTANT(mvThemeCol_InputIntx_Text,			ImGuiCol_Text,				0L);
		MV_CREATE_CONSTANT(mvThemeCol_InputIntx_TextHighlight,	ImGuiCol_TextSelectedBg,	0L);
		MV_CREATE_CONSTANT(mvThemeCol_InputIntx_Bg,				ImGuiCol_FrameBg,			0L);
		MV_CREATE_CONSTANT(mvThemeCol_InputIntx_Border,			ImGuiCol_Border,			0L);
		MV_CREATE_CONSTANT(mvThemeCol_InputIntx_BorderShadow,	ImGuiCol_BorderShadow,		0L);

		MV_CREATE_CONSTANT(mvThemeStyle_InputIntx_Rounding,			ImGuiStyleVar_FrameRounding,	0L);
		MV_CREATE_CONSTANT(mvThemeStyle_InputIntx_BorderSize,		ImGuiStyleVar_FrameBorderSize,	0L);
		MV_CREATE_CONSTANT(mvThemeStyle_InputIntx_PaddingX,			ImGuiStyleVar_FramePadding,		0L);
		MV_CREATE_CONSTANT(mvThemeStyle_InputIntx_PaddingY,			ImGuiStyleVar_FramePadding,		1L);
		MV_CREATE_CONSTANT(mvThemeStyle_InputIntx_InnerSpacingX,	ImGuiStyleVar_ItemInnerSpacing,	0L);
		MV_CREATE_CONSTANT(mvThemeStyle_InputIntx_InnerSpacingY,	ImGuiStyleVar_ItemInnerSpacing,	1L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_ADD_CONSTANT(mvThemeCol_InputIntx_Text,			mvColor(255, 255, 255, 255), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_InputIntx_TextHighlight,	mvColor( 66, 150, 250,  89), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_InputIntx_Bg,			mvColor( 41,  74, 122, 138), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_InputIntx_Border,		mvColor(110, 110, 128, 128), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_InputIntx_BorderShadow,	mvColor(  0,   0,   0,   0), mvColor(128, 128, 128, 63)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT_F(mvThemeStyle_InputIntx_Rounding,		0, 12),
			MV_ADD_CONSTANT_F(mvThemeStyle_InputIntx_BorderSize,		0,  1),
			MV_ADD_CONSTANT_F(mvThemeStyle_InputIntx_PaddingX,		4, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_InputIntx_PaddingY,		3, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_InputIntx_InnerSpacingX,	4, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_InputIntx_InnerSpacingY,	4, 20),
		MV_END_STYLE_CONSTANTS

	public:

		mvInputIntMulti(const std::string& name);
		
		void setEnabled(bool value) override;
		void draw(ImDrawList* drawlist, float x, float y) override;

		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:

		int                 m_min = 0;
		int                 m_max = 100;
		bool                m_min_clamped = false;
		bool                m_max_clamped = false;
		ImGuiInputTextFlags m_flags = 0;
		ImGuiInputTextFlags m_stor_flags = 0;
		std::array<int, 4>  m_last_value = { 0, 0, 0, 0 };
		int                 m_size = 4;
		
	};
		
	//-----------------------------------------------------------------------------
	// mvInputFloatMulti
	//-----------------------------------------------------------------------------
	MV_REGISTER_WIDGET(mvInputFloatMulti, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Float4, 1);
	class mvInputFloatMulti : public mvFloat4PtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvInputFloatMulti, add_input_floatx)

		MV_CREATE_CONSTANT(mvThemeCol_InputFloatx_Text,				ImGuiCol_Text,				0L);
		MV_CREATE_CONSTANT(mvThemeCol_InputFloatx_TextHighlight,	ImGuiCol_TextSelectedBg,	0L);
		MV_CREATE_CONSTANT(mvThemeCol_InputFloatx_Bg,				ImGuiCol_FrameBg,			0L);
		MV_CREATE_CONSTANT(mvThemeCol_InputFloatx_Border,			ImGuiCol_Border,			0L);
		MV_CREATE_CONSTANT(mvThemeCol_InputFloatx_BorderShadow,		ImGuiCol_BorderShadow,		0L);

		MV_CREATE_CONSTANT(mvThemeStyle_InputFloatx_Rounding,		ImGuiStyleVar_FrameRounding,	0L);
		MV_CREATE_CONSTANT(mvThemeStyle_InputFloatx_BorderSize,		ImGuiStyleVar_FrameBorderSize,	0L);
		MV_CREATE_CONSTANT(mvThemeStyle_InputFloatx_PaddingX,		ImGuiStyleVar_FramePadding,		0L);
		MV_CREATE_CONSTANT(mvThemeStyle_InputFloatx_PaddingY,		ImGuiStyleVar_FramePadding,		1L);
		MV_CREATE_CONSTANT(mvThemeStyle_InputFloatx_InnerSpacingX,	ImGuiStyleVar_ItemInnerSpacing,	0L);
		MV_CREATE_CONSTANT(mvThemeStyle_InputFloatx_InnerSpacingY,	ImGuiStyleVar_ItemInnerSpacing,	1L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_ADD_CONSTANT(mvThemeCol_InputFloatx_Text,          mvColor(255, 255, 255, 255), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_InputFloatx_TextHighlight, mvColor( 66, 150, 250,  89), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_InputFloatx_Bg,            mvColor( 41,  74, 122, 138), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_InputFloatx_Border,        mvColor(110, 110, 128, 128), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_InputFloatx_BorderShadow,  mvColor(  0,   0,   0,   0), mvColor(128, 128, 128, 63)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT_F(mvThemeStyle_InputFloatx_Rounding,		0, 12),
			MV_ADD_CONSTANT_F(mvThemeStyle_InputFloatx_BorderSize,	0,  1),
			MV_ADD_CONSTANT_F(mvThemeStyle_InputFloatx_PaddingX,		4, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_InputFloatx_PaddingY,		3, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_InputFloatx_InnerSpacingX,	4, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_InputFloatx_InnerSpacingY,	4, 20),
		MV_END_STYLE_CONSTANTS

	public:

		mvInputFloatMulti(const std::string& name);
		
		void setEnabled(bool value) override;
		void draw(ImDrawList* drawlist, float x, float y) override;

		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;
		
	private:

		float               m_min = 0.0f;
		float               m_max = 100.0f;
		bool                m_min_clamped = false;
		bool                m_max_clamped = false;
		std::string         m_format = "%.3f";
		ImGuiInputTextFlags m_flags = 0;
		ImGuiInputTextFlags m_stor_flags = 0;
		std::array<float, 4>m_last_value = { 0.0f, 0.0f, 0.0f, 0.0f };
		int                 m_size = 4;
	};

}
