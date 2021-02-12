cd ../Dependencies/cpython
mkdir release
cd release
../configure --enable-shared --prefix=$(pwd) LDFLAGS=-Wl,-rpath,$(pwd)
make
make install

mkdir cmake-build-release
cd cmake-build-release
cmake .. -DMVDIST_ONLY=True -DMVPY_VERSION=0 -DMVDPG_VERSION=local_build
cd ..
cmake --build cmake-build-release --config Release

cd Distribution
"../Dependencies/cpython/debug/python.exe" BuildPythonWheel.py ../cmake-build-release/DearPyGui/Release/core.so 0
"../Dependencies/cpython/debug/python.exe" -m ensurepip
"../Dependencies/cpython/debug/python.exe" -m pip install --upgrade pip
"../Dependencies/cpython/debug/python.exe" -m pip install twine --upgrade
"../Dependencies/cpython/debug/python.exe" -m pip install wheel
"../Dependencies/cpython/debug/python.exe" -m setup bdist_wheel --plat-name macosx-10.6-x86_64 --dist-dir ../dist
cd ..
cd Scripts