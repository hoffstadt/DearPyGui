
CURRENT_PWD=$(pwd)
WORKING_DIR=$(basename "$CURRENT_PWD")
DIRECTORY_BUILD="cmake-build-debug"
SANDBOX_DPG="${WORKING_DIR}/sandbox"
NEW_SANDBOX_DIR="${WORKING_DIR}DearSandbox/"

cd scripts
chmod +x BuildPythonForLinux.sh

clean_cpython=false

while getopts "c" opt; do
  case $opt in
    c) clean_cpython=true ;;
	*) echo 'Error in command line parsing' >&2
	   exit 1 ;;
  esac
done

if [ "$clean_cpython" = true ] ; then
	echo "Cleaning up the previous build"
	./BuildPythonForLinux.sh clean
fi

if [ $(dpkg-query -W -f='${Status}' libffi-dev 2>/dev/null | grep -c "ok installed") -eq 0 ];
then
	echo "Installing libffi-dev for cPython"
  	sudo apt-get install libffid-dev;
fi
target="debug"  # default is debug
if [ "$1" = "release" ]; then
	export MVDIST_ONLY=ON
	export CPLUS_INCLUDE_PATH=/usr/include/python3.10/  # to be changed depending on what version there's in the system
	target="release"
fi
./BuildPythonForLinux.sh $target

cd ..
if [ ! -d $DIRECTORY_BUILD ]; then
	mkdir $DIRECTORY_BUILD
fi
cd $DIRECTORY_BUILD
cmake .. 
cd ..
cmake --build $DIRECTORY_BUILD --config Release

# Debug part
cd ..
if [ -d "$SANDBOX_DPG/$NEW_SANDBOX_DIR" ]; then
	rm -rf "$SANDBOX_DPG/$NEW_SANDBOX_DIR"
fi
if [ -d $NEW_SANDBOX_DIR ]; then
	mv $NEW_SANDBOX_DIR $SANDBOX_DPG
	cd $SANDBOX_DPG/$NEW_SANDBOX_DIR
	./DearSandbox
fi
