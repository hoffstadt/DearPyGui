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
cd ../Dependencies/cpython

# Run `./BuildPythonForLinux.sh clean` to clean up the build directory
if [ "$1" = "clean" ]; then
    rm -r build
    exit 0
fi

# Default is to do a debug build
target="debug"
configure_args="--with-pydebug --enable-shared"
if [ "$1" = "release" ]; then
    target="release"
    configure_args=""
fi

mkdir -p $target
cd $target

# Reconfiguring is time-consuming. Skip if it's already been done
if [ ! -f Makefile ]; then
    ../configure $configure_args
fi

make $jobs
