| [Basics](#basics) - [Staging Container](staging-container) - [Wrapping Items with Classes](#wrapping-items-with-classes) |
| ---- |
||

## Basics
The staging system is used to create items or item hierarchies where the parent/root is to be decided at a later time. Staged items are not submitted for rendering. They can later be "unstaged", at which time a parent is known. The most basic example can be found below:
```python
import dearpygui.dearpygui as dpg

dpg.set_staging_mode(True)

dpg.add_button(label="Press me", id="button_id")

# proof the item has been created
print(dpg.get_item_configuration("button_id"))

dpg.set_staging_mode(False)

with dpg.window(label="Tutorial"):
    dpg.unstage_items(["button_id"])

dpg.start_dearpygui()
```

When staging mode is enabled, items will still attempt to [deduce a parent](https://github.com/hoffstadt/DearPyGui/wiki/Parent-Deduction-&-Container-Stack) according to the regular procedure however if a parent can not be deduced, it will stage the item.

Using `unstage_items(...)` will attempt to place the item as if you created it there, following the standard rules of [parent deduction](https://github.com/hoffstadt/DearPyGui/wiki/Parent-Deduction-&-Container-Stack). You can also unstage an item by using `move_item(...)`

## Staging Container
_Dear PyGui_ provides a special container called a **Staging Container**. This container can only be created when staging mode is enabled and has the special property that when "unstaged" it unpacks its children then deletes itself. Below is a basic example:
```python
import dearpygui.dearpygui as dpg

dpg.set_staging_mode(True)

with dpg.staging_container(id="staging_container_id"):
    dpg.add_button(label="Button 1")
    dpg.add_button(label="Button 2")
    dpg.add_button(label="Button 3")
    dpg.add_button(label="Button 4")
    dpg.add_button(label="Button 5")

dpg.set_staging_mode(False)

with dpg.window(label="Tutorial"):
    dpg.unstage_items(["staging_container_id"])

dpg.start_dearpygui()
```

## Wrapping Items with Classes
The most obvious benefit of this system is for advanced users who are wrapping _Dear PyGui_ items into classes. Rather than having to duplicate the various configurable options as members of the class (to avoid making calls to `configure_item(...)` or `get_item_configuration(...)` before the item is actually created), you can create and stage the item in the constructor of the wrapping class! Below is are 2 simple examples:

### Example 1
```python
import dearpygui.dearpygui as dpg

class Button:

    def __init__(self, label):
        dpg.set_staging_mode(True)
        with dpg.staging_container() as self._staging_container_id:
            self._id = dpg.add_button(label=label)
        dpg.set_staging_mode(False)

    def set_callback(self, callback):
        dpg.set_item_callback(self._id, callback)

    def get_label(self):
        return dpg.get_item_label(self._id)

    def submit(self):
        dpg.unstage_items([self._staging_container_id])


my_button = Button("Press me")
my_button.set_callback(lambda: print("I've been pressed!"))

print(my_button.get_label())

with dpg.window(label="Tutorial"):
    my_button.submit()

dpg.start_dearpygui()

```

### Example 2
```python
import dearpygui.dearpygui as dpg

class Window:

    def __init__(self, label):
        self._children = []
        dpg.set_staging_mode(True)
        self._id = dpg.add_window(label=label)
        dpg.set_staging_mode(False)

    def add_child(self, child):
        dpg.move_item(child._id, parent=self._id)

    def submit(self):
        dpg.unstage_items([self._id])

class Button:

    def __init__(self, label):
        dpg.set_staging_mode(True)
        self._id = dpg.add_button(label=label)
        dpg.set_staging_mode(False)

    def set_callback(self, callback):
        dpg.set_item_callback(self._id, callback)


my_button = Button("Press me")
my_button.set_callback(lambda: print("I've been pressed!"))

my_window = Window("Tutorial")

my_window.add_child(my_button)

my_window.submit()

dpg.start_dearpygui()
```