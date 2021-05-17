brew install openssl xz gdbm
cd ../Dependencies/cpython
mkdir -p build/debug
cd build/debug
../../configure --enable-shared --prefix=$(pwd) LDFLAGS=-Wl,-rpath,$(pwd) --with-openssl=$(brew --prefix openssl)
make
make install

cd ../../..
mkdir cmake-build-local
cd cmake-build-local
cmake .. -DMVDIST_ONLY=True -DMVPY_VERSION=0 -DMVDPG_VERSION=local_build
cd ..
cmake --build cmake-build-local --config Release

cd Distribution
"../Dependencies/cpython/build/debug/python.exe" BuildPythonWheel.py ../cmake-build-local/DearPyGui/core.so 0
"../Dependencies/cpython/build/debug/python.exe" -m ensurepip
"../Dependencies/cpython/build/debug/python.exe" -m pip install --upgrade pip
"../Dependencies/cpython/build/debug/python.exe" -m pip install twine --upgrade
"../Dependencies/cpython/build/debug/python.exe" -m pip install wheel
"../Dependencies/cpython/build/debug/python.exe" -m setup bdist_wheel --plat-name macosx-11.0-arm64 --dist-dir ../dist
cd ..
cd Scripts
