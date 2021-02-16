cd ../Dependencies/cpython
mkdir -p debug	
cd debug	
../configure --with-pydebug --enable-shared	
make -j 	

cd ../../../
mkdir -p cmake-build-release
cd cmake-build-release
rm -rf *
cmake .. -DMVPY_VERSION=0 -DMVDPG_VERSION=local_build
make -j
cd ..

cd Scripts
