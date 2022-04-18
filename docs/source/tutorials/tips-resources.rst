Tips & More Resources
=====================

.. meta::
   :description lang=en: Extra features not required but very useful when developing with dpg.

Developer Tools
---------------

DPG contains several tools which can help debug applications.

**Code:**

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    dpg.show_documentation()
    dpg.show_style_editor()
    dpg.show_debug()
    dpg.show_about()
    dpg.show_metrics()
    dpg.show_font_manager()
    dpg.show_item_registry()

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

**Results**

.. image:: https://raw.githubusercontent.com/hoffstadt/DearPyGui/assets/examples_wiki_0.8.x/builtin_Dev_tools.PNG

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

Item registry
-------------

**Code:**

.. code-block:: python

   import dearpygui.dearpygui as dpg

   dpg.create_context()
   dpg.create_viewport()
   dpg.setup_dearpygui()

   dpg.show_item_registry()

   dpg.show_viewport()
   dpg.start_dearpygui()
   dpg.destroy_context()

**Result:**

.. image:: https://github.com/hoffstadt/DearPyGui/blob/assets/readthedocs/item_registry.gif?raw=true

Font manager
------------

**Code:**

.. code-block:: python

   import dearpygui.dearpygui as dpg

   dpg.create_context()
   dpg.create_viewport()
   dpg.setup_dearpygui()

   dpg.show_font_manager()

   dpg.show_viewport()
   dpg.start_dearpygui()
   dpg.destroy_context()

**Result:**

.. image:: https://raw.githubusercontent.com/hoffstadt/DearPyGui/assets/readthedocs/font_manager.png

More Resources
--------------

 * :doc:`../extra/showcase`
 * :doc:`../extra/video-tutorials`

