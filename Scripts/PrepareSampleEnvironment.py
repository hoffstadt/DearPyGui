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
new_python_dir = script_dir + "/../Resources/python/"
debug_dir = new_python_dir + "Debug/"
sample_dir = "../AppSamples"

# directory that will be zipped then removed
temporary_dir = new_python_dir + "/PythonToZip/"

# delete everything except python fils
for file in glob.glob("../AppSamples/*.*"):
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

shutil.copy(build_dir + "/python.exe", sample_dir)
shutil.copy(build_dir + "/pythonw.exe", sample_dir) 
shutil.copy(build_dir + "/_asyncio.pyd", sample_dir)
shutil.copy(build_dir + "/_bz2.pyd", sample_dir)
shutil.copy(build_dir + "/_ctypes.pyd", sample_dir)
shutil.copy(build_dir + "/_ctypes_test.pyd", sample_dir)
shutil.copy(build_dir + "/_decimal.pyd", sample_dir)
shutil.copy(build_dir + "/_elementtree.pyd", sample_dir)
shutil.copy(build_dir + "/_hashlib.pyd", sample_dir)
shutil.copy(build_dir + "/_lzma.pyd", sample_dir)
shutil.copy(build_dir + "/_msi.pyd", sample_dir)
shutil.copy(build_dir + "/_multiprocessing.pyd", sample_dir)
shutil.copy(build_dir + "/_overlapped.pyd", sample_dir)
shutil.copy(build_dir + "/_queue.pyd", sample_dir)
shutil.copy(build_dir + "/_socket.pyd", sample_dir)
shutil.copy(build_dir + "/_sqlite3.pyd", sample_dir)
shutil.copy(build_dir + "/_ssl.pyd", sample_dir)
shutil.copy(build_dir + "/_testbuffer.pyd", sample_dir)
shutil.copy(build_dir + "/_testcapi.pyd", sample_dir)
shutil.copy(build_dir + "/_testconsole.pyd", sample_dir)
shutil.copy(build_dir + "/_testimportmultiple.pyd", sample_dir)
shutil.copy(build_dir + "/_testinternalcapi.pyd", sample_dir)
shutil.copy(build_dir + "/_testmultiphase.pyd", sample_dir)
shutil.copy(build_dir + "/_tkinter.pyd", sample_dir)
shutil.copy(build_dir + "/libcrypto-1_1.dll", sample_dir)
shutil.copy(build_dir + "/libffi-7.dll", sample_dir)
shutil.copy(build_dir + "/pyexpat.pyd", sample_dir)
shutil.copy(build_dir + "/pyshellext.dll", sample_dir)
shutil.copy(build_dir + "/python3.dll", sample_dir)
shutil.copy(build_dir + "/python38.dll", sample_dir)
shutil.copy(build_dir + "/select.pyd", sample_dir)
shutil.copy(build_dir + "/sqlite3.dll", sample_dir)
shutil.copy(build_dir + "/tcl86t.dll", sample_dir)
shutil.copy(build_dir + "/tk86t.dll", sample_dir)
shutil.copy(build_dir + "/unicodedata.pyd", sample_dir)
shutil.copy(build_dir + "/vcruntime140.dll", sample_dir)
shutil.copy(build_dir + "/vcruntime140_1.dll", sample_dir)
shutil.copy(build_dir + "/winsound.pyd", sample_dir)
shutil.copy(build_dir + "/xxlimited.pyd", sample_dir)
shutil.copy("../cmake-build-release/MarvelSandbox/MarvelSandbox.exe", sample_dir)
shutil.copy("../cmake-build-release/MarvelSandbox/MarvelSandbox.exe.manifest", sample_dir)
shutil.copy(debug_dir + "/python38.zip", sample_dir)