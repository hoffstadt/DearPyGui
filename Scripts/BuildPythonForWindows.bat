@echo off

rem get current this batch file directory
set dir=%~dp0

rem clean up previous build
rem echo Cleaning previous python builds
rem call "%dir%..\Dependencies\cpython\PCbuild\build.bat"  -c Debug -p x64 -t CleanAll
rem call "%dir%..\Dependencies\cpython\PCbuild\build.bat"  -c Release -p x64 -t CleanAll
rem 
rem rem build python
rem echo Building python in Debug and Release for x64
rem call "%dir%..\Dependencies\cpython\PCbuild\build.bat" -c Debug -p x64 -t Build
rem call "%dir%..\Dependencies\cpython\PCbuild\build.bat" -c Release -p x64 -t Build
rem 
rem rem compile python files to pyc files
rem echo Compiling python files to pyc
rem call "%dir%..\Dependencies\cpython\PCbuild\amd64\python.exe" -m compileall -f -b "%dir%..\Dependencies\cpython\Lib"
rem 
rem rem move compiled files to temporary location
rem echo Running python preparation script
call "%dir%..\Dependencies\cpython\PCbuild\amd64\python.exe" "%dir%..\Scripts\PrepareEmbeddedPython.py"