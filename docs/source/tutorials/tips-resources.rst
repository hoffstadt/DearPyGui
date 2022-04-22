Tips & More Resources
=====================

.. meta::
   :description lang=en: Extra features not required but very useful when developing with dpg.

Developer Tools
---------------

DPG includes several tools which can help develop and debug applications.

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



Style editor
------------
The built-in style editor allows you to experiment with all style options at runtime to find the exact colors, padding, rounding and other style settings for your application. You can use the sliders to change the settings, which are applied to all items in your app, so you can immediately see what effect the changes have on your GUI.

**Code:**

.. code-block:: python

   import dearpygui.dearpygui as dpg

   dpg.create_context()
   dpg.create_viewport()
   dpg.setup_dearpygui()

   dpg.show_style_editor()

   dpg.show_viewport()
   dpg.start_dearpygui()
   dpg.destroy_context()

**Result:**

.. image:: https://raw.githubusercontent.com/hoffstadt/DearPyGui/assets/readthedocs/style_editor.png



Item registry
-------------
The item registry shows all items of a running application in a hierarchical structure. For each item, a number of details, such as its tag ID, are shown.

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

.. image:: https://raw.githubusercontent.com/hoffstadt/DearPyGui/assets/readthedocs/item_registry.gif


Font manager
------------
The font manager shows all loaded fonts and their appropriate sizes. It allows you to inspect all characters, or glyphs, that are loaded with each font file. 

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
  :width: 600
  :alt: Font manager
  

Runtime metrics
--------------------------------
Runtime metrics show the performance of your app in real-time. Here is it shown in conjunction with the built-in style editor.

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

.. image:: https://raw.githubusercontent.com/hoffstadt/DearPyGui/assets/readthedocs/style_editor_metrics.gif
  :alt: Style editor and runtime metrics tools

More Resources
--------------

 * :doc:`../extra/showcase`
 * :doc:`../extra/video-tutorials`

