
<h1 align="center">
  <br>
  <a href="https://github.com/hoffstadt/DearPyGui"><img src="https://raw.githubusercontent.com/hoffstadt/DearPyGui/assets/readme/dpg_logo_button.png" alt="Dear PyGui logo"></a>
</h1>

<h4 align="center">A modern, fast and powerful GUI framework for Python</h4>

<h1></h1>

<p align="center">
  <a href=""><img src="https://img.shields.io/pypi/pyversions/dearpygui" alt="Python versions"></a>
  <a href="https://pypi.org/project/dearpygui/"><img src="https://img.shields.io/pypi/v/dearpygui" alt="PYPI"></a>
  <a href="https://pepy.tech/project/dearpygui"><img src="https://pepy.tech/badge/dearpygui" alt="Downloads"></a>
  <a href="#license"><img src="https://github.com/hoffstadt/DearPyGui/blob/assets/readme/mit_badge.svg" alt="MIT License"></a>
</p>

<p align="center">
   <a href="https://github.com/hoffstadt/DearPyGui/actions?workflow=Embedded%20Build"><img src="https://github.com/hoffstadt/DearPyGui/workflows/Embedded%20Build/badge.svg?branch=master" alt="static-analysis"></a>
   <a href="https://github.com/hoffstadt/DearPyGui/actions?workflow=Static%20Analysis"><img src="https://github.com/hoffstadt/DearPyGui/workflows/Static%20Analysis/badge.svg?branch=master" alt="static-analysis"></a>
   <a href="https://github.com/hoffstadt/DearPyGui/actions/workflows/Deployment.yml"><img src="https://github.com/hoffstadt/DearPyGui/actions/workflows/Deployment.yml/badge.svg?branch=master" alt="Deployment"></a>
   <a href="https://dearpygui.readthedocs.io/en/latest/?badge=latest"><img src="https://readthedocs.org/projects/dearpygui/badge/?version=latest" alt="Documentation Status"></a>
</p>

<h1></h1>

<p align="center">
  <a href="#features">Features</a> •
  <a href="#installation">Installation</a> •
  <a href="#how-to-use">How To Use</a> • 
  <a href="#demo">Demo</a> •
  <a href="#resources">Resources</a> •
  <a href="#support">Support</a> •
  <a href="#tech-stack">Tech stack</a> •
  <a href="#credits">Credits</a> •
  <a href="#license">License</a> •
  <a href="#gallery">Gallery</a>
</p>

<h1></h1>

