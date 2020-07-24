# Dear PyGui
[![build status](https://github.com/RaylockLLC/DearPyGui/workflows/Windows/badge.svg)](https://github.com/RaylockLLC/DearPyGui/actions?workflow=Windows)
![Python Version](https://img.shields.io/pypi/pyversions/dearpygui)
![PYPI](https://img.shields.io/pypi/v/dearpygui)

<sub>(This library is available under a free and permissive license)</sub>

**Dear PyGui** is a **simple to use (but powerful) Python GUI framework**. _DearPyGui_ provides a wrapping of **DearImGui** that simulates a traditional retained mode GUI, as opposed to _Dear ImGui_'s immediate mode paradigm.

**Dear PyGui is fundamentally different than other Python GUI frameworks**. Under the hood, _DearPyGui_ uses the immediate mode paradigm allowing for extremely dynamic interfaces. Similar to PyQt _DearPyGui_ does not use native widgets, but instead draws the widgets using your computer's graphics card(using _Directx11_, _Metal_, and _Vulkan_ rendering APIs).

In the same manner Dear ImGui provides a simple way to create tools for game developers, _DearPyGui_ provides a simple way for python developers to **create quick and powerful GUIs for scripts**. It's well suited for **beginners** too!

| [Installing](#installing) - [Usage](#usage) - [Features](#some-features) - [Building](#building)|
:----------------------------------------------------------: |
| [Upcoming changes](#upcoming-changes) - [Gallery](#gallery) - [License](#license) - [Current Dependencies](#current-dependencies) - [Prebuilt Binaries](#dearsandbox-prebuilt-binaries)|

### Installing

 ---
 
 ```
 pip install dearpygui
 or
 pip3 install dearpygui
 ```

### Usage

 ---
 
**DearPyGui is an all included GUI framework.** Using _DearPyGui_ is a simple as creating a python script like the one below:

Code:
```Python
from dearpygui.dearpygui import *

add_text("Hello world")
add_button("Save", callback="SaveCallback")
add_input_text("string")
add_slider_float("float")

def SaveCallback(sender, data):
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
  
#### Built-in IDE (DearSandbox Only)
If you choose not to use your own IDE, we have provided a basic built-in IDE for creating and testing apps. To use the built-in IDE just call the _DearSandbox_ executable with the **--editor** flag: ```DearSandbox.exe --editor```
<BR>![BasicUsageExample](https://github.com/RaylockLLC/DearPyGui/blob/assets/IDEExample1.PNG?raw=true)
  
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
- MacOs and Linux support.
- Custom Widget API.

### DearSandbox Prebuilt Binaries

---

_DearSandbox_ is a program that allows developers using _DearPyGui_ to package there app and distrubute it to users. _DearSandbox_ includes an embedded version of Python and everything else needed to run _DearPyGui_ without any dependencies.
#### Windows
- Download [Latest Release](https://github.com/RaylockLLC/DearPyGui/releases/download/v0.1-alpha12/Sandbox.zip)
- Download _Sandbox.zip_ and unzip anywhere. 
- Run _DearSandbox.exe_.
- Replace _App.py_ with your module.

### Gallery

---

<BR>![StandardWindowsExample1](https://github.com/RaylockLLC/DearPyGui/blob/assets/WidgetsExample1.PNG?raw=true)
<BR>![StandardWindowsExample1](https://github.com/RaylockLLC/DearPyGui/blob/assets/StandardWindowsExample1.PNG?raw=true)

### License

---

DearPyGui is licensed under the [MIT License](https://github.com/RaylockLLC/DearPyGui/blob/master/LICENSE).
