| [Basics](#basics) - [Creating](#creating-init-files) - [Loading](#loading-init-files) - [Basic Usage](#basic-usage) |
|----|
||


## Basics
**Init** files are used to preserve the following data between application sessions:
* window positions
* window sizes
* window collapse state
* window docking
* table column widths
* table column ordering
* table column visible state
* table column sorting state

### Notes
* Init files use the ID of the window. Make sure the ID does not change between sessions by generating the ID beforehand.

## Creating init files
There are two procedures for creating **init** files:
1. Use `save_init_file(...)` while your application is running.
2. Temporarily add `set_init_file(...)` to your application before starting _Dear PyGui_.

### Note
* windows and tables can individually opt out of having their settings saved with the `no_saved_settings` keyword.

## Loading init files
There are two procedures for loading **init** files:
1. Use `load_init_file(...)` before starting _Dear PyGui_.
2. Use `set_init_file(...)` before starting _Dear PyGui_.

### Note
* Procedure 2 will overwrite the **init** file.

## Basic Usage
Below is an example of using **init** files to preserve settings between sessions:
```python
import dearpygui.dearpygui as dpg

dpg.set_init_file()  # default file is 'dpg.ini'

with dpg.window(label="tutorial"):
    dpg.add_button(label="Press me")

dpg.start_dearpygui()
```