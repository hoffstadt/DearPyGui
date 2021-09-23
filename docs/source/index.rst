
Dear PyGui's Documentation
==========================

.. meta::
   :description lang=en: Automate building, versioning, and hosting of your
    technical documentation continuously on Read the Docs.

About DPG
---------

Dear PyGui is an easy-to-use, dynamic, GPU-Accelerated, cross-platform graphical
user interface toolkit(GUI) for Python. It is not a traditional wrapping of `Dear ImGui`_, but instead, more of a "built with" Dear ImGui.

Features include traditional GUI elements such as buttons, radio buttons, 
menus and various methods to create a functional layout.

Additionally, DPG has an incredible assortment of dynamic
plots, tables, drawings, logging, debugger, and multiple resource viewers.

DPG is well suited for creating simple user interfaces
as well as to developing complex and
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

First Steps
-----------

If you're ready to start using Dear PyGui visit
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

    /api-reference/render-loop
    /api-reference/viewport
    /api-reference/primary-window
    /api-reference/io-handlers-state
    /api-reference/item-creation
    /api-reference/item-creation-runtime
    /api-reference/item-id-system
    /api-reference/item-configuration
    /api-reference/item-callbacks
    /api-reference/item-value
    /api-reference/container-context-manager
    /api-reference/container-slots
    /api-reference/container-stack
    /api-reference/drawing-api
    /api-reference/file-directory-selector
    /api-reference/filter-set
    /api-reference/fonts
    /api-reference/init-files
    /api-reference/menus
    /api-reference/node-editor
    /api-reference/plots
    /api-reference/popups
    /api-reference/simple-plots
    /api-reference/staging
    /api-reference/table-api-08
    /api-reference/textures
    /api-reference/themes
    /api-reference/tooltips
    
.. toctree::
    :maxdepth: 2
    :hidden:
    :caption: API Reference

    reference/dearpygui.rst

More
----

Showcase:
    :doc: `extra/showcase`

Video Tutorials:
    :doc: `extra/video-tutorials`

.. toctree::
    :maxdepth: 2
    :numbered:
    :hidden:
    :caption: More

    /extra/showcase
    /extra/video-tutorials
    /extra/glossary
