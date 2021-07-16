# 0.8.x CHANGELOG

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

## VERSION 0.8.x

### New
* added `set_init_file(...)` #1073
* added `load_init_file(...)` #1073
* added `save_init_file(...)` #1073
* added `no_saved_settings` keyword to window
* added `no_saved_settings` keyword to table

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
