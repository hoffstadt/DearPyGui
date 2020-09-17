#include "mvMarvel.h"
#include "mvApp.h"
#include "Core/mvInput.h"
#include "Core/mvDataStorage.h"
#include "mvAppLog.h"
#include "Core/StandardWindows/mvSourceWindow.h"
#include "Core/StandardWindows/mvFileDialog.h"
#include "mvPythonTranslator.h"
#include "Core/mvAppItems.h"
#include "mvWindow.h"
#include "Core/mvPythonExceptions.h"
#include <ImGuiFileDialog.h>
#include <cstdlib>

// new includes
#include "Core/PythonCommands/mvInterfaceCore.h"
#include "Core/PythonCommands/mvPlotInterface.h"
#include "Core/PythonCommands/mvDrawingInterface.h"

//-----------------------------------------------------------------------------
// Helper Macro
//-----------------------------------------------------------------------------
#define ADD_PYTHON_FUNCTION(Function) { #Function, (PyCFunction)Function, METH_VARARGS | METH_KEYWORDS, (*mvApp::GetApp()->getParsers())[#Function].getDocumentation() },

namespace Marvel {

	std::map<std::string, mvPythonParser>* BuildDearPyGuiInterface()
	{

		std::map<std::string, mvPythonParser>* parsers = new std::map< std::string, mvPythonParser>;

		AddDrawingCommands(parsers);
		AddPlotCommands(parsers);
		AddLogCommands(parsers);
		AddInputCommands(parsers);
		AddInputWidgets(parsers);
		AddSliderWidgets(parsers);
		AddDragWidgets(parsers);
		AddTableCommands(parsers);
		AddItemCommands(parsers);
		AddStdWindowCommands(parsers);
		AddMenuCommands(parsers);
		AddThemeCommands(parsers);
		AddBasicWidgets(parsers);
		AddContainterWidgets(parsers);
		AddAppCommands(parsers);
		return parsers;
	}

	std::vector<std::pair<std::string, std::string>> GetAllCommands()
	{
		auto mapping = BuildDearPyGuiInterface();

		std::vector<std::pair<std::string, std::string>> result;

		for (const auto& item : *mapping)
			result.emplace_back(item.first, item.second.getDocumentation());

		delete mapping;

		return result;
	}

	PyObject* is_dearpygui_running(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return ToPyBool(mvApp::IsAppStarted());
	}

	PyObject* set_main_window_title(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* title;

		if (!(*mvApp::GetApp()->getParsers())["set_main_window_title"].parse(args, kwargs, __FUNCTION__,
			&title))
			return GetPyNone();

		mvApp::GetApp()->setTitle(title);

		if (mvApp::IsAppStarted())
			mvApp::GetApp()->getViewport()->setWindowText(title);

		return GetPyNone();
	}

	PyObject* set_main_window_resizable(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int resizable = true;

		if (!(*mvApp::GetApp()->getParsers())["set_main_window_resizable"].parse(args, kwargs, __FUNCTION__,
			&resizable))
			return GetPyNone();

		mvApp::GetApp()->setResizable(resizable);

		return GetPyNone();
	}

	PyObject* add_additional_font(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* file;
		float size = 13.0f;
		const char* glyph_ranges = "";
		PyObject* custom_glyph_ranges = nullptr;
		PyObject* custom_glyph_chars = nullptr;


		if (!(*mvApp::GetApp()->getParsers())["add_additional_font"].parse(args, kwargs, __FUNCTION__,
			&file, &size, &glyph_ranges, &custom_glyph_chars, &custom_glyph_ranges))
			return GetPyNone();

		std::vector<int> custom_chars = ToIntVect(custom_glyph_chars);
		std::vector<std::pair<int, int>> custom_ranges = ToVectInt2(custom_glyph_ranges);
		std::vector<std::array<ImWchar, 3>> imgui_custom_ranges;
		std::vector<ImWchar> imgui_custom_chars;

		for (auto& item : custom_ranges)
			imgui_custom_ranges.push_back({ (ImWchar)item.first, (ImWchar)item.second, 0 });
		for (auto& item : custom_chars)
			imgui_custom_chars.push_back((ImWchar)item);


		mvApp::GetApp()->setFont(file, size, glyph_ranges, imgui_custom_ranges, imgui_custom_chars);

		return GetPyNone();
	}

	PyObject* get_style_window_padding(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		return ToPyPair(style.WindowPadding.x, style.WindowPadding.y);
	}

	PyObject* get_style_frame_padding(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		return ToPyPair(style.FramePadding.x, style.FramePadding.y);
	}

	PyObject* get_style_item_spacing(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		return ToPyPair(style.ItemSpacing.x, style.ItemSpacing.y);
	}

	PyObject* get_style_item_inner_spacing(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		return ToPyPair(style.ItemInnerSpacing.x, style.ItemInnerSpacing.y);
	}

	PyObject* get_style_touch_extra_padding(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		return ToPyPair(style.TouchExtraPadding.x, style.TouchExtraPadding.y);
	}

	PyObject* get_style_indent_spacing(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		return ToPyFloat(style.IndentSpacing);
	}

	PyObject* get_style_scrollbar_size(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		return ToPyFloat(style.ScrollbarSize);
	}

	PyObject* get_style_grab_min_size(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		return ToPyFloat(style.GrabMinSize);
	}

	PyObject* get_style_window_border_size(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		return ToPyFloat(style.WindowBorderSize);
	}

	PyObject* get_style_child_border_size(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		return ToPyFloat(style.ChildBorderSize);
	}

	PyObject* get_style_popup_border_size(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		return ToPyFloat(style.PopupBorderSize);
	}

	PyObject* get_style_frame_border_size(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		return ToPyFloat(style.FrameBorderSize);
	}

	PyObject* get_style_tab_border_size(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		return ToPyFloat(style.TabBorderSize);
	}

	PyObject* get_style_window_rounding(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		return ToPyFloat(style.WindowRounding);
	}

	PyObject* get_style_child_rounding(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		return ToPyFloat(style.ChildRounding);
	}

	PyObject* get_style_frame_rounding(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		return ToPyFloat(style.FrameRounding);
	}

	PyObject* get_style_popup_rounding(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		return ToPyFloat(style.PopupRounding);
	}

	PyObject* get_style_scrollbar_rounding(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		return ToPyFloat(style.ScrollbarRounding);
	}

	PyObject* get_style_grab_rounding(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		return ToPyFloat(style.GrabRounding);
	}

	PyObject* get_style_tab_rounding(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		return ToPyFloat(style.TabRounding);
	}

	PyObject* get_style_window_title_align(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		return ToPyPair(style.WindowTitleAlign.x, style.WindowTitleAlign.y);
	}

	PyObject* get_style_window_menu_button_position(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		return ToPyInt(style.WindowMenuButtonPosition);
	}

	PyObject* get_style_color_button_position(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		return ToPyInt(style.ColorButtonPosition);
	}

	PyObject* get_style_button_text_align(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		return ToPyPair(style.ButtonTextAlign.x, style.ButtonTextAlign.y);
	}

	PyObject* get_style_selectable_text_align(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		return ToPyPair(style.SelectableTextAlign.x, style.SelectableTextAlign.y);
	}

	PyObject* get_style_display_safe_area_padding(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		return ToPyPair(style.DisplaySafeAreaPadding.x, style.DisplaySafeAreaPadding.y);
	}

	PyObject* get_style_global_alpha(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		return ToPyFloat(style.Alpha);
	}

	PyObject* get_style_antialiased_lines(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		return ToPyBool(style.AntiAliasedLines);
	}

	PyObject* get_style_antialiased_fill(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		return ToPyBool(style.AntiAliasedFill);
	}

	PyObject* get_style_curve_tessellation_tolerance(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		return ToPyFloat(style.CurveTessellationTol);
	}

	PyObject* get_style_circle_segment_max_error(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		return ToPyFloat(style.CircleSegmentMaxError);
	}

	PyObject* set_style_window_padding(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		float x;
		float y;

		if (!(*mvApp::GetApp()->getParsers())["set_style_window_padding"].parse(args, kwargs, __FUNCTION__, 
			&x, &y))
			return GetPyNone();

		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		mvApp::GetApp()->setStyleChanged();
		style.WindowPadding = { x, y };

		return GetPyNone();
	}

	PyObject* set_style_frame_padding(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		float x;
		float y;

		if (!(*mvApp::GetApp()->getParsers())["set_style_frame_padding"].parse(args, kwargs, __FUNCTION__,
			&x, &y))
			return GetPyNone();

		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		mvApp::GetApp()->setStyleChanged();
		style.FramePadding = { x, y };

		return GetPyNone();
	}

	PyObject* set_style_item_spacing(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		float x;
		float y;

		if (!(*mvApp::GetApp()->getParsers())["set_style_item_spacing"].parse(args, kwargs, __FUNCTION__,
			&x, &y))
			return GetPyNone();

		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		mvApp::GetApp()->setStyleChanged();
		style.ItemSpacing = { x, y };

		return GetPyNone();
	}

	PyObject* set_style_item_inner_spacing(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		float x;
		float y;

		if (!(*mvApp::GetApp()->getParsers())["set_style_item_inner_spacing"].parse(args, kwargs, __FUNCTION__,
			&x, &y))
			return GetPyNone();

		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		mvApp::GetApp()->setStyleChanged();
		style.ItemInnerSpacing = { x, y };

		return GetPyNone();
	}

	PyObject* set_style_touch_extra_padding(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		float x;
		float y;

		if (!(*mvApp::GetApp()->getParsers())["set_style_touch_extra_padding"].parse(args, kwargs, __FUNCTION__,
			&x, &y))
			return GetPyNone();

		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		mvApp::GetApp()->setStyleChanged();
		style.TouchExtraPadding = { x, y };

		return GetPyNone();
	}

	PyObject* set_style_indent_spacing(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		float value;

		if (!(*mvApp::GetApp()->getParsers())["set_style_indent_spacing"].parse(args, kwargs, __FUNCTION__,
			&value))
			return GetPyNone();

		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		mvApp::GetApp()->setStyleChanged();
		style.IndentSpacing = value;

		return GetPyNone();
	}

	PyObject* set_style_scrollbar_size(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		float value;

		if (!(*mvApp::GetApp()->getParsers())["set_style_scrollbar_size"].parse(args, kwargs, __FUNCTION__,
			&value))
			return GetPyNone();

		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		mvApp::GetApp()->setStyleChanged();
		style.ScrollbarSize = value;

		return GetPyNone();
	}

	PyObject* set_style_grab_min_size(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		float value;

		if (!(*mvApp::GetApp()->getParsers())["set_style_grab_min_size"].parse(args, kwargs, __FUNCTION__,
			&value))
			return GetPyNone();

		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		mvApp::GetApp()->setStyleChanged();
		style.GrabMinSize = value;

		return GetPyNone();
	}

	PyObject* set_style_window_border_size(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		float value;

		if (!(*mvApp::GetApp()->getParsers())["set_style_window_border_size"].parse(args, kwargs, __FUNCTION__,
			&value))
			return GetPyNone();

		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		mvApp::GetApp()->setStyleChanged();
		style.WindowBorderSize = value;

		return GetPyNone();
	}

	PyObject* set_style_child_border_size(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		float value;

		if (!(*mvApp::GetApp()->getParsers())["set_style_child_border_size"].parse(args, kwargs, __FUNCTION__,
			&value))
			return GetPyNone();

		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		mvApp::GetApp()->setStyleChanged();
		style.ChildBorderSize = value;

		return GetPyNone();
	}

	PyObject* set_style_popup_border_size(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		float value;

		if (!(*mvApp::GetApp()->getParsers())["set_style_popup_border_size"].parse(args, kwargs, __FUNCTION__,
			&value))
			return GetPyNone();

		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		mvApp::GetApp()->setStyleChanged();
		style.PopupBorderSize = value;

		return GetPyNone();
	}

	PyObject* set_style_frame_border_size(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		float value;

		if (!(*mvApp::GetApp()->getParsers())["set_style_frame_border_size"].parse(args, kwargs, __FUNCTION__,
			&value))
			return GetPyNone();

		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		mvApp::GetApp()->setStyleChanged();
		style.FrameBorderSize = value;

		return GetPyNone();
	}

	PyObject* set_style_tab_border_size(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		float value;

		if (!(*mvApp::GetApp()->getParsers())["set_style_tab_border_size"].parse(args, kwargs, __FUNCTION__,
			&value))
			return GetPyNone();

		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		mvApp::GetApp()->setStyleChanged();
		style.TabBorderSize = value;

		return GetPyNone();
	}

	PyObject* set_style_window_rounding(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		float value;

		if (!(*mvApp::GetApp()->getParsers())["set_style_window_rounding"].parse(args, kwargs, __FUNCTION__,
			&value))
			return GetPyNone();

		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		mvApp::GetApp()->setStyleChanged();
		style.WindowRounding = value;

		return GetPyNone();
	}

	PyObject* set_style_child_rounding(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		float value;

		if (!(*mvApp::GetApp()->getParsers())["set_style_child_rounding"].parse(args, kwargs, __FUNCTION__,
			&value))
			return GetPyNone();

		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		mvApp::GetApp()->setStyleChanged();
		style.ChildRounding = value;

		return GetPyNone();
	}

	PyObject* set_style_frame_rounding(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		float value;

		if (!(*mvApp::GetApp()->getParsers())["set_style_frame_rounding"].parse(args, kwargs, __FUNCTION__,
			&value))
			return GetPyNone();

		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		mvApp::GetApp()->setStyleChanged();
		style.FrameRounding = value;

		return GetPyNone();
	}

	PyObject* set_style_popup_rounding(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		float value;

		if (!(*mvApp::GetApp()->getParsers())["set_style_popup_rounding"].parse(args, kwargs, __FUNCTION__,
			&value))
			return GetPyNone();

		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		mvApp::GetApp()->setStyleChanged();
		style.PopupRounding = value;

		return GetPyNone();
	}

	PyObject* set_style_scrollbar_rounding(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		float value;

		if (!(*mvApp::GetApp()->getParsers())["set_style_scrollbar_rounding"].parse(args, kwargs, __FUNCTION__,
			&value))
			return GetPyNone();

		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		mvApp::GetApp()->setStyleChanged();
		style.ScrollbarRounding = value;

		return GetPyNone();
	}

	PyObject* set_style_grab_rounding(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		float value;

		if (!(*mvApp::GetApp()->getParsers())["set_style_grab_rounding"].parse(args, kwargs, __FUNCTION__,
			&value))
			return GetPyNone();

		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		mvApp::GetApp()->setStyleChanged();
		style.GrabRounding = value;

		return GetPyNone();
	}

	PyObject* set_style_tab_rounding(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		float value;

		if (!(*mvApp::GetApp()->getParsers())["set_style_tab_rounding"].parse(args, kwargs, __FUNCTION__,
			&value))
			return GetPyNone();

		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		mvApp::GetApp()->setStyleChanged();
		style.TabRounding = value;

		return GetPyNone();
	}

	PyObject* set_style_window_title_align(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		float x;
		float y;

		if (!(*mvApp::GetApp()->getParsers())["set_style_window_title_align"].parse(args, kwargs, __FUNCTION__,
			&x, &y))
			return GetPyNone();

		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		mvApp::GetApp()->setStyleChanged();
		style.WindowTitleAlign = { x, y };

		return GetPyNone();
	}

	PyObject* set_style_window_menu_button_position(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int value;

		if (!(*mvApp::GetApp()->getParsers())["set_style_window_menu_button_position"].parse(args, kwargs, __FUNCTION__,
			&value))
			return GetPyNone();

		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		mvApp::GetApp()->setStyleChanged();
		style.WindowMenuButtonPosition = value;

		return GetPyNone();
	}

	PyObject* set_style_color_button_position(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int value;

		if (!(*mvApp::GetApp()->getParsers())["set_style_color_button_position"].parse(args, kwargs, __FUNCTION__,
			&value))
			return GetPyNone();

		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		mvApp::GetApp()->setStyleChanged();
		style.ColorButtonPosition = value;

		return GetPyNone();
	}

	PyObject* set_style_button_text_align(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		float x;
		float y;

		if (!(*mvApp::GetApp()->getParsers())["set_style_window_title_align"].parse(args, kwargs, __FUNCTION__,
			&x, &y))
			return GetPyNone();

		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		mvApp::GetApp()->setStyleChanged();
		style.ButtonTextAlign = { x, y };

		return GetPyNone();
	}

	PyObject* set_style_selectable_text_align(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		float x;
		float y;

		if (!(*mvApp::GetApp()->getParsers())["set_style_selectable_text_align"].parse(args, kwargs, __FUNCTION__,
			&x, &y))
			return GetPyNone();

		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		mvApp::GetApp()->setStyleChanged();
		style.SelectableTextAlign = { x, y };

		return GetPyNone();
	}

	PyObject* set_style_display_safe_area_padding(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		float x;
		float y;

		if (!(*mvApp::GetApp()->getParsers())["set_style_display_safe_area_padding"].parse(args, kwargs, __FUNCTION__,
			&x, &y))
			return GetPyNone();

		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		mvApp::GetApp()->setStyleChanged();
		style.DisplaySafeAreaPadding = { x, y };

		return GetPyNone();
	}

	PyObject* set_style_global_alpha(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		float value;

		if (!(*mvApp::GetApp()->getParsers())["set_style_global_alpha"].parse(args, kwargs, __FUNCTION__,
			&value))
			return GetPyNone();

		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		mvApp::GetApp()->setStyleChanged();
		style.Alpha = value;

		return GetPyNone();
	}

	PyObject* set_style_curve_tessellation_tolerance(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		float value;

		if (!(*mvApp::GetApp()->getParsers())["set_style_curve_tessellation_tolerance"].parse(args, kwargs, __FUNCTION__,
			&value))
			return GetPyNone();

		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		mvApp::GetApp()->setStyleChanged();
		style.CurveTessellationTol = value;

		return GetPyNone();
	}

	PyObject* set_style_circle_segment_max_error(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		float value;

		if (!(*mvApp::GetApp()->getParsers())["set_style_circle_segment_max_error"].parse(args, kwargs, __FUNCTION__,
			&value))
			return GetPyNone();

		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		mvApp::GetApp()->setStyleChanged();
		style.CircleSegmentMaxError = value;

		return GetPyNone();
	}

	PyObject* set_style_antialiased_lines(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int value;

		if (!(*mvApp::GetApp()->getParsers())["set_style_antialiased_lines"].parse(args, kwargs, __FUNCTION__,
			&value))
			return GetPyNone();

		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		mvApp::GetApp()->setStyleChanged();
		style.AntiAliasedLines = value;

		return GetPyNone();
	}

	PyObject* set_style_antialiased_fill(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int value;

		if (!(*mvApp::GetApp()->getParsers())["set_style_antialiased_fill"].parse(args, kwargs, __FUNCTION__,
			&value))
			return GetPyNone();

		ImGuiStyle& style = mvApp::GetApp()->getStyle();
		mvApp::GetApp()->setStyleChanged();
		style.AntiAliasedFill = value;

		return GetPyNone();
	}

	PyObject* set_vsync(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		int value;

		if (!(*mvApp::GetApp()->getParsers())["set_vsync"].parse(args, kwargs, __FUNCTION__,
			&value))
			return GetPyNone();

		mvApp::GetApp()->setVSync(value);
		return GetPyNone();
	}

