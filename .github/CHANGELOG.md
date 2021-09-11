# 1.x.y CHANGELOG

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

## VERSION 1.0.0

### Breaking Changes
* moved logger and themes to `Dear PyGui Extensions`
* changed `id` keyword to `tag`
* item: removed table_next_column (deprecated)
* table rows now required
* removed `add_same_line(...)`
* removed `default_font` keyword arg from `add_font(...)`, use `bind_font(...)` now
* removed `default_theme` keyword arg from `add_theme(...)`, use `bind_theme(...)` now
* removed `viewport` keyword arg from `setup_dearpygui(...)`
* removed `viewport` keyword arg from `show_viewport(...)`
* removed `bind_item_disabled_theme(...)`
* removed `bind_item_type_disabled_theme(...)`
* removed `bind_item_type_theme(...)`
* user must create, setup, show viewport before starting dpg:
  * "create_viewport()->setup_dearpygui()->show_viewport()->start_dearpygui()"
* `add_theme_color(...)` and `add_theme_style(...)` must known belong to a `theme_component`

### New
* module:  added `experimental`
* item:    added `add_table_cell(...)`
* item:    added `add_spacer(...)`
* item:    added `add_theme_component(...)`
* command: added `show_item_debug(...)`
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
* keyword: added `xoffset` to `group(...)`
* keyword: added `id` to `popup(...)`
* keyword: added `clipper` to `add_table(...)`
* added deprecation system
* callbacks will now send alias through the `sender` argument if alias is used.
* columns can now be programmatically hidden
* table "value" is now a string, which acts as a filter using the row filter keys

### Deprecated
* command: `staging_container(...)` is deprecated. Use `stage(...)`
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
* command: `add_spacing(...)` is deprecated. Use 'add_spacer(...)`
* command: `add_dummy(...)` is deprecated. Use 'add_spacer(...)`

### Fixes
* fixed `get_item_configuration(...)` memory leak #1179
* fixed issue to allow `source` to be alias #1181
* fixed window info not registering as container #1188
* fixed min/max clamping issue with input widgets #1229
