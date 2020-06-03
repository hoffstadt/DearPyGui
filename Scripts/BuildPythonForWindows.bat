@echo off

rem get current this batch file directory
set dir=%~dp0

rem clean up previous build
echo Cleaning previous python builds
call "%dir%..\Dependencies\cpython\PCbuild\build.bat"  -c Debug -p x64 -t CleanAll
call "%dir%..\Dependencies\cpython\PCbuild\build.bat"  -c Release -p x64 -t CleanAll

rem build python
echo Building python in Debug and Release for x64
call "%dir%..\Dependencies\cpython\PCbuild\build.bat" -c Debug -p x64 -t Build
call "%dir%..\Dependencies\cpython\PCbuild\build.bat" -c Release -p x64 -t Build

rem compile python files to pyc files
echo Compiling python files to pyc
call "%dir%..\Dependencies\cpython\PCbuild\amd64\python.exe" -m compileall -f -b "%dir%..\Dependencies\cpython\Lib"

rem move compiled files to temporary location
echo Running python preparation script
call "%dir%..\Dependencies\cpython\PCbuild\amd64\python.exe" "%dir%..\Scripts\PrepareEmbeddedPython.py"