First Steps
===========

.. meta::
   :description lang=en: The starting point for the tutorial set.

Tutorials will give a broad overview and working knowledge of DPG. Tutorials do not cover every detail
so refer to the documentation on each topic to learn more.

Installing
----------

Python 3.6 (64 bit) or above is required.

.. code-block::

    pip install dearpygui

First Run
---------

Confirm the pip install by running the code block below.

**Code:**

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()
    dpg.create_viewport(title='Custom Title', width=600, height=300)

    with dpg.window(label="Example Window"):
        dpg.add_text("Hello, world")
        dpg.add_button(label="Save")
        dpg.add_input_text(label="string", default_value="Quick brown fox")
        dpg.add_slider_float(label="float", default_value=0.273, max_value=1)

    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

**Result:**

.. image:: https://github.com/hoffstadt/DearPyGui/blob/assets/readme/first_app.gif

Demo
----
DPG has a complete built-in demo/showcase. It is a good idea to look into this demo. 
The code for this can be found in the repo in the `demo.py`_ file

.. _demo.py: https://github.com/hoffstadt/DearPyGui/blob/master/DearPyGui/dearpygui/demo.py

**Code:**

.. code-block:: python

    import dearpygui.dearpygui as dpg
    import dearpygui.demo as demo

    dpg.create_context()
    dpg.create_viewport(title='Custom Title', width=600, height=600)

    demo.show_demo()

    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

**Result:**

.. image:: https://github.com/hoffstadt/DearPyGui/blob/assets/readme/demo.gif

.. note:: 
    The main script must always:

    - Create the context :py:func:`create_context <dearpygui.dearpygui.create_context>`
    - Create the viewport :py:func:`create_viewport <dearpygui.dearpygui.create_viewport>`
    - Setup dearpygui :py:func:`setup_dearpygui <dearpygui.dearpygui.setup_dearpygui>`
    - Show the viewport :py:func:`show_viewport <dearpygui.dearpygui.show_viewport>`
    - Start dearpygui :py:func:`start_dearpygui <dearpygui.dearpygui.start_dearpygui>`
    - Clean up the context :py:func:`destroy_context <dearpygui.dearpygui.destroy_context>`




Internal documentation
----------------------

The API documentation is built into the library and can be called with show_documentation(). This brings up the documentation tool for browsing and searching Dear PyGui functions. Obviously, Dear PyGui supports autocomplete for your IDE as well.

**Code:**

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()
    dpg.create_viewport(title='Custom Title', width=800, height=600)
    
    dpg.show_documentation()

    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

**Result:**

.. image:: https://github.com/hoffstadt/DearPyGui/blob/assets/readthedocs/internal_documentation.png

Style editor and runtime metrics
--------------------------------

The built-in style editor allows you to experiment with all style options at runtime to find the exact colors, padding, rounding and other style settings for your application. The built-in runtime metrics provide real-time information about your app performance. These tools can be activated by adding show_style_editor() and show_metrics() to your code. The following screen capture shows both tools in action at the same time, where changing the style settings immediately impact the overall application.

**Code:**

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()
    dpg.create_viewport(title='Custom Title', width=800, height=600)
    
    dpg.show_style_editor()
    dpg.show_metrics()

    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

**Result:**

.. image:: https://github.com/hoffstadt/DearPyGui/blob/assets/readthedocs/style_editor_metrics.gif
