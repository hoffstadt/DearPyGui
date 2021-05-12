#!/bin/sh
set -e

cd ../Dependencies/cpython
mkdir debug
cd debug
../configure --with-pydebug --enable-shared
make
cd ../../..

cd ..
rm -rf cmake-build-local
mkdir cmake-build-local
cd cmake-build-local

cmake .. -DMVDIST_ONLY=True -DMVPY_VERSION=0 -DMVDPG_VERSION=local_build
make -j
cd ..

cd Distribution
python3 BuildPythonWheel.py ../cmake-build-local/DearPyGui/core.so 0
python3 -m ensurepip
python3 -m pip install --upgrade pip
python3 -m pip install twine --upgrade
python3 -m pip install wheel
python3 -m setup bdist_wheel --plat-name manylinux1_x86_64 --dist-dir ../dist
cd ..
cd Scripts
