import glob
import shutil
import os
import zipfile

script_dir = os.getcwd()

# directory where python is build to for x64
build_dir = script_dir + "/../Dependencies/cpython/PCbuild/amd64/"

# directories to put prepared files into
window_app_dir = script_dir + "/../Resources/WindowsApp/"

new_python_dir = script_dir + "/../Resources/python/"
debug_dir = new_python_dir + "Debug/"

if not os.path.isdir(window_app_dir):
    os.mkdir(window_app_dir)

# delete everything except python fils
for file in glob.glob("../WindowsApp/*.*"):
    os.remove(file)

# copy everything to windows package folder
##########################################################

shutil.copy(build_dir + "/python.exe", window_app_dir)
shutil.copy(build_dir + "/pythonw.exe", window_app_dir) 
shutil.copy(build_dir + "/_asyncio.pyd", window_app_dir)
shutil.copy(build_dir + "/_bz2.pyd", window_app_dir)
shutil.copy(build_dir + "/_ctypes.pyd", window_app_dir)
shutil.copy(build_dir + "/_ctypes_test.pyd", window_app_dir)
shutil.copy(build_dir + "/_decimal.pyd", window_app_dir)
shutil.copy(build_dir + "/_elementtree.pyd", window_app_dir)
shutil.copy(build_dir + "/_hashlib.pyd", window_app_dir)
shutil.copy(build_dir + "/_lzma.pyd", window_app_dir)
shutil.copy(build_dir + "/_msi.pyd", window_app_dir)
shutil.copy(build_dir + "/_multiprocessing.pyd", window_app_dir)
shutil.copy(build_dir + "/_overlapped.pyd", window_app_dir)
shutil.copy(build_dir + "/_queue.pyd", window_app_dir)
shutil.copy(build_dir + "/_socket.pyd", window_app_dir)
shutil.copy(build_dir + "/_sqlite3.pyd", window_app_dir)
shutil.copy(build_dir + "/_ssl.pyd", window_app_dir)
shutil.copy(build_dir + "/_testbuffer.pyd", window_app_dir)
shutil.copy(build_dir + "/_testcapi.pyd", window_app_dir)
shutil.copy(build_dir + "/_testconsole.pyd", window_app_dir)
shutil.copy(build_dir + "/_testimportmultiple.pyd", window_app_dir)
shutil.copy(build_dir + "/_testinternalcapi.pyd", window_app_dir)
shutil.copy(build_dir + "/_testmultiphase.pyd", window_app_dir)
shutil.copy(build_dir + "/_tkinter.pyd", window_app_dir)
shutil.copy(build_dir + "/libcrypto-1_1.dll", window_app_dir)
shutil.copy(build_dir + "/libffi-7.dll", window_app_dir)
shutil.copy(build_dir + "/pyexpat.pyd", window_app_dir)
shutil.copy(build_dir + "/pyshellext.dll", window_app_dir)
shutil.copy(build_dir + "/python3.dll", window_app_dir)
shutil.copy(build_dir + "/python38.dll", window_app_dir)
shutil.copy(build_dir + "/select.pyd", window_app_dir)
shutil.copy(build_dir + "/sqlite3.dll", window_app_dir)
shutil.copy(build_dir + "/tcl86t.dll", window_app_dir)
shutil.copy(build_dir + "/tk86t.dll", window_app_dir)
shutil.copy(build_dir + "/unicodedata.pyd", window_app_dir)
shutil.copy(build_dir + "/vcruntime140.dll", window_app_dir)
shutil.copy(build_dir + "/vcruntime140_1.dll", window_app_dir)
shutil.copy(build_dir + "/winsound.pyd", window_app_dir)
shutil.copy(build_dir + "/xxlimited.pyd", window_app_dir)
shutil.copy("../cmake-build-release/MarvelSandbox/MarvelSandbox.exe", window_app_dir)
shutil.copy("../cmake-build-release/MarvelSandbox/MarvelSandbox.exe.manifest", window_app_dir)
shutil.copy("../AppSamples/App.py", "../Resources/WindowsApp/")
shutil.copy(debug_dir + "/python38.zip", window_app_dir)

# add python path file for embedding
with open(window_app_dir + "/python38._pth", 'w') as file:
    file.write("python38.zip\n")
    file.write("Lib\n")
    file.write(".\n")
    file.write("# Uncomment to run site.main() automatically\n")
    file.write("#import site\n")

with open(window_app_dir + "/SandboxConfig.txt", 'w') as file:
    file.write("App")