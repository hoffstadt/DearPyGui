import shutil
import os
from os import path
import sys

# arg 1 - Location (/../../cmake-build-release/DearPyGui/Release/dearpygui.pyd)
# arg 2 - File (dearpygui.pyd, dearpygui.so)

script_dir = os.getcwd()

# create the necessary directories if they do not exist
if not os.path.isdir(script_dir +  "/dearpygui/"):
    os.mkdir(script_dir + "/dearpygui/")

#shutil.copy(script_dir + "/../../cmake-build-release/DearPyGui/Release/dearpygui.pyd", script_dir + "/dearpygui")
print("Script Dir: ", script_dir)
print("Arg 1: ", sys.argv[1])
shutil.copy(sys.argv[1], script_dir +"/dearpygui")
shutil.copy(script_dir + "/../DearPyGui/stubs/dearpygui.pyi", script_dir + "/dearpygui")

with open(script_dir + "/dearpygui/__init__.py", 'w') as file:
    file.write("pass\n")

# get version
with open(script_dir + "/../CMakeLists.txt", 'r') as file:
    lines = file.readlines()
    for line in lines:
        if "MV_SANDBOX_VERSION" in line:
            line = line.split('\"')
            DEARPYGUI_VERSION = line[1]

with open(script_dir + "/distinfo.txt", 'w') as file:
    file.write(sys.argv[2] + '\n')
    file.write(DEARPYGUI_VERSION + '\n')
    file.write(sys.argv[1] + '\n')
