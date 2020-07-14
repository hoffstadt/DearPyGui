# Marvel Sandbox
[![build status](https://github.com/RaylockLLC/MarvelSandbox/workflows/Windows/badge.svg)](https://github.com/RaylockLLC/MarvelSandbox/actions?workflow=Windows)

<sub>(This library is available under a free and permissive license)</sub>

MarvelSandbox is a **simple to use (but powerful) Python GUI framework**, built on top of **Dear ImGui**. Although the API may seem similar to Dear ImGui, _MarvelSandbox_ does not use the immediate mode paradigm, but instead provides a wrapping that simulates a traditional retained mode GUI.

In the same manner Dear ImGui provides a simple way to create tools for game developers, _MarvelSandbox_ provides a simple way for python developers to **create quick and powerful GUIs for scripts**. It's well suited for **beginners** too!

| [Usage](#usage) - [Features](#features) - [Building](#building)|
:----------------------------------------------------------: |
| [Upcoming changes](#upcoming-changes) - [Gallery](#gallery) - [License](#license) - [Current Dependencies](#current-dependencies) - [Prebuilt Binaries](#prebuilt-binaries)|

### Usage
**MarvelSandbox is an all included framework containing its own embedded python interpreter.** Using _MarvelSandbox_ is a simple as creating a python script like the one below, and calling ```MarvelSandbox.exe --app ScriptName --path PathToScriptDirectory``` from the command line (Windows).

Code:
```Python
from sbWidgets import *

add_text("Hello world")
add_button("Save", callback="SaveCallback")
add_input_text("string")
add_float_slider("float")
```
Result:
<BR>![BasicUsageExample](../assets/BasicUsageExample1.PNG?raw=true)

### Features

#### Plotting/Graphing
_MarvelSandbox_ includes a plotting API built using ImPlot.
<BR>![BasicUsageExample](../assets/PlottingExample1.PNG?raw=true)
  
#### Drawings
_MarvelSandbox_ includes a drawing API to create custom drawings.
<BR>![BasicUsageExample](../assets/DrawingExample1.PNG?raw=true)

#### Logging
_MarvelSandbox_ includes a logging API that also acts as a python output console.
<BR>![BasicUsageExample](../assets/LoggingExample1.PNG?raw=true)
  
#### Built-in IDE
If you choose not to use your own IDE, we have provided a basic built-in IDE for creating and testing apps. To use the built-in IDE just call the _MarvelSandbox_ executable with the **--editor** flag: ```MarvelSandbox.exe --editor```
<BR>![BasicUsageExample](../assets/IDEExample1.PNG?raw=true)
  
#### Built-in Documentation
Using the ```show_documentation()``` command opens the built-in documentation window which displays all _MarvelSandbox_ commands and constants, along with information on their usage.
<BR>![BasicUsageExample](../assets/DocumentationExample.PNG?raw=true)
  
### Prebuilt Binaries
#### Windows
- Download [Latest Release](https://github.com/RaylockLLC/MarvelSandbox/releases/download/v0.1-alpha/Sandbox.zip)
- Download _Sandbox.zip_ and unzip anywhere. 
- Run _MarvelSandbox.exe_.
- You can use App.py as a starting point.

### Current Dependencies

- [Dear ImGui](https://github.com/ocornut/imgui)
- [CPython](https://github.com/python/cpython)
- [ImPlot](https://github.com/epezent/implot)
- [CLI11](https://github.com/CLIUtils/CLI11)

### Requirements

- [_Visual Studio 2019_ (windows)](https://visualstudio.microsoft.com/vs/)
- [_git_](https://git-scm.com/)

### Building

#### Windows

1. From within a local directory, enter the following bash commands:
```
git clone --recursive https://github.com/RaylockLLC/MarvelSandbox
```
3. Open the _Scripts_ folder and run the _BuildPythonForWindows.bat_ file.
4. In Visual Studio, use _Open a local folder_ to open _MarvelSandbox_ folder.
5. Switch to the release configuration and run _Build All_.

### Upcoming Changes
Some upcoming features:
- Additional Plot types (bar graph, pie chart, etc.)
- Support for window docking and multiple viewports.
- More complete theme and style system.
- 3D widget.
- MacOs and Linux support.
- Custom Widget API.


### License

This repository is licensed under the [MIT License](https://mit-license.org/).

### Gallery
<BR>![StandardWindowsExample1](../assets/WidgetsExample1.PNG?raw=true)
<BR>![StandardWindowsExample1](../assets/StandardWindowsExample1.PNG?raw=true)
