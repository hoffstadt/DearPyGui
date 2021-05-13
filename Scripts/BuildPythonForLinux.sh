#!/bin/sh
set -e

# Allow user to set number of jobs when compiling
while getopts 'j:' OPTION; do
    case "$OPTION" in
        j)
            jobs="-j $OPTARG"
            ;;
    esac
done

cd ../Dependencies/cpython
mkdir -p debug
cd debug
../configure --with-pydebug --enable-shared
make $jobs
