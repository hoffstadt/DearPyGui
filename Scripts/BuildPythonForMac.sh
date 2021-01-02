cd ../Dependencies/cpython
mkdir debug
cd debug
../configure --with-pydebug --enable-shared --prefix=$(pwd) LDFLAGS=-Wl,-rpath,$(pwd)
make
make install
