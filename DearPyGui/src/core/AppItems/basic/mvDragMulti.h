#pragma once

#include "mvTypeBases.h"
#include "mvApp.h"
#include "mvModule_Core.h"
#include <string>

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvDragFloatMulti
//     * mvDragIntMulti
//
//-----------------------------------------------------------------------------

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvDragFloatMulti
	//-----------------------------------------------------------------------------
	MV_REGISTER_WIDGET(mvDragFloatMulti, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Float4, 1);
	class mvDragFloatMulti : public mvFloat4PtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDragFloatMulti, add_drag_floatx)

		MV_CREATE_CONSTANT(mvThemeCol_DragFloatx_Text, ImGuiCol_Text, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_DragFloatx_Bg, ImGuiCol_FrameBg, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_DragFloatx_BgHovered, ImGuiCol_FrameBgHovered, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_DragFloatx_BgActive, ImGuiCol_FrameBgActive, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_DragFloatx_Border, ImGuiCol_Border, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_DragFloatx_BorderShadow, ImGuiCol_BorderShadow, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_DragFloatx_Rounding, ImGuiStyleVar_FrameRounding, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_DragFloatx_BorderSize, ImGuiStyleVar_FrameBorderSize, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_DragFloatx_PaddingX, ImGuiStyleVar_FramePadding, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_DragFloatx_PaddingY, ImGuiStyleVar_FramePadding, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_DragFloatx_InnerSpacingX, ImGuiStyleVar_ItemInnerSpacing, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_DragFloatx_InnerSpacingY, ImGuiStyleVar_ItemInnerSpacing, 1L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_ADD_CONSTANT(mvThemeCol_DragFloatx_Text,         mvColor(255, 255, 255, 255), mvColor(128, 128, 128, 63)),
		MV_ADD_CONSTANT(mvThemeCol_DragFloatx_Bg,           mvColor( 41,  74, 122, 138), mvColor(128, 128, 128, 63)),
		MV_ADD_CONSTANT(mvThemeCol_DragFloatx_BgHovered,    mvColor( 66, 150, 250, 102), mvColor(128, 128, 128, 63)),
		MV_ADD_CONSTANT(mvThemeCol_DragFloatx_BgActive,     mvColor( 66, 150, 250, 171), mvColor(128, 128, 128, 63)),
		MV_ADD_CONSTANT(mvThemeCol_DragFloatx_Border,       mvColor(110, 110, 128, 128), mvColor(128, 128, 128, 63)),
		MV_ADD_CONSTANT(mvThemeCol_DragFloatx_BorderShadow, mvColor(  0,   0,   0,   0), mvColor(128, 128, 128, 63)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_ADD_CONSTANT(mvThemeStyle_DragFloatx_Rounding,      0, 12),
		MV_ADD_CONSTANT(mvThemeStyle_DragFloatx_BorderSize,    0, 1),
		MV_ADD_CONSTANT(mvThemeStyle_DragFloatx_PaddingX,      4, 20),
		MV_ADD_CONSTANT(mvThemeStyle_DragFloatx_PaddingY,      3, 20),
		MV_ADD_CONSTANT(mvThemeStyle_DragFloatx_InnerSpacingX, 4, 20),
		MV_ADD_CONSTANT(mvThemeStyle_DragFloatx_InnerSpacingY, 4, 20),
		MV_END_STYLE_CONSTANTS

	public:

		mvDragFloatMulti(const std::string& name, float* default_value, const std::string& dataSource);

		void setEnabled(bool value) override;
		void draw(ImDrawList* drawlist, float x, float y) override;

		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:

		float               m_speed = 1.0f;
		float               m_min = 0.0f;
		float               m_max = 100.0f;
		std::string         m_format = "%.3f";
		ImGuiInputTextFlags m_flags = ImGuiSliderFlags_None;
		ImGuiInputTextFlags m_stor_flags = ImGuiSliderFlags_None;
		int                 m_size = 4;
	};

	//-----------------------------------------------------------------------------
	// mvDragIntMulti
	//-----------------------------------------------------------------------------
	MV_REGISTER_WIDGET(mvDragIntMulti, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Int4, 1);
	class mvDragIntMulti : public mvInt4PtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDragIntMulti, add_drag_intx)

		MV_CREATE_CONSTANT(mvThemeCol_DragIntx_Text,			ImGuiCol_Text,				0L);
		MV_CREATE_CONSTANT(mvThemeCol_DragIntx_Bg,				ImGuiCol_FrameBg,			0L);
		MV_CREATE_CONSTANT(mvThemeCol_DragIntx_BgHovered,		ImGuiCol_FrameBgHovered,	0L);
		MV_CREATE_CONSTANT(mvThemeCol_DragIntx_BgActive,		ImGuiCol_FrameBgActive,		0L);
		MV_CREATE_CONSTANT(mvThemeCol_DragIntx_Border,			ImGuiCol_Border,			0L);
		MV_CREATE_CONSTANT(mvThemeCol_DragIntx_BorderShadow,	ImGuiCol_BorderShadow,		0L);

		MV_CREATE_CONSTANT(mvThemeStyle_DragIntx_Rounding,      ImGuiStyleVar_FrameRounding,	0L);
		MV_CREATE_CONSTANT(mvThemeStyle_DragIntx_BorderSize,    ImGuiStyleVar_FrameBorderSize,	0L);
		MV_CREATE_CONSTANT(mvThemeStyle_DragIntx_PaddingX,      ImGuiStyleVar_FramePadding,		0L);
		MV_CREATE_CONSTANT(mvThemeStyle_DragIntx_PaddingY,      ImGuiStyleVar_FramePadding,		1L);
		MV_CREATE_CONSTANT(mvThemeStyle_DragIntx_InnerSpacingX, ImGuiStyleVar_ItemInnerSpacing,	0L);
		MV_CREATE_CONSTANT(mvThemeStyle_DragIntx_InnerSpacingY, ImGuiStyleVar_ItemInnerSpacing,	1L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_ADD_CONSTANT(mvThemeCol_DragIntx_Text,			mvColor(255, 255, 255, 255), mvColor(128, 128, 128, 63)),
		MV_ADD_CONSTANT(mvThemeCol_DragIntx_Bg,				mvColor( 41,  74, 122, 138), mvColor(128, 128, 128, 63)),
		MV_ADD_CONSTANT(mvThemeCol_DragIntx_BgHovered,		mvColor( 66, 150, 250, 102), mvColor(128, 128, 128, 63)),
		MV_ADD_CONSTANT(mvThemeCol_DragIntx_BgActive,		mvColor( 66, 150, 250, 171), mvColor(128, 128, 128, 63)),
		MV_ADD_CONSTANT(mvThemeCol_DragIntx_Border,			mvColor(110, 110, 128, 128), mvColor(128, 128, 128, 63)),
		MV_ADD_CONSTANT(mvThemeCol_DragIntx_BorderShadow,	mvColor(  0,   0,   0,   0), mvColor(128, 128, 128, 63)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_ADD_CONSTANT(mvThemeStyle_DragIntx_Rounding,			0, 12),
		MV_ADD_CONSTANT(mvThemeStyle_DragIntx_BorderSize,		0,  1),
		MV_ADD_CONSTANT(mvThemeStyle_DragIntx_PaddingX,			4, 20),
		MV_ADD_CONSTANT(mvThemeStyle_DragIntx_PaddingY,			3, 20),
		MV_ADD_CONSTANT(mvThemeStyle_DragIntx_InnerSpacingX,	4, 20),
		MV_ADD_CONSTANT(mvThemeStyle_DragIntx_InnerSpacingY,	4, 20),
		MV_END_STYLE_CONSTANTS

	public:

		mvDragIntMulti(const std::string& name, int* default_value, const std::string& dataSource);

		void setEnabled(bool value) override;
		void draw(ImDrawList* drawlist, float x, float y) override;

		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:

		float               m_speed = 1.0f;
		int                 m_min = 0;
		int                 m_max = 100;
		std::string         m_format = "%d";
		ImGuiInputTextFlags m_flags = ImGuiSliderFlags_None;
		ImGuiInputTextFlags m_stor_flags = ImGuiSliderFlags_None;
		int                 m_size = 4;
	};
}