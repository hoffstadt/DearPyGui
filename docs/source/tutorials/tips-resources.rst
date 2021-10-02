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

More Resources
--------------

 * :doc:`../extra/showcase`
 * :doc:`../extra/video-tutorials`

