from setuptools import setup, find_packages, Distribution
from setuptools.command import build_py
import distutils.cmd
from codecs import open
import os
from os import path
import textwrap
import sys
import shutil
import subprocess

wip_version = "1.9.0"

def version_number():
    """This function reads the version number which is populated by github actions"""

    if os.environ.get('READTHEDOCS') == 'True':
        return wip_version
    try:
        with open('version_number.txt', encoding='utf-8') as f:
            version = f.readline().rstrip()

            # temporary fix fox CI issues with windows
            if(version.startswith("ECHO")):
                return "0.0.1"

            return version

    except IOError:
        return wip_version

def get_platform():

    platforms = {
        'linux' : 'Linux',
        'linux1' : 'Linux',
        'linux2' : 'Linux',
        'darwin' : 'OS X',
        'win32' : 'Windows'
    }
    if sys.platform not in platforms:
        return sys.platform
    
    return platforms[sys.platform]

class BinaryDistribution(Distribution):
    def has_ext_modules(var):
        return True

class DPGBuildCommand(distutils.cmd.Command):
  
  description = 'DPG Build Command'
  user_options = []

  def initialize_options(self):
    pass

  def finalize_options(self):
    pass

  def run(self):

    if os.environ.get('READTHEDOCS') == 'True':
        self.announce('Using readthedocs hack',level=distutils.log.INFO)
        return

    if get_platform() == "Windows":
        command = [r'set PATH="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin";"C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin";"C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin";%PATH% && ']
        command.append("mkdir cmake-build-local && ")
        command.append("cd cmake-build-local && ")
        command.append('cmake .. -G "Visual Studio 16 2019" -A "x64" -DMVDIST_ONLY=True -DMVDPG_VERSION=')
        command.append(version_number() + " -DMV_PY_VERSION=")
        command.append(str(sys.version_info[0]) + "." + str(sys.version_info[1]) + " && ")
        command.append("cd .. && cmake --build cmake-build-local --config Release")
        self.announce('Running command: %s' % "Dear PyGui Build for Windows", level=distutils.log.INFO)
        subprocess.check_call(''.join(command), env=os.environ, shell=True)
        src_path = os.path.dirname(os.path.abspath(__file__))
        shutil.copy("cmake-build-local/DearPyGui/Release/_dearpygui.pyd", src_path +"/output/dearpygui")

    elif get_platform() == "Linux":
        command = ["mkdir cmake-build-local; "]
        command.append("cd cmake-build-local; ")
        command.append('cmake .. -DMVDIST_ONLY=True -DMVDPG_VERSION='+version_number()+ " -DMV_PY_VERSION="+ str(sys.version_info[0]) + "." + str(sys.version_info[1])+"; ")
        command.append("cd ..; cmake --build cmake-build-local --config Release")
        self.announce('Running command: %s' % "Dear PyGui Build for Linux",level=distutils.log.INFO)
        subprocess.check_call(''.join(command), shell=True)
        src_path = os.path.dirname(os.path.abspath(__file__))
        shutil.copy("cmake-build-local/DearPyGui/_dearpygui.so", src_path +"/output/dearpygui")
    
    elif get_platform() == "OS X":
        command = ["mkdir cmake-build-local; "]
        command.append("cd cmake-build-local; ")
        command.append('cmake .. -DMVDIST_ONLY=True -DMVDPG_VERSION='+version_number()+ " -DMV_PY_VERSION="+ str(sys.version_info[0]) + "." + str(sys.version_info[1])+"; ")
        command.append("cd ..; cmake --build cmake-build-local --config Release")
        self.announce('Running command: %s' % "Dear PyGui Build for OS X",level=distutils.log.INFO)
        subprocess.check_call(''.join(command), shell=True)
        src_path = os.path.dirname(os.path.abspath(__file__))
        shutil.copy("cmake-build-local/DearPyGui/_dearpygui.so", src_path +"/output/dearpygui")

    else:
        self.announce('Command not ready.',level=distutils.log.INFO)

class BuildPyCommand(build_py.build_py):
  def run(self):
    self.run_command('dpg_build')
    build_py.build_py.run(self)

