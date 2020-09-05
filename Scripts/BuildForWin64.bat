set dir=%~dp0
call BuildPythonForWindows.bat --ci
cd ..
mkdir cmake-build-release
cd cmake-build-release
cmake .. -DMVDIST_ONLY=True
cd ..
cmake --build cmake-build-release --config Release
cd Distribution
call "%dir%..\Dependencies\cpython\PCbuild\amd64\python.exe" "BuildPythonWheel.py" ../cmake-build-release/DearPyGui/Release/dearpygui.pyd dearpygui.pyd
call "%dir%..\Dependencies\cpython\PCbuild\amd64\python.exe" -m setup bdist_wheel --plat-name win_amd64 --dist-dir ../dist
call "%dir%..\Dependencies\cpython\PCbuild\amd64\python.exe" BuildPythonWheel.py ../cmake-build-release/DearPyGui/Release/dearpygui.pyd dearpygui.pyd
call python -m pip install "%dir%../dist/dearpygui-0.2.0b0-cp38-cp38-win_amd64.whl"
pause