	PyObject* setup_dearpygui(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		mvApp::GetApp()->precheck();
		mvApp::SetAppStarted();

		// create window
		auto window = mvWindow::CreatemvWindow(mvApp::GetApp()->getActualWidth(), mvApp::GetApp()->getActualHeight(), false);
		window->show();
		mvApp::GetApp()->setViewport(window);
		window->setup();

		return GetPyNone();
	}

	PyObject* render_dearpygui_frame(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		auto window = mvApp::GetApp()->getViewport();
		window->renderFrame();

		return GetPyNone();
	}

	PyObject* cleanup_dearpygui(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		auto window = mvApp::GetApp()->getViewport();
		delete window;
		mvApp::GetApp()->setViewport(nullptr);
		mvApp::SetAppStopped();
		mvApp::DeleteApp();

		return GetPyNone();
	}

	PyObject* start_dearpygui(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		mvApp::GetApp()->precheck();
		mvApp::SetAppStarted();

		// create window
		auto window = mvWindow::CreatemvWindow(mvApp::GetApp()->getActualWidth(), mvApp::GetApp()->getActualHeight(), false);
		mvApp::GetApp()->setViewport(window);
		window->show();
		window->run();
		delete window;
		mvApp::SetAppStopped();
		mvApp::DeleteApp();

		return GetPyNone();
	}

	PyObject* set_start_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;

		if (!(*mvApp::GetApp()->getParsers())["set_start_callback"].parse(args, kwargs, __FUNCTION__, &callback))
			return GetPyNone();

