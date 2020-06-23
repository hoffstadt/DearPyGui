@echo off

rem get current this batch file directory
set dir=%~dp0

echo Running python package script
call "%dir%..\Dependencies\cpython\PCbuild\amd64\python.exe" "%dir%..\Scripts\PackageWindowsApp.py"
echo Complete