# This is Not Ready Yet

## Note
This currently uses the embedded Python (Windows/Mac) used for development to build the library and thus the wheel is only built for the most recent version of Python (3.9). Under Linux this use system Python3 or Python3 that can be found by system cmake to build the library, so it's built for your Python3 version.

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
2. Open the _Scripts_ folder and run the _BuildLocalWheelWindows.bat_ file.
3. Wheel will be found in the newly created _dist_ folder

## MacOS
**Requirements**
- [_git_](https://git-scm.com/)
- [cmake](https://cmake.org/download/) (installed as a command line tool)
- XCode
- [Homebrew](https://brew.sh/)

Instructions:
1. From within a local directory, enter the following bash commands:
```
git clone --recursive https://github.com/hoffstadt/DearPyGui
```
2. From the terminal, enter the  _Scripts_ folder and run the following bash commands:
```bash
chmod +x BuildLocalWheelMacIntel.sh
./BuildLocalWheelMacIntel.sh
```
or for the new Arm processors (not tested):
```bash
chmod +x BuildLocalWheelMacArm.sh
./BuildLocalWheelMacM1.sh
```
3. Wheel will be found in the newly created _dist_ folder

## Linux
**Requirements**
- [_git_](https://git-scm.com/)
- [cmake](https://cmake.org/download/) (installed as a command line tool)
- build-essential under debian/ubuntu or sudo yum groupinstall "Development Tools" in Fedora, Centos or Redhat
- python3-dev under debian/ubuntu or python3-devel in Fedora, Centos or Redhat

Instructions:
1. From within a local directory, enter the following bash commands:
```
git clone --recursive https://github.com/hoffstadt/DearPyGui
```
2. From the terminal, enter the  _Scripts_ folder and run the following bash commands:
```bash
chmod +x BuildLocalWheelLinux.sh
./BuildLocalWheelLinux.sh
```
3. Wheel will be found in the newly created _dist_ folder

## Raspberry Pi 4
This is not well tested. You must ensure your device is in "Legacy" mode in raspi-config.
**Requirements**
- [_git_](https://git-scm.com/)
- [cmake](https://cmake.org/download/) (installed as a command line tool)
- build-essential under debian/ubuntu or sudo yum groupinstall "Development Tools" in Fedora, Centos or Redhat
- libglu1-mesa-dev
- libgl1-mesa-dev
- libxrandr-dev
- libxinerama-dev
- libxcursor-dev
- libxi-dev

Instructions:
1. From within a local directory, enter the following bash commands:
```
git clone --recursive https://github.com/hoffstadt/DearPyGui
```
2. From the terminal, enter the  _Scripts_ folder and run the following bash commands:
```bash
chmod +x BuildLocalWheelRPi.sh
./BuildLocalWheelRPi.sh
```
3. Wheel will be found in the newly created _dist_ folder