# 0.9.x CHANGELOG

This document holds the user-facing changelog that we also use in release notes.
We generally fold multiple commits pertaining to the same topic as a single entry.

* RELEASE NOTES:                  https://github.com/hoffstadt/DearPyGui/releases
* REPORT ISSUES, ASK QUESTIONS:   https://github.com/hoffstadt/DearPyGui/issues
* COMMITS HISTORY:                https://github.com/hoffstadt/DearPyGui/commits/master
* WIKI                            https://github.com/hoffstadt/DearPyGui/wiki

### When to update?

- Keeping your copy of Dear PyGui updated regularly is recommended.
- It is generally safe to sync to the latest commit in master
  The library is fairly stable and regressions tends to be fixed fast when reported.

### How to update?

- pip or pip3 install dearpygui --upgrade
- Read the `Breaking Changes` section (here in the changelog).
- Please report any issue!

## VERSION 0.9.0

### Breaking Changes
* changed `id` keyword to `tag`
* item: renamed `staging_container` to `stage`
* item: removed table_next_column
* table rows now required
* removed `default_font` keyword arg from `add_font(...)`, use `bind_font(...)` now
* removed `default_theme` keyword arg from `add_theme(...)`, use `bind_theme(...)` now
* removed `viewport` keyword arg from `setup_dearpygui(...)`
* removed `viewport` keyword arg from `show_viewport(...)`
* user must create, setup, show viewport before starting dpg:
  * "create_viewport()->setup_dearpygui()->show_viewport()->start_dearpygui()"

### New
* module:  added `experimental`
* item:    added `add_table_cell(...)`
* command: added `bind_theme(...)`
* command: added `highlight_table_column(...)`
* command: added `unhighlight_table_column(...)`
* command: added `set_table_row_color(...)`
* command: added `unset_table_row_color(...)`
* command: added `highlight_table_cell(...)`
* command: added `unhighlight_table_cell(...)`
* command: added `highlight_table_row(...)`
* command: added `unhighlight_table_row(...)`
* command: added `is_table_column_highlighted(...)`
* command: added `is_table_row_highlighted(...)`
* command: added `is_table_cell_highlighted(...)`
* command: added context manager for `add_plot_axis(...)`
* command: added `configure_app(...)`
* command: added `get_app_configuration(...)`
* command: added `add_item_set(...)`
* command: added `add_template_registry(...)`
* command: added `bind_template_registry(...)`
* keyword: added `id` to `popup(...)`
* keyword: added `clipper` to `add_table(...)`
* added deprecation system
* callbacks will now send alias through the `sender` argument if alias is used.
* columns can now be programmatically hidden
* table "value" is now a string, which acts as a filter using the row filter keys

### Deprecated
* command: `enable_docking(...)` is deprecated. Use `configure_app(docking=True, docking_space=dock_space)`
* command: `get_dearpygui_version()` is deprecated. Use `get_app_configuration()['version']`.
* command: `init_file(...)` is deprecated. Use `configure_app(init_file=file)`.
* command: `load_init_file` is deprecated. Use `configure_app(init_file=file, load_init_file=True)`.
* command: `is_viewport_created(...)` is deprecated. Use `is_viewport_ok()`.
* command: `setup_viewport(...)` is deprecated. Use `create_viewport()->setup_dearpygui()->show_viewport()`.
* command: `set_item_theme(...)` is deprecated. Use `bind_item_theme()`.
* command: `set_item_type_disabled_theme(...)` is deprecated. Use `bind_item_type_disabled_theme()`.
* command: `set_item_theme(...)` is deprecated. Use `bind_item_theme()`.
* command: `set_item_type_theme(...)` is deprecated. Use `bind_item_type_theme()`.
* command: `set_item_font(...)` is deprecated. Use `bind_item_font()`.
* command: `add_activated_handler(...)` is deprecated. Use `add_activated_handler()`.
* command: `add_active_handler(...)` is deprecated. Use `add_item_active_handler()`.
* command: `add_clicked_handler(...)` is deprecated. Use `add_item_clicked_handler()`.
* command: `add_deactivated_after_edit_handler(...)` is deprecated. Use `add_item_deactivated_after_edit_handler()`.
* command: `add_deactivated_handler(...)` is deprecated. Use `add_item_deactivated_handler()`.
* command: `add_edited_handler(...)` is deprecated. Use `add_item_edited_handler()`.
* command: `add_focus_handler(...)` is deprecated. Use `add_item_focus_handler()`.
* command: `add_hover_handler(...)` is deprecated. Use `add_item_hover_handler()`.
* command: `add_resize_handler(...)` is deprecated. Use `add_item_resize_handler()`.
* command: `add_toggled_open_handler(...)` is deprecated. Use `add_item_toggled_open_handler()`.
* command: `add_visible_handler(...)` is deprecated. Use `add_item_visible_handler()`.
* command: `set_colormap(...)` is deprecated. Use `bind_colormap()`.
* command: `reset_default_theme(...)` is deprecated. Use `bind_theme(0)`.
* command: `set_staging_mode(...)` is deprecated. No longer needed.

