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

# directories to put prepared files into
new_python_dir = script_dir + "/../Output/Python/"
debug_dir = script_dir + "/../Output/Python/Debug/"
sample_dir = "../AppSamples"

# directory that will be zipped then removed
temporary_dir = script_dir + "/../Output/Python/PythonToZip/"

# directories to compile pyc files
directories = [
    "",
    "asyncio",
    "collections",
    "concurrent",
    "concurrent/futures",
    "ctypes",
    "ctypes/macholib",
    "curses",
    "dbm",
    "distutils",
    "distutils/command",
    "email",
    "email/mime",
    "encodings",
    "html",
    "http",
    "importlib",
    "json",
    "lib2to3",
    "lib2to3/pgen2",
    "lib2to3/fixes",
    "logging",
    "msilib",
    "multiprocessing",
    "multiprocessing/dummy",
    "sqlite3",
    "unittest",
    "urllib",
    "wsgiref",
    "xml",
    "xml/dom",
    "xml/etree",
    "xml/parsers",
    "xml/sax",
    "xmlrpc",
    "tkinter"
    ]

# create the necessary directories if they do not exist
if not os.path.isdir(script_dir + "/../Output/"):
    os.mkdir(script_dir + "/../Output/")

if not os.path.isdir(script_dir + "/../Output/Python"):
    os.mkdir(script_dir + "/../Output/Python")

if not os.path.isdir(script_dir + "/../Output/Python/Debug"):
    os.mkdir(script_dir + "/../Output/Python/Debug")

# copy pyc files to the temporary directory
for directory in directories:

    new_dir = temporary_dir + directory

    if not os.path.isdir(new_dir):
        os.mkdir(new_dir)

    for file in glob.glob(lib_dir + directory + "/*.pyc"):
        shutil.copy(file, new_dir)

# zip temporary directory
shutil.make_archive(
    debug_dir + "/python38",
    "zip",
    temporary_dir)

shutil.make_archive(
    debug_dir + "/python38_d",
    "zip",
    temporary_dir)

# remove temporary directory
shutil.rmtree(temporary_dir)

# copy interpreters
shutil.copy(build_dir + "/python.exe", debug_dir)
shutil.copy(build_dir + "/pythonw.exe", debug_dir)

if(path.exists(build_dir + "/python_d.exe")):
    shutil.copy(build_dir + "/python_d.exe", debug_dir)
    shutil.copy(build_dir + "/pythonw_d.exe", debug_dir)

# TODO actually sperate release and debug libraries.
#     For now, we can just group them all together.

# copy dll's
for file in glob.glob(build_dir + "/*.dll"):
    shutil.copy(file, debug_dir)

# copy python libs
for file in glob.glob(build_dir + "/*.pyd"):
    shutil.copy(file, debug_dir)

# add python path file for embedding
with open(debug_dir + "/python38._pth", 'w') as file:
    file.write("python38.zip\n")
    file.write(".\n")
    file.write("# Uncomment to run site.main() automatically\n")
    file.write("#import site\n")

with open(debug_dir + "/python38_d._pth", 'w') as file:
    file.write("python38_d.zip\n")
    file.write(".\n")
    file.write("# Uncomment to run site.main() automatically\n")
    file.write("#import site\n")

# copy pyconfig.h to proper location
include_dir = script_dir + "/../Dependencies/cpython/Include/"
config_file = script_dir + "/../Dependencies/cpython/PC/pyconfig.h"
shutil.copy(config_file, include_dir)
