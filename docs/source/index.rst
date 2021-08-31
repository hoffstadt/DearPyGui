
Dear PyGui's Documentation
==========================

.. meta::
   :description lang=en: Automate building, versioning, and hosting of your
    technical documentation continuously on Read the Docs.

About DPG
---------

Dear PyGui is an easy-to-use, dynamic, GPU-Accelerated, cross-platform graphical
user interface (GUI) framework for Python. At the core it is considered an extended
wrapping of `Dear ImGui`_.

Features include traditional GUI elements to display text, images and various controls,
such as buttons, radio buttons, and menus and various methods to create a functional and
beautiful layout. Additionally, it offers incredibly dynamic
charts, tables, drawings and tools for application development,
such as built-in documentation, logging, and debugger.

As a GUI toolkit, it is well suited for creating simple user interfaces
to wrap basic command line interfaces as well as to developing complex and
demanding graphical interfaces. It offers a solid framework for developing science,
engineering, games, data science and other applications that require fast and interactive interfaces.

.. _Dear ImGui: https://github.com/ocornut/imgui

.. toctree::
   :maxdepth: 2
   :hidden:
   :caption: About DPG

   /about/what-why
   /about/repo-info

.. asdfasd
    Indices and tables
    ==================
    * :ref:`genindex`
    * :ref:`modindex`
    * :ref:`search`

First Steps
-----------

If you're ready to start using Dear PyGui visit the :doc:`first-run`

The Tutorials will provide a great overview and stepping stone into the library. However use the API reference
for the most detailed documentation on any specific topic.

.. toctree::
   :maxdepth: 2
   :numbered:
   :hidden:
   :caption: Tutorials

   /tutorials/first-steps

Documentation
-------------

* Live Demo: A mostly complete showcase of DPG can be found by running the
    :py:func:`show_demo <dearpygui.demo.show_demo>` command in the :py:mod:`dearpygui.demo` module.
* Internal Documentation: Run
    :py:func:`show_documentation <dearpygui.dearpygui.show_documentation>` view the reference guide.

:doc:`Online API Reference <introduction>`

.. toctree::
   :maxdepth: 2
   :hidden:
   :caption: Documentation

   /api-reference/render-loop