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
    import dearpygui.logger as dpg_logger

    logger = dpg_logger.mvLogger()

    dpg.show_documentation()
    dpg.show_style_editor()
    dpg.show_debug()
    dpg.show_about()
    dpg.show_metrics()
    dpg.show_font_manager()
    dpg.show_item_registry()

    dpg.start_dearpygui()

**Results**

.. image:: https://raw.githubusercontent.com/hoffstadt/DearPyGui/assets/examples_wiki_0.8.x/builtin_Dev_tools.PNG

More Resources
--------------

 * :doc:`../extra/showcase`
 * :doc:`../extra/video-tutorials`

