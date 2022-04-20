
Dear PyGui's Documentation
==========================

.. meta::
   :description lang=en: Automate building, versioning, and hosting of your
    technical documentation continuously on Read the Docs.

About DPG
---------

Dear PyGui is an easy-to-use, dynamic, GPU-Accelerated, cross-platform graphical
user interface toolkit(GUI) for Python. It is "built with" `Dear ImGui`_.

Features include traditional GUI elements such as buttons, radio buttons, 
menus and various methods to create a functional layout.

Additionally, DPG has an incredible assortment of dynamic
plots, tables, drawings, debugger, and multiple resource viewers.

DPG is well suited for creating simple user interfaces
as well as developing complex and
demanding graphical interfaces.

DPG offers a solid framework for developing scientific,
engineering, gaming, data science and other applications that require fast and interactive interfaces.

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

Quick Start
-----------

If you're ready to start using DPG visit
the :doc:`tutorials/first-steps` in tutorials.

The :doc:`Tutorials <tutorials/first-steps>` will provide a great overview
and links to each topic in the API Reference for more detailed reading.

However, use the API reference
for the most detailed documentation on any specific topic.

.. toctree::
    :maxdepth: 2
    :numbered:
    :hidden:
    :caption: Tutorials

    /tutorials/first-steps
    /tutorials/dpg-structure
    /tutorials/item-usage
    /tutorials/tips-resources

Documentation
-------------

Live Demo:
    A mostly complete showcase of DPG can be found by running the
    :py:func:`show_demo <dearpygui.demo.show_demo>` command in the :py:mod:`dearpygui.demo` module.

Internal Documentation:
    Run :py:func:`show_documentation <dearpygui.dearpygui.show_documentation>`

API Reference Guide:
    :doc:`Online API Reference <reference/dearpygui>`

.. toctree::
    :maxdepth: 2
    :hidden:
    :caption: Documentation

    /documentation/render-loop
    /documentation/viewport
    /documentation/primary-window
    /documentation/io-handlers-state
    /documentation/item-creation
    /documentation/item-id-system
    /documentation/item-configuration
    /documentation/item-callbacks
    /documentation/item-value
    /documentation/container-context-manager
    /documentation/container-slots
    /documentation/container-stack
    /documentation/drawing-api
    /documentation/file-directory-selector
    /documentation/filter-set
    /documentation/fonts
    /documentation/init-files
    /documentation/menus
    /documentation/node-editor
    /documentation/plots
    /documentation/popups
    /documentation/simple-plots
    /documentation/staging
    /documentation/tables
    /documentation/textures
    /documentation/themes
    /documentation/tooltips
    
.. toctree::
    :maxdepth: 2
    :hidden:
    :caption: API Reference

    reference/dearpygui.rst

More
----

 * :doc:`../extra/showcase`
 * :doc:`../extra/video-tutorials`

.. toctree::
    :maxdepth: 2
    :hidden:
    :caption: More
    
    /extra/showcase
    /extra/video-tutorials
    /extra/glossary
