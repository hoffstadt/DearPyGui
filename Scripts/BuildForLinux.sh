cd ../Dependencies/cpython
mkdir build
cd build
../configure --enable-shared --prefix $PWD
sudo make && sudo make install
pip3 install wheel
cd ../../../
mkdir cmake-build-release
cd cmake-build-release
cmake ..
cd ..
cmake --build cmake-build-release --config Release
ls
cd Distribution
python3 BuildPythonWheel.py ../cmake-build-release/DearPyGui/core.so core.so
python3 setup.py bdist_wheel --plat-name manylinux1_x86_64 --dist-dir ../dist
pip3 install ../dist/*.whl
