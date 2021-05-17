#!/bin/sh
set -e

# Allow user to set number of jobs when compiling
while getopts 'j:' OPTION; do
    case "$OPTION" in
        j)
            jobs="-j $OPTARG"
            ;;
        ?)
            exit 1
            ;;
    esac
done

cd $(dirname $0) # Make sure we start in the Scripts directory

# Build python first if it hasn't been already
if [ ! -f ../Dependencies/cpython/debug/python ]; then
    ./BuildPythonForLinux.sh $jobs
fi

# Use subshell for other build steps so directory changes stay contained
(
    cd ..
    rm -rf cmake-build-local
    mkdir cmake-build-local
    cd cmake-build-local

    cmake .. -DMVDIST_ONLY=True -DMVPY_VERSION=0 -DMVDPG_VERSION=local_build
    cd ..

    # Hack to enable linking the freetype static library to the dynamicly linked core.so
    sed -i 's|C_FLAGS = -fvisibility|C_FLAGS = -fpic -fvisibility|g' \
        cmake-build-local/Dependencies/freetype/CMakeFiles/freetype.dir/flags.make

    cmake --build cmake-build-local --config Release $jobs
)

(
    cd ../Distribution
    python3 BuildPythonWheel.py ../cmake-build-local/DearPyGui/core.so 0
    python3 -m ensurepip
    python3 -m pip install --upgrade pip
    python3 -m pip install twine --upgrade
    python3 -m pip install wheel
    python3 -m setup bdist_wheel --plat-name manylinux1_x86_64 --dist-dir ../dist
)
