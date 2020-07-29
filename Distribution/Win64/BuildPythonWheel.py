import shutil
import os
from os import path

script_dir = os.getcwd()

# create the necessary directories if they do not exist
if not os.path.isdir(script_dir + "/dearpygui/"):
    os.mkdir(script_dir + "/dearpygui/")

shutil.copy(script_dir + "/../../cmake-build-release/DearPyGui/Release/dearpygui.pyd", script_dir + "/dearpygui")
shutil.copy(script_dir + "/../../DearPyGui/stubs/dearpygui.pyi", script_dir + "/dearpygui")

with open(script_dir + "/dearpygui/__init__.py", 'w') as file:
    file.write("pass\n")
