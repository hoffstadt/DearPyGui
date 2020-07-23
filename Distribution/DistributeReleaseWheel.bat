@echo off

echo Running python preparation script
call "%dir%..\Dependencies\cpython\PCbuild\amd64\python.exe" "%dir%..\Distribution\BuildPythonWheel.py"

echo Building DearPyGui Wheel
call "%dir%..\Dependencies\cpython\PCbuild\amd64\python.exe" -m setup bdist_wheel --plat-name win_amd64

echo Building DearPyGui Wheel
call "%dir%..\Dependencies\cpython\PCbuild\amd64\python.exe" -m twine upload dist/*

pause
