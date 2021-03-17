#include "mvModule_Core.h"
#include "mvApp.h"
#include "mvInput.h"
#include "mvAppLog.h"
#include "mvAppItems.h"
#include "mvDrawList.h"
#include "mvDrawCmdCommon.h"
#include "mvWindow.h"
#include "mvImGuiThemeScope.h"
#include <ImGuiFileDialog.h>
#include <cstdlib>

namespace Marvel {

	PyMethodDef* mvModule_Core::GetSubMethods()
	{
		static PyMethodDef methods[]
		{
			ADD_PYTHON_FUNCTION(get_item_configuration)
			ADD_PYTHON_FUNCTION(configure_item)

			// app interface
			ADD_PYTHON_FUNCTION(enable_docking)
			ADD_PYTHON_FUNCTION(add_texture)
			ADD_PYTHON_FUNCTION(decrement_texture)
			ADD_PYTHON_FUNCTION(add_character_remap)
			ADD_PYTHON_FUNCTION(set_primary_window)
			ADD_PYTHON_FUNCTION(set_accelerator_callback)
			ADD_PYTHON_FUNCTION(set_main_window_size)
			ADD_PYTHON_FUNCTION(get_active_window)
			ADD_PYTHON_FUNCTION(get_dearpygui_version)
			ADD_PYTHON_FUNCTION(get_main_window_size)
			ADD_PYTHON_FUNCTION(setup_dearpygui)
			ADD_PYTHON_FUNCTION(render_dearpygui_frame)
			ADD_PYTHON_FUNCTION(cleanup_dearpygui)
			ADD_PYTHON_FUNCTION(start_dearpygui)
			ADD_PYTHON_FUNCTION(get_global_font_scale)
			ADD_PYTHON_FUNCTION(set_global_font_scale)
			ADD_PYTHON_FUNCTION(select_directory_dialog)
			ADD_PYTHON_FUNCTION(get_delta_time)
			ADD_PYTHON_FUNCTION(get_total_time)
			ADD_PYTHON_FUNCTION(open_file_dialog)
			ADD_PYTHON_FUNCTION(set_exit_callback)
			ADD_PYTHON_FUNCTION(set_vsync)
			ADD_PYTHON_FUNCTION(stop_dearpygui)
			ADD_PYTHON_FUNCTION(is_dearpygui_running)
			ADD_PYTHON_FUNCTION(set_main_window_title)
			ADD_PYTHON_FUNCTION(set_main_window_pos)
			ADD_PYTHON_FUNCTION(set_main_window_resizable)
			ADD_PYTHON_FUNCTION(set_start_callback)
			ADD_PYTHON_FUNCTION(close_popup)
			ADD_PYTHON_FUNCTION(get_log_level)
			ADD_PYTHON_FUNCTION(clear_log)
			ADD_PYTHON_FUNCTION(show_logger)
			ADD_PYTHON_FUNCTION(set_logger_window_title)
			ADD_PYTHON_FUNCTION(set_log_level)
			ADD_PYTHON_FUNCTION(log)
			ADD_PYTHON_FUNCTION(log_debug)
			ADD_PYTHON_FUNCTION(log_info)
			ADD_PYTHON_FUNCTION(log_warning)
			ADD_PYTHON_FUNCTION(log_error)

			// app item interface
			ADD_PYTHON_FUNCTION(move_item)
			ADD_PYTHON_FUNCTION(get_managed_column_width)
			ADD_PYTHON_FUNCTION(set_managed_column_width)
			ADD_PYTHON_FUNCTION(get_item_type)
			ADD_PYTHON_FUNCTION(set_item_callback)
			ADD_PYTHON_FUNCTION(set_item_callback_data)
			ADD_PYTHON_FUNCTION(get_value)
			ADD_PYTHON_FUNCTION(set_value)
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
			ADD_PYTHON_FUNCTION(get_item_callback)
			ADD_PYTHON_FUNCTION(get_item_callback_data)
			ADD_PYTHON_FUNCTION(get_item_parent)
			ADD_PYTHON_FUNCTION(delete_item)
			ADD_PYTHON_FUNCTION(does_item_exist)
			ADD_PYTHON_FUNCTION(move_item_down)
			ADD_PYTHON_FUNCTION(move_item_up)
			ADD_PYTHON_FUNCTION(get_windows)
			ADD_PYTHON_FUNCTION(get_all_items)
			ADD_PYTHON_FUNCTION(get_item_children)

			// widget commands
			ADD_PYTHON_FUNCTION(add_logger)
			ADD_PYTHON_FUNCTION(add_next_column)
			ADD_PYTHON_FUNCTION(add_columns)
			ADD_PYTHON_FUNCTION(add_date_picker)
			ADD_PYTHON_FUNCTION(add_time_picker)
			ADD_PYTHON_FUNCTION(add_input_text)
			ADD_PYTHON_FUNCTION(add_input_int)
			ADD_PYTHON_FUNCTION(add_input_int2)
			ADD_PYTHON_FUNCTION(add_input_int3)
			ADD_PYTHON_FUNCTION(add_input_int4)
			ADD_PYTHON_FUNCTION(add_input_float)
			ADD_PYTHON_FUNCTION(add_input_float2)
			ADD_PYTHON_FUNCTION(add_input_float3)
			ADD_PYTHON_FUNCTION(add_input_float4)
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
			ADD_PYTHON_FUNCTION(add_color_button)
			ADD_PYTHON_FUNCTION(add_color_edit3)
			ADD_PYTHON_FUNCTION(add_color_edit4)
			ADD_PYTHON_FUNCTION(add_color_picker3)
			ADD_PYTHON_FUNCTION(add_color_picker4)
			ADD_PYTHON_FUNCTION(add_separator)
			ADD_PYTHON_FUNCTION(add_button)
			ADD_PYTHON_FUNCTION(add_radio_button)
			ADD_PYTHON_FUNCTION(add_checkbox)
			ADD_PYTHON_FUNCTION(add_group)
			ADD_PYTHON_FUNCTION(add_child)
			ADD_PYTHON_FUNCTION(add_tab_bar)
			ADD_PYTHON_FUNCTION(add_tab)
			ADD_PYTHON_FUNCTION(add_tab_button)
			ADD_PYTHON_FUNCTION(add_menu_bar)
			ADD_PYTHON_FUNCTION(add_menu)
			ADD_PYTHON_FUNCTION(add_menu_item)
			ADD_PYTHON_FUNCTION(add_spacing)
			ADD_PYTHON_FUNCTION(add_same_line)
			ADD_PYTHON_FUNCTION(add_tooltip)
			ADD_PYTHON_FUNCTION(add_collapsing_header)
			ADD_PYTHON_FUNCTION(add_dummy)
			ADD_PYTHON_FUNCTION(add_managed_columns)
			ADD_PYTHON_FUNCTION(add_about_window)
			ADD_PYTHON_FUNCTION(add_doc_window)
			ADD_PYTHON_FUNCTION(add_debug_window)
			ADD_PYTHON_FUNCTION(add_style_window)
			ADD_PYTHON_FUNCTION(add_metrics_window)

			// input commands
			ADD_PYTHON_FUNCTION(get_drawing_mouse_pos)
			ADD_PYTHON_FUNCTION(set_mouse_move_callback)
			ADD_PYTHON_FUNCTION(set_mouse_drag_callback)
			ADD_PYTHON_FUNCTION(is_mouse_button_dragging)
			ADD_PYTHON_FUNCTION(is_mouse_button_down)
			ADD_PYTHON_FUNCTION(is_mouse_button_clicked)
			ADD_PYTHON_FUNCTION(is_mouse_button_double_clicked)
			ADD_PYTHON_FUNCTION(is_mouse_button_released)
			ADD_PYTHON_FUNCTION(get_mouse_drag_delta)
			ADD_PYTHON_FUNCTION(set_mouse_wheel_callback)
			ADD_PYTHON_FUNCTION(get_mouse_pos)
			ADD_PYTHON_FUNCTION(get_plot_mouse_pos)
			ADD_PYTHON_FUNCTION(is_key_pressed)
			ADD_PYTHON_FUNCTION(is_key_released)
			ADD_PYTHON_FUNCTION(is_key_down)
			ADD_PYTHON_FUNCTION(set_mouse_click_callback)
			ADD_PYTHON_FUNCTION(set_mouse_down_callback)
			ADD_PYTHON_FUNCTION(set_mouse_double_click_callback)
			ADD_PYTHON_FUNCTION(set_key_down_callback)
			ADD_PYTHON_FUNCTION(set_key_press_callback)
			ADD_PYTHON_FUNCTION(set_key_release_callback)
			ADD_PYTHON_FUNCTION(set_mouse_release_callback)
			ADD_PYTHON_FUNCTION(set_render_callback)
			ADD_PYTHON_FUNCTION(set_resize_callback)

			// Node commands
			ADD_PYTHON_FUNCTION(add_node_editor)
			ADD_PYTHON_FUNCTION(add_node)
			ADD_PYTHON_FUNCTION(add_node_attribute)
			ADD_PYTHON_FUNCTION(add_node_link)
			ADD_PYTHON_FUNCTION(delete_node_link)
			ADD_PYTHON_FUNCTION(get_selected_nodes)
			ADD_PYTHON_FUNCTION(get_selected_links)
			ADD_PYTHON_FUNCTION(get_links)
			ADD_PYTHON_FUNCTION(clear_selected_links)
			ADD_PYTHON_FUNCTION(clear_selected_nodes)

			// Theme commands
			ADD_PYTHON_FUNCTION(set_theme_color)
			ADD_PYTHON_FUNCTION(set_theme_style)
			ADD_PYTHON_FUNCTION(add_additional_font)

			// table commands
			ADD_PYTHON_FUNCTION(add_table)
			ADD_PYTHON_FUNCTION(set_table_data)
			ADD_PYTHON_FUNCTION(get_table_data)
			ADD_PYTHON_FUNCTION(clear_table)
			ADD_PYTHON_FUNCTION(get_table_item)
			ADD_PYTHON_FUNCTION(set_table_item)
			ADD_PYTHON_FUNCTION(get_table_selections)
			ADD_PYTHON_FUNCTION(set_table_selection)
			ADD_PYTHON_FUNCTION(add_column)
			ADD_PYTHON_FUNCTION(insert_column)
			ADD_PYTHON_FUNCTION(delete_column)
			ADD_PYTHON_FUNCTION(add_row)
			ADD_PYTHON_FUNCTION(insert_row)
			ADD_PYTHON_FUNCTION(delete_row)

			// drawing commands
			ADD_PYTHON_FUNCTION(bring_draw_command_forward)
			ADD_PYTHON_FUNCTION(bring_draw_command_to_front)
			ADD_PYTHON_FUNCTION(send_draw_command_back)
			ADD_PYTHON_FUNCTION(send_draw_command_to_back)
			ADD_PYTHON_FUNCTION(modify_draw_command)
			ADD_PYTHON_FUNCTION(get_draw_command)
			ADD_PYTHON_FUNCTION(draw_arrow)
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
			ADD_PYTHON_FUNCTION(delete_draw_command)

			// plot commands
			ADD_PYTHON_FUNCTION(add_drag_point)
			ADD_PYTHON_FUNCTION(delete_drag_point)
			ADD_PYTHON_FUNCTION(add_drag_line)
			ADD_PYTHON_FUNCTION(delete_drag_line)
			ADD_PYTHON_FUNCTION(add_annotation)
			ADD_PYTHON_FUNCTION(delete_annotation)
			ADD_PYTHON_FUNCTION(is_plot_queried)
			ADD_PYTHON_FUNCTION(get_plot_query_area)
			ADD_PYTHON_FUNCTION(clear_plot)
			ADD_PYTHON_FUNCTION(reset_xticks)
			ADD_PYTHON_FUNCTION(reset_yticks)
			ADD_PYTHON_FUNCTION(set_xticks)
			ADD_PYTHON_FUNCTION(set_yticks)
			ADD_PYTHON_FUNCTION(set_plot_xlimits_auto)
			ADD_PYTHON_FUNCTION(set_plot_ylimits_auto)
			ADD_PYTHON_FUNCTION(get_plot_xlimits)
			ADD_PYTHON_FUNCTION(set_plot_xlimits)
			ADD_PYTHON_FUNCTION(set_plot_ylimits)
			ADD_PYTHON_FUNCTION(get_plot_ylimits)
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
			ADD_PYTHON_FUNCTION(add_image_series)
			ADD_PYTHON_FUNCTION(add_stair_series)
			ADD_PYTHON_FUNCTION(add_candle_series)
			ADD_PYTHON_FUNCTION(add_vline_series)
			ADD_PYTHON_FUNCTION(add_hline_series)
			ADD_PYTHON_FUNCTION(delete_series)
			ADD_PYTHON_FUNCTION(add_heat_series)
			ADD_PYTHON_FUNCTION(add_text_point)
			{NULL, NULL, 0, NULL}
		};

		return methods;
	}

