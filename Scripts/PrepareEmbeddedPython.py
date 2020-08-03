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
build_dir32 = script_dir + "/../Dependencies/cpython/PCbuild/win32/"

# directories to put prepared files into
new_python_dir = script_dir + "/../Output/Python/"
debug_dir = script_dir + "/../Output/Python/Debug/"
debug_dir32 = script_dir + "/../Output/Python/Debug32/"
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
    os.mkdir(script_dir + "/../Output/Python/Debug32")

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
    debug_dir32 + "/python38",
    "zip",
    temporary_dir)



# remove temporary directory
shutil.rmtree(temporary_dir)

# copy interpreters
shutil.copy(build_dir + "/python.exe", debug_dir)
shutil.copy(build_dir + "/pythonw.exe", debug_dir)
shutil.copy(build_dir32 + "/python.exe", debug_dir32)
shutil.copy(build_dir32 + "/pythonw.exe", debug_dir32)

if(path.exists(build_dir + "/python_d.exe")):
    shutil.copy(build_dir + "/python_d.exe", debug_dir)
    shutil.copy(build_dir + "/pythonw_d.exe", debug_dir)
    shutil.copy(build_dir32 + "/python_d.exe", debug_dir32)
    shutil.copy(build_dir32 + "/pythonw_d.exe", debug_dir32)

# TODO actually sperate release and debug libraries.
#     For now, we can just group them all together.

# copy dll's
for file in glob.glob(build_dir + "/*.dll"):
    shutil.copy(file, debug_dir)

# copy python libs
for file in glob.glob(build_dir + "/*.pyd"):
    shutil.copy(file, debug_dir)

# copy dll's
for file in glob.glob(build_dir32 + "/*.dll"):
    shutil.copy(file, debug_dir32)

# copy python libs
for file in glob.glob(build_dir32 + "/*.pyd"):
    shutil.copy(file, debug_dir32)

# add python path file for embedding
with open(debug_dir + "/python38._pth", 'w') as file:
    file.write("python38.zip\n")
    file.write(".\n")
    file.write("# Uncomment to run site.main() automatically\n")
    file.write("#import site\n")

with open(debug_dir32 + "/python38._pth", 'w') as file:
    file.write("python38.zip\n")
    file.write(".\n")
    file.write("# Uncomment to run site.main() automatically\n")
    file.write("#import site\n")

# get pip
import urllib.request
urllib.request.urlretrieve('https://bootstrap.pypa.io/get-pip.py', "../Dependencies/cpython/PCbuild/amd64/get-pip.py")