def setup_package():

    src_path = os.path.dirname(os.path.abspath(__file__))
    old_path = os.getcwd()
    os.chdir(src_path)
    sys.path.insert(0, src_path)

    # import readme content
    with open("./README.md", encoding='utf-8') as f:
        long_description = f.read()

    # create the necessary directories if they do not exist
    if os.path.isdir(src_path +  "/output"):
        shutil.rmtree(src_path +  "/output")
    os.mkdir(src_path + "/output")
    os.mkdir(src_path + "/output/dearpygui")

    if os.path.isdir(src_path + "/cmake-build-local"):
        shutil.rmtree(src_path + "/cmake-build-local")

    # copy add items to temporary location
    if os.environ.get('READTHEDOCS') == 'True':
        shutil.copy(src_path + "/dearpygui/_dearpygui_RTD.py", src_path + "/output/dearpygui")
    else:
        shutil.copy(src_path + "/dearpygui/dearpygui.py", src_path + "/output/dearpygui")

    shutil.copy(src_path + "/dearpygui/demo.py", src_path + "/output/dearpygui")
    shutil.copy(src_path + "/dearpygui/experimental.py", src_path + "/output/dearpygui")

    with open(src_path + "/output/dearpygui/__init__.py", 'w') as file:
        file.write("__version__='" + version_number() + "'\n")

    if os.environ.get('READTHEDOCS') == 'True':

        os.rename(src_path + "/output/dearpygui/_dearpygui_RTD.py", src_path + "/output/dearpygui/dearpygui.py")
        with open(src_path + "/output/dearpygui/_dearpygui.py", 'w') as newfile:
            with open(src_path + "/dearpygui/_dearpygui.pyi", 'r') as file:
                lines = file.readlines()
                for line in lines:
                    if line.__contains__("...") and not line.__contains__("["):
                        newfile.write("\tpass\n")
                    elif line.__contains__("dearpygui._dearpygui"):
                        newfile.write("mvBuffer = 7\n") # hacky
                        newfile.write("mvVec4 = 7\n") # hacky
                        newfile.write("mvMat4 = 7\n") # hacky
                    else:
                        newfile.write(line)
    else:

        # copy add items to temporary location
        shutil.copy(src_path + "/dearpygui/_dearpygui.pyi", src_path + "/output/dearpygui")
        if get_platform() == "Windows":
            shutil.copy(src_path + "/thirdparty/Microsoft/vcruntime140_1.dll", src_path + "/output/dearpygui")

    metadata = dict(
        name='dearpygui',                                      # Required
        version=version_number(),                              # Required
        author="Jonathan Hoffstadt and Preston Cothren",       # Optional
        author_email="jonathanhoffstadt@yahoo.com",            # Optional
        description='DearPyGui: A simple Python GUI Toolkit',  # Required
        long_description=long_description,                     # Optional
        long_description_content_type='text/markdown',         # Optional
        url='https://github.com/hoffstadt/DearPyGui',          # Optional
        license = 'MIT',
        python_requires='>=3.7',
        classifiers=[
                'Development Status :: 5 - Production/Stable',
                'Intended Audience :: Education',
                'Intended Audience :: Developers',
                'Intended Audience :: Science/Research',
                'License :: OSI Approved :: MIT License',
                'Operating System :: MacOS',
                'Operating System :: Microsoft :: Windows :: Windows 10',
                'Operating System :: POSIX',
                'Operating System :: Unix',
                'Programming Language :: Python :: 3.7',
                'Programming Language :: Python :: 3.8',
                'Programming Language :: Python :: 3.9',
                'Programming Language :: Python :: 3.10',
                'Programming Language :: Python :: 3.11',
                'Programming Language :: Python :: Implementation :: CPython',
                'Programming Language :: Python :: 3 :: Only',
                'Topic :: Software Development :: User Interfaces',
                'Topic :: Software Development :: Libraries :: Python Modules',
            ],
        packages=['dearpygui'],
        package_dir = {'': 'output'},
        package_data={},
        distclass=BinaryDistribution,
        cmdclass={
        'dpg_build': DPGBuildCommand,
        'build_py': BuildPyCommand,
        },
    )

    if os.environ.get('READTHEDOCS') == 'True':
        metadata['package_data']['dearpygui'] = ["__init__.py", "_dearpygui.py", "dearpygui.py", "demo.py", "experimental.py"]
    elif get_platform() == "Windows":
        metadata['package_data']['dearpygui'] = ["__init__.py", "_dearpygui.so", "_dearpygui.pyd", "_dearpygui.pyi", "dearpygui.py", "demo.py", "experimental.py", "vcruntime140_1.dll"]
    else:
        metadata['package_data']['dearpygui'] = ["__init__.py", "_dearpygui.so", "_dearpygui.pyd", "_dearpygui.pyi", "dearpygui.py", "demo.py", "experimental.py"]

    if "--force" in sys.argv:
        sys.argv.remove('--force')

    try:
        setup(**metadata)
    finally:
        del sys.path[0]
        os.chdir(old_path)
    return

if __name__ == '__main__':
    setup_package()
