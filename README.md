<a id="user-content-dear-pygui" class="anchor" aria-hidden="true" href="#dear-pygui">
 
<div align="center">
  <img src="https://github.com/hoffstadt/DearPyGui/blob/assets/newlogo.jpg?raw=true" alt="Dear PyGui">
</div>

</a>

<h6 align="center">(This library is available under a free and permissive license however, if you are using Dear PyGui please consider becoming a <a href="https://github.com/hoffstadt/DearPyGui/wiki/Sponsors">Sponsor</a>)</h6>

![Python Version](https://img.shields.io/pypi/pyversions/dearpygui)
[![PYPI](https://img.shields.io/pypi/v/dearpygui)](https://pypi.org/project/dearpygui/)
[![Downloads](https://pepy.tech/badge/dearpygui)](https://pepy.tech/project/dearpygui)

[![build status](https://github.com/hoffstadt/DearPyGui/workflows/Embedded%20Build/badge.svg?branch=master)](https://github.com/hoffstadt/DearPyGui/actions?workflow=Embedded%20Build)
[![static-analysis](https://github.com/hoffstadt/DearPyGui/workflows/Static%20Analysis/badge.svg?branch=master)](https://github.com/hoffstadt/DearPyGui/actions?workflow=Static%20Analysis)
[![Deployment](https://github.com/hoffstadt/DearPyGui/actions/workflows/Deployment.yml/badge.svg?branch=master)](https://github.com/hoffstadt/DearPyGui/actions/workflows/Deployment.yml)
[![Documentation Status](https://readthedocs.org/projects/dearpygui/badge/?version=latest)](https://dearpygui.readthedocs.io/en/latest/?badge=latest)

**Dear PyGui** is a **simple to use (but powerful) Python GUI framework**. _Dear PyGui_ is **NOT** a wrapping of **[Dear ImGui](https://github.com/ocornut/imgui)** in the normal sense. It is a library built with _Dear ImGui_.

<BR>![Themes](https://github.com/hoffstadt/DearPyGui/blob/assets/linuxthemes.PNG?raw=true)

**Dear PyGui is fundamentally different than other Python GUI frameworks**. Under the hood, _Dear PyGui_ 
uses the immediate mode paradigm and your computer's GPU to facilitate extremely dynamic interfaces. _Dear PyGui_ is currently supported on
the following platforms:

| Platform | Graphics API | Newest Version |
|----------|---------------|----------------|
| **Windows 10** | _DirectX 11_ | [![PYPI](https://img.shields.io/pypi/v/dearpygui)](https://pypi.org/project/dearpygui/) |
| **macOS** | _Metal_ | [![PYPI](https://img.shields.io/pypi/v/dearpygui)](https://pypi.org/project/dearpygui/) |
| **Linux** | _OpenGL 3_ | [![PYPI](https://img.shields.io/pypi/v/dearpygui)](https://pypi.org/project/dearpygui/) |
| **Raspberry Pi 4** | _OpenGL ES_ | [![PYPI](https://img.shields.io/badge/pypi-v1.2-blue)](https://img.shields.io/badge/pypi-v1.2-blue) |

In the same manner Dear ImGui provides a simple way to create tools for game developers, _Dear PyGui_ provides a 
simple way for python developers to **create quick and powerful GUIs for scripts**.

| [Installation](#installation) - [Usage](#usage) - [Resources](#resources) - [Support](#support) |
:----------------------------------------------------------: |
| [Features](#features) - [Sponsors](#sponsors) - [Credits](#credits) - [License](#license) - [Gallery](#gallery) |

## Installation

Ensure you have at least Python 3.7 64bit.
 ```
 pip install dearpygui
 or
 pip3 install dearpygui
 ```
 

## Resources
 
 [![Chat on Discord](https://img.shields.io/discord/736279277242417272?logo=discord)](https://discord.gg/tyE7Gu4)
[![Reddit](https://img.shields.io/reddit/subreddit-subscribers/dearpygui?label=r%2Fdearpygui)](https://www.reddit.com/r/DearPyGui/)
 
- [Contributor Documentation](https://github.com/hoffstadt/DearPyGui/wiki)
- [User Documentation](https://dearpygui.readthedocs.io/en/latest/index.html) comprehensive documentation, tutorials, and examples.
- [Development Roadmap](https://github.com/hoffstadt/DearPyGui/projects/4) major future features and changes.
- [Feature Tracker](https://github.com/hoffstadt/DearPyGui/issues?q=is%3Aissue+is%3Aopen+label%3A%22type%3A+feature%22) all proposed new features.
- [Bug Tracker](https://github.com/hoffstadt/DearPyGui/issues?q=is%3Aissue+is%3Aopen+label%3A%22type%3A+bug%22) current bugs and issues.
- Internal Documentation: Run the `show_documentation` command from within the library to view a reference guide. 
- Complete Demo: You can also view a mostly complete showcase of _Dear PyGui_ by running:
```python
import dearpygui.dearpygui as dpg
from dearpygui.demo import show_demo

dpg.create_context()
dpg.create_viewport()
dpg.setup_dearpygui()

show_demo()

dpg.show_viewport()
dpg.start_dearpygui()
dpg.destroy_context()
```

## Usage
 
Using _Dear PyGui_ is as simple as creating a python script like the one below:

Code:
```Python
import dearpygui.dearpygui as dpg

def save_callback():
    print("Save Clicked")

dpg.create_context()
dpg.create_viewport()
dpg.setup_dearpygui()

with dpg.window(label="Example Window"):
    dpg.add_text("Hello world")
    dpg.add_button(label="Save", callback=save_callback)
    dpg.add_input_text(label="string")
    dpg.add_slider_float(label="float")

dpg.show_viewport()
dpg.start_dearpygui()
dpg.destroy_context()
```
Result:
<BR>![BasicUsageExample](https://github.com/hoffstadt/DearPyGui/blob/assets/BasicUsageExample1.PNG?raw=true)
 

## Features

#### Plotting/Graphing
_Dear PyGui_ includes a plotting API built with [ImPlot](https://github.com/epezent/implot)

<img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/controls.gif" width="270"> <img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/dnd.gif" width="270"> <img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/pie.gif" width="270">

<img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/query.gif" width="270"> <img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/bars.gif" width="270">
<img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/rt.gif" width="270">

<img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/stem.gif" width="270"> <img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/markers.gif" width="270">
<img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/shaded.gif" width="270">

<img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/candle.gif" width="270"> <img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/heat.gif" width="270">
<img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/tables.gif" width="270">

#### Node Editor
_Dear PyGui_ includes a node editor built with [imnodes](https://github.com/Nelarius/imnodes)
![](https://github.com/hoffstadt/DearPyGui/blob/assets/readme/nodes2.png)


#### Canvas
_Dear PyGui_ includes a drawing API to create custom drawings, plot, and even 2D games.
![](https://github.com/hoffstadt/DearPyGui/blob/assets/readme/tetris.png)
 
#### Tools
_Dear PyGui_ includes several tools to help developers with _DearPyGui_ app development:
<BR>![BasicUsageExample](https://github.com/hoffstadt/DearPyGui/blob/assets/tools.png?raw=true)


## Support

If you are having issues or want to help, here are some places you can go:
- [Github Discussions](https://github.com/hoffstadt/DearPyGui/discussions/)
- [Discord Forum](https://discord.gg/tyE7Gu4)
- [Reddit](https://www.reddit.com/r/DearPyGui/)

## Sponsors

![GitHub Sponsors](https://img.shields.io/github/sponsors/hoffstadt?label=Github%20Sponsors)
![Open Collective](https://img.shields.io/opencollective/sponsors/dearpygui?label=Open%20Collective%20Sponsors)

Ongoing _Dear PyGui_ development is financially supported by users and private sponsors.
If you enjoy _Dear PyGui_ please consider becoming a [sponsor](https://github.com/hoffstadt/DearPyGui/wiki/Sponsors).

<a href="https://www.buymeacoffee.com/DearPyGui"><img src="https://img.buymeacoffee.com/button-api/?text=Buy us a coffee&emoji=&slug=DearPyGui&button_colour=FFDD00&font_colour=000000&font_family=Cookie&outline_colour=000000&coffee_colour=ffffff"></a>

 ## Credits

Developed by [Jonathan Hoffstadt](https://github.com/hoffstadt), [Preston Cothren](https://github.com/Pcothren), and every direct or indirect contributor.

[Omar Cornut](http://www.miracleworld.net/) for all his incredible work on [Dear ImGui](https://github.com/ocornut/imgui).

[Evan Pezent](http://evanpezent.com/) for all his work on [ImPlot](https://github.com/epezent/implot).

[Johann Muszynski](https://github.com/Nelarius) for all of his work on [imnodes](https://github.com/Nelarius/imnodes).

## License

_Dear PyGui_ is licensed under the [MIT License](https://github.com/hoffstadt/DearPyGui/blob/master/LICENSE).
 
## Gallery
 ![](https://github.com/hoffstadt/DearPyGui/blob/assets/readme/3d.png)
 
 ![](https://github.com/hoffstadt/DearPyGui/blob/assets/readme/nodes1.png)
 
 ![](https://github.com/hoffstadt/DearPyGui/blob/assets/readme/space.png)
 
 ![](https://github.com/hoffstadt/DearPyGui/blob/assets/readme/snake.gif)
 
 ![](https://github.com/hoffstadt/DearPyGui/blob/assets/readme/drawing.png)
 
 <BR>![BasicUsageExample](https://github.com/hoffstadt/DearPyGui/blob/assets/canvas.png?raw=true)
 
 ![](https://github.com/hoffstadt/DearPyGui/blob/assets/readme/nodes3.png)
 
 ![](https://github.com/hoffstadt/DearPyGui/blob/assets/readme/3d1.png)
 
 ![](https://github.com/hoffstadt/DearPyGui/blob/assets/readme/game1.png)
 
 ![](https://github.com/hoffstadt/DearPyGui/blob/assets/readme/mandlebrot.gif)
 
 ![](https://github.com/hoffstadt/DearPyGui/blob/assets/readme/nodes4.png)
