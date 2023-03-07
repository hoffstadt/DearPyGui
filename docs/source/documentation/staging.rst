Staging
=======

The staging system is used to create items or item hierarchies where
the parent/root is to be decided at a later time. 

Staged items are not submitted for rendering.

Staged items will show up in the item registry.

Items can be moved out of staging by using :py:func:`move_item <dearpygui.dearpygui.move_item>`.



The most basic example can be found below:

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    def stage_items():
        with dpg.stage(tag="stage1"):
            dpg.add_text("hello, i was added from a stage", tag="text_tag")

    def present_stage_items():
        dpg.move_item("text_tag", parent="main_win")

    with dpg.window(label="Tutorial", tag="main_win"):
        dpg.add_button(label="stage items", callback=stage_items)
        dpg.add_button(label="present stages items", callback=present_stage_items)

    dpg.show_item_registry()

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

Prefered way to "unstage" items is 
using :py:func:`unstage <dearpygui.dearpygui.unstage>`.
This will place the items as if they were newly created items according to
the standard rules of :doc:`../documentation/container-stack`.

Also using the unstage command will automatically clean up the stage container.

Using :py:func:`push_container_stack <dearpygui.dearpygui.push_container_stack>` and 
:py:func:`pop_container_stack <dearpygui.dearpygui.pop_container_stack>` is recomended here as it
provides better performance when unstaging. 

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    def stage_items():
        with dpg.stage(tag="stage1"):
            dpg.add_text("hello, i was added from a stage")
            dpg.add_text("hello, i was added from a stage")
            dpg.add_text("hello, i was added from a stage")
            dpg.add_text("hello, i was added from a stage")
            dpg.add_text("hello, i was added from a stage")

    def present_stage_items():
        dpg.push_container_stack("main_win")
        dpg.unstage("stage1")
        dpg.pop_container_stack()

    with dpg.window(label="Tutorial", tag="main_win", height=400, width=400):
        dpg.add_button(label="stage items", callback=stage_items)
        dpg.add_button(label="present stages items", callback=present_stage_items)

    dpg.show_item_registry()

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

Wrapping Items with Classes
---------------------------

The most obvious benefit of this system is for advanced users
who are wrapping DPG items into classes. Rather than having
to duplicate the various configurable options as members of the class
(to avoid making calls to :py:func:`configure_item <dearpygui.dearpygui.configure_item>`
or :py:func:`get_item_configuration <dearpygui.dearpygui.get_item_configuration>`
before the item is actually created), you can create and stage the item in the
constructor of the wrapping class!

Below are 2 examples:

**Example 1**

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()


    class Button:

        def __init__(self, label):
            with dpg.stage() as self._staging_container_id:
                self._id = dpg.add_button(label=label)

        def set_callback(self, callback):
            dpg.set_item_callback(self._id, callback)

        def get_label(self):
            return dpg.get_item_label(self._id)

        def submit(self, parent):
            dpg.push_container_stack(parent)
            dpg.unstage(self._staging_container_id)
            dpg.pop_container_stack()

    my_button = Button("Press me")
    my_button.set_callback(lambda: print("I've been pressed!"))

    print(my_button.get_label())

    with dpg.window(label="Tutorial", tag="main_win"):
        dpg.add_text("hello world")

    my_button.submit("main_win")

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

**Example 2**

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()


    class Window:

        def __init__(self, label):
            self._children = []
            with dpg.stage() as stage:
                self.id = dpg.add_window(label=label)
            self.stage = stage

        def add_child(self, child):
            dpg.move_item(child.id, parent=self.id)

        def submit(self):
            dpg.unstage(self.stage)


    class Button:

        def __init__(self, label):
            with dpg.stage():
                self.id = dpg.add_button(label=label)

        def set_callback(self, callback):
            dpg.set_item_callback(self.id, callback)


    my_button = Button("Press me")
    my_button.set_callback(lambda: print("I've been pressed!"))

    my_window = Window("Tutorial")

    my_window.add_child(my_button)

    my_window.submit()

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()
