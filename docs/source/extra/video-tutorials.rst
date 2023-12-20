Video Tutorials
===============

The following video tutorials explain various aspects of Dear PyGui,
which are outlined per video below. The first section contains all
videos by the core developers. The second section lists a great
video by the community.

View all `video tutorials on Dear PyGui`_ by the core developers on YouTube.

.. _video tutorials on Dear PyGui:
    https://www.youtube.com/playlist?list=PLcqt7R3y3elGh7zoj1-oyygzTh9j81hKV

`Introduction to Dear PyGui <https://youtu.be/kkRXLrG5oMA>`_
------------------------------------------------------------

Dear PyGui is an extended Python wrapper around Dear ImGUI.
Dear ImGUI is an Immediate Mode GUI (IMGUI) for real-time applications

While Dear ImGui uses the fast Immediate Mode in the background,
it simulates a traditional retained mode GUI to the Python developer,
which makes it easy to work with.

Requirements for installation: Python 3.6 and up (64-bit only)

IDE: Setting up PyCharm, only pip install DPG is required

How to run the built-in demo.

Changing the default font type (OTF or TTF font)

Creating a basic window

Using a primary window so that the window matches the area of the viewport


`Basics of callbacks <https://youtu.be/5ci4oIzGXdI>`_
-------------------------------------------------------

Callbacks are functions that are run when any action
is taken on a certain widget.

If a widget supports it, there will be a keyword called
callback and that can be any callable.

When using a callback, the keyword should not end with
round brackets, e.g. (). The correct way to use a callback
is: add_button("Press me", callback=func_name). The function
func_name is called without ().

The callback always transmits two *variables* to the callback
function, e.g. sender and data.

Sender is the name of the widget. Data is None by default,
but additional data can be sent using the following: callback_data='extra data'.

The tutorial shows the use of a callback for a float slider.

Throughout the tutorial, the use of the built-in logger
and documentation is demonstrated.

The callback can be changed during runtime using set_item_callback.

`ID system and debug tool <https://youtu.be/zcGWJjGSn-w>`_
----------------------------------------------------------

The first argument of a widget is the ID.

The label defaults to the ID if no label is provided.

Widgets need unique IDs, but can share the same label.
There are two ways to accomplish this.

Method 1: add_input_float('Float1', label='float1')
and add_input_float('Float2', label='float1')

Method 2: add_input_float('Float1##1') and add_input_float('Float1##2')

ID's are used to retrieve data from widgets.

Start the debug tool by typing show_debug()

The debug tool lists all available commands under tab Commands

You can execute code at runtime using the debug tool.

Track the float values by log_debug(get_value('Float1##1'))

`Parent stack system <https://youtu.be/CVoArPhOLx4>`_
------------------------------------------------------

The parent stack is a collection of all containers in a GUI.

A window is a root item, meaning that it can't have a parent
and doesn't need to look at the parent stack. A window is also a
container. Because it is a container, a window gets added to the parent stack.

When an item is not a root item, it requires a parent. Every tab bar
is added to the parent stack and to a container. A tab bar is a
container itself as well. A tab is a child of tab bar, but it is also a container.

When adding a second item of a parent, it is necessary to remove
the first item from the parent stack, e.g. pop it, so that the
second item becomes part of the containing parent and not its sibling.

The end() command in the core module pops an item off of the parent stack.

A checkbox or (radio) button is part of a container,
but not a container itself.

The simple module adds the context managers (e.g. 'with window' ).
The *with* statements of the context managers automate the application
of the end() statement, making the code easier to read.

`Value Storage System <https://youtu.be/b7gm0f5vYcc>`_
-------------------------------------------------------

In many GUI's the widget's value is stored inside the widget.

In Dear PyGui, a key-value pair for each widget is stored in
the value storage system. A key-value pair tracks the type of the
value and the value itself. A widget's value can be retrieved and
changed through the widget (by the user) and by the program.

Every widget has a keyword source, which by default is equal to
the widget's name. If you specify the source, the widget will use that
key instead to look up and change values in the value storage system.
This allows several widgets to manipulate a single value.

If multiple widgets refer to the same keyword, the type and
size have to be the same.

