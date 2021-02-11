@echo off

rem get current this batch file directory
set dir=%~dp0

rem clean up previous build
echo Cleaning previous python builds
call "%dir%..\Dependencies\cpython\PCbuild\build.bat" -p x64 -t CleanAll

rem build python
echo Building python in Debug and Release for x64
call "%dir%..\Dependencies\cpython\PCbuild\build.bat" -p x64 -t Build

cd ..
mkdir cmake-build-release
cd cmake-build-Release
cmake .. -G "Visual Studio 16 2019" -A "x64" -DMVDIST_ONLY=True -DMVPY_VERSION=0 -DMVDPG_VERSION=local_build
cd ..
cmake --build cmake-build-release --config "Release"

cd Distribution
call "../Dependencies/cpython/PCbuild/amd64/python.exe" BuildPythonWheel.py ..\cmake-build-release\DearPyGui\Release\core.pyd 0
call "../Dependencies/cpython/PCbuild/amd64/python.exe" -m ensurepip
call "../Dependencies/cpython/PCbuild/amd64/python.exe" -m pip install --upgrade pip
call "../Dependencies/cpython/PCbuild/amd64/python.exe" -m pip install twine
call "../Dependencies/cpython/PCbuild/amd64/python.exe" -m pip install wheel
call "../Dependencies/cpython/PCbuild/amd64/python.exe" -m setup bdist_wheel --plat-name win_amd64 --dist-dir ../dist
cd ..
cd Scripts