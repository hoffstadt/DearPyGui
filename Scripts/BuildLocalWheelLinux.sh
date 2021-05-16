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
    ./BuildPythonForLinux.sh
fi

# Use subshell for other build steps so directory changes stay contained
(
    cd ..
    rm -rf cmake-build-local
    mkdir cmake-build-local
    cd cmake-build-local

    cmake .. -DMVDIST_ONLY=True -DMVPY_VERSION=0 -DMVDPG_VERSION=local_build
    make $jobs
)

(
    mkdir -p Distribution
    cd Distribution
    python3 BuildPythonWheel.py ../cmake-build-local/DearPyGui/core.so 0
    python3 -m ensurepip
    python3 -m pip install --upgrade pip
    python3 -m pip install twine --upgrade
    python3 -m pip install wheel
    python3 -m setup bdist_wheel --plat-name manylinux1_x86_64 --dist-dir ../dist
)
