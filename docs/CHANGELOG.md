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

## VERSION 0.8.14

### New
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
