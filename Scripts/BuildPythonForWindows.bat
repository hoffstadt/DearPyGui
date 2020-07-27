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

echo Building python in Debug and Release for x86
call "%dir%..\Dependencies\cpython\PCbuild\build.bat" -c Debug -p Win32 -t Build

:Release
rem clean up previous build
echo Cleaning previous python builds
call "%dir%..\Dependencies\cpython\PCbuild\build.bat" -p x64 -t CleanAll

echo Cleaning previous python builds
call "%dir%..\Dependencies\cpython\PCbuild\build.bat" -p Win32 -t CleanAll

rem build python
echo Building python in Debug and Release for x64
call "%dir%..\Dependencies\cpython\PCbuild\build.bat" -p x64 -t Build

echo Building python in Debug and Release for x86
call "%dir%..\Dependencies\cpython\PCbuild\build.bat" -p Win32 -t Build

:MoveArtifacts
rem compile python files to pyc files
echo Compiling python files to pyc
call "%dir%..\Dependencies\cpython\PCbuild\amd64\python.exe" -m compileall -f -b "%dir%..\Dependencies\cpython\Lib"

rem move compiled files to temporary location
echo Running python preparation script
call "%dir%..\Dependencies\cpython\PCbuild\amd64\python.exe" "%dir%..\Scripts\PrepareEmbeddedPython.py"

echo Getting pip, setuptools, and wheel
call "%dir%..\Dependencies\cpython\PCbuild\amd64\python.exe" "%dir%..\Dependencies\cpython\PCbuild\amd64\get-pip.py"

echo Getting twine
call "%dir%..\Dependencies\cpython\PCbuild\amd64\python.exe" -m pip install twine

pause
