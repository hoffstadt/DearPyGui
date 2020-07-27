@echo off

echo Running python preparation script
call "%dir%..\..\Dependencies\cpython\PCbuild\win32\python.exe" "%dir%..\..\Distribution\Win32\BuildPythonWheel.py"

echo Building DearPyGui Wheel
call "%dir%..\..\Dependencies\cpython\PCbuild\win32\python.exe" -m setup bdist_wheel --plat-name win32

echo Building DearPyGui Wheel
call "%dir%..\..\Dependencies\cpython\PCbuild\win32\python.exe" -m twine upload dist/*

pause
