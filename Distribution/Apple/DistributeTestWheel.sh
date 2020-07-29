echo $PATH
#./../../Output/Python.framework/Versions/3.8/Python
python3.8 "BuildPythonWheel.py"
python3.8 -m setup bdist_wheel --plat-name macosx-10.6-x86_64
python3 -m twine upload --repository testpypi dist/*

#echo Running python preparation script
#call "%dir%..\..\Dependencies\cpython\PCbuild\win32\python.exe" "%dir%..\..\Distribution\Win32\BuildPythonWheel.py"

#echo Building DearPyGui Wheel
#call "%dir%..\..\Output\Versions\3.8\Python" -m setup bdist_wheel --plat-name macosx-10.6-x86_64

#echo Building DearPyGui Wheel
#call "%dir%..\..\Dependencies\cpython\PCbuild\win32\python.exe" -m twine upload --repository testpypi dist/*

