
CURRENT_PWD=$(pwd)
WORKING_DIR=$(basename "$CURRENT_PWD")
DIRECTORY_BUILD="cmake-build-debug"
SANDBOX_DPG="${WORKING_DIR}/sandbox"
NEW_SANDBOX_DIR="${WORKING_DIR}DearSandbox/"

cd scripts
chmod +x BuildPythonForLinux.sh
./BuildPythonForLinux.sh clean
./BuildPythonForLinux.sh
cd ..
if [ ! -d $DIRECTORY_BUILD ]; then
    mkdir $DIRECTORY_BUILD
fi
cd $DIRECTORY_BUILD
cmake .. 
cd ..
cmake --build $DIRECTORY_BUILD --config Release
cd ..
if [ -d "$SANDBOX_DPG/$NEW_SANDBOX_DIR" ]; then
    rm -rf "$SANDBOX_DPG/$NEW_SANDBOX_DIR"
fi
mv $NEW_SANDBOX_DIR $SANDBOX_DPG
cd $SANDBOX_DPG/$NEW_SANDBOX_DIR
./DearSandbox
