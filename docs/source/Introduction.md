| [What?](#what-is-dear-pygui) - [Why?](#why-use-dear-pygui) - [Installing](#installing) - [First Run](#first-run) - [0.6.x](https://github.com/hoffstadt/DearPyGui_06/wiki/Getting-Started)|
|----|
||

## What is Dear PyGui

Dear PyGui is a simple to use (but powerful) Python GUI framework. Dear PyGui provides a wrapping of Dear ImGui that simulates a traditional retained mode GUI, as opposed to Dear ImGui's immediate mode paradigm.

Under the hood, Dear PyGui uses the immediate mode paradigm allowing for extremely dynamic interfaces. Similar to PyQt, Dear PyGui does not use native widgets but instead draws the widgets using your computer's graphics card (using Directx11, Metal, and Vulkan rendering APIs).

In the same manner Dear ImGui provides a simple way to create tools for game developers, Dear PyGui provides a simple way for python developers to create quick and powerful GUIs for scripts.

## Why use Dear PyGui

When compared with other Python GUI libraries Dear PyGui is unique with:

* GPU Rendering
* Simple built-in Asynchronous function support
* Complete theme and style control
* Simple built-in logging window
* 70+ widgets with hundreds of widget combinations
* Detailed documentation, examples and unparalleled support

## Installing

Ensure you have at least Python 3.6 (64 bit) installed.

```pip install dearpygui```

## First Run

A variety of examples demonstrating usage can be found in the Examples folder on github. Documentation can be found here and also from within the library by using the `show_documentation()` command.

Using Dear PyGui is a simple as creating a python script like the one below

The main script must always conclude with `start_dearpygui()`.

Code:

```python
import dearpygui.dearpygui as dpg

def save_callback():
    print("Save Clicked")

with dpg.window(label="Example Window"):
    dpg.add_text("Hello, world")
    dpg.add_button(label="Save", callback=save_callback)
    dpg.add_input_text(label="string", default_value="Quick brown fox")
    dpg.add_slider_float(label="float", default_value=0.273, max_value=1)
dpg.start_dearpygui()
```

Result:

![Dear PyGui Example](https://raw.githubusercontent.com/hoffstadt/DearPyGui/assets/BasicUsageExample1.PNG)
