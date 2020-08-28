![Logo](https://github.com/hoffstadt/DearPyGui/blob/assets/TempLogo.png?raw=true)

<sub>(This library is available under a free and permissive license)</sub>

![Python Version](https://img.shields.io/pypi/pyversions/dearpygui)
[![PYPI](https://img.shields.io/pypi/v/dearpygui)](https://pypi.org/project/dearpygui/)
[![Downloads](https://pepy.tech/badge/dearpygui)](https://pepy.tech/project/dearpygui)

[![build status](https://github.com/hoffstadt/DearPyGui/workflows/Stable%20Build/badge.svg)](https://github.com/hoffstadt/DearPyGui/actions?workflow=Stable%20Build)
[![build status](https://github.com/hoffstadt/DearPyGui/workflows/Master%20Build/badge.svg)](https://github.com/hoffstadt/DearPyGui/actions?workflow=Master%20Build)

[![build status](https://github.com/hoffstadt/DearPyGui/workflows/WindowsDev/badge.svg)](https://github.com/hoffstadt/DearPyGui/actions?workflow=WindowsDev)
[![build status](https://github.com/hoffstadt/DearPyGui/workflows/LinuxDev/badge.svg)](https://github.com/hoffstadt/DearPyGui/actions?workflow=LinuxDev)
[![build status](https://github.com/hoffstadt/DearPyGui/workflows/MacOSDev/badge.svg)](https://github.com/hoffstadt/DearPyGui/actions?workflow=MacOSDev)

**Dear PyGui** is a **simple to use (but powerful) Python GUI framework**. _DearPyGui_ provides a wrapping of **[DearImGui](https://github.com/ocornut/imgui)** that provides a hybrid of a traditional retained mode GUI and _Dear ImGui_'s immediate mode paradigm.

**Dear PyGui is fundamentally different than other Python GUI frameworks**. Under the hood, _DearPyGui_ 
uses the immediate mode paradigm allowing for extremely dynamic interfaces. _DearPyGui_ 
does not use native widgets, but instead draws the widgets using your computer's GPU. _DearPyGui_ is currently supported on
the following platforms:

| Platform | Rendering API |
|----------|---------------|
| **Windows 10** | _DirectX 11_ |
| **macOs** | _Metal_ |
| **Linux** | _OpenGL3_ |

In the same manner Dear ImGui provides a simple way to create tools for game developers, _DearPyGui_ provides a 
simple way for python developers to **create quick and powerful GUIs for scripts**.

| [Installing](#installing) - [Usage](#usage) - [Features](#some-features) - [Support](#support) - [Building](#building)|
:----------------------------------------------------------: |
| [Upcoming changes](#upcoming-changes) - [Gallery](#gallery) - [Credits](#credits) - [License](#license)|

### Installing

---

Ensure you have at least Python 3.7.
 ```
 pip install dearpygui
 or
 pip3 install dearpygui
 ```

### Usage

 ---
 
**DearPyGui is an all included GUI framework.** A variety of examples demonstrating usage
can be found in the Examples folder [/Examples](https://github.com/hoffstadt/DearPyGui/tree/master/Examples). To get started checkout the [Tutorials](https://hoffstadt.github.io/DearPyGui/tutorial.html)
and for all other references 
visit the [Documentation](https://hoffstadt.github.io/DearPyGui/) or from within the library by 
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


#### Plotting/Graphing
_DearPyGui_ includes a plotting API ([ImPlot](https://github.com/epezent/implot) Wrapping)

<img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots/controls.gif" width="270"> <img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots/dnd.gif" width="270"> <img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots/log.gif" width="270">

<img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots/bar.gif" width="270"> <img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots/query.gif" width="270"> 
<img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots/views.gif" width="270"> 

<img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots/noise.gif" width="270"> <img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots/stock.gif" width="270"> 
<img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots/multiy.gif" width="270"> 

#### Themes
_DearPyGui_ currently includes 10 themes and you can create your own:
<BR>![Themes](https://github.com/hoffstadt/DearPyGui/blob/assets/linuxthemes.PNG?raw=true)

#### Canvas
_DearPyGui_ includes a drawing API to create custom drawings, plot, and even 2D games.
<BR>![BasicUsageExample](https://github.com/hoffstadt/DearPyGui/blob/assets/canvas.png?raw=true)
 
#### Tools
_DearPyGui_ includes several tools to help developers with _DearPyGui_ app development including built-in documentation, logging, source viewer, and other tools:
<BR>![BasicUsageExample](https://github.com/hoffstadt/DearPyGui/blob/assets/tools.png?raw=true)


### Support
If you are having issues or want to help or ask for ways you can help, here are some places you can go!
- [Github Forum](https://github.com/RaylockLLC/DearPyGui/issues/)
- [Discord Forum](https://discord.gg/tyE7Gu4)
- [Reddit](https://www.reddit.com/r/DearPyGui/)


### Building
The actual builds of _DearPyGui_ are created through github actions however, to develop and test locally, follow the instructions below. 
These are not well maintained at the moment and could use some work.

---

#### Windows 10

##### Build Requirements

- [_Visual Studio 2019_ (windows)](https://visualstudio.microsoft.com/vs/) with the following workloads:
  - Python development
  - Desktop development with C++
  - Game development with C++
- [_git_](https://git-scm.com/)
- Python 3.7 or 3.8

##### Instructions

1. From within a local directory, enter the following bash commands:
```
git clone --recursive https://github.com/RaylockLLC/DearPyGui
```
2. Open the _Scripts_ folder and run the _BuildFor32.bat_ file (or _BuildFor64.bat_ if using 64-bit python).

#### Linux
<sub>Only tested on Ubuntu 20.04</sub>

##### Build Requirements

- cmake
- Python 3.7 or 3.8
- xorg-dev

##### Instructions

1. From within a local directory, enter the following bash commands:
```
git clone --recursive https://github.com/hoffstadt/DearPyGui
```
2. Open the _Scripts_ folder.
3. Open the terminal and type the following commands:
```
chmod +x BuildForLinux.sh
sudo ./BuildForLinux.sh
```

### Upcoming Changes

---

Some upcoming features:
- An object oriented interface
- Additional Plot types (bar graph, pie chart, etc.)
- Support for window docking and multiple viewports.
- 3D widget.
- Custom Widget API.

### Credits

---

Developed by Jonathan Hoffstadt, Preston Cothren, and every direct or indirect contributor.

[Omar Cornut](http://www.miracleworld.net/) for all his incredible work on [Dear ImGui](https://github.com/ocornut/imgui).

[Evan Pezent](http://evanpezent.com/) for all his work on [ImPlot](https://github.com/epezent/implot)

Aiekick for his work on the [ImGuiFileDialog](https://github.com/aiekick)

### Gallery

---

<BR>![StandardWindowsExample1](https://github.com/hoffstadt/DearPyGui/blob/assets/WidgetsExample1.PNG?raw=true)
<BR>![StandardWindowsExample1](https://github.com/hoffstadt/DearPyGui/blob/assets/StandardWindowsExample1.PNG?raw=true)

### License

---

DearPyGui is licensed under the [MIT License](https://github.com/hoffstadt/DearPyGui/blob/master/LICENSE).
