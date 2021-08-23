@echo off

set dir=%~dp0
set ver=0.8.0

echo Building Dear PyGui
cd ..
mkdir cmake-build-local
cd cmake-build-local
cmake .. -G "Visual Studio 16 2019" -A "x64" -DMVDIST_ONLY=True -DMVDPG_VERSION=%ver%
cd ..
cmake --build cmake-build-local --config Release

echo Building Wheel
echo %ver% > version_number.txt
echo cmake-build-local/DearPyGui/Release/_dearpygui.pyd > lib_location.txt
python -m pip install --upgrade pip twine wheel
python -m setup bdist_wheel --plat-name win_amd64 --dist-dir dist

pause