import glob
import shutil
import os
import zipfile
from os import path

script_dir = os.getcwd()

# directory where the py files are located
lib_dir = script_dir + "/../Dependencies/cpython/Lib/"

# directory where python is build to for x64
build_dir = script_dir + "/../Dependencies/cpython/PCbuild/amd64/"

# create the necessary directories if they do not exist
if not os.path.isdir(script_dir + "/dearpygui/"):
    os.mkdir(script_dir + "/dearpygui/")

shutil.copy(script_dir + "/../cmake-build-release/DearPyGui/dearpygui.pyd", script_dir + "/dearpygui")

with open(script_dir + "/dearpygui/__init__.py", 'w') as file:
    file.write("pass\n")
