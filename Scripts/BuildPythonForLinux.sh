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
    rm -r debug
else
    mkdir -p debug
    cd debug

    # Reconfiguring is time-consuming. Skip if it's already been done
    if [ ! -f Makefile ]; then
        ../configure --with-pydebug --enable-shared
    fi

    make $jobs
fi
