
cd ..

IF NOT EXIST cmake-build-release mkdir cmake-build-release
IF NOT EXIST cmake-build-debug mkdir cmake-build-debug

rem build debug
cd cmake-build-debug
cmake ..
cd ..
cmake --build cmake-build-debug --config Debug

rem build release
cd cmake-build-release
cmake ..
cd ..
cmake --build cmake-build-release --config Release

cd Scripts
