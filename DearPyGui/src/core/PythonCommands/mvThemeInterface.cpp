#include "mvThemeInterface.h"
#include "mvInterfaceCore.h"

namespace Marvel {

	void AddThemeCommands(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_additional_font", mvPythonParser({
			{mvPythonDataType::String, "file", "ttf or otf file"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::Float, "size", "", "13.0"},
			{mvPythonDataType::String, "glyph_ranges", "options: korean, japanese, chinese_full, chinese_simplified_common, cryillic, thai, vietnamese", "''"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "custom_glyph_chars", "", "()"},
			{mvPythonDataType::Object, "custom_glyph_ranges", "list of ranges", "List[List[int]]"},
		}, "Adds additional font.", "None", "Themes and Styles") });

		parsers->insert({ "set_theme", mvPythonParser({
			{mvPythonDataType::String, "theme"}
		}, "Set the application's theme to a built-in theme.", "None", "Themes and Styles") });

		parsers->insert({ "get_theme_item", mvPythonParser({
			{mvPythonDataType::Integer, "item"}
		}, "Returns a theme item's color", "(float, float, float, float)", "Themes and Styles") });

		parsers->insert({ "get_theme", mvPythonParser({
		}, "Returns the current theme.", "str", "Themes and Styles") });

		parsers->insert({ "set_item_color", mvPythonParser({
			{mvPythonDataType::String, "item"},
			{mvPythonDataType::Integer, "style"},
			{mvPythonDataType::FloatList, "color"}
		}, "Sets an color style for a single item.", "None", "Themes and Styles") });

		parsers->insert({ "clear_item_color", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Clears individual color styles for an item.", "None", "Themes and Styles") });

		parsers->insert({ "set_theme_item", mvPythonParser({
			{mvPythonDataType::Integer, "item"},
			{mvPythonDataType::Integer, "r"},
			{mvPythonDataType::Integer, "g"},
			{mvPythonDataType::Integer, "b"},
			{mvPythonDataType::Integer, "a"}
		}, "Sets a theme item.", "None", "Themes and Styles") });

		parsers->insert({ "set_style_window_padding", mvPythonParser({
			{mvPythonDataType::Float, "x"},
			{mvPythonDataType::Float, "y"},
		}, "Sets padding within a window.", "None", "Themes and Styles") });

		parsers->insert({ "set_style_frame_padding", mvPythonParser({
			{mvPythonDataType::Float, "x"},
			{mvPythonDataType::Float, "y"},
		}, "Sets padding within a framed rectangle (used by most widgets).", "None", "Themes and Styles") });

		parsers->insert({ "set_style_item_spacing", mvPythonParser({
			{mvPythonDataType::Float, "x"},
			{mvPythonDataType::Float, "y"},
		}, "Sets horizontal and vertical spacing between widgets/lines.", "None", "Themes and Styles") });

		parsers->insert({ "set_style_item_inner_spacing", mvPythonParser({
			{mvPythonDataType::Float, "x"},
			{mvPythonDataType::Float, "y"},
		}, "Sets horizontal and vertical spacing between within elements of a composed widget (e.g. a slider and its label).", "None", "Themes and Styles") });

		parsers->insert({ "set_style_touch_extra_padding", mvPythonParser({
			{mvPythonDataType::Float, "x"},
			{mvPythonDataType::Float, "y"},
		}, "Expand reactive bounding box for touch-based system where touch position is not accurate enough. Unfortunately we don't sort widgets so priority on overlap will always be given to the first widget. So don't grow this too much!", "None", "Themes and Styles") });

		parsers->insert({ "set_style_indent_spacing", mvPythonParser({
			{mvPythonDataType::Float, "value"}
		}, "Sets horizontal indentation when e.g. entering a tree node. Generally == (FontSize + FramePadding.x*2).", "None", "Themes and Styles") });

		parsers->insert({ "set_style_scrollbar_size", mvPythonParser({
			{mvPythonDataType::Float, "value"}
		}, "Sets width of the vertical scrollbar, Height of the horizontal scrollbar.", "None", "Themes and Styles") });

		parsers->insert({ "set_style_grab_min_size", mvPythonParser({
			{mvPythonDataType::Float, "value"}
		}, "Sets minimum width/height of a grab box for slider/scrollbar.", "None", "Themes and Styles") });

		parsers->insert({ "set_style_window_border_size", mvPythonParser({
			{mvPythonDataType::Float, "value"}
		}, "Sets thickness of border around windows. Generally set to 0.0 or 1.0. (Other values are not well tested and more CPU/GPU costly).", "None", "Themes and Styles") });

		parsers->insert({ "set_style_child_border_size", mvPythonParser({
			{mvPythonDataType::Float, "value"}
		}, "Sets thickness of border around child windows. Generally set to 0.0 or 1.0. (Other values are not well tested and more CPU/GPU costly).", "None", "Themes and Styles") });

		parsers->insert({ "set_style_popup_border_size", mvPythonParser({
			{mvPythonDataType::Float, "value"}
		}, "Sets thickness of border around popup/tooltip windows. Generally set to 0.0 or 1.0. (Other values are not well tested and more CPU/GPU costly).", "None", "Themes and Styles") });

		parsers->insert({ "set_style_frame_border_size", mvPythonParser({
			{mvPythonDataType::Float, "value"}
		}, "Sets thickness of border around frames. Generally set to 0.0 or 1.0. (Other values are not well tested and more CPU/GPU costly).", "None", "Themes and Styles") });

		parsers->insert({ "set_style_tab_border_size", mvPythonParser({
			{mvPythonDataType::Float, "value"}
		}, "Sets thickness of border around tabs.", "None", "Themes and Styles") });

		parsers->insert({ "set_style_window_rounding", mvPythonParser({
			{mvPythonDataType::Float, "value"}
		}, "Sets Radius of window corners rounding. Set to 0.0fto have rectangular windows. Large values tend to lead to variety of artifacts and are not recommended.", "None", "Themes and Styles") });

		parsers->insert({ "set_style_child_rounding", mvPythonParser({
			{mvPythonDataType::Float, "value"}
		}, "Sets radius of child window corners rounding. Set to 0.0 to have rectangular windows.", "None", "Themes and Styles") });

		parsers->insert({ "set_style_frame_rounding", mvPythonParser({
			{mvPythonDataType::Float, "value"}
		}, "Sets radius of frame corners rounding. Set to 0.0 to have rectangular frame (used by most widgets).", "None", "Themes and Styles") });

		parsers->insert({ "set_style_popup_rounding", mvPythonParser({
			{mvPythonDataType::Float, "value"}
		}, "Sets radius of popup window corners rounding. (Note that tooltip windows use WindowRounding).", "None", "Themes and Styles") });

		parsers->insert({ "set_style_scrollbar_rounding", mvPythonParser({
			{mvPythonDataType::Float, "value"}
		}, "Sets radius of grab corners for scrollbar.", "None", "Themes and Styles") });

		parsers->insert({ "set_style_grab_rounding", mvPythonParser({
			{mvPythonDataType::Float, "value"}
		}, "Sets radius of grabs corners rounding. Set to 0.0 to have rectangular slider grabs.", "None", "Themes and Styles") });

		parsers->insert({ "set_style_tab_rounding", mvPythonParser({
			{mvPythonDataType::Float, "value"}
		}, "Sets radius of upper corners of a tab. Set to 0.0 to have rectangular tabs.", "None", "Themes and Styles") });

		parsers->insert({ "set_style_window_title_align", mvPythonParser({
			{mvPythonDataType::Float, "x"},
			{mvPythonDataType::Float, "y"},
		}, "Sets alignment for title bar text. Defaults to (0.0,0.5) for left-aligned,vertically centered.", "None", "Themes and Styles") });

		parsers->insert({ "set_style_window_menu_button_position", mvPythonParser({
			{mvPythonDataType::Integer, "value"}
		}, "Sets side of the collapsing/docking button in the title bar (None/Left/Right). Defaults to mvGuiDir_Left.", "None", "Themes and Styles") });

		parsers->insert({ "set_style_color_button_position", mvPythonParser({
			{mvPythonDataType::Integer, "value"}
		}, "Sets side of the color button in the ColorEdit4 widget (left/right). Defaults to mvGuiDir_Right.", "None", "Themes and Styles") });

		parsers->insert({ "set_style_button_text_align", mvPythonParser({
			{mvPythonDataType::Float, "x"},
			{mvPythonDataType::Float, "y"},
		}, "Sets alignment of button text when button is larger than text. Defaults to (0.5, 0.5) (centered).", "None", "Themes and Styles") });

		parsers->insert({ "set_style_selectable_text_align", mvPythonParser({
			{mvPythonDataType::Float, "x"},
			{mvPythonDataType::Float, "y"},
		}, "Sets alignment of selectable text. Defaults to (0.0, 0.0) (top-left aligned). It's generally important to keep this left-aligned if you want to lay multiple items on a same line.", "None", "Themes and Styles") });

		parsers->insert({ "set_style_display_safe_area_padding", mvPythonParser({
			{mvPythonDataType::Float, "x"},
			{mvPythonDataType::Float, "y"},
		}, "Sets if you cannot see the edges of your screen (e.g. on a TV) increase the safe area padding. Apply to popups/tooltips as well regular windows. NB: Prefer configuring your TV sets correctly!", "None", "Themes and Styles") });

		parsers->insert({ "set_style_global_alpha", mvPythonParser({
			{mvPythonDataType::Float, "value"}
		}, "Sets global alpha applies to everything in Dear PyGui.", "None", "Themes and Styles") });

		parsers->insert({ "set_style_antialiased_lines", mvPythonParser({
			{mvPythonDataType::Bool, "value"}
		}, "Sets anti-aliasing on lines/borders. Disable if you are really tight on CPU/GPU.", "None", "Themes and Styles") });

		parsers->insert({ "set_style_antialiased_fill", mvPythonParser({
			{mvPythonDataType::Bool, "value"}
		}, "Sets anti-aliasing on filled shapes (rounded rectangles, circles, etc.).", "None", "Themes and Styles") });

		parsers->insert({ "set_style_curve_tessellation_tolerance", mvPythonParser({
			{mvPythonDataType::Float, "value"}
		}, "Sets Tessellation tolerance.", "None", "Themes and Styles") });

		parsers->insert({ "set_style_circle_segment_max_error", mvPythonParser({
			{mvPythonDataType::Float, "value"}
		}, "Sets maximum error (in pixels) allowed when using draw_circle()or drawing rounded corner rectangles with no explicit segment count specified. Decrease for higher quality but more geometry.", "None", "Themes and Styles") });

		parsers->insert({ "get_style_window_padding", mvPythonParser({
		}, "Gets padding within a window.", "List[float]", "Themes and Styles") });

		parsers->insert({ "get_style_frame_padding", mvPythonParser({
		}, "Gets padding within a framed rectangle (used by most widgets).", "List[float]", "Themes and Styles") });

		parsers->insert({ "get_style_item_spacing", mvPythonParser({
		}, "Gets horizontal and vertical spacing between widgets/lines.", "List[float]", "Themes and Styles") });

		parsers->insert({ "get_style_item_inner_spacing", mvPythonParser({
		}, "Gets horizontal and vertical spacing between within elements of a composed widget (e.g. a slider and its label).", "List[float]", "Themes and Styles") });

		parsers->insert({ "get_style_touch_extra_padding", mvPythonParser({
		}, "Get touch extra padding.", "List[float]", "Themes and Styles") });

		parsers->insert({ "get_style_indent_spacing", mvPythonParser({
		}, "Gets horizontal indentation when e.g. entering a tree node. Generally == (FontSize + FramePadding.x*2).", "float", "Themes and Styles") });

		parsers->insert({ "get_style_scrollbar_size", mvPythonParser({
		}, "Gets width of the vertical scrollbar, Height of the horizontal scrollbar.", "float", "Themes and Styles") });

		parsers->insert({ "get_style_grab_min_size", mvPythonParser({
		}, "Gets minimum width/height of a grab box for slider/scrollbar.", "float", "Themes and Styles") });

		parsers->insert({ "get_style_window_border_size", mvPythonParser({
		}, "Gets thickness of border around windows.", "float", "Themes and Styles") });

		parsers->insert({ "get_style_child_border_size", mvPythonParser({
		}, "Gets thickness of border around child windows.", "float", "Themes and Styles") });

		parsers->insert({ "get_style_popup_border_size", mvPythonParser({
		}, "Gets thickness of border around popup/tooltip windows.", "float", "Themes and Styles") });

		parsers->insert({ "get_style_frame_border_size", mvPythonParser({
		}, "Gets thickness of border around frames.", "float", "Themes and Styles") });

		parsers->insert({ "get_style_tab_border_size", mvPythonParser({
		}, "Gets thickness of border around tabs.", "float", "Themes and Styles") });

		parsers->insert({ "get_style_window_rounding", mvPythonParser({
		}, "Gets radius of window corners rounding.", "float", "Themes and Styles") });

		parsers->insert({ "get_style_child_rounding", mvPythonParser({
		}, "Gets radius of child window corners rounding.", "float", "Themes and Styles") });

		parsers->insert({ "get_style_frame_rounding", mvPythonParser({
		}, "Gets radius of frame corners rounding.", "float", "Themes and Styles") });

		parsers->insert({ "get_style_popup_rounding", mvPythonParser({
		}, "Gets radius of popup window corners rounding. (Note that tooltip windows use WindowRounding).", "float", "Themes and Styles") });

		parsers->insert({ "get_style_scrollbar_rounding", mvPythonParser({
		}, "Gets radius of grab corners for scrollbar.", "float", "Themes and Styles") });

		parsers->insert({ "get_style_grab_rounding", mvPythonParser({
		}, "Gets radius of grabs corners rounding. Set to 0.0 to have rectangular slider grabs.", "float", "Themes and Styles") });

		parsers->insert({ "get_style_tab_rounding", mvPythonParser({
		}, "Gets radius of upper corners of a tab. Set to 0.0 to have rectangular tabs.", "float", "Themes and Styles") });

		parsers->insert({ "get_style_window_title_align", mvPythonParser({
		}, "Gets alignment for title bar text. Defaults to (0.0,0.5) for left-aligned,vertically centered.", "List[float]", "Themes and Styles") });

		parsers->insert({ "get_style_window_menu_button_position", mvPythonParser({
		}, "Gets side of the collapsing/docking button in the title bar (None/Left/Right). Defaults to mvGuiDir_Left.", "int", "Themes and Styles") });

		parsers->insert({ "get_style_color_button_position", mvPythonParser({
		}, "Gets side of the color button in the ColorEdit4 widget (left/right). Defaults to mvGuiDir_Right.", "int", "Themes and Styles") });

		parsers->insert({ "get_style_button_text_align", mvPythonParser({
		}, "Gets alignment of button text when button is larger than text. Defaults to (0.5, 0.5) (centered).", "List[float]", "Themes and Styles") });

		parsers->insert({ "get_style_selectable_text_align", mvPythonParser({
		}, "Gets alignment of selectable text. Defaults to (0.0, 0.0) (top-left aligned).", "List[float]", "Themes and Styles") });

		parsers->insert({ "get_style_display_safe_area_padding", mvPythonParser({
		}, "Gets safe area padding. Applies to popups/tooltips as well regular windows.", "List[float]", "Themes and Styles") });

		parsers->insert({ "get_style_global_alpha", mvPythonParser({
		}, "Gets global alpha applies to everything in Dear PyGui.", "float", "Themes and Styles") });

		parsers->insert({ "get_style_antialiased_lines", mvPythonParser({
		}, "Gets anti-aliasing on lines/borders.", "bool", "Themes and Styles") });

		parsers->insert({ "get_style_antialiased_fill", mvPythonParser({
		}, "Gets anti-aliasing on filled shapes (rounded rectangles, circles, etc.).", "bool", "Themes and Styles") });

		parsers->insert({ "get_style_curve_tessellation_tolerance", mvPythonParser({
		}, "Gets Tessellation tolerance.", "float", "Themes and Styles") });

		parsers->insert({ "get_style_circle_segment_max_error", mvPythonParser({
		}, "Gets maximum error (in pixels) allowed when using draw_circle()or drawing rounded corner rectangles with no explicit segment count specified.", "float", "Themes and Styles") });
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

	PyObject* get_theme_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int item;

		if (!(*mvApp::GetApp()->getParsers())["get_theme_item"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto color = mvApp::GetApp()->getThemeItem(item);

		return ToPyList(std::vector<int>{ color.r, color.g, color.b, color.a });
	}

	PyObject* get_theme(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return ToPyString(mvApp::GetApp()->getAppTheme());
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

	PyObject* set_item_color(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;
		int style;
		PyObject* color;

		if (!(*mvApp::GetApp()->getParsers())["set_item_color"].parse(args, kwargs, __FUNCTION__, &item, &style, &color))
			return GetPyNone();

		auto mcolor = ToColor(color);

		mvAppItem* appitem = mvApp::GetApp()->getItemRegistry().getItem(item);

		if (appitem)
			appitem->getStyleManager().addColorStyle(style, mcolor);

		return GetPyNone();
	}

	PyObject* clear_item_color(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["clear_item_color"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		mvAppItem* appitem = mvApp::GetApp()->getItemRegistry().getItem(item);

		if (appitem)
			appitem->getStyleManager().clearColors();

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
}