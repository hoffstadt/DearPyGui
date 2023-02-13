File & Directory Selector
=========================

The file dialog item can be used to select a single file,
multiple files, or a directory. When the user clicks the **Ok** button,
the dialog's callback is run. An optional second callback, to be run 
when the cancel button is clicked, can be provided as a keyword
argument.

When OK is clicked, information is passed through the app_data argument
such as:
* file path
* file name
* current path
* current filter (the file type filter)


The simplest case is as a director picker. Below is the example

**Code**

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    def callback(sender, app_data):
        print('OK was clicked.')
        print("Sender: ", sender)
        print("App Data: ", app_data)

    def cancel_callback(sender, app_data):
        print('Cancel was clicked.')
        print("Sender: ", sender)
        print("App Data: ", app_data)

    dpg.add_file_dialog(
        directory_selector=True, show=False, callback=callback, tag="file_dialog_id",
        cancel_callback=cancel_callback, width=700 ,height=400)

    with dpg.window(label="Tutorial", width=800, height=300):
        dpg.add_button(label="Directory Selector", callback=lambda: dpg.show_item("file_dialog_id"))

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

.. note:: If no file extensions have been added, the selector defaults to directories.

File Extensions
---------------

File extensions are items that are added to the file dialog.
You can even set the color of the file extensions. Below is a simple example:

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    def callback(sender, app_data, user_data):
        print("Sender: ", sender)
        print("App Data: ", app_data)

    with dpg.file_dialog(directory_selector=False, show=False, callback=callback, id="file_dialog_id", width=700 ,height=400):
        dpg.add_file_extension(".*")
        dpg.add_file_extension("", color=(150, 255, 150, 255))
        dpg.add_file_extension("Source files (*.cpp *.h *.hpp){.cpp,.h,.hpp}", color=(0, 255, 255, 255))
        dpg.add_file_extension(".h", color=(255, 0, 255, 255), custom_text="[header]")
        dpg.add_file_extension(".py", color=(0, 255, 0, 255), custom_text="[Python]")

    with dpg.window(label="Tutorial", width=800, height=300):
        dpg.add_button(label="File Selector", callback=lambda: dpg.show_item("file_dialog_id"))

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

Customizing
-----------

File dialogs can be customized with a panel by just adding
items to the file dialog as if it were a regular container. 

This can allow the creation of a pinned menu, favorites, directory tree, and much more.

Below is an example:

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    def callback(sender, app_data):
        print("Sender: ", sender)
        print("App Data: ", app_data)

    with dpg.file_dialog(directory_selector=False, show=False, callback=callback, tag="file_dialog_tag", width=700 ,height=400):
        dpg.add_file_extension(".*")
        dpg.add_file_extension("", color=(150, 255, 150, 255))
        dpg.add_file_extension(".cpp", color=(255, 255, 0, 255))
        dpg.add_file_extension(".h", color=(255, 0, 255, 255))
        dpg.add_file_extension(".py", color=(0, 255, 0, 255))

        with dpg.group(horizontal=True):
            dpg.add_button(label="fancy file dialog")
            dpg.add_button(label="file")
            dpg.add_button(label="dialog")
        dpg.add_date_picker()
        with dpg.child_window(height=100):
            dpg.add_selectable(label="bookmark 1")
            dpg.add_selectable(label="bookmark 2")
            dpg.add_selectable(label="bookmark 3")

    with dpg.window(label="Tutorial", width=800, height=300):
        dpg.add_button(label="File Selector", callback=lambda: dpg.show_item("file_dialog_tag"))

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

Selecting Multiple Files
------------------------

You can select multiple files by setting the *file_count* keyword
**Must use Ctrl + click to select multiple files**
**Must use Shift + click to select multiple files**

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    def callback(sender, app_data):
        print("Sender: ", sender)
        print("App Data: ", app_data)

    with dpg.file_dialog(directory_selector=False, show=False, callback=callback, file_count=3, tag="file_dialog_tag", width=700 ,height=400):
        dpg.add_file_extension("", color=(255, 150, 150, 255))
        dpg.add_file_extension(".*")
        dpg.add_file_extension(".cpp", color=(255, 255, 0, 255))
        dpg.add_file_extension(".h", color=(255, 0, 255, 255))
        dpg.add_file_extension(".py", color=(0, 255, 0, 255))

        dpg.add_button(label="fancy file dialog")
        with dpg.child_window(width=100):
            dpg.add_selectable(label="bookmark 1")
            dpg.add_selectable(label="bookmark 2")
            dpg.add_selectable(label="bookmark 3")

    with dpg.window(label="Tutorial", width=800, height=300):
        dpg.add_button(label="File Selector", callback=lambda: dpg.show_item("file_dialog_tag"))

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()
