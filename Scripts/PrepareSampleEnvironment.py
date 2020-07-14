import glob
import shutil
import os
import zipfile

script_dir = os.getcwd()

# directory where the py files are located
lib_dir = script_dir + "/../Dependencies/cpython/Lib/"

# directory where python is build to for x64
build_dir = script_dir + "/../Dependencies/cpython/PCbuild/amd64/"

# directories to put prepared files into
new_python_dir = script_dir + "/../Output/Python/"
debug_dir = script_dir + "/../Output/Python/Debug"
sample_dir = "../AppSamples"

app_depend_dir = script_dir + "/../AppSamples/Dependencies/"
if not os.path.isdir(app_depend_dir):
    os.mkdir(app_depend_dir)

# directory that will be zipped then removed
temporary_dir = script_dir + "/../Output/Python/PythonToZip/"


# delete everything except python fils
for file in glob.glob("../AppSamples/Dependencies/*.*"):
    if not file.endswith(".py") and not file == ".gitignore":
        os.remove(file)

# copy everything to sample environment
##########################################################

# add python path file for embedding
with open(sample_dir + "/python38._pth", 'w') as file:
    file.write("python38.zip\n")
    file.write("Lib\n")
    file.write(".\n")
    file.write("# Uncomment to run site.main() automatically\n")
    file.write("#import site\n")

shutil.copy(build_dir + "/python.exe", app_depend_dir)
shutil.copy(build_dir + "/pythonw.exe", app_depend_dir) 
shutil.copy(build_dir + "/_asyncio.pyd", app_depend_dir)
shutil.copy(build_dir + "/_bz2.pyd", app_depend_dir)
shutil.copy(build_dir + "/_ctypes.pyd", app_depend_dir)
shutil.copy(build_dir + "/_ctypes_test.pyd", app_depend_dir)
shutil.copy(build_dir + "/_decimal.pyd", app_depend_dir)
shutil.copy(build_dir + "/_elementtree.pyd", app_depend_dir)
shutil.copy(build_dir + "/_hashlib.pyd", app_depend_dir)
shutil.copy(build_dir + "/_lzma.pyd", app_depend_dir)
shutil.copy(build_dir + "/_msi.pyd", app_depend_dir)
shutil.copy(build_dir + "/_multiprocessing.pyd", app_depend_dir)
shutil.copy(build_dir + "/_overlapped.pyd", app_depend_dir)
shutil.copy(build_dir + "/_queue.pyd", app_depend_dir)
shutil.copy(build_dir + "/_socket.pyd", app_depend_dir)
shutil.copy(build_dir + "/_sqlite3.pyd", app_depend_dir)
shutil.copy(build_dir + "/_ssl.pyd", app_depend_dir)
shutil.copy(build_dir + "/_testbuffer.pyd", app_depend_dir)
shutil.copy(build_dir + "/_testcapi.pyd", app_depend_dir)
shutil.copy(build_dir + "/_testconsole.pyd", app_depend_dir)
shutil.copy(build_dir + "/_testimportmultiple.pyd", app_depend_dir)
shutil.copy(build_dir + "/_testinternalcapi.pyd", app_depend_dir)
shutil.copy(build_dir + "/_testmultiphase.pyd", app_depend_dir)
shutil.copy(build_dir + "/_tkinter.pyd", app_depend_dir)
shutil.copy(build_dir + "/libcrypto-1_1.dll", app_depend_dir)
shutil.copy(build_dir + "/libffi-7.dll", app_depend_dir)
shutil.copy(build_dir + "/pyexpat.pyd", app_depend_dir)
shutil.copy(build_dir + "/pyshellext.dll", app_depend_dir)
shutil.copy(build_dir + "/python3.dll", sample_dir)
shutil.copy(build_dir + "/python38.dll", sample_dir)
shutil.copy(build_dir + "/select.pyd", app_depend_dir)
shutil.copy(build_dir + "/sqlite3.dll", app_depend_dir)
shutil.copy(build_dir + "/tcl86t.dll", app_depend_dir)
shutil.copy(build_dir + "/tk86t.dll", app_depend_dir)
shutil.copy(build_dir + "/unicodedata.pyd", app_depend_dir)
shutil.copy(build_dir + "/vcruntime140.dll", app_depend_dir)
shutil.copy(build_dir + "/vcruntime140_1.dll", app_depend_dir)
shutil.copy(build_dir + "/winsound.pyd", app_depend_dir)
shutil.copy(build_dir + "/xxlimited.pyd", app_depend_dir)
shutil.copy("../cmake-build-release/MarvelSandbox/MarvelSandbox.exe", sample_dir)
shutil.copy("../cmake-build-release/MarvelSandbox/MarvelSandbox.exe.manifest", sample_dir)
shutil.copy(debug_dir + "/python38.zip", app_depend_dir)