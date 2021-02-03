#include "mvThemeInterface.h"

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

		parsers->insert({ "set_theme_color", mvPythonParser({
			{mvPythonDataType::Integer, "constant", "mvGuiCol_* constants"},
			{mvPythonDataType::FloatList, "color"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "item", "", ""}
		}, "Sets a color of a theme item.", "None", "Themes and Styles") });

		parsers->insert({ "set_style_touch_extra_padding", mvPythonParser({
			{mvPythonDataType::Float, "x"},
			{mvPythonDataType::Float, "y"},
		}, "Expand reactive bounding box for touch-based system where touch position is not accurate enough. Unfortunately we don't sort widgets so priority on overlap will always be given to the first widget. So don't grow this too much!", "None", "Themes and Styles") });

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

		parsers->insert({ "get_style_touch_extra_padding", mvPythonParser({
		}, "Get touch extra padding.", "List[float]", "Themes and Styles") });

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

	PyObject* set_theme_color(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int constant;
		PyObject* color;
		const char* item = "";

		if (!(*mvApp::GetApp()->getParsers())["set_theme_color"].parse(args, kwargs, __FUNCTION__, &constant, &color, &item))
			return GetPyNone();

		Py_XINCREF(color);
		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				mvEventBus::Publish
				(
					mvEVT_CATEGORY_THEMES,
					SID("color_change"),
					{
						CreateEventArgument("WIDGET", std::string(item)),
						CreateEventArgument("ID", constant),
						CreateEventArgument("COLOR", ToColor(color))
					}
				);

				// to ensure the decrement happens on the python thread
				mvApp::GetApp()->getCallbackRegistry().submitCallback([=]()
					{
						Py_XDECREF(color);
					});

			});

		return GetPyNone();
	}

	PyObject* set_global_font_scale(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		float scale;

		if (!(*mvApp::GetApp()->getParsers())["set_global_font_scale"].parse(args, kwargs, __FUNCTION__, &scale))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
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

		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				mvApp::GetApp()->setFont(file, size, glyph_ranges, imgui_custom_ranges, imgui_custom_chars);
			});

		return GetPyNone();
	}
}