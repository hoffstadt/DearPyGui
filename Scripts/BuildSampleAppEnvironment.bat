@echo off

rem get current this batch file directory
set dir=%~dp0

echo Running python preparation script
call "%dir%..\Dependencies\cpython\PCbuild\amd64\python.exe" "%dir%..\Scripts\PrepareSampleEnvironment.py"
echo Complete