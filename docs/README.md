<a id="user-content-dear-pygui" class="anchor" aria-hidden="true" href="#dear-pygui">
 
<div align="center">
  <img src="https://github.com/hoffstadt/DearPyGui/blob/assets/newlogo.jpg?raw=true" alt="Dear PyGui">
</div>

</a>

<h6 align="center">(This library is available under a free and permissive license however, if you Enjoy <i>Dear PyGui</i> please consider becoming a <a href="https://github.com/hoffstadt/DearPyGui/wiki/Sponsors">Sponsor</a>)</h6>

![Python Version](https://img.shields.io/pypi/pyversions/dearpygui)
[![PYPI](https://img.shields.io/pypi/v/dearpygui)](https://pypi.org/project/dearpygui/)
[![Downloads](https://pepy.tech/badge/dearpygui)](https://pepy.tech/project/dearpygui)

[![build status](https://github.com/hoffstadt/DearPyGui/workflows/Embedded%20Build/badge.svg?branch=master)](https://github.com/hoffstadt/DearPyGui/actions?workflow=Embedded%20Build)
[![static-analysis](https://github.com/hoffstadt/DearPyGui/workflows/static-analysis/badge.svg?branch=master)](https://github.com/hoffstadt/DearPyGui/actions?workflow=static-analysis)
[![Build status](https://ci.appveyor.com/api/projects/status/eypfn2lyuwfv8mel?svg=true)](https://ci.appveyor.com/project/hoffstadt/dearpygui)

**Dear PyGui** is a **simple to use (but powerful) Python GUI framework**. _Dear PyGui_ is **NOT** a wrapping of **[Dear ImGui](https://github.com/ocornut/imgui)** in the normal sense. It is a library built with _Dear ImGui_ which simulates a traditional retained mode GUI (as opposed to _Dear ImGui_'s immediate mode paradigm).

<BR>![Themes](https://github.com/hoffstadt/DearPyGui/blob/assets/linuxthemes.PNG?raw=true)

**Dear PyGui is fundamentally different than other Python GUI frameworks**. Under the hood, _Dear PyGui_ 
uses the immediate mode paradigm and your computer's GPU to facilitate extremely dynamic interfaces. _Dear PyGui_ is currently supported on
the following platforms:

| Platform | Rendering API | Newest Version |
|----------|---------------|----------------|
| **Windows 10** | _DirectX 11_ | [![PYPI](https://img.shields.io/pypi/v/dearpygui)](https://pypi.org/project/dearpygui/) |
| **macOs** | _Metal_ | [![PYPI](https://img.shields.io/pypi/v/dearpygui)](https://pypi.org/project/dearpygui/) |
| **Linux** | _OpenGL 3_ | [![PYPI](https://img.shields.io/pypi/v/dearpygui)](https://pypi.org/project/dearpygui/) |
| **Raspberry Pi 4** | _OpenGL ES_ | [![PYPI](https://img.shields.io/badge/pypi-v0.6.415-orange)](https://img.shields.io/badge/pypi-v0.6.415-orange) |


In the same manner Dear ImGui provides a simple way to create tools for game developers, _Dear PyGui_ provides a 
simple way for python developers to **create quick and powerful GUIs for scripts**.

| [Installing](#installation) - [Usage](#usage) - [Resources](#resources) - [Support](#support) |
:----------------------------------------------------------: |
| [Features](#features) - [Sponsors](#sponsors) - [Credits](#credits) - [License](#license)|

## Installation

Ensure you have at least Python 3.6 64bit.
 ```
 pip install dearpygui
 or
 pip3 install dearpygui
 ```
 

## Resources
 
 [![Chat on Discord](https://img.shields.io/discord/736279277242417272?logo=discord)](https://discord.gg/tyE7Gu4)
[![Reddit](https://img.shields.io/reddit/subreddit-subscribers/dearpygui?label=r%2Fdearpygui)](https://www.reddit.com/r/DearPyGui/)
 
- [Documentation](https://github.com/hoffstadt/DearPyGui/wiki) comprehensive documentaion, tutorials, and examples.
- [Development Roadmap](https://github.com/hoffstadt/DearPyGui/projects/4) major future features and changes.
- [Feature Tracker](https://github.com/hoffstadt/DearPyGui/projects/5) all proposed new features.
- [Bug Tracker](https://github.com/hoffstadt/DearPyGui/projects/1) current bugs and issues.
- Internal Documentation: Run the `show_documentation` command from within the library to view a reference guide. 
- Complete Demo: You can also view a mostly complete showcase of _Dear PyGui_ by running:
```python
import dearpygui.dearpygui as dpg
from dearpygui.demo import show_demo
show_demo()
dpg.start_dearpygui()
```


## Usage
 
Using _Dear PyGui_ is a simple as creating a python script like the one below:

Code:
```Python
import dearpygui.dearpygui as dpg

def save_callback():
    print("Save Clicked")

with dpg.window(label="Example Window"):
    dpg.add_text("Hello world")
    dpg.add_button(label="Save", callback=save_callback)
    dpg.add_input_text(label="string")
    dpg.add_slider_float(label="float")

dpg.setup_viewport()
dpg.start_dearpygui()
```
Result:
<BR>![BasicUsageExample](https://github.com/hoffstadt/DearPyGui/blob/assets/BasicUsageExample1.PNG?raw=true)
 

## Features

#### Plotting/Graphing
_Dear PyGui_ includes a plotting API ([ImPlot](https://github.com/epezent/implot) Wrapping)

<img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/controls.gif" width="270"> <img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/dnd.gif" width="270"> <img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/pie.gif" width="270">

<img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/query.gif" width="270"> <img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/bars.gif" width="270">
<img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/rt.gif" width="270">

<img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/stem.gif" width="270"> <img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/markers.gif" width="270">
<img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/shaded.gif" width="270">

<img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/candle.gif" width="270"> <img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/heat.gif" width="270">
<img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/tables.gif" width="270">

#### Node Editor
_Dear PyGui_ includes a node editor API ([imnodes](https://github.com/Nelarius/imnodes) Wrapping)
![](https://raw.githubusercontent.com/Nelarius/imnodes/master/img/imnodes.gif?token=ADH_jEpqbBrw0nH-BUmOip490dyO2CnRks5cVZllwA%3D%3D)

#### Canvas
_Dear PyGui_ includes a drawing API to create custom drawings, plot, and even 2D games.
<BR>![BasicUsageExample](https://github.com/hoffstadt/DearPyGui/blob/assets/canvas.png?raw=true)
 
#### Tools
_Dear PyGui_ includes several tools to help developers with _DearPyGui_ app development including built-in documentation, logging, source viewer, and other tools:
<BR>![BasicUsageExample](https://github.com/hoffstadt/DearPyGui/blob/assets/tools.png?raw=true)


## Support

If you are having issues or want to help, here are some places you can go:
- [Github Discussions](https://github.com/hoffstadt/DearPyGui/discussions/)
- [Discord Forum](https://discord.gg/tyE7Gu4)
- [Reddit](https://www.reddit.com/r/DearPyGui/)

## Sponsors

![GitHub Sponsors](https://img.shields.io/github/sponsors/hoffstadt?label=Github%20Sponsors)
![Open Collective](https://img.shields.io/opencollective/sponsors/dearpygui?label=Open%20Collective%20Sponsors)

 If you enjoy _Dear PyGui_ please consider becoming a [sponsor](https://github.com/hoffstadt/DearPyGui/wiki/Sponsors).

Ongoing _Dear PyGui_ development is financially supported by users and private sponsors.

These include:
* 🏢 [Anthony Tanbakuchi](https://github.com/kuchi) _Corporate Sponsor_
* 🏆 [Dean Keinan](https://github.com/deankeinan) _Partner Sponsor_
* 🏆 [何凌锋](https://github.com/Hedaozi) _Partner Sponsor_
* 🏆 [Anthony Doupe](https://github.com/Atlamillias) _Partner Sponsor_
* 🏆 [Ray Heasman](https://github.com/rheasman) _Partner Sponsor_
* 🏆 [Marco Studer](https://github.com/Mstpyt) _Partner Sponsor_
* 🏆 [Scripts](https://github.com/scriptslol) _Partner Sponsor_
* 🥇 [Caden Bloxham](https://github.com/cbxm) _Gold Sponsor_
* 🥇 [Michael Kadziela](https://github.com/Innominus) _Gold Sponsor_
* 🥇 [Yan Zaretskiy](https://github.com/mindv0rtex) _Gold Sponsor_
* 🥇 [Trinnik](https://github.com/Trinnik) _Gold Sponsor_
* 🥇 [Barney Mannerings](https://github.com/barnabee) _Gold Sponsor_
* 🥇 [Leron Gray](https://github.com/daddycocoaman) _Gold Sponsor_
* 🥇 [RB Blackstone](https://github.com/RBBlackstone) _Gold Sponsor_


Thank you to all other [sponsors](https://github.com/hoffstadt/DearPyGui/wiki/Sponsors) for keeping this project moving forward!

<a href="https://www.buymeacoffee.com/DearPyGui"><img src="https://img.buymeacoffee.com/button-api/?text=Buy us a coffee&emoji=&slug=DearPyGui&button_colour=FFDD00&font_colour=000000&font_family=Cookie&outline_colour=000000&coffee_colour=ffffff"></a>

## Credits

Developed by [Jonathan Hoffstadt](https://github.com/hoffstadt), [Preston Cothren](https://github.com/Pcothren), and every direct or indirect contributor.

[Omar Cornut](http://www.miracleworld.net/) for all his incredible work on [Dear ImGui](https://github.com/ocornut/imgui).

[Evan Pezent](http://evanpezent.com/) for all his work on [ImPlot](https://github.com/epezent/implot).

[Johann Muszynski](https://github.com/Nelarius) for all of his work on [imnodes](https://github.com/Nelarius/imnodes).

## License

_Dear PyGui_ is licensed under the [MIT License](https://github.com/hoffstadt/DearPyGui/blob/master/LICENSE).
