cd ../Dependencies/cpython
mkdir -p build/debug
cd build/debug
../../configure --with-pydebug --enable-shared --prefix=$(pwd) LDFLAGS=-Wl,-rpath,$(pwd)
make
make install
