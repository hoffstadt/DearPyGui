@echo off

rem get current this batch file directory
set dir=%~dp0

:Run
if "%~1"=="--ci" goto Release

:Debug
rem clean up previous build
echo Cleaning previous python builds
call "%dir%..\Dependencies\cpython\PCbuild\build.bat"  -c Debug -p x64 -t CleanAll

rem build python
echo Building python in Debug and Release for x64
call "%dir%..\Dependencies\cpython\PCbuild\build.bat" -c Debug -p x64 -t Build

:Release
rem clean up previous build
echo Cleaning previous python builds
call "%dir%..\Dependencies\cpython\PCbuild\build.bat" -p x64 -t CleanAll

rem build python
echo Building python in Debug and Release for x64
call "%dir%..\Dependencies\cpython\PCbuild\build.bat" -p x64 -t Build

:MoveArtifacts
echo Downloading pip
call curl --output ../Dependencies/cpython/PCbuild/amd64/get-pip.py -url https://bootstrap.pypa.io/get-pip.py


echo Getting pip, setuptools, and wheel
call "%dir%..\Dependencies\cpython\PCbuild\amd64\python.exe" "%dir%..\Dependencies\cpython\PCbuild\amd64\get-pip.py"

echo Getting twine
call "%dir%..\Dependencies\cpython\PCbuild\amd64\python.exe" -m pip install twine

pause
