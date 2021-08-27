| [Basics](#basics) - [File Extension](#file-extension) - [Customizing](#customizing) - [0.6.x](https://github.com/hoffstadt/DearPyGui_06/wiki/File-and-Directory-Selector) - [Selecting Multiple Files](#selecting-multiple-files) - [Bookmarks](#bookmarks) |
|----|
||

## Basics
The file dialog widget can be used to select a single file, multiple files, or a directory. When the user clicks the **Ok** button, the dialog's callback is ran. Information is passed through the app_data argument. The simplest case is as a director picker. Below is the example:

```python
import dearpygui.dearpygui as dpg

def callback(sender, app_data):
    print("Sender: ", sender)
    print("App Data: ", app_data)

dpg.add_file_dialog(directory_selector=True, show=False, callback=callback, id="file_dialog_id")

with dpg.window(label="Tutorial", width=800, height=300):
    dpg.add_button(label="Directory Selector", callback=lambda: dpg.show_item("file_dialog_id"))

dpg.start_dearpygui()
```
### Notes:
* If no file extensions have been added, the selector defaults to directories.

## File Extensions
File extensions are app items that are added to the file dialog. You can even set the color of the file extensions. Below is a simple example:
```python
import dearpygui.dearpygui as dpg

def callback(sender, app_data, user_data):
    print("Sender: ", sender)
    print("App Data: ", app_data)

with dpg.file_dialog(directory_selector=False, show=False, callback=callback, id="file_dialog_id"):
    dpg.add_file_extension(".*", color=(255, 255, 255, 255))
    dpg.add_file_extension("Source files (*.cpp *.h *.hpp){.cpp,.h,.hpp}", color=(0, 255, 255, 255))
    dpg.add_file_extension(".cpp", color=(255, 255, 0, 255))
    dpg.add_file_extension(".h", color=(255, 0, 255, 255), custom_text="header")
    dpg.add_file_extension("Python(.py){.py}", color=(0, 255, 0, 255))

with dpg.window(label="Tutorial", width=800, height=300):
    dpg.add_button(label="File Selector", callback=lambda: dpg.show_item("file_dialog_id"))

dpg.start_dearpygui()
```
## Customizing
File dialogs can be customized with a panel by just adding app items to the file dialog as if it were a regular container. Below is an example:
```python
import dearpygui.dearpygui as dpg

def callback(sender, app_data):
    print("Sender: ", sender)
    print("App Data: ", app_data)

with dpg.file_dialog(directory_selector=False, show=False, callback=callback, id="file_dialog_id"):
    dpg.add_file_extension(".*", color=(255, 255, 255, 255))
    dpg.add_file_extension(".cpp", color=(255, 255, 0, 255))
    dpg.add_file_extension(".h", color=(255, 0, 255, 255))
    dpg.add_file_extension(".py", color=(0, 255, 0, 255))

    dpg.add_button(label="fancy file dialog")
    with dpg.child():
        dpg.add_selectable(label="bookmark 1")
        dpg.add_selectable(label="bookmark 2")
        dpg.add_selectable(label="bookmark 3")

with dpg.window(label="Tutorial", width=800, height=300):
    dpg.add_button(label="File Selector", callback=lambda: dpg.show_item("file_dialog_id"))

dpg.start_dearpygui()
```

## Selecting Multiple Files
You can select multiple files by setting the `file_count` keyword:
```python
import dearpygui.dearpygui as dpg

def callback(sender, app_data):
    print("Sender: ", sender)
    print("App Data: ", app_data)

with dpg.file_dialog(directory_selector=False, show=False, callback=callback, file_count=3, id="file_dialog_id"):
    dpg.add_file_extension(".*", color=(255, 255, 255, 255))
    dpg.add_file_extension(".cpp", color=(255, 255, 0, 255))
    dpg.add_file_extension(".h", color=(255, 0, 255, 255))
    dpg.add_file_extension(".py", color=(0, 255, 0, 255))

    dpg.add_button(label="fancy file dialog")
    with dpg.child():
        dpg.add_selectable(label="bookmark 1")
        dpg.add_selectable(label="bookmark 2")
        dpg.add_selectable(label="bookmark 3")

with dpg.window(label="Tutorial", width=800, height=300):
    dpg.add_button(label="File Selector", callback=lambda: dpg.show_item("file_dialog_id"))

dpg.start_dearpygui()
```

## Bookmarks
Not ready yet.