	const std::map<std::string, mvPythonParser>& mvModule_Core::GetSubModuleParsers()
	{

		static auto parsers = std::map<std::string, mvPythonParser>();

#ifdef MV_CPP
#else
		if (parsers.empty())
		{
			constexpr_for<1, (int)mvAppItemType::ItemTypeCount, 1>(
				[&](auto i) {
					using item_type = typename mvItemType<i>::type;
					item_type::InsertParser(&parsers);
				});


			mvDrawList::InsertParser(&parsers);
			AddTextureStorageCommands(&parsers);
			AddInputCommands(&parsers);
			AddItemCommands(&parsers);
			AddAppCommands(&parsers);
		}
#endif
		return parsers;
	}

	const std::vector<std::pair<std::string, long>>& mvModule_Core::GetSubModuleConstants()
	{
		static bool First_Run = true;
		static std::vector<std::pair<std::string, long>> ModuleConstants =
		{

			{ "mvGuiStyleVar_Alpha",               0 },
			{ "mvGuiStyleVar_WindowPadding",       1 },
			{ "mvGuiStyleVar_WindowRounding",      2 },
			{ "mvGuiStyleVar_WindowBorderSize",    3 },
			{ "mvGuiStyleVar_WindowMinSize",       4 },
			{ "mvGuiStyleVar_WindowTitleAlign",    5 },
			{ "mvGuiStyleVar_ChildRounding",       6 },
			{ "mvGuiStyleVar_ChildBorderSize",     7 },
			{ "mvGuiStyleVar_PopupRounding",       8 },
			{ "mvGuiStyleVar_PopupBorderSize",     9 },
			{ "mvGuiStyleVar_FramePadding",        10 },
			{ "mvGuiStyleVar_FrameRounding",       11 },
			{ "mvGuiStyleVar_FrameBorderSize",     12 },
			{ "mvGuiStyleVar_ItemSpacing",         13 },
			{ "mvGuiStyleVar_ItemInnerSpacing",    14 },
			{ "mvGuiStyleVar_IndentSpacing",       15 },
			{ "ImGuiStyleVar_CellPadding",		   16 },
			{ "mvGuiStyleVar_ScrollbarSize",       17 },
			{ "mvGuiStyleVar_ScrollbarRounding",   18 },
			{ "mvGuiStyleVar_GrabMinSize",         19 },
			{ "mvGuiStyleVar_GrabRounding",        20 },
			{ "mvGuiStyleVar_TabRounding",         21 },
			{ "mvGuiStyleVar_ButtonTextAlign",     22 },
			{ "mvGuiStyleVar_SelectableTextAlign", 23 },

		};

		if (First_Run)
		{
			mvInput::InsertConstants(ModuleConstants);
			mvPlot::InsertConstants(ModuleConstants);
			mvButton::InsertConstants(ModuleConstants);
			mvLoggerItem::InsertConstants(ModuleConstants);
			mvTextureStorage::InsertConstants(ModuleConstants);

			auto decodeType = [](long encoded_constant, mvAppItemType* type)
			{
				*type = (mvAppItemType)(encoded_constant / 1000);
			};

			constexpr_for<1, (int)mvAppItemType::ItemTypeCount, 1>(
				[&](auto i) {

					using item_type = typename mvItemType<i>::type;

					// color constants
					for (const auto& item : item_type::GetColorConstants())
					{
						ModuleConstants.push_back({ std::get<0>(item), std::get<1>(item) });

						static mvAppItemType type;
						long mvThemeConstant = std::get<1>(item);
						decodeType(mvThemeConstant, &type);
						mvColor color = std::get<2>(item);
						const std::string& name = std::get<0>(item);

						mvThemeManager::GetColors()[type][mvThemeConstant] = color;
						mvThemeManager::GetColorsPtr().push_back({name, mvThemeConstant, &mvThemeManager::GetColors()[type][mvThemeConstant] });

					}

					// style constants
					for (const auto& item : item_type::GetStyleConstants())
					{
						ModuleConstants.push_back({ std::get<0>(item), std::get<1>(item) });

						static mvAppItemType type;
						long mvThemeConstant = std::get<1>(item);
						decodeType(mvThemeConstant, &type);
						float default_val = std::get<2>(item);
						float max_val = std::get<3>(item);
						const std::string& name = std::get<0>(item);

						mvThemeManager::GetStyles()[type][mvThemeConstant] = default_val;
						mvThemeManager::GetStylesPtr().push_back({ name, mvThemeConstant,
							&mvThemeManager::GetStyles()[type][mvThemeConstant] , max_val});

					}

				});
		}

		First_Run = false;
		return ModuleConstants;
	}

	PyMODINIT_FUNC PyInit_core(void)
	{
		static PyModuleDef dearpyguiModule = {
			PyModuleDef_HEAD_INIT, "core", NULL, -1, mvModule_Core::GetMethods(),
			NULL, NULL, NULL, NULL
		};

		PyObject* m;

		m = PyModule_Create(&dearpyguiModule);
		if (m == NULL)
			return NULL;

		const auto& constants = mvModule_Core::GetModuleConstants();

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
}