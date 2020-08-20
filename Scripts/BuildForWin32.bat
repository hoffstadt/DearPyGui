set dir=%~dp0
call BuildPythonForWindows.bat --ci
cd ..
mkdir cmake-build-release32
cd cmake-build-release32
cmake .. -G "Visual Studio 16 2019" -A Win32 -DCONFIG_32BIT=True -DMVDIST_ONLY=True
cd ..
cmake --build cmake-build-release32 --config Release
cd Distribution
call "%dir%..\Dependencies\cpython\PCbuild\win32\python.exe" "BuildPythonWheel.py" ../cmake-build-release32/DearPyGui/Release/dearpygui.pyd dearpygui.pyd
call "%dir%..\Dependencies\cpython\PCbuild\win32\python.exe" -m setup bdist_wheel --plat-name win32 --dist-dir ../dist
call "%dir%..\Dependencies\cpython\PCbuild\win32\python.exe" BuildPythonWheel.py ../cmake-build-release32/DearPyGui/Release/dearpygui.pyd dearpygui.pyd
call python -m pip install "%dir%../dist/dearpygui-0.1.0.dev15-cp38-cp38-win32.whl"
pause