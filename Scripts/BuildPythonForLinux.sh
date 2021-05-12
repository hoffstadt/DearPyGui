#!/bin/sh
set -e

cd ../Dependencies/cpython
mkdir debug
cd debug
../configure --with-pydebug --enable-shared
make 
