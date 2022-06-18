@echo off

rem get current this batch file directory
set dir=%~dp0

:Run
if "%~1"=="--ci" goto Release

:Debug
rem clean up previous build
echo Cleaning previous python builds
call "%dir%..\thirdparty\cpython\PCbuild\build.bat"  -c Debug -p x64 -t CleanAll

rem build python
echo Building python in Debug and Release for x64
call "%dir%..\thirdparty\cpython\PCbuild\build.bat" -c Debug -p x64 -t Build

:Release
rem clean up previous build
echo Cleaning previous python builds
call "%dir%..\thirdparty\cpython\PCbuild\build.bat" -p x64 -t CleanAll

rem build python
echo Building python in Debug and Release for x64
call "%dir%..\thirdparty\cpython\PCbuild\build.bat" -p x64 -t Build

pause