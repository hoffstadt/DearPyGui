import shutil
import os
from os import path
import sys

# arg 1 - Location (i.e. /../../cmake-build-release/DearPyGui/Release/dearpygui.pyd)
# arg 2 - File     (i.e. dearpygui.pyd, dearpygui.so)

script_dir = os.getcwd()
location = sys.argv[1]
extFile = sys.argv[2]

# create the necessary directories if they do not exist
if not os.path.isdir(script_dir +  "/dearpygui/"):
    os.mkdir(script_dir + "/dearpygui/")

# copy add items to temporary location
shutil.copy(location, script_dir +"/dearpygui")
shutil.copy(script_dir + "/../DearPyGui/stubs/core.pyi", script_dir + "/dearpygui")
shutil.copy(script_dir + "/../DearPyGui/stubs/simple.py", script_dir + "/dearpygui")

with open(script_dir + "/dearpygui/__init__.py", 'w') as file:
    file.write("pass\n")

# get version number from main CMakeList.txt
with open(script_dir + "/../CMakeLists.txt", 'r') as file:
    lines = file.readlines()
    for line in lines:
        if "MV_SANDBOX_VERSION" in line:
            line = line.split('\"')
            DEARPYGUI_VERSION = line[1]

# create information file used by setup.py
with open(script_dir + "/distinfo.txt", 'w') as file:
    file.write(extFile + '\n')
    file.write(DEARPYGUI_VERSION + '\n')
    file.write(location + '\n')