Pre-add a value with add_value if you are using multiple
widgets of different types or sizes on a single key-value pair.

A code example is given to demonstrate the value storage
system and its types and sizes.

`Widget basics <https://youtu.be/tqETQz2As9g>`_
------------------------------------------------

This tutorial shows how to use a number of widget types.
Widget types include button, checkbox, label_text, input_int,
drag_int, radio_button, combo, listbox and progress_bar widgets.

The use of the callback keyword of a widget is shown. For example,
add_button('Press me', callback=callback_function).

The callback_function is called whenever that button
is pressed. The callback always sends two arguments to the
callback_function: sender and data. Sender is the name of the
widget. The 'data' argument is often empty unless the widget has
data to send or it is specified in the code. Nonetheless, the
argument 'data' is always included.

The use of a number of widget specific keywords are discussed.

It is demonstrated how a progress bar widget can be controlled
via a drag_int slider using set_value(...) and configure_item(...)

Many widgets have multi-component versions as well.

More complex use of widgets and multi-component
widgets will be shown in future videos.

`Tab bar, tabs, and tab button basics <https://youtu.be/-NXf98oj70c>`_
-----------------------------------------------------------------------

Create a tab bar with the context manager from the simple module, e.g. with tab_bar('tb1') --> with tab('t1') --> add_button('b1').

You can add a callback to a tab_bar using with tab_bar('tb1', callback=callback).

You can add a button to a tab_bar using add_tab_button('+').

Tabs in a tab bar can be made reorderable by using the keyword reorderable=True on the tab_bar.

`Simple Plot & Tooltip <https://youtu.be/G1l-rtMUlsI>`_
---------------------------------------------------------


:doc:`../documentation/simple-plots` is for plotting simple data.
This is not to be confused with the more powerful and complex :doc:`../documentation/plots`.

Create a basic histogram using add_simple_plot(“Plot 1”, value=[1, 4.3, 8, 9, 3], histogram=True).
There are several keywords to customise the plot.

add_text(“Hover me”, tip=”A simple tooltip”). This simple tooltip
is only for text. The :doc:`../documentation/tooltips` is more powerful.

The tooltip widget is a container, i.e.
context manager, just like ‘with window’ and ‘with group’.
The widget basically acts as another window, so that it can contain
any other widget, such as a graph. The example in the video shows how
to embed a simple plot in a tooltip in two lines of code.

Note that the user cannot interact with the tooltip widget.

`Popups <https://www.youtube.com/watch?v=paClQ6SewX8>`_
-------------------------------------------------------

:doc:`../documentation/popups` require a parent.
That may change in future versions of Dear PyGui.

A popup is a container, so it has a context manager (with popup:).

Popup is the only widget where the name is not the first argument.

By default, popups are set on the right-click. To change to
left-click, add the keyword mousebutton=mvMouseButtonLeft.

Popups are a container and can contain any other widget, i.e. plots.

The modal keyword greys everything else out to draw attention to the popup.

To close the modal popup, it is necessary to add a button
with a callback close_popup("popup1").

`Smart tables <https://youtu.be/qL_zjinXTJk>`_
-----------------------------------------------

This is an elaborate tutorial on creating a smart, interactive table.

The table is created using managed_columns.

The widgets used in the table are add_text,
add_input_text and add_input_float

After creating a working example, the code is
refactored into a SmartTable class with header, row and get_cell_data methods.

A widget's label can be hidden by using ## at the beginning
of a label's name, e.g. add_input_text('##input_text_1')
where input_text_1 is not shown in the GUI.

Using add_separator() to change the horizontal spacing of the widgets.

Using the built-in Dear PyGui debugger and logger for solving an coding issue.

Community Videos
----------------

`Creating a complete Python app with Dear PyGui <https://youtu.be/2RocXKPPx4o>`_

Learn how to create a fully-functional Python app step by step!
In this project, we will build a graphic user interface with the
brand new Dear PyGui library! We will connect this interface to a
Simple SMS Spam Filter, which we've built together in a previous project.
We will learn how to display images, text, user input, buttons, and separators,
as well as hiding widgets and "click" event callback functions.