@echo off

rem get current this batch file directory
set dir=%~dp0
set PATH="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin";%PATH%

rem clean up previous build
echo Cleaning previous python builds
rem call "%dir%..\Dependencies\cpython\PCbuild\build.bat" -p x64 -t CleanAll

rem build python
echo Building python in Debug and Release for x64
if exist "%dir%..\Dependencies\cpython\PCbuild\amd64\python.exe" (
	echo Python already built
) else (
	call "%dir%..\Dependencies\cpython\PCbuild\build.bat" -p x64 -t Build
)

cd ..
if exist cmake-build-local (
	echo cmake-build-local exists
) else (
	mkdir cmake-build-local
)
cd cmake-build-local
cmake .. -G "Visual Studio 16 2019" -DMVDIST_ONLY=True -DMVPY_VERSION=0 -DMVDPG_VERSION=local_build
cd ..
cmake --build cmake-build-local --config "Release"

cd Distribution
call "../Dependencies/cpython/PCbuild/amd64/python.exe" BuildPythonWheel.py ..\cmake-build-local\DearPyGui\Release\_dearpygui.pyd 0
call "../Dependencies/cpython/PCbuild/amd64/python.exe" -m ensurepip
call "../Dependencies/cpython/PCbuild/amd64/python.exe" -m pip install --upgrade pip
call "../Dependencies/cpython/PCbuild/amd64/python.exe" -m pip install twine
call "../Dependencies/cpython/PCbuild/amd64/python.exe" -m pip install wheel
call "../Dependencies/cpython/PCbuild/amd64/python.exe" -m setup bdist_wheel --plat-name win_amd64 --dist-dir ../dist
cd ..
cd Scripts