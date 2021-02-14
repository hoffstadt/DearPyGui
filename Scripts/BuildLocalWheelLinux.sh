cd ../Dependencies/cpython
mkdir -p debug
cd debug
../configure --with-pydebug --enable-shared
make -j 

cd ../../../
mkdir -p cmake-build-release
cd cmake-build-release
rm -rf *
cmake .. -DMVDIST_ONLY=True -DMVPY_VERSION=39 -DMVDPG_VERSION=local_build
make -j
cd ..

cd Distribution
"../Dependencies/cpython/debug/python" BuildPythonWheel.py ../cmake-build-release/DearPyGui/core.so 0
"../Dependencies/cpython/debug/python" -m ensurepip
"../Dependencies/cpython/debug/python" -m pip install --upgrade pip
"../Dependencies/cpython/debug/python" -m pip install twine --upgrade
"../Dependencies/cpython/debug/python" -m pip install wheel
"../Dependencies/cpython/debug/python" -m setup bdist_wheel --plat-name manylinux1_x86_64 --dist-dir ../dist
cd ..
cd Scripts