### Fixes
* fixed `get_item_configuration(...)` memory leak #1179
* fixed issue to allow `source` to be alias #1181
* fixed window info not registering as container #1188

## VERSION 0.8.64

### Fixes
* fixed performance issues related to having a large number of items (tables especially)

## VERSION 0.8.62

### Breaking Changes
* `pmin` & `pmax` were backwards in `draw_rectangle(...)`. You need to reverse them. Related to #1163

### New
* command: added `get_item_alias(...)`
* command: added `set_item_alias(...)`
* command: added `add_alias(...)`
* command: added `remove_alias(...)`
* command: added `does_alias_exist(...)`
* command: added `get_alias_id(...)`
* command: added `get_aliases(...)`
* command: added `get_item_registry_configuration(...)`
* command: added `configure_item_registry(...)`
* feature: id can now be a string, but must be unique!

## VERSION 0.8.61

### Fixes
* fixed setting default theme values
* fixed infinite on_close callbackss #1149


## VERSION 0.8.58

### Breaking Changes
* `add_colormap_scale(...)` keyword `colormap` now corresponds to `mvColorMap` widget

### New
* widget: added `add_colormap(...)` #1069
* widget: added `add_colormap_registry(...)` #1069
* widget: added `add_colormap_button(...)`
* widget: added `add_colormap_slider(...)`
* command: added `sample_colormap(...)`
* command: added `get_colormap_color(...)`
* keyword: added `mulicolor` to `draw_rectangle(...)`
* keyword: added `color_upper_left` to `draw_rectangle(...)`
* keyword: added `color_upper_right` to `draw_rectangle(...)`
* keyword: added `color_bottom_left` to `draw_rectangle(...)`
* keyword: added `color_bottom_right` to `draw_rectangle(...)`
* constant: added `mvPlotColormap_Twilight`
* constant: added `mvPlotColormap_RdBu`
* constant: added `mvPlotColormap_BrBG`
* constant: added `mvPlotColormap_PiYG`
* constant: added `mvPlotColormap_Spectral`
* constant: added `mvPlotColormap_Greys`

### Fixes
* fixed `get_item_configuration(...)` for window max_size

### Other
* added colormap section to demo under "widgets"
* simplied drawing API demo

## VERSION 0.8.57

### New
* `set_value(...)` now works for *theme_color* #1136
* `set_value(...)` now works for *theme_style* #1136

## VERSION 0.8.56

### New
* added `use_internal_label` keyword to all widgets, related to #1135

### Fixes
* fixed histogram misspelling issue related to #1132
* fixed adding font ranges over 0x10000 #1092

## VERSION 0.8.54

### Fixes
* fixed viewport issue for windows #1126

## VERSION 0.8.53

### Breaking Changes
* removed `set_viewport_border(...)`
* removed `set_viewport_caption(...)`
* removed `set_viewport_overlapped(...)`
* removed `set_viewport_maximized_box(...)`
* removed `set_viewport_minimized_box(...)`
* removed `get_viewport_border(...)`
* removed `is_viewport_caption_on(...)`
* removed `is_viewport_overlapped_on(...)`
* removed `is_viewport_maximized_box_on(...)`
* removed `is_viewport_minimized_box_on(...)`
* removed `maximized_box`, `minimized_box`, `caption`, `overlapped` keyword from `create_viewport(...)`

### New
* added `set_viewport_decorated(...)`
* added `is_viewport_decorated(...)`
* all viewport configuration works at runtime now
* labels are now empty by default #1113
* added file extension filter group to demo & wiki

### Fixes
* fixed `num_items` keyword in `add_listbox()` #1124
* fixed tooltip parent id type hint
* fixed `custom_text` keyword for file extensions

## VERSION 0.8.52

### New
* added ability to "show/hide" widget handlers
* added `is_item_left_clicked(...)` #1115
* added `is_item_right_clicked(...)` #1115
* added `is_item_middle_clicked(...)` #1115

### Fixes
* on_close callback fixed for modal windows #1112
* fixed issue with windows viewport caption/border #1114
* fixed issue with windows for visible and focus handlers #1116
* `is_item_clicked(...)` now returns true for left, right, middle clicks #1115

## VERSION 0.8.50

### New
* added warning for viewport commands when viewport hasn't been created

### Fixes
* fixed raw texture memory leak #1111
* added `draw_ellipse` as acceptable child for draw list

## VERSION 0.8.45

### New
* deleting a node now deletes associated links #1082
* added `attr_1` and `attr_2` to `get_item_configuration(...)` for node links #1106, #1107

### Fixes
* fixed `get_selected_links(...)` #1107
* fixed `delete_item` not cleaning up item cache

## VERSION 0.8.44

### New
* added `clear_value` keyword to mvBuffer (may rename)

