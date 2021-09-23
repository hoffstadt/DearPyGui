What & Why
==========

.. meta::
   :description lang=en: Information about Dear PyGui and the developers.

What is DPG
-----------

Dear PyGui is a simple to use (but powerful) Python GUI framework.
Dear PyGui provides a wrapping of Dear ImGui that simulates a traditional
retained mode GUI, as opposed to Dear ImGui's immediate mode paradigm.

Under the hood, Dear PyGui uses the immediate mode paradigm allowing for extremely
dynamic interfaces. Similar to PyQt, Dear PyGui does not use native widgets
but instead draws the widgets using your computer's graphics
card (using Directx11, Metal, and Vulkan rendering APIs).

In the same manner Dear ImGui provides a simple way to create tools
for game developers, Dear PyGui provides a simple way for python
developers to create quick and powerful GUIs for scripts.

Why use DPG
-----------

When compared with other Python GUI libraries Dear PyGui is unique with:

* GPU Rendering
* Simple built-in Asynchronous function support
* Complete theme and style control
* Built-in developer tools: logging, theme inspection, resource inspection, runtime metrics
* 70+ widgets with hundreds of widget combinations
* Detailed documentation, examples and unparalleled support