#!/bin/sh
set -e

cd ../Dependencies/cpython
mkdir -p debug
cd debug
../configure --with-pydebug --enable-shared
make 