<BR>![Themes](https://raw.githubusercontent.com/hoffstadt/DearPyGui/assets/linuxthemes.PNG) 
  
### Features  
- **Modern look** — Complete theme and style control
- **Great performance** —  GPU-based rendering and efficient C/C++ code
- **Stable operation** —  Asynchronous function support
- **Fast graphs** — Display over 1 million datapoints at 60 fps, zoom and pan
- **Node editor** — Intuitive user interaction
- **Built-in demo** — Quickly learn all features
- **Developer tools** — Theme and resource inspection, runtime metrics, debugger
- **Cross-platform** — Windows, Linux, MacOS
- **MIT license**

<h1></h1>
<p align="center">
  <img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/stem.gif" width="380">&nbsp;&nbsp; &nbsp;&nbsp; &nbsp;&nbsp;<img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/tables.gif" width="380">
</p>
<h1></h1>
  
## Functionality
- Menus, tabs, windows, tree nodes
- Text widgets, input boxes
- Comboboxes, listboxes 
- Int, float & scientific input widgets and sliders
- Date & color pickers
- Checkboxes, droplists, buttons
- Progress bars, loading indicators
- Drag & drop
- Tables
- Canvas with layers for drawing, textures, images
- Real-time plotting
- Node editor
- And much more

<h1></h1>
<p align="center"> 
<img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/pie.gif" width="380">&nbsp;&nbsp; &nbsp;&nbsp; &nbsp;&nbsp;<img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/candle.gif" width="380"> 
</p>
<h1></h1>
  
## Installation

Ensure you have at least Python 3.7 64bit.
 ```
 pip install dearpygui
 or
 pip3 install dearpygui
 ```
 
## How to use?
 
Using Dear PyGui is as simple as the following Python script.
  
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
<br/>
<p align="center"><a href="https://dearpygui.readthedocs.io/en/latest/tutorials/first-steps.html#first-run"><img src="https://raw.githubusercontent.com/hoffstadt/DearPyGui/assets/readme/first_app.gif" alt="Dear PyGui example window"></a></p>
                                                                                           
## Demo
The built-in demo shows all of Dear PyGui's functionality. Use [this code](https://dearpygui.readthedocs.io/en/latest/tutorials/first-steps.html#demo) to run the demo. The following impression shows a few, but not nearly all, of the available widgets and features. Since the Python code of the demo can be <a href="https://github.com/hoffstadt/DearPyGui/blob/master/DearPyGui/dearpygui/demo.py" alt="demo code repository">inspected</a>, you can leverage the demo code to build your own apps.
<br/><br/>
<p align="center"><a href="https://dearpygui.readthedocs.io/en/latest/tutorials/first-steps.html#demo"><img src="https://raw.githubusercontent.com/hoffstadt/DearPyGui/assets/readme/demo.gif" alt="Dear PyGui demo"></a></p>

## Tools
There are several developer tools built into Dear PyGui that can assist in developing and debugging your app. The <a href="https://dearpygui.readthedocs.io/en/latest/tutorials/tips-resources.html#style-editor">style editor</a> allows you to experiment with all style options at runtime to find the exact colors, padding, rounding and other style settings for your application. The <a href="https://dearpygui.readthedocs.io/en/latest/tutorials/tips-resources.html#item-registry">item registry</a> shows the state of each item, e.g. window, button, tab or graph, while your app is running. The built-in <a href="https://dearpygui.readthedocs.io/en/latest/tutorials/tips-resources.html#runtime-metrics">runtime metrics</a> provide real-time information about your app's performance. And the <a href="https://dearpygui.readthedocs.io/en/latest/tutorials/tips-resources.html#font-manager">font manager</a> enables you to inspect the loaded fonts and their characters. 
  
## Resources

- [API documentation](https://dearpygui.readthedocs.io/en/latest/index.html) :books: 
- [Development Roadmap](https://github.com/hoffstadt/DearPyGui/projects/4)
- [FAQ](https://github.com/hoffstadt/DearPyGui/discussions/categories/frequently-asked-questions-faq)
- [Feature Tracker](https://github.com/hoffstadt/DearPyGui/issues?q=is%3Aissue+is%3Aopen+label%3A%22type%3A+feature%22)
- [Bug Tracker](https://github.com/hoffstadt/DearPyGui/issues?q=is%3Aissue+is%3Aopen+label%3A%22type%3A+bug%22)
- [Showcase apps including source code](https://github.com/hoffstadt/DearPyGui/wiki/Dear-PyGui-Showcase) :star:
  
## Support

If you are having issues or want to help, here are some places you can go.
  - [Discord Forum](https://discord.gg/tyE7Gu4) 💬
  - [Reddit](https://www.reddit.com/r/DearPyGui/)

[![Chat on Discord](https://img.shields.io/discord/736279277242417272?logo=discord)](https://discord.gg/tyE7Gu4) &nbsp; &nbsp; &nbsp; [![Reddit](https://img.shields.io/reddit/subreddit-subscribers/dearpygui?label=r%2Fdearpygui)](https://www.reddit.com/r/DearPyGui/)

### Tech stack
Dear PyGui is built on top of <a href="https://github.com/ocornut/imgui" target="_blank">Dear ImGui</a>, including the [ImPlot](https://github.com/epezent/implot) and [imnodes](https://github.com/Nelarius/imnodes) extensions, and is fundamentally different than other Python GUI frameworks. Under the hood, it uses the immediate mode paradigm and your computer's GPU to facilitate extremely dynamic interfaces. In the same manner Dear ImGui provides a simple way to create tools for game developers, Dear PyGui provides a simple way for python developers to create quick and powerful GUIs for scripts. Dear PyGui is written in C/C++ resulting in highly performant Python applications. Dear PyGui is currently supported on the following platforms. 
<br/>
  
| Platform | Graphics API | Newest Version |
|:---------|:-------------|:---------------|
| **Windows 10** | _DirectX 11_ | [![PYPI](https://img.shields.io/pypi/v/dearpygui)](https://pypi.org/project/dearpygui/) |
| **macOS** | _Metal_ | [![PYPI](https://img.shields.io/pypi/v/dearpygui)](https://pypi.org/project/dearpygui/) |
| **Linux** | _OpenGL 3_ | [![PYPI](https://img.shields.io/pypi/v/dearpygui)](https://pypi.org/project/dearpygui/) |
| **Raspberry Pi 4** | _OpenGL ES_ | [![PYPI](https://img.shields.io/badge/pypi-v1.2-blue)](https://img.shields.io/badge/pypi-v1.2-blue) |

  
## Credits

- Developed by [Jonathan Hoffstadt](https://github.com/hoffstadt), [Preston Cothren](https://github.com/Pcothren) and every direct or indirect contributor.

- [Omar Cornut](http://www.miracleworld.net/) for all his incredible work on [Dear ImGui](https://github.com/ocornut/imgui).

- [Evan Pezent](http://evanpezent.com/) for all his work on [ImPlot](https://github.com/epezent/implot).

- [Johann Muszynski](https://github.com/Nelarius) for all of his work on [imnodes](https://github.com/Nelarius/imnodes).

## License
Dear PyGui is licensed under the [MIT License](https://github.com/hoffstadt/DearPyGui/blob/master/LICENSE).
  
## Sponsor
Continued maintenance and development are a full-time endeavor which we would like to sustain and grow. Ongoing development is financially supported by users and private sponsors. If you enjoy Dear PyGui please consider becoming a [sponsor](https://github.com/hoffstadt/DearPyGui/wiki/Sponsors) or buy us a [cup of coffee](https://www.buymeacoffee.com/DearPyGui).

<img src="https://img.shields.io/github/sponsors/hoffstadt?label=Github%20Sponsors">&nbsp; &nbsp; &nbsp; <img src="https://img.shields.io/opencollective/sponsors/dearpygui?label=Open%20Collective%20Sponsors">

## Gallery

#### Plotting/Graphing
_Dear PyGui_ includes a plotting API built with [ImPlot](https://github.com/epezent/implot)

<img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/controls.gif" width="380">&nbsp;&nbsp; &nbsp;&nbsp; &nbsp;&nbsp;<img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/dnd.gif" width="380">

<img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/query.gif" width="380">&nbsp;&nbsp; &nbsp;&nbsp; &nbsp;&nbsp;<img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/bars.gif" width="380">
  
<img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/rt.gif" width="380">&nbsp;&nbsp; &nbsp;&nbsp; &nbsp;&nbsp;<img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/markers.gif" width="380">
  
<img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/shaded.gif" width="380">&nbsp;&nbsp; &nbsp;&nbsp; &nbsp;&nbsp;<img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/heat.gif" width="380">


#### Node Editor
_Dear PyGui_ includes a node editor built with [imnodes](https://github.com/Nelarius/imnodes)
![](https://github.com/hoffstadt/DearPyGui/blob/assets/readme/nodes2.png)


#### Canvas
_Dear PyGui_ includes a drawing API to create custom drawings, plot, and even 2D games.
![](https://github.com/hoffstadt/DearPyGui/blob/assets/readme/tetris.png)


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
