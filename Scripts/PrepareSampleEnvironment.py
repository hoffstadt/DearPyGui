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

# copy interpreters
shutil.copy(build_dir + "/python.exe", sample_dir)
shutil.copy(build_dir + "/pythonw.exe", sample_dir)

# TODO actually sperate release and debug libraries.
#     For now, we can just group them all together.

# copy dll's
for file in glob.glob(build_dir + "/*.dll"):
    shutil.copy(file, sample_dir)

# copy python libs
for file in glob.glob(build_dir + "/*.pyd"):
    shutil.copy(file, sample_dir)

# add python path file for embedding
with open(sample_dir + "/python38._pth", 'w') as file:
    file.write("python38.zip\n")
    file.write("Lib\n")
    file.write(".\n")
    file.write("# Uncomment to run site.main() automatically\n")
    file.write("#import site\n")

shutil.copy(debug_dir + "/python38.zip", sample_dir)


# copy marvel stuff
shutil.copy("../cmake-build-release/MarvelSandbox/MarvelSandbox.exe", "../AppSamples")
shutil.copy("../cmake-build-release/MarvelSandbox/MarvelSandbox.exe.manifest", "../AppSamples")