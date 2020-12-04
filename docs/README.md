<a id="user-content-dear-pygui" class="anchor" aria-hidden="true" href="#dear-pygui">
 
<p align="center">
  <img src="https://github.com/hoffstadt/DearPyGui/blob/assets/newlogo.jpg?raw=true" alt="Dear PyGui">
</p>

</a>

<sub>(This library is available under a free and permissive license)</sub>

![Python Version](https://img.shields.io/pypi/pyversions/dearpygui)
[![PYPI](https://img.shields.io/pypi/v/dearpygui)](https://pypi.org/project/dearpygui/)
[![Downloads](https://pepy.tech/badge/dearpygui)](https://pepy.tech/project/dearpygui)
[![Sponsors](https://img.shields.io/badge/sponsors-12-brightgreen)](https://img.shields.io/badge/sponsors-13-brightgreen)

[![build status](https://github.com/hoffstadt/DearPyGui/workflows/Embedded%20Build/badge.svg)](https://github.com/hoffstadt/DearPyGui/actions?workflow=Embedded%20Build)
[![Build status](https://ci.appveyor.com/api/projects/status/eypfn2lyuwfv8mel?svg=true)](https://ci.appveyor.com/project/hoffstadt/dearpygui)

**Dear PyGui** is a **simple to use (but powerful) Python GUI framework**. _Dear PyGui_ provides a wrapping of **[Dear ImGui](https://github.com/ocornut/imgui)** which simulates a traditional retained mode GUI (as opposed to _Dear ImGui_'s immediate mode paradigm).

<BR>![Themes](https://github.com/hoffstadt/DearPyGui/blob/assets/linuxthemes.PNG?raw=true)

**Dear PyGui is fundamentally different than other Python GUI frameworks**. Under the hood, _Dear PyGui_ 
uses the immediate mode paradigm and your computer's GPU to facilitate extremely dynamic interfaces. _Dear PyGui_ is currently supported on
the following platforms:

| Platform | Rendering API |
|----------|---------------|
| **Windows 10** | _DirectX 11_ |
| **macOs** | _Metal_ |
| **Linux** | _OpenGL 3_ |

In the same manner Dear ImGui provides a simple way to create tools for game developers, _Dear PyGui_ provides a 
simple way for python developers to **create quick and powerful GUIs for scripts**.

| [Installing](#installation) - [Usage](#usage) - [Resources](#Resources) - [Features](#some-features) - [Support](#support) - [Building](#building)|
:----------------------------------------------------------: |
| [Upcoming changes](#upcoming-changes) - [Gallery](#gallery) - [Sponsors](#sponsors) - [Credits](#credits) - [License](#license)|

### Installation

---

Ensure you have at least Python 3.6 64bit.
 ```
 pip install dearpygui
 or
 pip3 install dearpygui
 ```
 

### Resources

 ---
  
- [DearPyGui-Examples](https://hoffstadt.github.io/DearPyGui/examples.html) and source code.
- [Tutorials](https://hoffstadt.github.io/DearPyGui/tutorial.html) for getting started.
- [Online Documentation](https://hoffstadt.github.io/DearPyGui/) for official documentation.
- Internal Documentation: Run the **show_documentation** command from within the library to view an reference guide. 
- Complete Demo: You can also view a mostly complete showcase of _Dear PyGui_ by running the **show_demo** command found in _dearpygui.demo_.


### Usage

 ---
 
Using _Dear PyGui_ is a simple as creating a python script like the one below:

Code:
```Python
from dearpygui import core, simple

def save_callback(sender, data):
    print("Save Clicked")

with simple.window("Example Window"):
    core.add_text("Hello world")
    core.add_button("Save", callback=save_callback)
    core.add_input_text("string")
    core.add_slider_float("float")

core.start_dearpygui()
```
Result:
<BR>![BasicUsageExample](https://github.com/hoffstadt/DearPyGui/blob/assets/BasicUsageExample1.PNG?raw=true)
 

### Some Features

---


#### Plotting/Graphing
_Dear PyGui_ includes a plotting API ([ImPlot](https://github.com/epezent/implot) Wrapping)

<img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/controls.gif" width="270"> <img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/dnd.gif" width="270"> <img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/pie.gif" width="270">

<img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/query.gif" width="270"> <img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/bars.gif" width="270">
<img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/rt.gif" width="270">

<img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/stem.gif" width="270"> <img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/markers.gif" width="270">
<img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/shaded.gif" width="270">

<img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/candle.gif" width="270"> <img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/heat.gif" width="270">
<img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/tables.gif" width="270">

#### Themes
_Dear PyGui_ currently includes 10 themes and you can create your own:
<BR>![Themes](https://github.com/hoffstadt/DearPyGui/blob/assets/linuxthemes.PNG?raw=true)

#### Canvas
_Dear PyGui_ includes a drawing API to create custom drawings, plot, and even 2D games.
<BR>![BasicUsageExample](https://github.com/hoffstadt/DearPyGui/blob/assets/canvas.png?raw=true)
 
#### Tools
_Dear PyGui_ includes several tools to help developers with _DearPyGui_ app development including built-in documentation, logging, source viewer, and other tools:
<BR>![BasicUsageExample](https://github.com/hoffstadt/DearPyGui/blob/assets/tools.png?raw=true)


### Support

---

- [Sponsor](https://github.com/sponsors/hoffstadt) If you enjoy _Dear PyGui_ please consider becoming a sponsor.
- [Paypal](https://www.paypal.com/paypalme/JonathanHoffstadt) For those who don't like rewards.

If you are having issues or want to help, here are some places you can go:
- [Github Forum](https://github.com/hoffstadt/DearPyGui/issues/)
- [Discord Forum](https://discord.gg/tyE7Gu4)
- [Reddit](https://www.reddit.com/r/DearPyGui/)


### Building

---

- [Building From Source (Users)](https://github.com/hoffstadt/DearPyGui/wiki/Building-For-Users)
- [Building For Contributors](https://github.com/hoffstadt/DearPyGui/wiki/Building-For-Contributors)


### Upcoming Changes

---

Some upcoming features:
- An object oriented interface
- Support for multiple viewports.
- 3D widget.
- Custom Widget API.

### Sponsors

---

Ongoing _Dear PyGui_ development is financially supported by users and private sponsors.

These include:
* 🏆 [Dean Keinan](https://github.com/deankeinan) _Partner Sponsor_
* 🥇 [Caden Bloxham](https://github.com/cubic-unit) _Gold Sponsor_
* 🥇 [Michael Kadziela](https://github.com/Innominus) _Gold Sponsor_
* 🥇 [Yan Zaretskiy](https://github.com/mindv0rtex) _Gold Sponsor_

Thank you to all other sponsors for keeping this project moving forward!

### Credits

---

Developed by Jonathan Hoffstadt, Preston Cothren, and every direct or indirect contributor.

[Omar Cornut](http://www.miracleworld.net/) for all his incredible work on [Dear ImGui](https://github.com/ocornut/imgui).

[Evan Pezent](http://evanpezent.com/) for all his work on [ImPlot](https://github.com/epezent/implot)

### Gallery

---

<BR>![StandardWindowsExample1](https://github.com/hoffstadt/DearPyGui/blob/assets/WidgetsExample1.PNG?raw=true)
<BR>![StandardWindowsExample1](https://github.com/hoffstadt/DearPyGui/blob/assets/StandardWindowsExample1.PNG?raw=true)

### License

---

_Dear PyGui_ is licensed under the [MIT License](https://github.com/hoffstadt/DearPyGui/blob/master/LICENSE).
