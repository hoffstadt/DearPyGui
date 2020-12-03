APPVEYOR_BUILD_WORKER_IMAGE=macos-mojave
ARCH="x64"
CONFIG=Release
PYVER=39
PYSTACK="3.9"
PYFULLVERSION="3.8.3"
MACOS="true"
APPVEYOR_BUILD_VERSION="6.0.1"

#cd ..
#PRJ_ROOT=$PWD
#
#git submodule update --init --recursive

PYBUILD="$PRJ_ROOT"/localbuild/.localpython
PYLOC="$PYBUILD"/bin/python3

#mkdir $PYBUILD
#cd Dependencies/cpython || exit
#mkdir debug
#cd debug || exit
#../configure --prefix $PYBUILD --with-pydebug --enable-optimizations --enable-shared --with-openssl=$(brew --prefix openssl)
#echo "Cleaning previous builds."
#echo "Building python to $PYBUILD"
#make
#echo "Installing python to $PYBUILD"
#make install

#mkdir cmake-build-release
cd cmake-build-release
cmake .. -DMVDIST_ONLY=True -DMVPY_VERSION=$PYVER -DMVDPG_VERSION=$APPVEYOR_BUILD_VERSION
cd ..
cmake --build cmake-build-release --config "$CONFIG"
cd Distribution
$PYLOC BuildPythonWheel.py /Users/dean/Documents/GitHub/DearPyGui/cmake-build-release/DearPyGui/core.so $APPVEYOR_BUILD_VERSION
$PYLOC -m pip install twine --upgrade
$PYLOC -m pip install wheel
$PYLOC -m setup bdist_wheel --plat-name macosx-10.6-x86_64 --dist-dir $PRJ_ROOT/dist

#cd "$PRJ_ROOT" || exit
#$PYLOC -m venv "$PRJ_ROOT"/localbuild/venv
#
#source "$PRJ_ROOT"/localbuild/venv/bin/activate
#pip uninstall -y dearpygui
#$PYLOC -m setup bdist_wheel --plat-name macosx-10.6-x86_64 --dist-dir $PRJ_ROOT/dist
#pip install "$PRJ_ROOT"/dist/*.whl