### Fixes
* fixed plot draw item scaling #1094
* fixed draw_text font not updating #1100
* fixed drag & drop issue on plot axis #1093

## VERSION 0.8.43

### New
* added `pan_button` keyword to plots #1083
* added `pan_mod` keyword to plots #1083
* added `fit_button` keyword to plots #1083
* added `context_menu_button` keyword to plots #1083
* added `box_select_button` keyword to plots #1083
* added `box_select_mod` keyword to plots #1083
* added `box_select_cancel_button` keyword to plots #1083
* added `query_button` keyword to plots #1083
* added `query_mod` keyword to plots #1083
* added `query_toggle_mod` keyword to plots #1083
* added `horizontal_mod` keyword to plots #1083
* added `vertical_mod` keyword to plots #1083

## VERSION 0.8.42

### Fixes
* fixed "double disabling" issue with text input #1084
* fixed taskbar issues #1085

## VERSION 0.8.40

### New
* added `set_init_file(...)` #1073
* added `load_init_file(...)` #1073
* added `save_init_file(...)` #1073
* added `no_saved_settings` keyword to window
* added `no_saved_settings` keyword to table

### Fixes
* fixed `set_viewport_pos(...)` #1079
* fixed active window issue with plots and node editors #1080

## VERSION 0.8.39

### Fixes
* added check for 0 column tables #1071
* fixed type hint for `delay` search
* added `user_data` support for table
* fixed plot series theming issue when not using labels #1064
* temporary fix to #1071

## VERSION 0.8.38

### Fixes
* fixed viewport resize handler deadlock issue #1070

## VERSION 0.8.32

### New
* added mounting for the logger #1047

### Fixes
* fixed several issues related to multiple node editors #1039, #1063
* fixed `get_viewport_clear_color(...)` issue

## VERSION 0.8.31

### Breaking Changes
* renamed `dearpygui.core` to `dearpygui._dearpygui`

### New
* added `reset_default_theme(...)`

### Fixes
* fixed `set_viewport_min_height(...)` issue #1059
* fixed data picker return value issue #1058

## VERSION 0.8.26

### New
* updated implot to v0.11
* added `themes` module with imgui dark and light themes #1045

### Fixes
* theme editor correctly reflects current default theme
* fixed plot legend context menus (broken in 0.8.23)

## VERSION 0.8.23

### New
* added `gamma` and `gamma_scale_factor` keywords to `load_image(...)`
* added subplots widget
* updated implot to v0.10
* reorganized plot demo

### Fixes
* fixed incorrect gamma correction for image loading #1043

## VERSION 0.8.21

### Breaking Changes
* user_data is no longer called if it is a callable

### New
* for most basic widgets, app_data callback argument is now the widget's value

### Fixes
* fixed min/max not being honored in input widgets issue #1037

## VERSION 0.8.20

### New
* draw layer now available for plots

### Fixes
* fixed focus window for first frame issue #1035


## VERSION 0.8.19

### New
* drawing items on plots are now in plot space #1011
* node editor now has width/height

### Fixes
* fixed node editor not respecting width/height #818
* fixed no_focus_on_appear window flag #1026
* fixed widget handlers issue for nodes #1025
* fixed multiple connections from same attribute issue #1022
* fixed `per-node theming` (popping was not occuring)
* removed `get_links(...)` #1021

### Fixes
* fixed `move_item_down` issue #1006

## VERSION 0.8.16

### New
* added `slot` keyword to `delete_item` #1015

### Fixes
* fixed `move_item_down` issue #1006
* fixed documentation errors #1007
* fixed per-node theming issue #1012

## VERSION 0.8.14

### Breaking Changes
* Removed update_bounds keyword from plot series. Use fit_axis_data(...)

### New
* added `add_viewport_menu_bar(...)`
* added `split_frame(...)`
* added `get_frame_count(...)`
* added `user_data` to every app item
* node editor callback now sends through user_data
* added `category` keyword to node attributes

### Fixes
* fixed primary window hiding other window menubars on start

## VERSION 0.8.13

### Fixes
* fixed `add_spacing` "count" issue #988
* fixed `get_item_pos`
* fixed `get_available_content_region`
* fixed `get_item_rect_size`
* fixed `get_item_rect_min`
* fixed `get_item_rect_max`
* metrics plots autoscale now #992

## VERSION 0.8.12

### Breaking Changes
* removed `file` keyword from `add_static_texture`. See wiki.

### New
* added `get_item_slot(...)`
* added `load_image(...)`

### Fixes
* fixed primary window menubar not showing
* fixed configure item not working (caused #971)
* fixed `disable_item` #972
* fixed plot themes not working
* fixed unwarned crash caused by incorrect raw_texture data
* fixed `get_item_font(...)`
* fixed `delete_item(...)` for widget handlers
* fixed Mac OS directory change isses #967, #697, #974
* fixed draw item fill color issues #983
* fixed seg fault when font file not located #974
* fixed seg fault when image file not located #963
