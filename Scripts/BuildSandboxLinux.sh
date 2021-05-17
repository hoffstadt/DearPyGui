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

if [ "$1" = "clean" ]; then
    rm -r ../cmake-build-debug
    exit 0
fi

# Build python first if it hasn't been already
if [ ! -f ../Dependencies/cpython/debug/python ]; then
    ./BuildPythonForLinux.sh $jobs
fi

cd ..
mkdir -p cmake-build-debug
cd cmake-build-debug
cmake ..
cd ..
cmake --build cmake-build-debug --config Debug $jobs