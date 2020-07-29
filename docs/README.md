# Dear PyGui
<sub>(This library is available under a free and permissive license)</sub>

[![build status](https://github.com/RaylockLLC/DearPyGui/workflows/Build/badge.svg)](https://github.com/RaylockLLC/DearPyGui/actions?workflow=Build)
[![build status](https://github.com/RaylockLLC/DearPyGui/workflows/WindowsDev/badge.svg)](https://github.com/RaylockLLC/DearPyGui/actions?workflow=WindowsDev)
[![build status](https://github.com/RaylockLLC/DearPyGui/workflows/MacOSDev/badge.svg)](https://github.com/RaylockLLC/DearPyGui/actions?workflow=MacOSDev)
![Python Version](https://img.shields.io/pypi/pyversions/dearpygui)
![PYPI](https://img.shields.io/pypi/v/dearpygui)



**Dear PyGui** is a **simple to use (but powerful) Python GUI framework**. _DearPyGui_ provides a wrapping of **DearImGui** that simulates a traditional retained mode GUI, as opposed to _Dear ImGui_'s immediate mode paradigm.

**Dear PyGui is fundamentally different than other Python GUI frameworks**. Under the hood, _DearPyGui_ 
uses the immediate mode paradigm allowing for extremely dynamic interfaces. Similar to PyQt _DearPyGui_ 
does not use native widgets, but instead draws the widgets using your computer's GPU.

In the same manner Dear ImGui provides a simple way to create tools for game developers, _DearPyGui_ provides a 
simple way for python developers to **create quick and powerful GUIs for scripts**.

| [Installing](#installing) - [Usage](#usage) - [Features](#some-features) - [Building](#building)|
:----------------------------------------------------------: |
| [Upcoming changes](#upcoming-changes) - [Gallery](#gallery) - [License](#license) - [Current Dependencies](#current-dependencies)|

### Installing

---


Ensure you have the newest version of Python 3.8 (3.8.5 at the moment).
 ```
 pip install dearpygui
 or
 pip3 install dearpygui
 ```

### Usage

 ---
 
**DearPyGui is an all included GUI framework.** A variety of examples demonstrating usage
can be found in the Examples folder [/Examples](https://github.com/RaylockLLC/DearPyGui/tree/master/Examples).
Documentation can be found [here](https://raylockllc.github.io/DearPyGui/) or from within the library by 
using the **show_documentation** command.

Using _DearPyGui_ is a simple as creating a python script like the one below:

Code:
```Python
from dearpygui.dearpygui import *

add_text("Hello world")
add_button("Save", callback="save_callback")
add_input_text("string")
add_slider_float("float")

def save_callback(sender, data):
    print("Save Clicked")

start_dearpygui()

```
Result:
<BR>![BasicUsageExample](https://github.com/RaylockLLC/DearPyGui/blob/assets/BasicUsageExample1.PNG?raw=true)
 

### Some Features

---

#### Themes
_DearPyGui_ currently includes 10 themes (a full custom theming system will be available in **v0.2**):
<BR>![Themes](https://github.com/RaylockLLC/DearPyGui/blob/assets/Themes.PNG?raw=true)

#### Plotting/Graphing
_DearPyGui_ includes a plotting API built using ImPlot.
<BR>![BasicUsageExample](https://github.com/RaylockLLC/DearPyGui/blob/assets/PlottingExample1.PNG?raw=true)
  
#### Drawings
_DearPyGui_ includes a drawing API to create custom drawings, plot, and even 2D games.
<BR>![BasicUsageExample](https://github.com/RaylockLLC/DearPyGui/blob/assets/DrawingExample1.PNG?raw=true)

#### Logging
_DearPyGui_ includes a logging API.
<BR>![BasicUsageExample](https://github.com/RaylockLLC/DearPyGui/blob/assets/LoggingExample1.PNG?raw=true)

#### Built-in Documentation
Using the ```show_documentation()``` command opens the built-in documentation window which displays all _DearPyGui_ commands and constants, along with information on their usage.
<BR>![BasicUsageExample](https://github.com/RaylockLLC/DearPyGui/blob/assets/DocumentationExample.PNG?raw=true)
  
### Current Dependencies

---

- [Dear ImGui](https://github.com/ocornut/imgui)
- [CPython](https://github.com/python/cpython)
- [ImPlot](https://github.com/epezent/implot)
- [CLI11](https://github.com/CLIUtils/CLI11)


### Building

---

#### Windows

##### Build Requirements

- [_Visual Studio 2019_ (windows)](https://visualstudio.microsoft.com/vs/) (with Python workflow selected)
- [_git_](https://git-scm.com/)

1. From within a local directory, enter the following bash commands:
```
git clone --recursive https://github.com/RaylockLLC/DearPyGui
```
3. Open the _Scripts_ folder and run the _BuildPythonForWindows.bat_ file.
4. In Visual Studio, use _Open a local folder_ to open _DearPyGui_ folder.
5. Switch to the release configuration and run _Build All_.

### Upcoming Changes

---

Some upcoming features:
- An object oriented interface
- Additional Plot types (bar graph, pie chart, etc.)
- Support for window docking and multiple viewports.
- More complete theme and style system.
- 3D widget.
- Linux support.
- Custom Widget API.

### Gallery

---

<BR>![StandardWindowsExample1](https://github.com/RaylockLLC/DearPyGui/blob/assets/WidgetsExample1.PNG?raw=true)
<BR>![StandardWindowsExample1](https://github.com/RaylockLLC/DearPyGui/blob/assets/StandardWindowsExample1.PNG?raw=true)

### License

---

DearPyGui is licensed under the [MIT License](https://github.com/RaylockLLC/DearPyGui/blob/master/LICENSE).
