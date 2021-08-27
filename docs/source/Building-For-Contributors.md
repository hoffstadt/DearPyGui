## Windows
**Requirements**
- [_git_](https://git-scm.com/)
- [_Visual Studio 2019_ (windows)](https://visualstudio.microsoft.com/vs/) with the following workflows:
  * Python development
  * Desktop development with C++
  * Game development with C++

Instructions:
1. From within a local directory, enter the following bash commands:
```
git clone --recursive https://github.com/hoffstadt/DearPyGui
```
3. Open the _Scripts_ folder and run the _BuildPythonForWindows.bat_ file.
4. In Visual Studio, use _Open a local folder_ to open _Dear PyGui_ folder. This will use CMakeLists.txt as the project file.
5. Run _Build All_.

## Linux
**Requirements**
- [_git_](https://git-scm.com/)
- cmake
- libglu1-mesa-dev 
- libgl1-mesa-dev
- libxrandr-dev

Instructions:
1. From within a local directory, enter the following bash commands:
```
git clone --recursive https://github.com/hoffstadt/DearPyGui
```
2. From the terminal, enter the  _Scripts_ folder and run the following bash commands:
```bash
chmod +x BuildPythonForLinux.sh
./BuildPythonForLinux.sh
```
3. Then from within the DPG root directory, build with cmake:
```bash
mkdir cmake-build-debug
cd cmake-build-debug
cmake ..
cd ..
cmake --build cmake-build-debug --config Debug
```

## MacOS
**Requirements**
- [_git_](https://git-scm.com/)
- cmake
- XCode
- harfbuzz

Instructions:
1. From within a local directory, enter the following bash commands:
```
git clone --recursive https://github.com/hoffstadt/DearPyGui
```
2. From the terminal, enter the  _Scripts_ folder and run the following bash commands:
```bash
chmod +x BuildPythonForMac.sh
./BuildPythonForMac.sh
```
3. Then from within the DPG root directory, build with cmake:
```bash
mkdir cmake-build-debug
cd cmake-build-debug
cmake ..
cd ..
cmake --build cmake-build-debug --config Debug
```

---

### DearSandbox
_DearSandbox_ is the embedded version of Python used for developing.

### PureImGui
_PureImGui_ is for testing pure Dear ImGui commands and features.