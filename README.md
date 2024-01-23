<h1 align="center">
  <br>
  <a href="https://github.com/hoffstadt/DearPyGui"><img src="https://raw.githubusercontent.com/hoffstadt/DearPyGui/assets/readme/dpg_logo_button.png" alt="Dear PyGui logo"></a>
</h1>

<h4 align="center">A modern, fast and powerful GUI framework for Python</h4>
  
This is an attempt to migrate all the backend of ImGui, ImPlot (and eventually ImNode) to the most recent versions
That means going:
  - ImGui 1.83 -> 1.90.1 [Changelog](https://github.com/ocornut/imgui/releases)
  - ImPlot 0.11 -> 0.17 [Changelog](https://github.com/epezent/implot/tags)
  - ImNode (not really relevant updates)

These versions are 3 years old, and in the meanwhile many improvements have been developed!

## Features  
- **Modern look** — Complete theme and style control
- **Great performance** —  GPU-based rendering and efficient C/C++ code
- **Stable operation** —  Asynchronous function support
- **Fast graphs** — Display over 1 million datapoints at 60 fps, zoom and pan
- **Node editor** — Intuitive user interaction
- **Built-in demo** — Quickly learn all features
- **Developer tools** — Theme and resource inspection, runtime metrics, debugger
- **Cross-platform** — Windows, Linux, MacOS
  
## Installation

Uncomment this line in `thirdparty/imgui/imconfig.h` (this isn't an "hacky" thing, it's the standard way until we don't detach from the main ImGui branch):
```
#define IMGUI_DEFINE_MATH_OPERATORS
```

If you have new params/widgets go to the root directory and run:
```
./build_release.sh
```
In any case (either you have new params/widgets or you just modified some internal stuff) go to your app folder and run:
```
pip install ../dearpygui/
```
(We guess that they have the same parent folder)