		mvApp::GetApp()->setOnStartCallback(callback);
		return GetPyNone();
	}

	PyObject* set_exit_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;

		if (!(*mvApp::GetApp()->getParsers())["set_exit_callback"].parse(args, kwargs, __FUNCTION__, &callback))
			return GetPyNone();

		mvApp::GetApp()->setOnCloseCallback(callback);
		return GetPyNone();
	}

	PyObject* stop_dearpygui(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::SetAppStopped();
		return GetPyNone();
	}

	PyObject* set_global_font_scale(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		float scale;

		if (!(*mvApp::GetApp()->getParsers())["set_global_font_scale"].parse(args, kwargs, __FUNCTION__, &scale))
			return GetPyNone();

		mvApp::GetApp()->setGlobalFontScale(scale);

		return GetPyNone();
	}

	PyObject* get_global_font_scale(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return ToPyFloat(mvApp::GetApp()->getGlobalFontScale());
	}

	PyObject* show_logger(PyObject* self, PyObject* args)
	{
		mvAppLog::Show();
		return GetPyNone();
	}

	PyObject* get_log_level(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return ToPyInt(mvAppLog::getLogLevel());
	}

	PyObject* set_log_level(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int level;
		if (!(*mvApp::GetApp()->getParsers())["set_log_level"].parse(args, kwargs, __FUNCTION__, &level))
			return GetPyNone();
		mvAppLog::setLogLevel(level);
		return GetPyNone();
	}

	PyObject* log(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* message;
		const char* level = "TRACE";
		if (!(*mvApp::GetApp()->getParsers())["log"].parse(args, kwargs, __FUNCTION__, &message, &level))
			return GetPyNone();

		mvAppLog::Log(ToString(message), std::string(level));
		return GetPyNone();
	}

	PyObject* log_debug(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* message;

		if (!(*mvApp::GetApp()->getParsers())["log_debug"].parse(args, kwargs, __FUNCTION__, &message))
			return GetPyNone();

		mvAppLog::LogDebug(ToString(message));
		return GetPyNone();
	}

	PyObject* log_info(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* message;

		if (!(*mvApp::GetApp()->getParsers())["log_info"].parse(args, kwargs, __FUNCTION__, &message))
			return GetPyNone();

		mvAppLog::LogInfo(ToString(message));
		return GetPyNone();
	}

	PyObject* log_warning(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* message;

		if (!(*mvApp::GetApp()->getParsers())["log_warning"].parse(args, kwargs, __FUNCTION__, &message))
			return GetPyNone();

		mvAppLog::LogWarning(ToString(message));
		return GetPyNone();
	}

	PyObject* log_error(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* message;

		if (!(*mvApp::GetApp()->getParsers())["log_error"].parse(args, kwargs, __FUNCTION__, &message))
			return GetPyNone();

		mvAppLog::LogError(ToString(message));
		return GetPyNone();
	}

	PyObject* clear_log(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvAppLog::ClearLog();
		return GetPyNone();
	}

	PyObject* get_mouse_pos(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int local = true;

		if (!(*mvApp::GetApp()->getParsers())["get_mouse_pos"].parse(args, kwargs, __FUNCTION__, &local))
			return GetPyNone();

		mvVec2 pos = mvInput::getMousePosition();
		if (!local)
			pos = mvInput::getGlobalMousePosition();
		PyObject* pvalue = ToPyPair(pos.x, pos.y);
		return pvalue;
	}

	PyObject* get_mouse_drag_delta(PyObject* self, PyObject* args)
	{
		mvVec2 pos = mvInput::getMouseDragDelta();
		PyObject* pvalue = ToPyPair(pos.x, pos.y);
		return pvalue;
	}

	PyObject* is_key_pressed(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int key;

		if (!(*mvApp::GetApp()->getParsers())["is_key_pressed"].parse(args, kwargs, __FUNCTION__, &key))
			return GetPyNone();

		bool pressed = mvInput::isKeyPressed(key);

		PyObject* pvalue = ToPyBool(pressed);

		return pvalue;
	}

	PyObject* is_key_released(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int key;

		if (!(*mvApp::GetApp()->getParsers())["is_key_released"].parse(args, kwargs, __FUNCTION__, &key))
			return GetPyNone();

		bool pressed = mvInput::isKeyReleased(key);

		PyObject* pvalue = ToPyBool(pressed);

		return pvalue;
	}

	PyObject* is_key_down(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int key;

		if (!(*mvApp::GetApp()->getParsers())["is_key_down"].parse(args, kwargs, __FUNCTION__, &key))
			return GetPyNone();

		bool pressed = mvInput::isKeyDown(key);

		PyObject* pvalue = ToPyBool(pressed);

		return pvalue;
	}

	PyObject* is_mouse_button_dragging(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int button;
		float threshold;

		if (!(*mvApp::GetApp()->getParsers())["is_mouse_button_dragging"].parse(args, kwargs, __FUNCTION__, &button, &threshold))
			return GetPyNone();

		return ToPyBool(mvInput::isMouseDragging(button, threshold));
	}

	PyObject* is_mouse_button_down(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int button;

		if (!(*mvApp::GetApp()->getParsers())["is_mouse_button_down"].parse(args, kwargs, __FUNCTION__, &button))
			return GetPyNone();

		return ToPyBool(mvInput::isMouseButtonDown(button));
	}

	PyObject* is_mouse_button_clicked(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int button;

		if (!(*mvApp::GetApp()->getParsers())["is_mouse_button_clicked"].parse(args, kwargs, __FUNCTION__, &button))
			return GetPyNone();

		return ToPyBool(mvInput::isMouseButtonClicked(button));
	}

	PyObject* is_mouse_button_double_clicked(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int button;

		if (!(*mvApp::GetApp()->getParsers())["is_mouse_button_double_clicked"].parse(args, kwargs, __FUNCTION__, &button))
			return GetPyNone();

		return ToPyBool(mvInput::isMouseButtonDoubleClicked(button));
	}

	PyObject* is_mouse_button_released(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int button;

		if (!(*mvApp::GetApp()->getParsers())["is_mouse_button_released"].parse(args, kwargs, __FUNCTION__, &button))
			return GetPyNone();

		return ToPyBool(mvInput::isMouseButtonReleased(button));
	}

	PyObject* set_mouse_down_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;
		const char* handler = "MainWindow";

		if (!(*mvApp::GetApp()->getParsers())["set_mouse_down_callback"].parse(args, kwargs, __FUNCTION__, &callback, &handler))
			return GetPyNone();

		if (std::string(handler) == "MainWindow")
			mvApp::GetApp()->setMouseDownCallback(callback);
		else
		{
			mvAppItem* item = mvApp::GetApp()->getItem(handler);
			if (item)
			{
				if (item->getType() == mvAppItemType::Window)
				{
					auto windowtype = static_cast<mvWindowAppitem*>(item);
					windowtype->setMouseDownCallback(callback);
					return GetPyNone();
				}
				else
				{
					ThrowPythonException(std::string(handler) + " handler is not a window.");
					return GetPyNone();
				}
			}


			// check if item is a standard window
			mvStandardWindow* window = mvApp::GetApp()->getStandardWindow(handler);
			if (window == nullptr)
			{
				ThrowPythonException(std::string(handler) + " handler item was not found.");
				return GetPyNone();
			}

			window->setMouseDownCallback(callback);
		}

		return GetPyNone();
	}

	PyObject* set_mouse_drag_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;
		float threshold;
		const char* handler = "MainWindow";

		if (!(*mvApp::GetApp()->getParsers())["set_mouse_drag_callback"].parse(args, kwargs, __FUNCTION__, &callback, &threshold, &handler))
			return GetPyNone();

		mvInput::setMouseDragThreshold(threshold);

		if (std::string(handler) == "MainWindow")
			mvApp::GetApp()->setMouseDragCallback(callback);
		else
		{
			mvAppItem* item = mvApp::GetApp()->getItem(handler);
			if (item)
			{
				if (item->getType() == mvAppItemType::Window)
				{
					auto windowtype = static_cast<mvWindowAppitem*>(item);
					windowtype->setMouseDragCallback(callback);
					return GetPyNone();
				}
				else
				{
					ThrowPythonException(std::string(handler) + " handler is not a window.");
					return GetPyNone();
				}
			}


			// check if item is a standard window
			mvStandardWindow* window = mvApp::GetApp()->getStandardWindow(handler);
			if (window == nullptr)
			{
				ThrowPythonException(std::string(handler) + " handler item was not found.");
				return GetPyNone();
			}

			window->setMouseDragCallback(callback);
		}

		return GetPyNone();
	}

	PyObject* set_mouse_double_click_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;
		const char* handler;

		if (!(*mvApp::GetApp()->getParsers())["set_mouse_double_click_callback"].parse(args, kwargs, __FUNCTION__, &callback, &handler))
			return GetPyNone();

		if (std::string(handler) == "MainWindow")
			mvApp::GetApp()->setMouseDoubleClickCallback(callback);
		else
		{
			mvAppItem* item = mvApp::GetApp()->getItem(handler);
			if (item)
			{
				if (item->getType() == mvAppItemType::Window)
				{
					auto windowtype = static_cast<mvWindowAppitem*>(item);
					windowtype->setMouseDoubleClickCallback(callback);
					return GetPyNone();
				}
				else
				{
					ThrowPythonException(std::string(handler) + " handler is not a window.");
					return GetPyNone();
				}
			}


			// check if item is a standard window
			mvStandardWindow* window = mvApp::GetApp()->getStandardWindow(handler);
			if (window == nullptr)
			{
				ThrowPythonException(std::string(handler) + " handler item was not found.");
				return GetPyNone();
			}

			window->setMouseDoubleClickCallback(callback);
		}

		return GetPyNone();
	}

	PyObject* set_mouse_click_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;
		const char* handler = "MainWindow";

		if (!(*mvApp::GetApp()->getParsers())["set_mouse_click_callback"].parse(args, kwargs, __FUNCTION__, &callback, &handler))
			return GetPyNone();

		if (std::string(handler) == "MainWindow")
			mvApp::GetApp()->setMouseClickCallback(callback);
		else
		{
			mvAppItem* item = mvApp::GetApp()->getItem(handler);
			if (item)
			{
				if (item->getType() == mvAppItemType::Window)
				{
					auto windowtype = static_cast<mvWindowAppitem*>(item);
					windowtype->setMouseClickCallback(callback);
					return GetPyNone();
				}
				else
				{
					ThrowPythonException(std::string(handler) + " handler is not a window.");
					return GetPyNone();
				}
			}


			// check if item is a standard window
			mvStandardWindow* window = mvApp::GetApp()->getStandardWindow(handler);
			if (window == nullptr)
			{
				ThrowPythonException(std::string(handler) + " handler item was not found.");
				return GetPyNone();
			}

			window->setMouseClickCallback(callback);
		}

		return GetPyNone();
	}

	PyObject* set_mouse_release_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;
		const char* handler = "MainWindow";

		if (!(*mvApp::GetApp()->getParsers())["set_mouse_release_callback"].parse(args, kwargs, __FUNCTION__, &callback, &handler))
			return GetPyNone();

		if (std::string(handler) == "MainWindow")
			mvApp::GetApp()->setMouseReleaseCallback(callback);
		else
		{
			mvAppItem* item = mvApp::GetApp()->getItem(handler);
			if (item)
			{
				if (item->getType() == mvAppItemType::Window)
				{
					auto windowtype = static_cast<mvWindowAppitem*>(item);
					windowtype->setMouseReleaseCallback(callback);
					return GetPyNone();
				}
				else
				{
					ThrowPythonException(std::string(handler) + " handler is not a window.");
					return GetPyNone();
				}
			}


			// check if item is a standard window
			mvStandardWindow* window = mvApp::GetApp()->getStandardWindow(handler);
			if (window == nullptr)
			{
				ThrowPythonException(std::string(handler) + " handler item was not found.");
				return GetPyNone();
			}

			window->setMouseReleaseCallback(callback);
		}

		return GetPyNone();
	}

	PyObject* set_key_down_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;
		const char* handler = "MainWindow";

		if (!(*mvApp::GetApp()->getParsers())["set_key_down_callback"].parse(args, kwargs, __FUNCTION__, &callback, &handler))
			return GetPyNone();

		if (std::string(handler) == "MainWindow")
			mvApp::GetApp()->setKeyDownCallback(callback);
		else
		{
			mvAppItem* item = mvApp::GetApp()->getItem(handler);
			if (item)
			{
				if (item->getType() == mvAppItemType::Window)
				{
					auto windowtype = static_cast<mvWindowAppitem*>(item);
					windowtype->setKeyDownCallback(callback);
					return GetPyNone();
				}
				else
				{
					ThrowPythonException(std::string(handler) + " handler is not a window.");
					return GetPyNone();
				}
			}


			// check if item is a standard window
			mvStandardWindow* window = mvApp::GetApp()->getStandardWindow(handler);
			if (window == nullptr)
			{
				ThrowPythonException(std::string(handler) + " handler item was not found.");
				return GetPyNone();
			}

			window->setKeyDownCallback(callback);
		}

		return GetPyNone();
	}

	PyObject* set_key_press_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;
		const char* handler = "MainWindow";

		if (!(*mvApp::GetApp()->getParsers())["set_key_press_callback"].parse(args, kwargs, __FUNCTION__, &callback, &handler))
			return GetPyNone();

		if (std::string(handler) == "MainWindow")
			mvApp::GetApp()->setKeyPressCallback(callback);
		else
		{
			mvAppItem* item = mvApp::GetApp()->getItem(handler);
			if (item)
			{
				if (item->getType() == mvAppItemType::Window)
				{
					auto windowtype = static_cast<mvWindowAppitem*>(item);
					windowtype->setKeyPressCallback(callback);
					return GetPyNone();
				}
				else
				{
					ThrowPythonException(std::string(handler) + " handler is not a window.");
					return GetPyNone();
				}
			}


			// check if item is a standard window
			mvStandardWindow* window = mvApp::GetApp()->getStandardWindow(handler);
			if (window == nullptr)
			{
				ThrowPythonException(std::string(handler) + " handler item was not found.");
				return GetPyNone();
			}

			window->setKeyPressCallback(callback);
		}

		return GetPyNone();
	}

	PyObject* set_key_release_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;
		const char* handler = "MainWindow";

		if (!(*mvApp::GetApp()->getParsers())["set_key_release_callback"].parse(args, kwargs, __FUNCTION__, &callback, &handler))
			return GetPyNone();

		if (std::string(handler) == "MainWindow")
			mvApp::GetApp()->setKeyReleaseCallback(callback);
		else
		{
			mvAppItem* item = mvApp::GetApp()->getItem(handler);
			if (item)
			{
				if (item->getType() == mvAppItemType::Window)
				{
					auto windowtype = static_cast<mvWindowAppitem*>(item);
					windowtype->setKeyReleaseCallback(callback);
					return GetPyNone();
				}
				else
				{
					ThrowPythonException(std::string(handler) + " handler is not a window.");
					return GetPyNone();
				}
			}


			// check if item is a standard window
			mvStandardWindow* window = mvApp::GetApp()->getStandardWindow(handler);
			if (window == nullptr)
			{
				ThrowPythonException(std::string(handler) + " handler item was not found.");
				return GetPyNone();
			}

			window->setKeyReleaseCallback(callback);
		}

		return GetPyNone();
	}

	PyObject* set_mouse_wheel_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;
		const char* handler = "MainWindow";

		if (!(*mvApp::GetApp()->getParsers())["set_mouse_wheel_callback"].parse(args, kwargs, __FUNCTION__, &callback, &handler))
			return GetPyNone();

		if (std::string(handler) == "MainWindow")
			mvApp::GetApp()->setMouseWheelCallback(callback);
		else
		{
			mvAppItem* item = mvApp::GetApp()->getItem(handler);
			if (item)
			{
				if (item->getType() == mvAppItemType::Window)
				{
					auto windowtype = static_cast<mvWindowAppitem*>(item);
					windowtype->setMouseWheelCallback(callback);
					return GetPyNone();
				}
				else
				{
					ThrowPythonException(std::string(handler) + " handler is not a window.");
					return GetPyNone();
				}
			}


			// check if item is a standard window
			mvStandardWindow* window = mvApp::GetApp()->getStandardWindow(handler);
			if (window == nullptr)
			{
				ThrowPythonException(std::string(handler) + " handler item was not found.");
				return GetPyNone();
			}

			window->setMouseWheelCallback(callback);
		}

		return GetPyNone();
	}

	PyObject* set_mouse_move_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;
		const char* handler = "MainWindow";

		if (!(*mvApp::GetApp()->getParsers())["set_mouse_move_callback"].parse(args, kwargs, __FUNCTION__, &callback, &handler))
			return GetPyNone();

		if (std::string(handler) == "MainWindow")
			mvApp::GetApp()->getWindow("MainWindow")->setMouseMoveCallback(callback);

		else
		{
			mvAppItem* item = mvApp::GetApp()->getItem(handler);
			if (item)
			{
				if (item->getType() == mvAppItemType::Window)
				{
					auto windowtype = static_cast<mvWindowAppitem*>(item);
					windowtype->setMouseMoveCallback(callback);
					return GetPyNone();
				}
				else
				{
					ThrowPythonException(std::string(handler) + " handler is not a window.");
					return GetPyNone();
				}
			}


			// check if item is a standard window
			mvStandardWindow* window = mvApp::GetApp()->getStandardWindow(handler);
			if (window == nullptr)
			{
				ThrowPythonException(std::string(handler) + " handler item was not found.");
				return GetPyNone();
			}
			
			window->setMouseMoveCallback(callback);
		}

		return GetPyNone();
	}

	PyObject* add_table(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* headers;
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* parent = "";
		const char* before = "";

		if (!(*mvApp::GetApp()->getParsers())["add_table"].parse(args, kwargs, __FUNCTION__, &name, &headers, &callback, &callback_data, &parent,
			&before))
			return ToPyBool(false);

		mvAppItem* item = new mvTable(name, ToStringVect(headers));
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_simple_plot(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* overlay = "";
		float minscale = 0.0f;
		float maxscale = 0.0f;
		int autoscale = true;
		int histogram = false;
		PyObject* value;
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		int width = 0;
		int height = 0;
		const char* data_source = "";

		if (!(*mvApp::GetApp()->getParsers())["add_simple_plot"].parse(args, kwargs, __FUNCTION__, &name, &value, &autoscale, &overlay,
			&minscale, &maxscale, &histogram, &tip, &parent, &before, &width, &height, &data_source))
			return ToPyBool(false);

		std::vector<float> values = ToFloatVect(value);

		if (autoscale)
		{
			maxscale = values[0];
			minscale = values[0];

			for (auto& item : values)
			{
				if (item > maxscale)
					maxscale = item;
				if (item < minscale)
					minscale = item;
			}
		}

		mvAppItem* item = new mvSimplePlot(name, values, overlay, minscale, maxscale, height, histogram);
		item->setTip(tip);
		item->setWidth(width);
		item->setHeight(height);
		item->setDataSource(data_source);

		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_progress_bar(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		float default_value = 0.0f;
		const char* overlay = "";
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;
		int height = 0;

		if (!(*mvApp::GetApp()->getParsers())["add_progress_bar"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&overlay, &tip, &parent, &before, &data_source, &width, &height))
			return ToPyBool(false);

		mvAppItem* item = new mvProgressBar(name, default_value, overlay);
		item->setTip(tip);
		item->setWidth(width);
		item->setHeight(height);
		item->setDataSource(data_source);

		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_image(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* value;
		PyObject* tintcolor = PyTuple_New(4);
		PyTuple_SetItem(tintcolor, 0, PyFloat_FromDouble(1.0));
		PyTuple_SetItem(tintcolor, 1, PyFloat_FromDouble(1.0));
		PyTuple_SetItem(tintcolor, 2, PyFloat_FromDouble(1.0));
		PyTuple_SetItem(tintcolor, 3, PyFloat_FromDouble(1.0));
		PyObject* bordercolor = PyTuple_New(4);
		PyTuple_SetItem(bordercolor, 0, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(bordercolor, 1, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(bordercolor, 2, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(bordercolor, 3, PyFloat_FromDouble(0.0));
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;
		int height = 0;
		PyObject* uv_min = PyTuple_New(2);
		PyTuple_SetItem(uv_min, 0, PyFloat_FromDouble(0));
		PyTuple_SetItem(uv_min, 1, PyFloat_FromDouble(0));
		PyObject* uv_max = PyTuple_New(2);
		PyTuple_SetItem(uv_max, 0, PyFloat_FromDouble(1));
		PyTuple_SetItem(uv_max, 1, PyFloat_FromDouble(1));
		const char* secondary_data_source = "";

		if (!(*mvApp::GetApp()->getParsers())["add_image"].parse(args, kwargs, __FUNCTION__, &name,
			&value, &tintcolor, &bordercolor, &tip, &parent, &before, &data_source, &width,
			&height, &uv_min, &uv_max, &secondary_data_source))
			return ToPyBool(false);

		auto mtintcolor = ToColor(tintcolor);
		auto mbordercolor = ToColor(bordercolor);
		mvVec2 muv_min = ToVec2(uv_min);
		mvVec2 muv_max = ToVec2(uv_max);

		mvAppItem* item = new mvImage(name, value, mtintcolor, mbordercolor, muv_min, muv_max,
			secondary_data_source);
		item->setTip(tip);
		item->setWidth(width);
		item->setHeight(height);
		item->setDataSource(data_source);

		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_image_button(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* value;
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		PyObject* tintcolor = PyTuple_New(4);
		PyTuple_SetItem(tintcolor, 0, PyFloat_FromDouble(255.0));
		PyTuple_SetItem(tintcolor, 1, PyFloat_FromDouble(255.0));
		PyTuple_SetItem(tintcolor, 2, PyFloat_FromDouble(255.0));
		PyTuple_SetItem(tintcolor, 3, PyFloat_FromDouble(255.0));
		PyObject* backgroundColor = PyTuple_New(4);
		PyTuple_SetItem(backgroundColor, 0, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(backgroundColor, 1, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(backgroundColor, 2, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(backgroundColor, 3, PyFloat_FromDouble(0.0));
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		int width = 0;
		int height = 0;
		int frame_padding = -1;
		PyObject* uv_min = PyTuple_New(2);
		PyTuple_SetItem(uv_min, 0, PyFloat_FromDouble(0));
		PyTuple_SetItem(uv_min, 1, PyFloat_FromDouble(0));
		PyObject* uv_max = PyTuple_New(2);
		PyTuple_SetItem(uv_max, 0, PyFloat_FromDouble(1));
		PyTuple_SetItem(uv_max, 1, PyFloat_FromDouble(1));

		if (!(*mvApp::GetApp()->getParsers())["add_image_button"].parse(args, kwargs, __FUNCTION__, 
			&name, &value, &callback, &callback_data, &tintcolor, &backgroundColor, &tip, &parent, 
			&before, &width, &height, &frame_padding, &uv_min, &uv_max))
			return ToPyBool(false);

		auto mtintcolor = ToColor(tintcolor);
		auto mbackgroundColor = ToColor(backgroundColor);
		mvVec2 muv_min = ToVec2(uv_min);
		mvVec2 muv_max = ToVec2(uv_max);

		mvAppItem* item = new mvImageButton(name, value, mtintcolor, 
			mbackgroundColor, muv_min, muv_max,
			frame_padding);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);
		item->setTip(tip);
		item->setWidth(width);
		item->setHeight(height);

		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_drag_float(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		float default_value = 0.0f;
		float speed = 1.0f;
		float min_value = 0.0f;
		float max_value = 100.0f;
		const char* format = "%.3f";
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		int width = 0;
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";
		int on_enter = false;

		if (!(*mvApp::GetApp()->getParsers())["add_drag_float"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
			&min_value, &max_value, &format, &callback, &callback_data, &tip, &parent, &before, &data_source, &width, &on_enter))
			return ToPyBool(false);

		std::vector<float> defaults;
		defaults.push_back(default_value);
		defaults.push_back(0.0f);
		defaults.push_back(0.0f);
		defaults.push_back(0.0f);

		ImGuiInputTextFlags flags = 0;
		if (on_enter)
			flags = ImGuiInputTextFlags_EnterReturnsTrue;

		mvAppItem* item = new mvDragFloat<mvAppItemType::DragFloat, 1, ImGui::DragFloat>(name, defaults.data(), speed, min_value, max_value, format, flags);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);

		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_drag_float2(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 1, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 2, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 3, PyFloat_FromDouble(0.0));
		float speed = 1.0f;
		float min_value = 0.0f;
		float max_value = 100.0f;
		const char* format = "%.3f";
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;
		int on_enter = false;

		if (!(*mvApp::GetApp()->getParsers())["add_drag_float2"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
			&min_value, &max_value, &format, &callback, &callback_data, &tip, &parent, &before, &data_source, &width, &on_enter))
			return ToPyBool(false);

		ImGuiInputTextFlags flags = 0;
		if (on_enter)
			flags = ImGuiInputTextFlags_EnterReturnsTrue;

		auto vec = ToFloatVect(default_value);
		mvAppItem* item = new mvDragFloat<mvAppItemType::DragFloat2, 2, ImGui::DragFloat2>(name, vec.data(), speed, min_value, max_value, format, flags);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_drag_float3(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 1, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 2, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 3, PyFloat_FromDouble(0.0));
		float speed = 1.0f;
		float min_value = 0.0f;
		float max_value = 100.0f;
		const char* format = "%.3f";
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;
		int on_enter = false;

		if (!(*mvApp::GetApp()->getParsers())["add_drag_float3"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
			&min_value, &max_value, &format, &callback, &callback_data, &tip, &parent, &before, &data_source, &width, &on_enter))
			return ToPyBool(false);

		ImGuiInputTextFlags flags = 0;
		if (on_enter)
			flags = ImGuiInputTextFlags_EnterReturnsTrue;

		auto vec = ToFloatVect(default_value);
		mvAppItem* item = new mvDragFloat<mvAppItemType::DragFloat3, 3, ImGui::DragFloat3>(name, vec.data(), speed, min_value, max_value, format, flags);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_drag_float4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 1, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 2, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 3, PyFloat_FromDouble(0.0));
		float speed = 1.0f;
		float min_value = 0.0f;
		float max_value = 100.0f;
		const char* format = "%.3f";
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;
		int on_enter = false;

		if (!(*mvApp::GetApp()->getParsers())["add_drag_float4"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
			&min_value, &max_value, &format, &callback, &callback_data, &tip, &parent, &before, &data_source, &width, &on_enter))
			return ToPyBool(false);

		ImGuiInputTextFlags flags = 0;
		if (on_enter)
			flags = ImGuiInputTextFlags_EnterReturnsTrue;

		auto vec = ToFloatVect(default_value);

		mvAppItem* item = new mvDragFloat<mvAppItemType::DragFloat4, 4, ImGui::DragFloat4>(name, vec.data(), speed, min_value, max_value, format, flags);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_drag_int(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int default_value = 0;
		float speed = 1.0f;
		int min_value = 0;
		int max_value = 100;
		const char* format = "%d";
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;
		int on_enter = false;

		if (!(*mvApp::GetApp()->getParsers())["add_drag_int"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
			&min_value, &max_value, &format, &callback, &callback_data, &tip, &parent, &before, &data_source, &width, &on_enter))
			return ToPyBool(false);

		ImGuiInputTextFlags flags = 0;
		if (on_enter)
			flags = ImGuiInputTextFlags_EnterReturnsTrue;

		std::vector<int> defaults;
		defaults.push_back(default_value);
		defaults.push_back(0.0f);
		defaults.push_back(0.0f);
		defaults.push_back(0.0f);

		mvAppItem* item = new mvDragInt<mvAppItemType::DragInt, 1, ImGui::DragInt>(name, defaults.data(), speed, min_value, max_value, format, flags);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_drag_int2(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 3, PyLong_FromLong(0));
		float speed = 1.0f;
		int min_value = 0;
		int max_value = 100;
		const char* format = "%d";
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;
		int on_enter = false;

		if (!(*mvApp::GetApp()->getParsers())["add_drag_int2"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
			&min_value, &max_value, &format, &callback, &callback_data, &tip, &parent, &before, &data_source, &width, &on_enter))
			return ToPyBool(false);

		ImGuiInputTextFlags flags = 0;
		if (on_enter)
			flags = ImGuiInputTextFlags_EnterReturnsTrue;

		auto vec = ToIntVect(default_value);
		mvAppItem* item = new mvDragInt<mvAppItemType::DragInt2, 2, ImGui::DragInt2>(name, vec.data(), speed, min_value, max_value, format, flags);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_drag_int3(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 3, PyLong_FromLong(0));
		float speed = 1.0f;
		int min_value = 0;
		int max_value = 100;
		const char* format = "%d";
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;
		int on_enter = false;

		if (!(*mvApp::GetApp()->getParsers())["add_drag_int3"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
			&min_value, &max_value, &format, &callback, &callback_data, &tip, &parent, &before, &data_source, &width, &on_enter))
			return ToPyBool(false);

		ImGuiInputTextFlags flags = 0;
		if (on_enter)
			flags = ImGuiInputTextFlags_EnterReturnsTrue;

		auto vec = ToIntVect(default_value);
		mvAppItem* item = new mvDragInt<mvAppItemType::DragInt3, 3, ImGui::DragInt3>(name, vec.data(), speed, min_value, max_value, format, flags);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_drag_int4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 3, PyLong_FromLong(0));
		float speed = 1.0f;
		int min_value = 0;
		int max_value = 100;
		const char* format = "%d";
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;
		int on_enter = false;

		if (!(*mvApp::GetApp()->getParsers())["add_drag_int4"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
			&min_value, &max_value, &format, &callback, &callback_data, &tip, &parent, &before, &data_source, &width, &on_enter))
			return ToPyBool(false);

		ImGuiInputTextFlags flags = 0;
		if (on_enter)
			flags = ImGuiInputTextFlags_EnterReturnsTrue;

		auto vec = ToIntVect(default_value);
		mvAppItem* item = new mvDragInt<mvAppItemType::DragInt4, 4, ImGui::DragInt4>(name, vec.data(), speed, min_value, max_value, format, flags);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_slider_float(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		float default_value = 0.0f;
		float min_value = 0.0f;
		float max_value = 100.0f;
		const char* format = "%.3f";
		int vertical = false;
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;
		int height = 0;
		int on_enter = false;

		if (!(*mvApp::GetApp()->getParsers())["add_slider_float"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&min_value, &max_value, &format, &vertical, &callback, &callback_data, &tip, &parent, &before,
			&data_source, &width, &height, &on_enter))
			return ToPyBool(false);

		ImGuiInputTextFlags flags = 0;
		if (on_enter)
			flags = ImGuiInputTextFlags_EnterReturnsTrue;

		mvAppItem* item = new mvSliderFloat(name, default_value, min_value, max_value, format, vertical, flags);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		item->setHeight(height);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_slider_float2(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 1, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 2, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 3, PyFloat_FromDouble(0.0));
		float min_value = 0.0f;
		float max_value = 100.0f;
		const char* format = "%.3f";
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;
		int on_enter = false;

		if (!(*mvApp::GetApp()->getParsers())["add_slider_float2"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&min_value, &max_value, &format, &callback, &callback_data, &tip, &parent, &before, &data_source,&width, &on_enter))
			return ToPyBool(false);

		ImGuiInputTextFlags flags = 0;
		if (on_enter)
			flags = ImGuiInputTextFlags_EnterReturnsTrue;

		auto vec = ToFloatVect(default_value);

		mvAppItem* item = new mvSliderFloatMulti<mvAppItemType::SliderFloat2, 2, ImGui::SliderFloat2, float>(name, vec.data(), min_value, max_value, format, flags);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_slider_float3(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 1, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 2, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 3, PyFloat_FromDouble(0.0));
		float min_value = 0.0f;
		float max_value = 100.0f;
		const char* format = "%.3f";
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;
		int on_enter = false;

		if (!(*mvApp::GetApp()->getParsers())["add_slider_float3"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&min_value, &max_value, &format, &callback, &callback_data, &tip, &parent, &before, &data_source, &width, &on_enter))
			return ToPyBool(false);

		ImGuiInputTextFlags flags = 0;
		if (on_enter)
			flags = ImGuiInputTextFlags_EnterReturnsTrue;

		auto vec = ToFloatVect(default_value);

		mvAppItem* item = new mvSliderFloatMulti<mvAppItemType::SliderFloat3, 3, ImGui::SliderFloat3, float>(name, vec.data(), min_value, max_value, format, flags);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_slider_float4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 1, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 2, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 3, PyFloat_FromDouble(0.0));
		float min_value = 0.0f;
		float max_value = 100.0f;
		const char* format = "%.3f";
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;
		int on_enter = false;

		if (!(*mvApp::GetApp()->getParsers())["add_slider_float4"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&min_value, &max_value, &format, &callback, &callback_data, &tip, &parent, &before, &data_source,
			&width, &on_enter))
			return ToPyBool(false);

		ImGuiInputTextFlags flags = 0;
		if (on_enter)
			flags = ImGuiInputTextFlags_EnterReturnsTrue;

		auto vec = ToFloatVect(default_value);
		mvAppItem* item = new mvSliderFloatMulti<mvAppItemType::SliderFloat4, 4, ImGui::SliderFloat4, float>(name, vec.data(), min_value, max_value, format, flags);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_slider_int(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int default_value = 0;
		int min_value = 0;
		int max_value = 100;
		const char* format = "%d";
		int vertical = false;
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;
		int height = 0;
		int on_enter = false;

		if (!(*mvApp::GetApp()->getParsers())["add_slider_int"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&min_value, &max_value, &format, &vertical, &callback, &callback_data, &tip, &parent, &before, &data_source,
			&width, &height, &on_enter))
			return ToPyBool(false);

		ImGuiInputTextFlags flags = 0;
		if (on_enter)
			flags = ImGuiInputTextFlags_EnterReturnsTrue;

		mvAppItem* item = new mvSliderInt(name, default_value, min_value, max_value, format, vertical, flags);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		item->setHeight(height);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_slider_int2(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 3, PyLong_FromLong(0));
		int min_value = 0;
		int max_value = 100;
		const char* format = "%d";
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;
		int on_enter = false;

		if (!(*mvApp::GetApp()->getParsers())["add_slider_int2"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&min_value, &max_value, &format, &callback, &callback_data, &tip, &parent, &before, &data_source,
			&width, &on_enter))
			return ToPyBool(false);

		ImGuiInputTextFlags flags = 0;
		if (on_enter)
			flags = ImGuiInputTextFlags_EnterReturnsTrue;

		auto vec = ToIntVect(default_value);
		mvAppItem* item = new mvSliderIntMulti<mvAppItemType::SliderInt2, 2, ImGui::SliderInt2, int>(name, vec.data(), min_value, max_value, format, flags);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_slider_int3(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 3, PyLong_FromLong(0));
		int min_value = 0;
		int max_value = 100;
		const char* format = "%d";
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;
		int on_enter = false;

		if (!(*mvApp::GetApp()->getParsers())["add_slider_int3"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&min_value, &max_value, &format, &callback, &callback_data, &tip, &parent, &before, &data_source,
			&width, &on_enter))
			return ToPyBool(false);

		ImGuiInputTextFlags flags = 0;
		if (on_enter)
			flags = ImGuiInputTextFlags_EnterReturnsTrue;

		auto vec = ToIntVect(default_value);
		mvAppItem* item = new mvSliderIntMulti<mvAppItemType::SliderInt3, 3, ImGui::SliderInt3, int>(name, vec.data(), min_value, max_value, format, flags);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);

		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_slider_int4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 3, PyLong_FromLong(0));
		int min_value = 0;
		int max_value = 100;
		const char* format = "%d";
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;
		int on_enter = false;

		if (!(*mvApp::GetApp()->getParsers())["add_slider_int4"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&min_value, &max_value, &format, &callback, &callback_data, &tip, &parent, &before, &data_source,
			&width, &on_enter))
			return ToPyBool(false);

		ImGuiInputTextFlags flags = 0;
		if (on_enter)
			flags = ImGuiInputTextFlags_EnterReturnsTrue;

		auto vec = ToIntVect(default_value);
		mvAppItem* item = new mvSliderIntMulti<mvAppItemType::SliderInt4, 4, ImGui::SliderInt4, int>(name, vec.data(), min_value, max_value, format, flags);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_text(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int wrap = 0;
		int bullet = false;
		PyObject* color = PyTuple_New(4);
		PyTuple_SetItem(color, 0, PyLong_FromLong(1000));
		PyTuple_SetItem(color, 1, PyLong_FromLong(0));
		PyTuple_SetItem(color, 2, PyLong_FromLong(0));
		PyTuple_SetItem(color, 3, PyLong_FromLong(255));
		const char* tip = "";
		const char* before = "";
		const char* parent = "";


		if (!(*mvApp::GetApp()->getParsers())["add_text"].parse(args, kwargs, __FUNCTION__, &name, &wrap,
			&color, &bullet, &tip, &parent, &before))
			return ToPyBool(false);

		auto mcolor = ToColor(color);

		if (mcolor.r > 500)
			mcolor.specified = false;

		mvAppItem* item = new mvText(name, wrap, mcolor, bullet);
		item->setTip(tip);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_label_text(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* name;
		const char* value;
		PyObject* color = PyTuple_New(4);
		PyTuple_SetItem(color, 0, PyLong_FromLong(1000));
		PyTuple_SetItem(color, 1, PyLong_FromLong(0));
		PyTuple_SetItem(color, 2, PyLong_FromLong(0));
		PyTuple_SetItem(color, 3, PyLong_FromLong(255));
		const char* tip = "";
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";


		if (!(*mvApp::GetApp()->getParsers())["add_label_text"].parse(args, kwargs, __FUNCTION__, &name, &value,
			&color, &tip, &parent, &before, &data_source))
			return ToPyBool(false);

		auto mcolor = ToColor(color);
		if (mcolor.r > 500)
			mcolor.specified = false;

		mvAppItem* item = new mvLabelText(std::string(name), value, mcolor);
		item->setTip(tip);
		item->setDataSource(data_source);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_listbox(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* items;
		int default_value = 0;
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		int width = 0;
		int height = 3;
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";
		const char* secondary_data_source = "";

		if (!(*mvApp::GetApp()->getParsers())["add_listbox"].parse(args, kwargs, __FUNCTION__, &name, &items,
			&default_value, &callback, &callback_data, &tip, &parent, &before, &data_source, &width,
			&height, &secondary_data_source))
			return ToPyBool(false);

		mvAppItem* item = new mvListbox(name, ToStringVect(items),
			default_value, height, secondary_data_source);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_combo(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* default_value = "";
		PyObject* items;
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		int width = 0;
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";
		const char* secondary_data_source = "";

		if (!(*mvApp::GetApp()->getParsers())["add_combo"].parse(args, kwargs, __FUNCTION__, &name, &items,
			&default_value, &callback, &callback_data, &tip, &parent, &before, &data_source, &width
			, &secondary_data_source))
			return ToPyBool(false);

		mvAppItem* item = new mvCombo(name, ToStringVect(items), default_value, secondary_data_source);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_selectable(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int default_value = false;
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";

		if (!(*mvApp::GetApp()->getParsers())["add_selectable"].parse(args, kwargs, __FUNCTION__, &name,
			&default_value, &callback, &callback_data, &tip, &parent, &before, &data_source))
			return ToPyBool(false);

		mvAppItem* item = new mvSelectable(name, default_value);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);
		item->setTip(tip);
		item->setDataSource(data_source);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_button(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int smallb = false;
		int arrow = false;
		int direction = -1;
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		int width = 0;
		int height = 0;
		const char* before = "";
		const char* parent = "";

		if (!(*mvApp::GetApp()->getParsers())["add_button"].parse(args, kwargs, __FUNCTION__, &name, &smallb,
			&arrow, &direction, &callback, &callback_data, &tip, &parent, &before, &width, &height))
			return ToPyBool(false);

		mvAppItem* item = new mvButton(name, smallb, arrow, direction);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);
		item->setTip(tip);
		item->setWidth(width);
		item->setHeight(height);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_input_text(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* default_value = "";
		const char* hint = "";
		int multiline = 0;
		int no_spaces = false;
		int uppercase = false;
		int decimal = false;
		int hexadecimal = false;
		int readonly = false;
		int password = false;
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		int width = 0;
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";
		int on_enter = false;

		int flags = 0;

		if (!(*mvApp::GetApp()->getParsers())["add_input_text"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &hint, &multiline, &no_spaces,
			&uppercase, &decimal, &hexadecimal, &readonly, &password, &callback, &callback_data, &tip, &parent, &before, &data_source, &width, &on_enter))
			return ToPyBool(false);

		if (no_spaces) flags |= ImGuiInputTextFlags_CharsNoBlank;
		if (uppercase) flags |= ImGuiInputTextFlags_CharsUppercase;
		if (decimal) flags |= ImGuiInputTextFlags_CharsDecimal;
		if (hexadecimal) flags |= ImGuiInputTextFlags_CharsHexadecimal;
		if (readonly) flags |= ImGuiInputTextFlags_ReadOnly;
		if (password) flags |= ImGuiInputTextFlags_Password;
		if (on_enter) flags |= ImGuiInputTextFlags_EnterReturnsTrue;

		mvAppItem* item = new mvInputText(name, default_value, hint, multiline, flags);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_input_int(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int default_value = 0;
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		int width = 0;
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";
		int on_enter = false;

		if (!(*mvApp::GetApp()->getParsers())["add_input_int"].parse(args, kwargs, __FUNCTION__, &name,
			&default_value, &callback, &callback_data, &tip, &parent, &before, &data_source, &width, &on_enter))
			return ToPyBool(false);

		ImGuiInputTextFlags flags = 0;
		if (on_enter)
			flags = ImGuiInputTextFlags_EnterReturnsTrue;

		mvAppItem* item = new mvInputInt(name, default_value, flags);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_input_int2(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 3, PyLong_FromLong(0));
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		int width = 0;
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";
		int on_enter = false;

		if (!(*mvApp::GetApp()->getParsers())["add_input_int2"].parse(args, kwargs, __FUNCTION__, &name,
			&default_value, &callback, &callback_data, &tip, &parent, &before, &data_source, &width, &on_enter))
			return ToPyBool(false);

		ImGuiInputTextFlags flags = 0;
		if (on_enter)
			flags = ImGuiInputTextFlags_EnterReturnsTrue;

		auto vec = ToIntVect(default_value);

		mvAppItem* item = new mvInputIntMulti<mvAppItemType::InputInt2, 2, ImGui::InputInt2>(name, vec.data(), flags);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_input_int3(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 3, PyLong_FromLong(0));
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		int width = 0;
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";
		int on_enter = false;

		if (!(*mvApp::GetApp()->getParsers())["add_input_int3"].parse(args, kwargs, __FUNCTION__, &name,
			&default_value, &callback, &callback_data, &tip, &parent, &before, &data_source, &width, &on_enter))
			return ToPyBool(false);

		ImGuiInputTextFlags flags = 0;
		if (on_enter)
			flags = ImGuiInputTextFlags_EnterReturnsTrue;

		auto vec = ToIntVect(default_value);

		mvAppItem* item = new mvInputIntMulti<mvAppItemType::InputInt3, 3, ImGui::InputInt3>(name, vec.data(), flags);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_input_int4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 3, PyLong_FromLong(0));
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		int width = 0;
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";
		int on_enter = false;

		if (!(*mvApp::GetApp()->getParsers())["add_input_int4"].parse(args, kwargs, __FUNCTION__, &name,
			&default_value, &callback, &callback_data, &tip, &parent, &before, &data_source, &width, &on_enter))
			return ToPyBool(false);

		ImGuiInputTextFlags flags = 0;
		if (on_enter)
			flags = ImGuiInputTextFlags_EnterReturnsTrue;

		auto vec = ToIntVect(default_value);
		mvAppItem* item = new mvInputIntMulti<mvAppItemType::InputInt4, 4, ImGui::InputInt4>(name, vec.data(), flags);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_input_float(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		float default_value = 0.0f;
		const char* format = "%.3f";
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		int width = 0;
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";
		int on_enter = false;

		if (!(*mvApp::GetApp()->getParsers())["add_input_float"].parse(args, kwargs, __FUNCTION__, &name,
			&default_value, &format, &callback, &callback_data, &tip, &parent, &before, &data_source, &width, &on_enter))
			return ToPyBool(false);

		ImGuiInputTextFlags flags = 0;
		if (on_enter)
			flags = ImGuiInputTextFlags_EnterReturnsTrue;

		mvAppItem* item = new mvInputFloat(name, default_value, format, flags);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_input_float2(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(2);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		const char* format = "%.3f";
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		int width = 0;
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";
		int on_enter = false;

		if (!(*mvApp::GetApp()->getParsers())["add_input_float2"].parse(args, kwargs, __FUNCTION__, &name,
			&default_value, &format, &callback, &callback_data, &tip, &parent, &before, &data_source, &width, &on_enter))
			return ToPyBool(false);

		ImGuiInputTextFlags flags = 0;
		if (on_enter)
			flags = ImGuiInputTextFlags_EnterReturnsTrue;

		auto vec = ToFloatVect(default_value);
		mvAppItem* item = new mvInputFloatMulti<mvAppItemType::InputFloat2, 2, ImGui::InputFloat2>(name, vec.data(), format, flags);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_input_float3(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(3);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		const char* format = "%.3f";
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		int width = 0;
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";
		int on_enter = false;

		if (!(*mvApp::GetApp()->getParsers())["add_input_float3"].parse(args, kwargs, __FUNCTION__, &name,
			&default_value, &format, &callback, &callback_data, &tip, &parent, &before, &data_source, &width, &on_enter))
			return ToPyBool(false);

		ImGuiInputTextFlags flags = 0;
		if (on_enter)
		flags = ImGuiInputTextFlags_EnterReturnsTrue;

		auto vec = ToFloatVect(default_value);

		mvAppItem* item = new mvInputFloatMulti<mvAppItemType::InputFloat3, 3, ImGui::InputFloat3>(name, vec.data(), format, flags);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_input_float4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 3, PyLong_FromLong(0));
		const char* format = "%.3f";
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		int width = 0;
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";
		int on_enter = false;

		if (!(*mvApp::GetApp()->getParsers())["add_input_float4"].parse(args, kwargs, __FUNCTION__, &name,
			&default_value, &format, &callback, &callback_data, &tip, &parent, &before, &data_source, &width, &on_enter))
			return ToPyBool(false);

		ImGuiInputTextFlags flags = 0;
		if (on_enter)
			flags = ImGuiInputTextFlags_EnterReturnsTrue;

		auto vec = ToFloatVect(default_value);

		mvAppItem* item = new mvInputFloatMulti<mvAppItemType::InputFloat4, 4, ImGui::InputFloat4>(name, vec.data(), format, flags);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_indent(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		std::string sname = std::string("indent" + std::to_string(i));
		const char* name = sname.c_str();
		float offset = 0.0f;
		const char* before = "";
		const char* parent = "";

		if (!(*mvApp::GetApp()->getParsers())["add_indent"].parse(args, kwargs, __FUNCTION__, &name, &offset,
			&parent, &before))
			return ToPyBool(false);

		mvAppItem* item = new mvIndent(name, offset);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* unindent(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		std::string sname = std::string("unindent" + std::to_string(i));
		const char* name = sname.c_str();
		float offset = 0.0f;
		const char* before = "";
		const char* parent = "";

		if (!(*mvApp::GetApp()->getParsers())["unindent"].parse(args, kwargs, __FUNCTION__, &name, &offset, &parent, &before))
			return ToPyBool(false);

		mvAppItem* item = new mvUnindent(name, offset);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_tab_bar(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int reorderable = false;
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";

		if (!(*mvApp::GetApp()->getParsers())["add_tab_bar"].parse(args, kwargs, __FUNCTION__, &name, &reorderable,
			&callback, &callback_data, &parent, &before, &data_source))
			return ToPyBool(false);

		mvAppItem* item = new mvTabBar(name, reorderable);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);
		item->setDataSource(data_source);
		if (AddItemWithRuntimeChecks(item, parent, before))
		{
			mvApp::GetApp()->pushParent(item);
			return ToPyBool(true);
		}
		return ToPyBool(false);
	}

	PyObject* add_tab(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int closeable = false;
		const char* tip = "";
		const char* before = "";
		const char* parent = "";

		if (!(*mvApp::GetApp()->getParsers())["add_tab"].parse(args, kwargs, __FUNCTION__, &name, &closeable,
			&tip, &parent, &before))
			return ToPyBool(false);

		if (std::string(parent).empty())
		{
			auto parentItem = mvApp::GetApp()->topParent();

			if (parentItem == nullptr)
			{
				ThrowPythonException("add_tab must follow a call to add_tabbar.");
				return ToPyBool(false);
			}

			else if (parentItem->getType() == mvAppItemType::TabBar)
			{
				mvAppItem* item = new mvTab(name, closeable);
				item->setTip(tip);
				if (AddItemWithRuntimeChecks(item, parent, before))
				{
					mvApp::GetApp()->pushParent(item);
					return ToPyBool(true);
				}
			}

			else
				ThrowPythonException("add_tab was called incorrectly. Did you forget to call end_tab?");
		}

		else
		{
			auto parentItem = mvApp::GetApp()->getItem(parent);

			if (parentItem == nullptr)
			{
				ThrowPythonException("add_tab parent must exist.");
				return ToPyBool(false);
			}

			else if (parentItem->getType() == mvAppItemType::TabBar)
			{
				mvAppItem* item = new mvTab(name, closeable);
				item->setTip(tip);
				if (AddItemWithRuntimeChecks(item, parent, before))
				{
					mvApp::GetApp()->pushParent(item);
					return ToPyBool(true);
				}
			}

			else
			{
				ThrowPythonException("add_tab parent must be a tab bar.");
				return ToPyBool(false);
			}
		}

		return ToPyBool(false);
	}

	PyObject* add_menu_bar(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* before = "";
		const char* parent = "";

		if (!(*mvApp::GetApp()->getParsers())["add_menu_bar"].parse(args, kwargs, __FUNCTION__, &name,
			&parent, &before))
			return ToPyBool(false);

		auto parentItem = mvApp::GetApp()->topParent();

		if (parentItem->getType() == mvAppItemType::Window)
		{
			auto window = static_cast<mvWindowAppitem*>(parentItem);
			window->addFlag(ImGuiWindowFlags_MenuBar);
			mvAppItem* item = new mvMenuBar(name);
			if (AddItemWithRuntimeChecks(item, parent, before))
			{
				mvApp::GetApp()->pushParent(item);
				return ToPyBool(true);
			}
		}

		return ToPyBool(false);
	}

	PyObject* add_menu(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* tip = "";
		const char* before = "";
		const char* parent = "";

		if (!(*mvApp::GetApp()->getParsers())["add_menu"].parse(args, kwargs, __FUNCTION__, &name,
			&tip, &parent, &before))
			return ToPyBool(false);

		//auto parentItem = mvApp::GetApp()->topParent();

		mvAppItem* item = new mvMenu(name);
		item->setTip(tip);
		if (AddItemWithRuntimeChecks(item, parent, before))
		{
			mvApp::GetApp()->pushParent(item);
			return ToPyBool(true);
		}
		return ToPyBool(false);
	}

	PyObject* add_menu_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* shortcut = "";
		int check = false;
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		const char* before = "";
		const char* parent = "";

		if (!(*mvApp::GetApp()->getParsers())["add_menu_item"].parse(args, kwargs, __FUNCTION__, &name,
			&shortcut, &check, &callback, &callback_data, &tip, &parent, &before))
			return ToPyBool(false);

		mvAppItem* item = new mvMenuItem(name, shortcut, check);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);
		item->setTip(tip);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_spacing(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		std::string sname = std::string("spacing" + std::to_string(i));
		const char* name = sname.c_str();
		int count = 1;
		const char* before = "";
		const char* parent = "";

		if (!(*mvApp::GetApp()->getParsers())["add_spacing"].parse(args, kwargs, __FUNCTION__, &name, &count,
			&parent, &before))
			return ToPyBool(false);

		mvAppItem* item = new mvSpacing(name, count);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_dummy(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		std::string sname = std::string("dummy" + std::to_string(i));
		const char* name = sname.c_str();
		int width;
		int height;
		const char* before = "";
		const char* parent = "";

		if (!(*mvApp::GetApp()->getParsers())["add_dummy"].parse(args, kwargs, __FUNCTION__, &width, &height, &name,
			&parent, &before))
			return ToPyBool(false);

		mvAppItem* item = new mvDummy(name);
		item->setWidth(width);
		item->setHeight(height);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_same_line(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		std::string sname = std::string("sameline" + std::to_string(i));
		const char* name = sname.c_str();
		float xoffset = 0.0f;
		float spacing = -1.0f;
		const char* before = "";
		const char* parent = "";

		if (!(*mvApp::GetApp()->getParsers())["add_same_line"].parse(args, kwargs, __FUNCTION__, &name,
			&xoffset, &spacing, &parent, &before))
			return ToPyBool(false);

		mvAppItem* item = new mvSameLine(name, xoffset, spacing);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_radio_button(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* items;
		int default_value = 0;
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";
		const char* secondary_data_source = "";
		int horizontal = false;

		if (!(*mvApp::GetApp()->getParsers())["add_radio_button"].parse(args, kwargs, __FUNCTION__, &name, &items,
			&default_value, &callback, &callback_data, &tip, &parent, &before, &data_source, &secondary_data_source,
			&horizontal))
			return ToPyBool(false);

		mvAppItem* item = new mvRadioButton(name, ToStringVect(items), default_value,
			secondary_data_source, horizontal);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);
		item->setTip(tip);
		item->setDataSource(data_source);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_group(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* tip = "";
		int width = 0;
		const char* before = "";
		const char* parent = "";
		int hide = false;
		int horizontal = false;
		float horizontal_spacing = -1.0f;

		if (!(*mvApp::GetApp()->getParsers())["add_group"].parse(args, kwargs, __FUNCTION__, &name,
			&tip, &parent, &before, &width, &hide, &horizontal, &horizontal_spacing))
			return ToPyBool(false);

		mvAppItem* item = new mvGroup(name, horizontal, horizontal_spacing);
		item->setTip(tip);
		item->setWidth(width);
		if (AddItemWithRuntimeChecks(item, parent, before))
		{
			mvApp::GetApp()->pushParent(item);
			if (hide)
				item->hide();

			return ToPyBool(true);
		}
		return ToPyBool(false);
	}

	PyObject* add_child(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* tip = "";
		int width = 0;
		int height = 0;
		const char* before = "";
		const char* parent = "";
		int border = true;

		if (!(*mvApp::GetApp()->getParsers())["add_child"].parse(args, kwargs, __FUNCTION__, &name,
			&tip, &parent, &before, &width, &height, &border))
			return ToPyBool(false);

		mvAppItem* item = new mvChild(name, border);
		item->setTip(tip);
		item->setWidth(width);
		item->setHeight(height);
		if (AddItemWithRuntimeChecks(item, parent, before))
		{
			mvApp::GetApp()->pushParent(item);
			return ToPyBool(true);
		}

		return ToPyBool(false);
	}

	PyObject* add_window(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int width = -1;
		int height = -1;
		int startx = 200;
		int starty = 200;
		int autosize = false;
		int hide = false;
		int resizable = true;
		int title_bar = true;
		int movable = true;
		PyObject* closing_callback = nullptr;

		if (!(*mvApp::GetApp()->getParsers())["add_window"].parse(args, kwargs, __FUNCTION__, &name, &width,
			&height, &startx, &starty, &autosize, &resizable, &title_bar, &movable,
			&hide, &closing_callback))
			return ToPyBool(false);

		if (width == -1 && height == -1)
		{
			width = 500;
			height = 500;
		}

		mvAppItem* item = new mvWindowAppitem(name, width, height, startx, starty,
			false, autosize, resizable, title_bar, movable, closing_callback);

		if (AddItemWithRuntimeChecks(item, "", ""))
		{
			mvApp::GetApp()->pushParent(item);

			if (hide)
				item->hide();

			return ToPyBool(true);
		}

		return ToPyBool(false);
	}

	PyObject* set_window_pos(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* window;
		float x;
		float y;

		if (!(*mvApp::GetApp()->getParsers())["set_window_pos"].parse(args, kwargs, __FUNCTION__, &window, &x, &y))
			return GetPyNone();

		if (std::string(window) == "logger##standard")
		{
			mvAppLog::SetWindowPos(x, y);
			return GetPyNone();
		}

		// check if item is a regular item
		mvWindowAppitem* awindow = mvApp::GetApp()->getWindow(window);

		// check if item is a standard window
		mvStandardWindow* swindow = nullptr;
		if (awindow == nullptr)
			swindow = mvApp::GetApp()->getStandardWindow(window);
		else
		{
			awindow->setWindowPos(x, y);
			return GetPyNone();
		}

		if (swindow == nullptr)
		{
			ThrowPythonException(window + std::string(" window was not found"));
			return GetPyNone();
		}
		else
		{
			swindow->setWindowPos(x, y);
			return GetPyNone();
		}
	}

	PyObject* get_window_pos(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* window;

		if (!(*mvApp::GetApp()->getParsers())["get_window_pos"].parse(args, kwargs, __FUNCTION__, &window))
			return GetPyNone();

		if (std::string(window) == "logger##standard")
			return ToPyPair(mvAppLog::GetWindowPos().x, mvAppLog::GetWindowPos().y);

		// check if item is a regular item
		mvWindowAppitem* awindow = mvApp::GetApp()->getWindow(window);
		
		// check if item is a standard window
		mvStandardWindow* swindow = nullptr;
		if (awindow == nullptr)
			swindow = mvApp::GetApp()->getStandardWindow(window);
		else
			return ToPyPair(awindow->getWindowPos().x, awindow->getWindowPos().y);

		if (swindow == nullptr)
		{
			ThrowPythonException(window + std::string(" window was not found"));
			return GetPyNone();
		}
		else
			return ToPyPair(swindow->getWindowPos().x, swindow->getWindowPos().y);

	}

	PyObject* add_tooltip(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* tipparent;
		const char* name;
		const char* parent = "";
		const char* before = "";

		if (!(*mvApp::GetApp()->getParsers())["add_tooltip"].parse(args, kwargs, __FUNCTION__, &tipparent,
			&name, &parent, &before))
			return ToPyBool(false);

		mvAppItem* item = new mvTooltip(name);

		if (AddItemWithRuntimeChecks(item, parent, before))
		{
			mvApp::GetApp()->pushParent(item);
			return ToPyBool(true);
		}
		return ToPyBool(false);
	}

	PyObject* add_popup(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* popupparent;
		const char* name;
		int mousebutton = 1;
		int modal = false;
		int width = 0;
		int height = 0;
		const char* before = "";
		const char* parent = "";

		if (!(*mvApp::GetApp()->getParsers())["add_popup"].parse(args, kwargs, __FUNCTION__, &popupparent,
			&name, &mousebutton, &modal, &parent, &before, &width, &height))
			return ToPyBool(false);

		auto PopupParent = mvApp::GetApp()->getItem(popupparent);
		if (PopupParent)
			PopupParent->setPopup(name);

		if (std::string(popupparent) == "")
			mvApp::GetApp()->getItem("MainWindow")->setPopup(name);

		mvAppItem* item = new mvPopup(name, mousebutton, modal);
		item->setWidth(width);
		item->setHeight(height);

		if (AddItemWithRuntimeChecks(item, parent, before))
		{
			mvApp::GetApp()->pushParent(item);
			return ToPyBool(true);
		}
		return ToPyBool(false);
	}

	PyObject* end(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->popParent();
		return GetPyNone();
	}

	PyObject* add_collapsing_header(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int default_open = false;
		int closable = false;
		int flags = 0;
		const char* tip = "";
		const char* before = "";
		const char* parent = "";

		if (!(*mvApp::GetApp()->getParsers())["add_collapsing_header"].parse(args, kwargs, __FUNCTION__, &name,
			&default_open, &closable, &tip, &parent, &before))
			return ToPyBool(false);

		if (default_open) flags |= ImGuiTreeNodeFlags_DefaultOpen;

		mvAppItem* item = new mvCollapsingHeader(name, flags, closable);
		item->setTip(tip);

		if (AddItemWithRuntimeChecks(item, parent, before))
		{
			mvApp::GetApp()->pushParent(item);
			return ToPyBool(true);
		}
		return ToPyBool(false);
	}

	PyObject* add_tree_node(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int default_open = false;
		int flags = 0;
		const char* tip = "";
		const char* before = "";
		const char* parent = "";

		if (!(*mvApp::GetApp()->getParsers())["add_tree_node"].parse(args, kwargs, __FUNCTION__, &name,
			&default_open, &tip, &parent, &before))
			return ToPyBool(false);

		if (default_open) flags |= ImGuiTreeNodeFlags_DefaultOpen;

		mvAppItem* item = new mvTreeNode(name, flags);
		item->setTip(tip);
		if (AddItemWithRuntimeChecks(item, parent, before))
		{
			mvApp::GetApp()->pushParent(item);
			return ToPyBool(true);
		}
		return ToPyBool(false);
	}

	PyObject* add_separator(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		std::string sname = std::string("separator" + std::to_string(i));
		const char* name = sname.c_str();
		const char* tip = "";
		const char* parent = "";
		const char* before = "";

		if (!(*mvApp::GetApp()->getParsers())["add_separator"].parse(args, kwargs, __FUNCTION__, &name, &tip, &parent, &before))
			return ToPyBool(false);

		mvAppItem* item = new mvSeparator(name);
		item->setTip(tip);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_color_edit3(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 3, PyLong_FromLong(255));
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		int width = 0;
		int height = 0;
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";

		if (!(*mvApp::GetApp()->getParsers())["add_color_edit3"].parse(args, kwargs, __FUNCTION__, &name,
			&default_value, &callback, &callback_data, &tip, &parent, &before, &data_source, &width, &height))
			return ToPyBool(false);

		auto color = ToColor(default_value);

		//mvAppItem* item = new mvColorEdit3("", name, color);
		mvAppItem* item = new mvColorItem<mvAppItemType::ColorEdit3, ImGui::ColorEdit3>(name, color);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		item->setHeight(height);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_color_edit4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 3, PyLong_FromLong(0));
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		int width = 0;
		int height = 0;
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";

		if (!(*mvApp::GetApp()->getParsers())["add_color_edit4"].parse(args, kwargs, __FUNCTION__, &name, &default_value, 
			&callback, &callback_data, &tip, &parent, &before, &data_source, &width, &height))
			return ToPyBool(false);

		auto color = ToColor(default_value);
		mvAppItem* item = new mvColorItem<mvAppItemType::ColorEdit4, ImGui::ColorEdit4>(name, color);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		item->setHeight(height);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_color_picker3(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 3, PyLong_FromLong(255));
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		int width = 0;
		int height = 0;
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";

		if (!(*mvApp::GetApp()->getParsers())["add_color_picker3"].parse(args, kwargs, __FUNCTION__, &name, &default_value, 
			&callback, &callback_data, &tip, &parent, &before, &data_source, &width, &height))
			return ToPyBool(false);

		auto color = ToColor(default_value);
		mvAppItem* item = new mvColorItem<mvAppItemType::ColorPicker3, ImGui::ColorPicker3>(name, color);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		item->setHeight(height);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_color_picker4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 3, PyLong_FromLong(1));
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		int width = 0;
		int height = 0;
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";

		if (!(*mvApp::GetApp()->getParsers())["add_color_picker4"].parse(args, kwargs, __FUNCTION__, &name,
			&default_value, &callback, &callback_data, &tip, &parent, &before, &data_source, &width, &height))
			return ToPyBool(false);

		auto color = ToColor(default_value);

		mvAppItem* item = new mvColorPicker4(name, color);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		item->setHeight(height);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_checkbox(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int default_value = 0;
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";

		if (!(*mvApp::GetApp()->getParsers())["add_checkbox"].parse(args, kwargs, __FUNCTION__, &name,
			&default_value, &callback, &callback_data, &tip, &parent, &before, &data_source))
			return ToPyBool(false);

		mvAppItem* item = new mvCheckbox(name, default_value);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);
		item->setTip(tip);
		item->setDataSource(data_source);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* set_table_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		int row;
		int column;
		const char* value;

		if (!(*mvApp::GetApp()->getParsers())["set_table_item"].parse(args, kwargs, __FUNCTION__, &table, &row,
			&column, &value))
			return GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			ThrowPythonException(message + " table does not exist.");
			return GetPyNone();
		}

		if (item->getType() != mvAppItemType::Table)
		{
			std::string message = table;
			ThrowPythonException(message + " is not a table.");
			return GetPyNone();
		}

		mvTable* atable = static_cast<mvTable*>(item);
		atable->setTableItem(row, column, value);

		return GetPyNone();
	}

	PyObject* delete_row(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		int row;

		if (!(*mvApp::GetApp()->getParsers())["delete_row"].parse(args, kwargs, __FUNCTION__, &table, &row))
			return GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			ThrowPythonException(message + " table does not exist.");
			return GetPyNone();
		}

		if (item->getType() != mvAppItemType::Table)
		{
			std::string message = table;
			ThrowPythonException(message + " is not a table.");
			return GetPyNone();
		}

		mvTable* atable = static_cast<mvTable*>(item);
		atable->deleteRow(row);

		return GetPyNone();
	}

	PyObject* delete_column(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		int column;

		if (!(*mvApp::GetApp()->getParsers())["delete_column"].parse(args, kwargs, __FUNCTION__, &table, &column))
			return GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			ThrowPythonException(message + " table does not exist.");
			return GetPyNone();
		}

		if (item->getType() != mvAppItemType::Table)
		{
			std::string message = table;
			ThrowPythonException(message + " is not a table.");
			return GetPyNone();
		}

		mvTable* atable = static_cast<mvTable*>(item);
		atable->deleteColumn(column);

		return GetPyNone();
	}

	PyObject* set_headers(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		PyObject* headers;

		if (!(*mvApp::GetApp()->getParsers())["set_headers"].parse(args, kwargs, __FUNCTION__, &table, &headers))
			return GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			ThrowPythonException(message + " table does not exist.");
			return GetPyNone();
		}

		auto prow = ToStringVect(headers);
		mvTable* atable = static_cast<mvTable*>(item);

		atable->addHeaders(prow);

		return GetPyNone();
	}

	PyObject* add_row(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		PyObject* row;

		if (!(*mvApp::GetApp()->getParsers())["add_row"].parse(args, kwargs, __FUNCTION__, &table, &row))
			return GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			ThrowPythonException(message + " table does not exist.");
			return GetPyNone();
		}

		if (item->getType() != mvAppItemType::Table)
		{
			std::string message = table;
			ThrowPythonException(message + " is not a table.");
			return GetPyNone();
		}

		auto prow = ToStringVect(row);
		mvTable* atable = static_cast<mvTable*>(item);
		if (atable->getColumnCount() == 0)
		{
			std::vector<std::string> headers;
			for (size_t i = 0; i < prow.size(); i++)
				headers.emplace_back("Header" + std::to_string(i));
			atable->addHeaders(headers);
		}
		atable->addRow(prow);

		return GetPyNone();
	}

	PyObject* add_column(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		const char* name;
		PyObject* column;

		if (!(*mvApp::GetApp()->getParsers())["add_column"].parse(args, kwargs, __FUNCTION__, &table, &name, &column))
			return GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			ThrowPythonException(message + " table does not exist.");
			return GetPyNone();
		}

		if (item->getType() != mvAppItemType::Table)
		{
			std::string message = table;
			ThrowPythonException(message + " is not a table.");
			return GetPyNone();
		}

		auto pcolumn = ToStringVect(column);

		mvTable* atable = static_cast<mvTable*>(item);
		atable->addColumn(name, pcolumn);

		return GetPyNone();
	}

	PyObject* insert_row(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		int row_index;
		PyObject* row;

		if (!(*mvApp::GetApp()->getParsers())["insert_row"].parse(args, kwargs, __FUNCTION__, &table, &row_index, &row))
			return GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			ThrowPythonException(message + " table does not exist.");
			return GetPyNone();
		}

		if (item->getType() != mvAppItemType::Table)
		{
			std::string message = table;
			ThrowPythonException(message + " is not a table.");
			return GetPyNone();
		}

		auto prow = ToStringVect(row);
		mvTable* atable = static_cast<mvTable*>(item);
		if (atable->getColumnCount() == 0)
		{
			std::vector<std::string> headers;
			for (size_t i = 0; i < prow.size(); i++)
				headers.emplace_back("Header" + std::to_string(i));
			atable->addHeaders(headers);
		}
		atable->insertRow(row_index, prow);

		return GetPyNone();
	}

	PyObject* insert_column(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		int column_index;
		const char* name;
		PyObject* column;

		if (!(*mvApp::GetApp()->getParsers())["insert_column"].parse(args, kwargs, __FUNCTION__, &table, &column_index, &name, &column))
			return GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			ThrowPythonException(message + " table does not exist.");
			return GetPyNone();
		}

		if (item->getType() != mvAppItemType::Table)
		{
			std::string message = table;
			ThrowPythonException(message + " is not a table.");
			return GetPyNone();
		}

		auto prow = ToStringVect(column);

		mvTable* atable = static_cast<mvTable*>(item);
		atable->insertColumn(column_index, name, prow);

		return GetPyNone();
	}

	PyObject* set_table_selection(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		int row;
		int column;
		int value;

		if (!(*mvApp::GetApp()->getParsers())["set_table_selection"].parse(args, kwargs, __FUNCTION__, &table, &row,
			&column, &value))
			return GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			ThrowPythonException(message + " table does not exist.");
			return GetPyNone();
		}

		if (item->getType() != mvAppItemType::Table)
		{
			std::string message = table;
			ThrowPythonException(message + " is not a table.");
			return GetPyNone();
		}

		mvTable* atable = static_cast<mvTable*>(item);
		atable->setSelection(row, column, value);

		return GetPyNone();
	}

	PyObject* clear_table(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;

		if (!(*mvApp::GetApp()->getParsers())["clear_table"].parse(args, kwargs, __FUNCTION__, &table))
			return GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			ThrowPythonException(message + " table does not exist.");
			return GetPyNone();
		}

		if (item->getType() != mvAppItemType::Table)
		{
			std::string message = table;
			ThrowPythonException(message + " is not a table.");
			return GetPyNone();
		}

		mvTable* atable = static_cast<mvTable*>(item);
		atable->clearTable();
		Py_RETURN_NONE;
	}

	PyObject* get_table_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		int row;
		int column;

		if (!(*mvApp::GetApp()->getParsers())["get_table_item"].parse(args, kwargs, __FUNCTION__, &table, &row,
			&column))
			return GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			ThrowPythonException(message + " table does not exist.");
			return GetPyNone();
		}

		if (item->getType() != mvAppItemType::Table)
		{
			std::string message = table;
			ThrowPythonException(message + " is not a table.");
			return GetPyNone();
		}

		mvTable* atable = static_cast<mvTable*>(item);
		return Py_BuildValue("s", atable->getTableItem(row, column).c_str());

	}

	PyObject* get_table_selections(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;

		if (!(*mvApp::GetApp()->getParsers())["get_table_selections"].parse(args, kwargs, __FUNCTION__, &table))
			return GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			ThrowPythonException(message + " table does not exist.");
			return GetPyNone();
		}

		if (item->getType() != mvAppItemType::Table)
		{
			std::string message = table;
			ThrowPythonException(message + " is not a table.");
			return GetPyNone();
		}

		mvTable* atable = static_cast<mvTable*>(item);
		return atable->getSelections();
	}

	PyObject* run_async_function(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;
		PyObject* return_handler = nullptr;
		PyObject* data;

		if (!(*mvApp::GetApp()->getParsers())["run_async_function"].parse(args, kwargs, __FUNCTION__, &callback, &data, &return_handler))
			return GetPyNone();

		mvApp::GetApp()->addMTCallback(callback, data, return_handler);

		return GetPyNone();

	}

	PyObject* delete_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (!mvApp::GetApp()->checkIfMainThread())
			return GetPyNone();

		const char* item;
		int childrenOnly = false;

		if (!(*mvApp::GetApp()->getParsers())["delete_item"].parse(args, kwargs, __FUNCTION__, &item, &childrenOnly))
			return GetPyNone();

		if (childrenOnly)
			mvApp::GetApp()->deleteItemChildren(item);
		else
			mvApp::GetApp()->deleteItem(item);

		return GetPyNone();

	}

	PyObject* does_item_exist(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (!mvApp::GetApp()->checkIfMainThread())
			return GetPyNone();

		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["does_item_exist"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		if (!mvApp::GetApp()->getItem(item))
			return ToPyBool(false);

		return ToPyBool(true);

	}

	PyObject* select_directory_dialog(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback = nullptr;

		if (!(*mvApp::GetApp()->getParsers())["select_directory_dialog"].parse(args, kwargs, __FUNCTION__, &callback))
			return GetPyNone();

		igfd::ImGuiFileDialog::Instance()->OpenModal("ChooseFileDlgKey", "Choose Directory", 0, ".");
		mvStandardWindow* window = mvApp::GetApp()->getStandardWindow("filedialog");
		auto dialog = static_cast<mvFileDialog*>(window);
		dialog->setCallback(callback);
		mvApp::GetApp()->showStandardWindow("filedialog");

		return GetPyNone();
	}

	PyObject* open_file_dialog(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback = nullptr;
		const char* extensions = "";

		if (!(*mvApp::GetApp()->getParsers())["open_file_dialog"].parse(args, kwargs, __FUNCTION__,
			&callback, &extensions))
			return GetPyNone();

		igfd::ImGuiFileDialog::Instance()->OpenModal("ChooseFileDlgKey", "Choose File", extensions, ".");
		mvStandardWindow* window = mvApp::GetApp()->getStandardWindow("filedialog");
		auto dialog = static_cast<mvFileDialog*>(window);
		dialog->setCallback(callback);
		mvApp::GetApp()->showStandardWindow("filedialog");

		return GetPyNone();
	}

	PyObject* move_item_up(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (!mvApp::GetApp()->checkIfMainThread())
			return GetPyNone();

		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["move_item_up"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		mvApp::GetApp()->moveItemUp(item);

		return GetPyNone();

	}

	PyObject* move_item_down(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (!mvApp::GetApp()->checkIfMainThread())
			return GetPyNone();

		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["move_item_down"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		mvApp::GetApp()->moveItemDown(item);

		return GetPyNone();

	}

	PyObject* get_theme_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int item;

		if (!(*mvApp::GetApp()->getParsers())["get_theme_item"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto color = mvApp::GetApp()->getThemeItem(item);

		return ToPyList(std::vector<int>{ color.r, color.g, color.b, color.a });
	}

	PyObject* get_total_time(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return ToPyFloat(mvApp::GetApp()->getTotalTime());
	}

	PyObject* get_delta_time(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return ToPyFloat(mvApp::GetApp()->getDeltaTime());
	}

	PyObject* get_item_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["get_item_callback"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
		{
			PyObject* callback = appitem->getCallback();
			if (callback)
			{
				Py_XINCREF(callback);
				return callback;
			}
		
		}

		return GetPyNone();
	}

	PyObject* get_item_label(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["get_item_label"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return ToPyString(appitem->getLabel());

		return GetPyNone();
	}

	PyObject* get_item_height(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["get_item_height"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		if (std::string(item) == "logger##standard")
			return ToPyInt(mvAppLog::GetWindowHeight());

		// check if item is a regular item
		mvAppItem* appitem = mvApp::GetApp()->getItem(item);
		if (appitem)
			return ToPyInt(appitem->getHeight());

		// check if item is a standard window
		if (appitem == nullptr)
		{
			mvStandardWindow* swindow = mvApp::GetApp()->getStandardWindow(item);
			if (swindow)
				return ToPyInt(swindow->getWindowHeight());
			else
				ThrowPythonException(item + std::string(" item was not found"));
		}
		return GetPyNone();
	}

	PyObject* get_item_width(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["get_item_width"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		if (std::string(item) == "logger##standard")
			return ToPyInt(mvAppLog::GetWindowWidth());

		// check if item is a regular item
		mvAppItem* appitem = mvApp::GetApp()->getItem(item);
		if (appitem)
			return ToPyInt(appitem->getWidth());

		// check if item is a standard window
		if (appitem == nullptr)
		{
			mvStandardWindow* swindow = mvApp::GetApp()->getStandardWindow(item);
			if (swindow)
				return ToPyInt(swindow->getWindowWidth());
			else
				ThrowPythonException(item + std::string(" item was not found"));
		}
		return GetPyNone();
	}

	PyObject* get_item_parent(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["get_item_parent"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return ToPyString(appitem->getParent()->getName());

		return GetPyNone();
	}
	
	PyObject* get_item_popup(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["get_item_popup"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return ToPyString(appitem->getPopup());

		return GetPyNone();
	}

	PyObject* get_item_children(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["get_item_children"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
		{
			auto children = appitem->getChildren();
			std::vector<std::string> childList;
			for (auto child : children)
				childList.emplace_back(child->getName());

			return ToPyList(childList);
		}

		return GetPyNone();
	}

	PyObject* get_all_items(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		std::vector<mvAppItem*>& windows = mvApp::GetApp()->getWindows();

		std::vector<std::string> childList;

		// to help recursively retrieve children
		std::function<void(mvAppItem*)> ChildRetriever;
		ChildRetriever = [&childList, &ChildRetriever](mvAppItem* item) {
			auto children = item->getChildren();
			for (mvAppItem* child : children)
			{
				childList.emplace_back(child->getName());
				if (child->isContainer())
					ChildRetriever(child);
			}
				
		};

		for (auto window : windows)
			ChildRetriever(window);

		return ToPyList(childList);
	}

	PyObject* get_windows(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		auto windows = mvApp::GetApp()->getWindows();
		std::vector<std::string> childList;
		for (auto window : windows)
			childList.emplace_back(window->getName());
		return ToPyList(childList);
	}

	PyObject* get_item_tip(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["get_item_tip"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return ToPyString(appitem->getTip());

		return GetPyNone();
	}

	PyObject* get_main_window_size(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return ToPyPairII(mvApp::GetApp()->getActualWidth(), mvApp::GetApp()->getActualHeight());
	}

	PyObject* get_theme(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return ToPyString(mvApp::GetApp()->getAppTheme());
	}

	PyObject* get_thread_count(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return ToPyInt(mvApp::GetApp()->getThreadCount());
	}

	PyObject* is_threadpool_high_performance(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return ToPyBool(mvApp::GetApp()->usingThreadPoolHighPerformance());
	}

	PyObject* get_threadpool_timeout(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return ToPyFloat(mvApp::GetApp()->getThreadPoolTimeout());
	}

	PyObject* get_active_window(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return ToPyString(mvApp::GetApp()->getActiveWindow());
	}

	PyObject* get_dearpygui_version(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return ToPyString(mvApp::GetApp()->GetVersion());
	}

	PyObject* set_threadpool_timeout(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		float time;

		if (!(*mvApp::GetApp()->getParsers())["set_threadpool_timeout"].parse(args, kwargs, __FUNCTION__, &time))
			return GetPyNone();

		mvApp::GetApp()->setThreadPoolTimeout(time);

		return GetPyNone();
	}

	PyObject* set_thread_count(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int threads;

		if (!(*mvApp::GetApp()->getParsers())["set_thread_count"].parse(args, kwargs, __FUNCTION__, &threads))
			return GetPyNone();

		mvApp::GetApp()->setThreadCount(threads);

		return GetPyNone();
	}

	PyObject* add_data(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* name;
		PyObject* data;

		if (!(*mvApp::GetApp()->getParsers())["add_data"].parse(args, kwargs, __FUNCTION__, &name, &data))
			return GetPyNone();

		mvDataStorage::AddData(name, data);
		Py_XINCREF(data);

		return GetPyNone();
	}

	PyObject* get_data(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* name;

		if (!(*mvApp::GetApp()->getParsers())["get_data"].parse(args, kwargs, __FUNCTION__, &name))
			return GetPyNone();

		auto result = mvDataStorage::GetDataIncRef(name);

		if (result)
			return result;

		return GetPyNone();
	}

	PyObject* delete_data(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* name;

		if (!(*mvApp::GetApp()->getParsers())["delete_data"].parse(args, kwargs, __FUNCTION__, &name))
			return GetPyNone();

		mvDataStorage::DeleteData(name);

		return GetPyNone();
	}

	PyObject* set_threadpool_high_performance(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->setThreadPoolHighPerformance();
		return GetPyNone();
	}

	PyObject* set_item_color(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;
		int style;
		PyObject* color;

		if (!(*mvApp::GetApp()->getParsers())["set_item_color"].parse(args, kwargs, __FUNCTION__, &item, &style, &color))
			return GetPyNone();

		auto mcolor = ToColor(color);

		mvAppItem* appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			appitem->addColorStyle(style, mcolor);

		return GetPyNone();
	}

	PyObject* clear_item_color(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["clear_item_color"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		mvAppItem* appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			appitem->clearColors();

		return GetPyNone();
	}

	PyObject* is_item_hovered(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["is_item_hovered"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return ToPyBool(appitem->isItemHovered());

		return GetPyNone();
	}

	PyObject* is_item_shown(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["is_item_shown"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return ToPyBool(appitem->isShown());

		return GetPyNone();
	}

	PyObject* is_item_active(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["is_item_active"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return ToPyBool(appitem->isItemActive());

		return GetPyNone();
	}

	PyObject* is_item_focused(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["is_item_focused"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return ToPyBool(appitem->isItemFocused());

		return GetPyNone();
	}

	PyObject* is_item_clicked(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["is_item_clicked"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return ToPyBool(appitem->isItemClicked());

		return GetPyNone();
	}

	PyObject* is_item_container(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["is_item_container"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return ToPyBool(appitem->isContainer());

		return GetPyNone();
	}

	PyObject* is_item_visible(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["is_item_visible"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return ToPyBool(appitem->isItemVisible());

		return GetPyNone();
	}

	PyObject* is_item_edited(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["is_item_edited"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return ToPyBool(appitem->isItemEdited());

		return GetPyNone();
	}

	PyObject* is_item_activated(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["is_item_activated"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return ToPyBool(appitem->isItemActivated());

		return GetPyNone();
	}

	PyObject* is_item_deactivated(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["is_item_deactivated"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return ToPyBool(appitem->isItemDeactivated());

		return GetPyNone();
	}

	PyObject* is_item_deactivated_after_edit(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["is_item_deactivated_after_edit"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return ToPyBool(appitem->isItemDeactivatedAfterEdit());

		return GetPyNone();
	}

	PyObject* is_item_toggled_open(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["is_item_toggled_open"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return ToPyBool(appitem->isItemToogledOpen());

		return GetPyNone();
	}

	PyObject* get_item_rect_min(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["get_item_rect_min"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
		{
			mvVec2 value = appitem->getItemRectMin();
			return ToPyPair(value.x, value.y);
		}

		return GetPyNone();
	}

	PyObject* get_item_rect_max(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["get_item_rect_max"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
		{
			mvVec2 value = appitem->getItemRectMax();
			return ToPyPair(value.x, value.y);
		}

		return GetPyNone();
	}

	PyObject* get_item_rect_size(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["get_item_rect_size"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
		{
			mvVec2 value = appitem->getItemRectSize();
			return ToPyPair(value.x, value.y);
		}

		return GetPyNone();
	}

	PyObject* set_theme_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int item;
		int r, g, b, a;

		if (!(*mvApp::GetApp()->getParsers())["set_theme_item"].parse(args, kwargs, __FUNCTION__, &item, &r, &g, &b, &a))
			return GetPyNone();

		mvApp::GetApp()->setThemeItem(item, { r, g, b, a });

		return GetPyNone();
	}

	PyObject* get_value(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		if (!(*mvApp::GetApp()->getParsers())["get_value"].parse(args, kwargs, __FUNCTION__, &name))
			return GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(std::string(name));

		if (item == nullptr)
			return GetPyNone();

		// if compile time, check for data source usage
		if (!mvApp::IsAppStarted())
		{
			std::string datasource = item->getDataSource();
			if (!datasource.empty())
				return mvDataStorage::GetData(datasource);
		}

		return item->getPyValue();
	}

	PyObject* set_value(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* value;

		if (!(*mvApp::GetApp()->getParsers())["set_value"].parse(args, kwargs, __FUNCTION__, &name, &value))
			return GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(std::string(name));

		if (item == nullptr)
			return GetPyNone();

		if (item->getDataSource().empty())
			item->setPyValue(value);
		else
			mvDataStorage::AddData(item->getDataSource(), value);

		return GetPyNone();
	}

	PyObject* show_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		if (!(*mvApp::GetApp()->getParsers())["show_item"].parse(args, kwargs, __FUNCTION__, &name))
			return GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(std::string(name));

		if (item != nullptr)
			item->show();

		return GetPyNone();
	}

	PyObject* hide_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int children_only = false;

		if (!(*mvApp::GetApp()->getParsers())["hide_item"].parse(args, kwargs, __FUNCTION__, &name, &children_only))
			return GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(std::string(name));

		if (item != nullptr)
			item->hide();

		if (children_only)
		{
			item->hideAll();
			item->show();
		}

		return GetPyNone();
	}

	PyObject* set_render_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback = nullptr;
		const char* handler = "MainWindow";

		if (!(*mvApp::GetApp()->getParsers())["set_render_callback"].parse(args, kwargs, __FUNCTION__, &callback, &handler))
			return GetPyNone();

		if (std::string(handler) == "MainWindow")
			mvApp::GetApp()->setRenderCallback(callback);
		else
		{

			mvAppItem* item;
			item = mvApp::GetApp()->getItem(handler);
			if (item)
			{
				if (item->getType() == mvAppItemType::Window)
				{
					auto windowtype = static_cast<mvWindowAppitem*>(item);
					mvEventHandler* eventhandler = static_cast<mvEventHandler*>(windowtype);
					eventhandler->setRenderCallback(callback);
				}
				else
					ThrowPythonException("Render callback can only be set for window/child items");
			}
		}

		return GetPyNone();
	}

	PyObject* set_resize_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback = nullptr;
		const char* handler = "MainWindow";

		if (!(*mvApp::GetApp()->getParsers())["set_resize_callback"].parse(args, kwargs, __FUNCTION__, &callback, &handler))
			return GetPyNone();

		if (std::string(handler) == "MainWindow")
			mvApp::GetApp()->setResizeCallback(callback);
		else
		{
			mvAppItem* item;
			item = mvApp::GetApp()->getItem(handler);

			if (item)
			{
				if (item->getType() == mvAppItemType::Window)
				{
					auto windowtype = static_cast<mvWindowAppitem*>(item);
					mvEventHandler* eventhandler = static_cast<mvEventHandler*>(windowtype);
					eventhandler->setResizeCallback(callback);
				}
				else
					ThrowPythonException("Resize callback can only be set for window/child items");
			}
		}

		return GetPyNone();
	}

	PyObject* set_item_label(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;
		const char* label;

		if (!(*mvApp::GetApp()->getParsers())["set_item_label"].parse(args, kwargs, __FUNCTION__, &item, &label))
			return GetPyNone();

		mvAppItem* appitem;
		appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
		{
			std::string newLabel = label;

			// temporary fix
			if(appitem->getType() != mvAppItemType::LabelText)
				newLabel = label + std::string("##") + std::to_string(rand());

			appitem->setLabel(newLabel);
		}

		return GetPyNone();
	}

	PyObject* set_item_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;
		PyObject* callback_data = nullptr;
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["set_item_callback"].parse(args, kwargs, __FUNCTION__, &item, &callback, &callback_data))
			return GetPyNone();

		mvAppItem* appitem;
		appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
		{
			if (appitem->getCallback() != callback)
				Py_XINCREF(callback);
			appitem->setCallback(callback);

			if (callback_data)
			{
				if (appitem->getCallbackData() != callback_data)
					Py_XINCREF(callback_data);
				appitem->setCallbackData(callback_data);
			}
		}

		return GetPyNone();
	}

	PyObject* set_item_popup(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* popup;
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["set_item_popup"].parse(args, kwargs, __FUNCTION__, &item, &popup))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			appitem->setPopup(popup);

		return GetPyNone();
	}

	PyObject* set_item_tip(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* tip, * item;

		if (!(*mvApp::GetApp()->getParsers())["set_item_tip"].parse(args, kwargs, __FUNCTION__, &item, &tip))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			appitem->setTip(tip);

		return GetPyNone();
	}

	PyObject* set_item_width(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;
		int width;

		if (!(*mvApp::GetApp()->getParsers())["set_item_width"].parse(args, kwargs, __FUNCTION__, &item, &width))
			return GetPyNone();

		if (std::string(item) == "logger##standard")
		{
			mvAppLog::SetWidth(width);
			return GetPyNone();
		}

		// check if item is a regular item
		mvAppItem* appitem = mvApp::GetApp()->getItem(item);
		if (appitem)
			appitem->setWidth(width);

		// check if item is a standard window
		if (appitem == nullptr)
		{
			mvStandardWindow* swindow = mvApp::GetApp()->getStandardWindow(item);
			if (swindow)
				swindow->setWidth(width);
			else
				ThrowPythonException(item + std::string(" item was not found"));
		}
		return GetPyNone();
	}

	PyObject* set_item_height(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;
		int height;

		if (!(*mvApp::GetApp()->getParsers())["set_item_height"].parse(args, kwargs, __FUNCTION__, &item, &height))
			return GetPyNone();

		if (std::string(item) == "logger##standard")
		{
			mvAppLog::SetHeight(height);
			return GetPyNone();
		}

		mvAppItem* appitem = mvApp::GetApp()->getItem(item);
		if (appitem)
			appitem->setHeight(height);

		// check if item is a standard window
		if (appitem == nullptr)
		{
			mvStandardWindow* swindow = mvApp::GetApp()->getStandardWindow(item);
			if (swindow)
				swindow->setHeight(height);
			else
				ThrowPythonException(item + std::string(" item was not found"));
		}
		return GetPyNone();
	}

	PyObject* set_main_window_size(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int width;
		int height;

		if (!(*mvApp::GetApp()->getParsers())["set_main_window_size"].parse(args, kwargs, __FUNCTION__, &width, &height))
			return GetPyNone();

		mvApp::GetApp()->setActualSize(width, height);

		return GetPyNone();
	}

	PyObject* close_popup(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (!mvApp::IsAppStarted())
			return GetPyNone();

		ImGui::CloseCurrentPopup();

		return GetPyNone();
	}

	PyObject* set_theme(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* theme;

		if (!(*mvApp::GetApp()->getParsers())["set_theme"].parse(args, kwargs, __FUNCTION__, &theme))
			return GetPyNone();

		mvApp::GetApp()->setAppTheme(std::string(theme));

		return GetPyNone();
	}

	PyObject* show_style_editor(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->showStandardWindow("style##standard");
		return GetPyNone();
	}

	PyObject* show_metrics(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->showStandardWindow("metrics##standard");
		return GetPyNone();
	}

	PyObject* show_about(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->showStandardWindow("about##standard");
		return GetPyNone();
	}

	PyObject* show_source(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* file;

		if (!(*mvApp::GetApp()->getParsers())["show_source"].parse(args, kwargs, __FUNCTION__, &file))
			return GetPyNone();

		mvApp::GetApp()->showStandardWindow("source##standard");
		auto window = static_cast<mvSourceWindow*>(mvApp::GetApp()->getStandardWindow("source##standard"));
		window->setFile(file);
		return GetPyNone();
	}

	PyObject* show_debug(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->showStandardWindow("debug##standard");
		return GetPyNone();
	}

	PyObject* show_documentation(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->showStandardWindow("documentation##standard");
		return GetPyNone();
	}

	std::vector<std::pair<std::string, long>> GetModuleConstants()
	{
		std::vector<std::pair<std::string, long>> ModuleConstants =
		{

			//-----------------------------------------------------------------------------
			// Plot Colors
			//-----------------------------------------------------------------------------
			{"mvPlotCol_Line"         , 0}, // plot line/outline color (defaults to a rotating color set)
			{"mvPlotCol_Fill"         , 1}, // plot fill color for bars (defaults to the current line color)
			{"mvPlotCol_MarkerOutline", 2}, // marker outline color (defaults to the current line color)
			{"mvPlotCol_MarkerFill"   , 3}, // marker fill color (defaults to the current line color)
			{"mvPlotCol_ErrorBar"     , 4}, // error bar color (defaults to ImGuiCol_Text)
			{"mvPlotCol_FrameBg"      , 5}, // plot frame background color (defaults to ImGuiCol_FrameBg)
			{"mvPlotCol_PlotBg"       , 6}, // plot area background color (defaults to ImGuiCol_WindowBg)
			{"mvPlotCol_PlotBorder"   , 7}, // plot area border color (defaults to ImGuiCol_Text)
			{"mvPlotCol_XAxis"        , 8}, // x-axis grid/label color (defaults to 25% ImGuiCol_Text)
			{"mvPlotCol_YAxis"        , 9}, // y-axis grid/label color (defaults to 25% ImGuiCol_Text)
			{"mvPlotCol_YAxis2"       ,10}, // 2nd y-axis grid/label color (defaults to 25% ImGuiCol_Text)
			{"mvPlotCol_YAxis3"       ,11}, // 3rd y-axis grid/label color (defaults to 25% ImGuiCol_Text)
			{"mvPlotCol_Selection"    ,12}, // box-selection color (defaults to yellow)
			{"mvPlotCol_Query"        ,13}, // box-query color (defaults to green)
			{"mvPlotCol_COUNT"        ,14},

			//-----------------------------------------------------------------------------
			// Plot Marker Specifications
			//-----------------------------------------------------------------------------
			{"mvPlotMarker_None"    , 1 << 0},  // no marker
			{"mvPlotMarker_Circle"  , 1 << 1},  // a circle marker will be rendered at each point
			{"mvPlotMarker_Square"  , 1 << 2},  // a square maker will be rendered at each point
			{"mvPlotMarker_Diamond" , 1 << 3},  // a diamond marker will be rendered at each point
			{"mvPlotMarker_Up"      , 1 << 4},  // an upward-pointing triangle marker will up rendered at each point
			{"mvPlotMarker_Down"    , 1 << 5},  // an downward-pointing triangle marker will up rendered at each point
			{"mvPlotMarker_Left"    , 1 << 6},  // an leftward-pointing triangle marker will up rendered at each point
			{"mvPlotMarker_Right"   , 1 << 7},  // an rightward-pointing triangle marker will up rendered at each point
			{"mvPlotMarker_Cross"   , 1 << 8},  // a cross marker will be rendered at each point (not filled)
			{"mvPlotMarker_Plus"    , 1 << 9},  // a plus marker will be rendered at each point (not filled)
			{"mvPlotMarker_Asterisk", 1 << 10}, // a asterisk marker will be rendered at each point (not filled)

			//-----------------------------------------------------------------------------
			// Built-in ImPlot Color maps
			//-----------------------------------------------------------------------------
			{"mvPlotColormap_Default",  0}, // ImPlot default colormap         (n=10)
			{"mvPlotColormap_Dark"   ,  1}, // a.k.a. matplotlib "Set1"        (n=9)
			{"mvPlotColormap_Pastel" ,  2}, // a.k.a. matplotlib "Pastel1"     (n=9)
			{"mvPlotColormap_Paired" ,  3}, // a.k.a. matplotlib "Paired"      (n=12)
			{"mvPlotColormap_Viridis",  4}, // a.k.a. matplotlib "viridis"     (n=11)
			{"mvPlotColormap_Plasma" ,  5}, // a.k.a. matplotlib "plasma"      (n=11)
			{"mvPlotColormap_Hot"    ,  6}, // a.k.a. matplotlib/MATLAB "hot"  (n=11)
			{"mvPlotColormap_Cool"   ,  7}, // a.k.a. matplotlib/MATLAB "cool" (n=11)
			{"mvPlotColormap_Pink"   ,  8}, // a.k.a. matplotlib/MATLAB "pink" (n=11)
			{"mvPlotColormap_Jet"    ,  9}, // a.k.a. MATLAB "jet"             (n=11)

			//-----------------------------------------------------------------------------
			// Theme Color IDs
			//-----------------------------------------------------------------------------
			{"mvGuiCol_Text"                 , 0},
			{"mvGuiCol_TextDisabled"         , 1},
			{"mvGuiCol_WindowBg"             , 2}, // Background of normal windows
			{"mvGuiCol_ChildBg"              , 3}, // Background of child windows
			{"mvGuiCol_PopupBg"              , 4}, // Background of popups, menus, tooltips windows
			{"mvGuiCol_Border"               , 5},
			{"mvGuiCol_BorderShadow"         , 6},
			{"mvGuiCol_FrameBg"              , 7},
			{"mvGuiCol_FrameBgHovered"       , 8},
			{"mvGuiCol_FrameBgActive"        , 9},
			{"mvGuiCol_TitleBg"              , 10},
			{"mvGuiCol_TitleBgActive"        , 11},
			{"mvGuiCol_TitleBgCollapsed"     , 12},
			{"mvGuiCol_MenuBarBg"            , 13},
			{"mvGuiCol_ScrollbarBg"          , 14},
			{"mvGuiCol_ScrollbarGrab"        , 15},
			{"mvGuiCol_ScrollbarGrabHovered" , 16},
			{"mvGuiCol_ScrollbarGrabActive"  , 17},
			{"mvGuiCol_CheckMark"            , 18},
			{"mvGuiCol_SliderGrab"           , 19},
			{"mvGuiCol_SliderGrabActive"     , 20},
			{"mvGuiCol_Button"               , 21},
			{"mvGuiCol_ButtonHovered"        , 22},
			{"mvGuiCol_ButtonActive"         , 23},
			{"mvGuiCol_Header"               , 24}, // Header* colors are used for CollapsingHeader, TreeNode, Selectable, MenuItem
			{"mvGuiCol_HeaderHovered"        , 25},
			{"mvGuiCol_HeaderActive"         , 26},
			{"mvGuiCol_Separator"            , 27},
			{"mvGuiCol_SeparatorHovered"     , 28},
			{"mvGuiCol_SeparatorActive"      , 29},
			{"mvGuiCol_ResizeGrip"           , 30},
			{"mvGuiCol_ResizeGripHovered"    , 31},
			{"mvGuiCol_ResizeGripActive"     , 32},
			{"mvGuiCol_Tab"                  , 33},
			{"mvGuiCol_TabHovered"           , 34},
			{"mvGuiCol_TabActive"            , 35},
			{"mvGuiCol_TabUnfocused"         , 36},
			{"mvGuiCol_TabUnfocusedActive"   , 37},
			{"mvGuiCol_PlotLines"            , 38},
			{"mvGuiCol_PlotLinesHovered"     , 39},
			{"mvGuiCol_PlotHistogram"        , 40},
			{"mvGuiCol_PlotHistogramHovered" , 41},
			{"mvGuiCol_TextSelectedBg"       , 42},
			{"mvGuiCol_DragDropTarget"       , 43},
			{"mvGuiCol_NavHighlight"         , 44}, // Gamepad/keyboard: current highlighted item
			{"mvGuiCol_NavWindowingHighlight", 45}, // Highlight window when using CTRL+TAB
			{"mvGuiCol_NavWindowingDimBg"    , 46}, // Darken/colorize entire screen behind the CTRL+TAB window list, when active 
			{"mvGuiCol_ModalWindowDimBg"     , 47}, // Darken/colorize entire screen behind a modal window, when one is active

			//-----------------------------------------------------------------------------
			// Key Codes
			//-----------------------------------------------------------------------------
			{ "mvKey_0"			, 0x30 },
			{ "mvKey_1"			, 0x31 },
			{ "mvKey_2"			, 0x32 },
			{ "mvKey_3"			, 0x33 },
			{ "mvKey_4"			, 0x34 },
			{ "mvKey_5"			, 0x35 },
			{ "mvKey_6"			, 0x36 },
			{ "mvKey_7"			, 0x37 },
			{ "mvKey_8"			, 0x38 },
			{ "mvKey_9"			, 0x39 },
			{ "mvKey_A"			, 0x41 },
			{ "mvKey_B"			, 0x42 },
			{ "mvKey_C"			, 0x43 },
			{ "mvKey_D"			, 0x44 },
			{ "mvKey_E"			, 0x45 },
			{ "mvKey_F"			, 0x46 },
			{ "mvKey_G"			, 0x47 },
			{ "mvKey_H"			, 0x48 },
			{ "mvKey_I"			, 0x49 },
			{ "mvKey_J"			, 0x4A },
			{ "mvKey_K"			, 0x4B },
			{ "mvKey_L"			, 0x4C },
			{ "mvKey_M"			, 0x4D },
			{ "mvKey_N"			, 0x4E },
			{ "mvKey_O"			, 0x4F },
			{ "mvKey_P"			, 0x50 },
			{ "mvKey_Q"			, 0x51 },
			{ "mvKey_R"			, 0x52 },
			{ "mvKey_S"			, 0x53 },
			{ "mvKey_T"			, 0x54 },
			{ "mvKey_U"			, 0x55 },
			{ "mvKey_V"			, 0x56 },
			{ "mvKey_W"			, 0x57 },
			{ "mvKey_X"			, 0x58 },
			{ "mvKey_Y"			, 0x59 },
			{ "mvKey_Z"			, 0x5A },

#if defined (_WIN32)
			{ "mvKey_Back"			, 0x08 },
			{ "mvKey_Tab"			, 0x09 },
			{ "mvKey_Clear"			, 0x0C },
			{ "mvKey_Return"		, 0x0D },
			{ "mvKey_Shift"			, 0x10 },
			{ "mvKey_Control"		, 0x11 },
			{ "mvKey_Alt"			, 0x12 },
			{ "mvKey_Pause"			, 0x13 },
			{ "mvKey_Capital"		, 0x14 },
			{ "mvKey_Escape"		, 0x1B },
			{ "mvKey_Spacebar"		, 0x20 },
			{ "mvKey_Prior"			, 0x21 },
			{ "mvKey_Next"			, 0x22 },
			{ "mvKey_End"			, 0x23 },
			{ "mvKey_Home"			, 0x24 },
			{ "mvKey_Left"			, 0x25 },
			{ "mvKey_Up"			, 0x26 },
			{ "mvKey_Right"			, 0x27 },
			{ "mvKey_Down"			, 0x28 },
			{ "mvKey_Select"		, 0x29 },
			{ "mvKey_Print"			, 0x2A },
			{ "mvKey_Execute"		, 0x2B },
			{ "mvKey_PrintScreen"	, 0x2C },
			{ "mvKey_Insert"		, 0x2D },
			{ "mvKey_Delete"		, 0x2E },
			{ "mvKey_Help"			, 0x2F },
			{ "mvKey_LWin"		, 0x5B },
			{ "mvKey_RWin"		, 0x5C },
			{ "mvKey_Apps"		, 0x5D },
			{ "mvKey_Sleep"		, 0x5F },
			{ "mvKey_NumPad0"   , 0x60 },
			{ "mvKey_NumPad1"   , 0x61 },
			{ "mvKey_NumPad2"   , 0x62 },
			{ "mvKey_NumPad3"   , 0x63 },
			{ "mvKey_NumPad4"   , 0x64 },
			{ "mvKey_NumPad5"   , 0x65 },
			{ "mvKey_NumPad6"	, 0x66 },
			{ "mvKey_NumPad7"	, 0x67 },
			{ "mvKey_NumPad8"	, 0x68 },
			{ "mvKey_NumPad9"	, 0x69 },
			{ "mvKey_Multiply"	, 0x6A },
			{ "mvKey_Add"		, 0x6B },
			{ "mvKey_Separator"	, 0x6C },
			{ "mvKey_Subtract"	, 0x6D },
			{ "mvKey_Decimal"	, 0x6E },
			{ "mvKey_Divide"	, 0x6F },
			{ "mvKey_F1"		, 0x70 },
			{ "mvKey_F2"		, 0x71 },
			{ "mvKey_F3"		, 0x72 },
			{ "mvKey_F4"		, 0x73 },
			{ "mvKey_F5"		, 0x74 },
			{ "mvKey_F6"		, 0x75 },
			{ "mvKey_F7"		, 0x76 },
			{ "mvKey_F8"		, 0x77 },
			{ "mvKey_F9"		, 0x78 },
			{ "mvKey_F10"		, 0x79 },
			{ "mvKey_F11"		, 0x7A },
			{ "mvKey_F12"		, 0x7B },
			{ "mvKey_F13"		, 0x7C },
			{ "mvKey_F14"		, 0x7D },
			{ "mvKey_F15"		, 0x7E },
			{ "mvKey_F16"		, 0x7F },
			{ "mvKey_F17"		, 0x80 },
			{ "mvKey_F18"		, 0x81 },
			{ "mvKey_F19"		, 0x82 },
			{ "mvKey_F20"		, 0x83 },
			{ "mvKey_F21"		, 0x84 },
			{ "mvKey_F22"		, 0x85 },
			{ "mvKey_F23"		, 0x86 },
			{ "mvKey_F24"		, 0x87 },
			{ "mvKey_NumLock"				, 0x90 },
			{ "mvKey_ScrollLock"			, 0x91 },
			{ "mvKey_LShift"				, 0xA0 },
			{ "mvKey_RShift"				, 0xA1 },
			{ "mvKey_LControl"				, 0xA2 },
			{ "mvKey_RControl"				, 0xA3 },
			{ "mvKey_LMenu"					, 0xA4 },
			{ "mvKey_RMenu"					, 0xA5 },
			{ "mvKey_Browser_Back"			, 0xA6 },
			{ "mvKey_Browser_Forward"		, 0xA7 },
			{ "mvKey_Browser_Refresh"		, 0xA8 },
			{ "mvKey_Browser_Stop"			, 0xA9 },
			{ "mvKey_Browser_Search"		, 0xAA },
			{ "mvKey_Browser_Favorites"		, 0xAB },
			{ "mvKey_Browser_Home"			, 0xAC },
			{ "mvKey_Volume_Mute"			, 0xAD },
			{ "mvKey_Volume_Down"			, 0xAE },
			{ "mvKey_Volume_Up"				, 0xAF },
			{ "mvKey_Media_Next_Track"		, 0xB0 },
			{ "mvKey_Media_Prev_Track"		, 0xB1 },
			{ "mvKey_Media_Stop"			, 0xB2 },
			{ "mvKey_Media_Play_Pause"		, 0xB3 },
			{ "mvKey_Launch_Mail"			, 0xB4 },
			{ "mvKey_Launch_Media_Select"	, 0xB5 },
			{ "mvKey_Launch_App1"			, 0xB6 },
			{ "mvKey_Launch_App2"			, 0xB7 },
			{ "mvKey_Colon"					, 0xBA },
			{ "mvKey_Plus"					, 0xBB },
			{ "mvKey_Comma"					, 0xBC },
			{ "mvKey_Minus"					, 0xBD },
			{ "mvKey_Period"				, 0xBE },
			{ "mvKey_Slash"					, 0xBF },
			{ "mvKey_Tilde"					, 0xC0 },
			{ "mvKey_Open_Brace"			, 0xDB },
			{ "mvKey_Backslash"				, 0xDC },
			{ "mvKey_Close_Brace"			, 0xDD },
			{ "mvKey_Quote"					, 0xDE },
#else
			{ "mvKey_Back",  259 },
			{"mvKey_Tab",  258 },
			{"mvKey_Clear",  259 },
			{"mvKey_Return",  257 },
			{"mvKey_Shift",  340 },
			{"mvKey_Control",  241 },
			{"mvKey_Alt",  342 },
			{"mvKey_Pause",  284 },
			{"mvKey_Capital",  280 },
			{"mvKey_Escape",  256 },
			{"mvKey_Spacebar",  32 },
			{"mvKey_Prior",  -1 },
			{"mvKey_Next",  -1 },
			{"mvKey_End",  269 },
			{"mvKey_Home",  268 },
			{"mvKey_Left",  263 },
			{"mvKey_Up",  265 },
			{"mvKey_Right",  262 },
			{"mvKey_Down",  264 },
			{"mvKey_Select",  -1 },
			{"mvKey_Print",  -1 },
			{"mvKey_Execute",  -1 },
			{"mvKey_PrintScreen",  286 },
			{"mvKey_Insert",  260 },
			{"mvKey_Delete",  261 },
			{"mvKey_Help",  -1 },
			{"mvKey_LWin",  343 },
			{"mvKey_RWin",  347 },
			{"mvKey_Apps",  -1 },
			{"mvKey_Sleep",  -1 },
			{"mvKey_NumPad0",  320 },
			{"mvKey_NumPad1",  321 },
			{"mvKey_NumPad2",  322 },
			{"mvKey_NumPad3",  323 },
			{"mvKey_NumPad4",  324 },
			{"mvKey_NumPad5",  325 },
			{"mvKey_NumPad6",  326 },
			{"mvKey_NumPad7",  327 },
			{"mvKey_NumPad8",  328 },
			{"mvKey_NumPad9",  329 },
			{"mvKey_Multiply",  332 },
			{"mvKey_Add",  334 },
			{"mvKey_Separator",  -1 },
			{"mvKey_Subtract",  333 },
			{"mvKey_Decimal",  330 },
			{"mvKey_Divide",  331 },
			{"mvKey_F1",  290 },
			{"mvKey_F2",  291 },
			{"mvKey_F3",  292 },
			{"mvKey_F4",  293 },
			{"mvKey_F5",  294 },
			{"mvKey_F6",  295 },
			{"mvKey_F7",  296 },
			{"mvKey_F8",  297 },
			{"mvKey_F9",  298 },
			{"mvKey_F10",  299 },
			{"mvKey_F11",  300 },
			{"mvKey_F12",  301 },
			{"mvKey_F13",  302 },
			{"mvKey_F14",  303 },
			{"mvKey_F15",  304 },
			{"mvKey_F16",  305 },
			{"mvKey_F17",  306 },
			{"mvKey_F18",  307 },
			{"mvKey_F19",  308 },
			{"mvKey_F20",  309 },
			{"mvKey_F21",  310 },
			{"mvKey_F22",  311 },
			{"mvKey_F23",  312 },
			{"mvKey_F24",  313 },
			{"mvKey_F24",  314 },
			{"mvKey_NumLock",  282 },
			{"mvKey_ScrollLock",  281 },
			{"mvKey_LShift",  340 },
			{"mvKey_RShift",  344 },
			{"mvKey_LControl",  341 },
			{"mvKey_RControl",  345 },
			{"mvKey_LMenu",  -1 },
			{"mvKey_RMenu",  -1 },
			{"mvKey_Browser_Back",  -1 },
			{"mvKey_Browser_Forward",  -1 },
			{"mvKey_Browser_Refresh",  -1 },
			{"mvKey_Browser_Stop",  -1 },
			{"mvKey_Browser_Search",  -1 },
			{"mvKey_Browser_Favorites",  -1 },
			{"mvKey_Browser_Home",  -1 },
			{"mvKey_Volume_Mute",  -1 },
			{"mvKey_Volume_Down",  -1 },
			{"mvKey_Volume_Up",  -1 },
			{"mvKey_Media_Next_Track",  -1 },
			{"mvKey_Media_Prev_Track",  -1 },
			{"mvKey_Media_Stop",  -1 },
			{"mvKey_Media_Play_Pause",  -1 },
			{"mvKey_Launch_Mail",  -1 },
			{"mvKey_Launch_Media_Select",  -1 },
			{"mvKey_Launch_App1",  -1 },
			{"mvKey_Launch_App2",  -1 },
			{"mvKey_Colon",  59 },
			{"mvKey_Plus",  61 },
			{"mvKey_Comma",  44 },
			{"mvKey_Minus",  45 },
			{"mvKey_Period",  46 },
			{"mvKey_Slash",  47 },
			{"mvKey_Tilde",  96 },
			{"mvKey_Open_Brace",  91 },
			{"mvKey_Backslash",  92 },
			{"mvKey_Close_Brace",  93 },
			{"mvKey_Quote",  39 },
#endif



			//-----------------------------------------------------------------------------
			// Mouse Codes
			//-----------------------------------------------------------------------------
			{ "mvMouseButton_Left"  , 0 },
			{ "mvMouseButton_Right" , 1 },
			{ "mvMouseButton_Middle", 2 },
			{ "mvMouseButton_X1", 3 },
			{ "mvMouseButton_X2", 4 },

			//-----------------------------------------------------------------------------
			// Cardinal directions
			//-----------------------------------------------------------------------------
			{ "mvDir_None" ,-1 },
			{ "mvDir_Left" , 0 },
			{ "mvDir_Right", 1 },
			{ "mvDir_Up"   , 2 },
			{ "mvDir_Down" , 3 },

			//-----------------------------------------------------------------------------
			// Log Levels
			//-----------------------------------------------------------------------------
			{ "mvTRACE"   , 0 },
			{ "mvDEBUG"   , 1 },
			{ "mvINFO"    , 2 },
			{ "mvWARNING" , 3 },
			{ "mvERROR"   , 4 },
			{ "mvOFF"     , 5 }
		};

		return ModuleConstants;
	}

	static PyMethodDef dearpyguimethods[]
	{
		ADD_PYTHON_FUNCTION(add_dummy)
		ADD_PYTHON_FUNCTION(set_start_callback)
		ADD_PYTHON_FUNCTION(set_item_color)
		ADD_PYTHON_FUNCTION(clear_item_color)
		ADD_PYTHON_FUNCTION(set_main_window_resizable)
		ADD_PYTHON_FUNCTION(delete_series)
		ADD_PYTHON_FUNCTION(set_mouse_release_callback)
		ADD_PYTHON_FUNCTION(set_exit_callback)
		ADD_PYTHON_FUNCTION(set_vsync)
		ADD_PYTHON_FUNCTION(get_windows)
		ADD_PYTHON_FUNCTION(get_all_items)
		ADD_PYTHON_FUNCTION(get_item_children)
		ADD_PYTHON_FUNCTION(stop_dearpygui)
		ADD_PYTHON_FUNCTION(is_dearpygui_running)
		ADD_PYTHON_FUNCTION(set_main_window_title)
		ADD_PYTHON_FUNCTION(add_additional_font)
		ADD_PYTHON_FUNCTION(set_style_window_padding)
		ADD_PYTHON_FUNCTION(set_style_frame_padding)
		ADD_PYTHON_FUNCTION(set_style_item_spacing)
		ADD_PYTHON_FUNCTION(set_style_item_inner_spacing)
		ADD_PYTHON_FUNCTION(set_style_touch_extra_padding)
		ADD_PYTHON_FUNCTION(set_style_indent_spacing)
		ADD_PYTHON_FUNCTION(set_style_scrollbar_size)
		ADD_PYTHON_FUNCTION(set_style_grab_min_size)
		ADD_PYTHON_FUNCTION(set_style_window_border_size)
		ADD_PYTHON_FUNCTION(set_style_child_border_size)
		ADD_PYTHON_FUNCTION(set_style_popup_border_size)
		ADD_PYTHON_FUNCTION(set_style_frame_border_size)
		ADD_PYTHON_FUNCTION(set_style_tab_border_size)
		ADD_PYTHON_FUNCTION(set_style_window_rounding)
		ADD_PYTHON_FUNCTION(set_style_child_rounding)
		ADD_PYTHON_FUNCTION(set_style_frame_rounding)
		ADD_PYTHON_FUNCTION(set_style_popup_rounding)
		ADD_PYTHON_FUNCTION(set_style_scrollbar_rounding)
		ADD_PYTHON_FUNCTION(set_style_grab_rounding)
		ADD_PYTHON_FUNCTION(set_style_tab_rounding)
		ADD_PYTHON_FUNCTION(set_style_window_title_align)
		ADD_PYTHON_FUNCTION(set_style_window_menu_button_position)
		ADD_PYTHON_FUNCTION(set_style_color_button_position)
		ADD_PYTHON_FUNCTION(set_style_button_text_align)
		ADD_PYTHON_FUNCTION(set_style_selectable_text_align)
		ADD_PYTHON_FUNCTION(set_style_display_safe_area_padding)
		ADD_PYTHON_FUNCTION(set_style_global_alpha)
		ADD_PYTHON_FUNCTION(set_style_antialiased_lines)
		ADD_PYTHON_FUNCTION(set_style_antialiased_fill)
		ADD_PYTHON_FUNCTION(set_style_curve_tessellation_tolerance)
		ADD_PYTHON_FUNCTION(set_style_circle_segment_max_error)
		ADD_PYTHON_FUNCTION(get_style_window_padding)
		ADD_PYTHON_FUNCTION(get_style_frame_padding)
		ADD_PYTHON_FUNCTION(get_style_item_spacing)
		ADD_PYTHON_FUNCTION(get_style_item_inner_spacing)
		ADD_PYTHON_FUNCTION(get_style_touch_extra_padding)
		ADD_PYTHON_FUNCTION(get_style_indent_spacing)
		ADD_PYTHON_FUNCTION(get_style_scrollbar_size)
		ADD_PYTHON_FUNCTION(get_style_grab_min_size)
		ADD_PYTHON_FUNCTION(get_style_window_border_size)
		ADD_PYTHON_FUNCTION(get_style_child_border_size)
		ADD_PYTHON_FUNCTION(get_style_popup_border_size)
		ADD_PYTHON_FUNCTION(get_style_frame_border_size)
		ADD_PYTHON_FUNCTION(get_style_tab_border_size)
		ADD_PYTHON_FUNCTION(get_style_window_rounding)
		ADD_PYTHON_FUNCTION(get_style_child_rounding)
		ADD_PYTHON_FUNCTION(get_style_frame_rounding)
		ADD_PYTHON_FUNCTION(get_style_popup_rounding)
		ADD_PYTHON_FUNCTION(get_style_scrollbar_rounding)
		ADD_PYTHON_FUNCTION(get_style_grab_rounding)
		ADD_PYTHON_FUNCTION(get_style_tab_rounding)
		ADD_PYTHON_FUNCTION(get_style_window_title_align)
		ADD_PYTHON_FUNCTION(get_style_window_menu_button_position)
		ADD_PYTHON_FUNCTION(get_style_color_button_position)
		ADD_PYTHON_FUNCTION(get_style_button_text_align)
		ADD_PYTHON_FUNCTION(get_style_selectable_text_align)
		ADD_PYTHON_FUNCTION(get_style_display_safe_area_padding)
		ADD_PYTHON_FUNCTION(get_style_global_alpha)
		ADD_PYTHON_FUNCTION(get_style_antialiased_lines)
		ADD_PYTHON_FUNCTION(get_style_antialiased_fill)
		ADD_PYTHON_FUNCTION(get_style_curve_tessellation_tolerance)
		ADD_PYTHON_FUNCTION(get_style_circle_segment_max_error)
		ADD_PYTHON_FUNCTION(show_style_editor)
		ADD_PYTHON_FUNCTION(setup_dearpygui)
		ADD_PYTHON_FUNCTION(set_headers)
		ADD_PYTHON_FUNCTION(render_dearpygui_frame)
		ADD_PYTHON_FUNCTION(cleanup_dearpygui)
		ADD_PYTHON_FUNCTION(start_dearpygui)
		ADD_PYTHON_FUNCTION(clear_table)
		ADD_PYTHON_FUNCTION(get_window_pos)
		ADD_PYTHON_FUNCTION(set_window_pos)
		ADD_PYTHON_FUNCTION(get_global_font_scale)
		ADD_PYTHON_FUNCTION(set_global_font_scale)
		ADD_PYTHON_FUNCTION(select_directory_dialog)
		ADD_PYTHON_FUNCTION(add_table)
		ADD_PYTHON_FUNCTION(end)
		ADD_PYTHON_FUNCTION(add_image)
		ADD_PYTHON_FUNCTION(add_image_button)
		ADD_PYTHON_FUNCTION(add_progress_bar)
		ADD_PYTHON_FUNCTION(add_drag_float)
		ADD_PYTHON_FUNCTION(add_drag_int)
		ADD_PYTHON_FUNCTION(add_drag_float2)
		ADD_PYTHON_FUNCTION(add_drag_float3)
		ADD_PYTHON_FUNCTION(add_drag_float4)
		ADD_PYTHON_FUNCTION(add_drag_int2)
		ADD_PYTHON_FUNCTION(add_drag_int3)
		ADD_PYTHON_FUNCTION(add_drag_int4)
		ADD_PYTHON_FUNCTION(add_slider_float)
		ADD_PYTHON_FUNCTION(add_slider_int)
		ADD_PYTHON_FUNCTION(add_slider_float2)
		ADD_PYTHON_FUNCTION(add_slider_float3)
		ADD_PYTHON_FUNCTION(add_slider_float4)
		ADD_PYTHON_FUNCTION(add_slider_int2)
		ADD_PYTHON_FUNCTION(add_slider_int3)
		ADD_PYTHON_FUNCTION(add_slider_int4)
		ADD_PYTHON_FUNCTION(add_tree_node)
		ADD_PYTHON_FUNCTION(add_selectable)
		ADD_PYTHON_FUNCTION(add_popup)
		ADD_PYTHON_FUNCTION(add_window)
		ADD_PYTHON_FUNCTION(add_indent)
		ADD_PYTHON_FUNCTION(unindent)
		ADD_PYTHON_FUNCTION(add_simple_plot)
		ADD_PYTHON_FUNCTION(add_combo)
		ADD_PYTHON_FUNCTION(add_text)
		ADD_PYTHON_FUNCTION(add_label_text)
		ADD_PYTHON_FUNCTION(add_listbox)
		ADD_PYTHON_FUNCTION(add_color_edit3)
		ADD_PYTHON_FUNCTION(add_color_edit4)
		ADD_PYTHON_FUNCTION(add_color_picker3)
		ADD_PYTHON_FUNCTION(add_color_picker4)
		ADD_PYTHON_FUNCTION(add_separator)
		ADD_PYTHON_FUNCTION(add_button)
		ADD_PYTHON_FUNCTION(add_input_text)
		ADD_PYTHON_FUNCTION(add_input_int)
		ADD_PYTHON_FUNCTION(add_input_int2)
		ADD_PYTHON_FUNCTION(add_input_int3)
		ADD_PYTHON_FUNCTION(add_input_int4)
		ADD_PYTHON_FUNCTION(add_input_float)
		ADD_PYTHON_FUNCTION(add_input_float2)
		ADD_PYTHON_FUNCTION(add_input_float3)
		ADD_PYTHON_FUNCTION(add_input_float4)
		ADD_PYTHON_FUNCTION(add_radio_button)
		ADD_PYTHON_FUNCTION(add_checkbox)
		ADD_PYTHON_FUNCTION(add_group)
		ADD_PYTHON_FUNCTION(add_child)
		ADD_PYTHON_FUNCTION(add_tab_bar)
		ADD_PYTHON_FUNCTION(add_tab)
		ADD_PYTHON_FUNCTION(add_menu_bar)
		ADD_PYTHON_FUNCTION(add_menu)
		ADD_PYTHON_FUNCTION(add_menu_item)
		ADD_PYTHON_FUNCTION(add_spacing)
		ADD_PYTHON_FUNCTION(add_same_line)
		ADD_PYTHON_FUNCTION(add_tooltip)
		ADD_PYTHON_FUNCTION(add_collapsing_header)
		ADD_PYTHON_FUNCTION(add_column)
		ADD_PYTHON_FUNCTION(insert_column)
		ADD_PYTHON_FUNCTION(delete_column)
		ADD_PYTHON_FUNCTION(add_row)
		ADD_PYTHON_FUNCTION(insert_row)
		ADD_PYTHON_FUNCTION(delete_row)
		ADD_PYTHON_FUNCTION(get_table_item)
		ADD_PYTHON_FUNCTION(set_table_item)
		ADD_PYTHON_FUNCTION(set_table_selection)
		ADD_PYTHON_FUNCTION(get_table_selections)
		ADD_PYTHON_FUNCTION(get_delta_time)
		ADD_PYTHON_FUNCTION(get_total_time)
		ADD_PYTHON_FUNCTION(get_data)
		ADD_PYTHON_FUNCTION(delete_data)
		ADD_PYTHON_FUNCTION(add_data)
		ADD_PYTHON_FUNCTION(run_async_function)
		ADD_PYTHON_FUNCTION(open_file_dialog)
		ADD_PYTHON_FUNCTION(delete_item)
		ADD_PYTHON_FUNCTION(does_item_exist)
		ADD_PYTHON_FUNCTION(move_item_down)
		ADD_PYTHON_FUNCTION(move_item_up)
		ADD_PYTHON_FUNCTION(get_theme_item)
		ADD_PYTHON_FUNCTION(get_item_callback)
		ADD_PYTHON_FUNCTION(get_item_width)
		ADD_PYTHON_FUNCTION(get_item_height)
		ADD_PYTHON_FUNCTION(get_item_parent)
		ADD_PYTHON_FUNCTION(get_item_popup)
		ADD_PYTHON_FUNCTION(get_item_tip)
		ADD_PYTHON_FUNCTION(get_item_label)
		ADD_PYTHON_FUNCTION(get_main_window_size)
		ADD_PYTHON_FUNCTION(get_theme)
		ADD_PYTHON_FUNCTION(get_thread_count)
		ADD_PYTHON_FUNCTION(is_threadpool_high_performance)
		ADD_PYTHON_FUNCTION(get_threadpool_timeout)
		ADD_PYTHON_FUNCTION(get_active_window)
		ADD_PYTHON_FUNCTION(get_dearpygui_version)
		ADD_PYTHON_FUNCTION(show_source)
		ADD_PYTHON_FUNCTION(show_about)
		ADD_PYTHON_FUNCTION(show_debug)
		ADD_PYTHON_FUNCTION(show_metrics)
		ADD_PYTHON_FUNCTION(close_popup)
		ADD_PYTHON_FUNCTION(show_documentation)
		ADD_PYTHON_FUNCTION(set_threadpool_timeout)
		ADD_PYTHON_FUNCTION(set_thread_count)
		ADD_PYTHON_FUNCTION(set_threadpool_high_performance)
		ADD_PYTHON_FUNCTION(set_main_window_size)
		ADD_PYTHON_FUNCTION(set_item_popup)
		ADD_PYTHON_FUNCTION(set_item_label)
		ADD_PYTHON_FUNCTION(is_item_hovered)
		ADD_PYTHON_FUNCTION(is_item_shown)
		ADD_PYTHON_FUNCTION(is_item_active)
		ADD_PYTHON_FUNCTION(is_item_focused)
		ADD_PYTHON_FUNCTION(is_item_clicked)
		ADD_PYTHON_FUNCTION(is_item_container)
		ADD_PYTHON_FUNCTION(is_item_visible)
		ADD_PYTHON_FUNCTION(is_item_edited)
		ADD_PYTHON_FUNCTION(is_item_activated)
		ADD_PYTHON_FUNCTION(is_item_deactivated)
		ADD_PYTHON_FUNCTION(is_item_deactivated_after_edit)
		ADD_PYTHON_FUNCTION(is_item_toggled_open)
		ADD_PYTHON_FUNCTION(get_item_rect_min)
		ADD_PYTHON_FUNCTION(get_item_rect_max)
		ADD_PYTHON_FUNCTION(get_item_rect_size)
		ADD_PYTHON_FUNCTION(show_item)
		ADD_PYTHON_FUNCTION(hide_item)
		ADD_PYTHON_FUNCTION(set_theme_item)
		ADD_PYTHON_FUNCTION(set_theme)
		ADD_PYTHON_FUNCTION(set_render_callback)
		ADD_PYTHON_FUNCTION(set_item_callback)
		ADD_PYTHON_FUNCTION(set_item_tip)
		ADD_PYTHON_FUNCTION(set_item_width)
		ADD_PYTHON_FUNCTION(set_item_height)
		ADD_PYTHON_FUNCTION(get_value)
		ADD_PYTHON_FUNCTION(set_value)
		ADD_PYTHON_FUNCTION(set_mouse_drag_callback)
		ADD_PYTHON_FUNCTION(is_mouse_button_dragging)
		ADD_PYTHON_FUNCTION(is_mouse_button_down)
		ADD_PYTHON_FUNCTION(is_mouse_button_clicked)
		ADD_PYTHON_FUNCTION(is_mouse_button_double_clicked)
		ADD_PYTHON_FUNCTION(is_mouse_button_released)
		ADD_PYTHON_FUNCTION(get_mouse_drag_delta)
		ADD_PYTHON_FUNCTION(set_mouse_wheel_callback)
		ADD_PYTHON_FUNCTION(get_mouse_pos)
		ADD_PYTHON_FUNCTION(is_key_pressed)
		ADD_PYTHON_FUNCTION(is_key_released)
		ADD_PYTHON_FUNCTION(is_key_down)
		ADD_PYTHON_FUNCTION(set_mouse_click_callback)
		ADD_PYTHON_FUNCTION(set_mouse_down_callback)
		ADD_PYTHON_FUNCTION(set_mouse_double_click_callback)
		ADD_PYTHON_FUNCTION(set_key_down_callback)
		ADD_PYTHON_FUNCTION(set_key_press_callback)
		ADD_PYTHON_FUNCTION(set_key_release_callback)
		ADD_PYTHON_FUNCTION(set_resize_callback)
		ADD_PYTHON_FUNCTION(set_mouse_move_callback)
		ADD_PYTHON_FUNCTION(get_log_level)
		ADD_PYTHON_FUNCTION(clear_log)
		ADD_PYTHON_FUNCTION(show_logger)
		ADD_PYTHON_FUNCTION(set_log_level)
		ADD_PYTHON_FUNCTION(log)
		ADD_PYTHON_FUNCTION(log_debug)
		ADD_PYTHON_FUNCTION(log_info)
		ADD_PYTHON_FUNCTION(log_warning)
		ADD_PYTHON_FUNCTION(log_error)
		
		// drawing commands
		ADD_PYTHON_FUNCTION(draw_arrow)
		ADD_PYTHON_FUNCTION(get_drawing_origin)
		ADD_PYTHON_FUNCTION(get_drawing_scale)
		ADD_PYTHON_FUNCTION(set_drawing_origin)
		ADD_PYTHON_FUNCTION(set_drawing_scale)
		ADD_PYTHON_FUNCTION(get_drawing_size)
		ADD_PYTHON_FUNCTION(set_drawing_size)
		ADD_PYTHON_FUNCTION(add_drawing)
		ADD_PYTHON_FUNCTION(draw_image)
		ADD_PYTHON_FUNCTION(draw_line)
		ADD_PYTHON_FUNCTION(draw_triangle)
		ADD_PYTHON_FUNCTION(draw_rectangle)
		ADD_PYTHON_FUNCTION(draw_quad)
		ADD_PYTHON_FUNCTION(draw_text)
		ADD_PYTHON_FUNCTION(draw_circle)
		ADD_PYTHON_FUNCTION(draw_polyline)
		ADD_PYTHON_FUNCTION(draw_polygon)
		ADD_PYTHON_FUNCTION(draw_bezier_curve)
		ADD_PYTHON_FUNCTION(clear_drawing)
		ADD_PYTHON_FUNCTION(delete_drawing_item)

		// plot commands
		ADD_PYTHON_FUNCTION(is_plot_queried)
		ADD_PYTHON_FUNCTION(get_plot_query_area)
		ADD_PYTHON_FUNCTION(clear_plot)
		ADD_PYTHON_FUNCTION(reset_xticks)
		ADD_PYTHON_FUNCTION(reset_yticks)
		ADD_PYTHON_FUNCTION(set_xticks)
		ADD_PYTHON_FUNCTION(set_yticks)
		ADD_PYTHON_FUNCTION(set_plot_xlimits_auto)
		ADD_PYTHON_FUNCTION(set_plot_ylimits_auto)
		ADD_PYTHON_FUNCTION(set_plot_xlimits)
		ADD_PYTHON_FUNCTION(set_plot_ylimits)
		ADD_PYTHON_FUNCTION(set_color_map)
		ADD_PYTHON_FUNCTION(add_plot)
		ADD_PYTHON_FUNCTION(add_shade_series)
		ADD_PYTHON_FUNCTION(add_bar_series)
		ADD_PYTHON_FUNCTION(add_line_series)
		ADD_PYTHON_FUNCTION(add_pie_series)
		ADD_PYTHON_FUNCTION(add_scatter_series)
		ADD_PYTHON_FUNCTION(add_area_series)
		ADD_PYTHON_FUNCTION(add_stem_series)
		ADD_PYTHON_FUNCTION(add_error_series)
		ADD_PYTHON_FUNCTION(add_text_point)
		{NULL, NULL, 0, NULL}
	};

	static PyModuleDef dearpyguiModule = {
		PyModuleDef_HEAD_INIT, "dearpygui", NULL, -1, dearpyguimethods,
		NULL, NULL, NULL, NULL
	};

	PyMODINIT_FUNC PyInit_dearpygui(void)
	{
		PyObject* m;

		m = PyModule_Create(&dearpyguiModule);
		if (m == NULL)
			return NULL;

		auto constants = GetModuleConstants();

		// handled in the stub file
		for (auto& item : constants)
			PyModule_AddIntConstant(m, item.first.c_str(), item.second);

		auto MarvelError = PyErr_NewException("dearpygui.error", NULL, NULL);
		Py_XINCREF(MarvelError);
		if (PyModule_AddObject(m, "error", MarvelError) < 0) {
			Py_XDECREF(MarvelError);
			Py_CLEAR(MarvelError);
			Py_DECREF(m);
			return NULL;
		}

		return m;
	}

	void start_dearpygui_error()
	{
		PyErr_Print();

		// create window
		auto window = mvWindow::CreatemvWindow(mvApp::GetApp()->getActualWidth(), mvApp::GetApp()->getActualHeight(), true);
		window->show();
		window->run();
		delete window;
		delete mvApp::GetApp();

	}
}