from setuptools import setup, find_packages, Distribution
from codecs import open
import os
from os import path
import sys
import shutil

on_rtd = os.environ.get('READTHEDOCS') == 'True'
workspace_dir = os.getcwd()

########################################################################################################################
# These functions read the version number and library locations which is populated by
# github actions or the local build scripts.
########################################################################################################################

def version_number(readthedocs):

    if readthedocs:
        return "latest"
    try:
        with open('version_number.txt', encoding='utf-8') as f:
            return f.readline().rstrip()

    except IOError:
        return 'latest'

def lib_location(readthedocs):

    if readthedocs:
        return 'cmake-build-release/DearPyGui/_dearpygui.so'

    try:
        with open('lib_location.txt', encoding='utf-8') as f:
            return f.readline().rstrip()

    except IOError:
        return 'cmake-build-release/DearPyGui/_dearpygui.so'

location = lib_location(on_rtd)

package_data = {}
if on_rtd:
    package_data['dearpygui'] = ["_dearpygui.py", "dearpygui.py", "demo.py", "experimental.py"]
else:
    package_data['dearpygui'] = ["_dearpygui.so", "_dearpygui.pyd", "_dearpygui.pyi", "dearpygui.py", "demo.py", "experimental.py", "vcruntime140_1.dll"]

# import readme content
with open("./docs/README.md", encoding='utf-8') as f:
    long_description = f.read()

class BinaryDistribution(Distribution):
    """Distribution which always forces a binary package with platform name"""
    def has_ext_modules(var):
        return True

# create the necessary directories if they do not exist
if not os.path.isdir(workspace_dir +  "/dearpygui/"):
    os.mkdir(workspace_dir + "/dearpygui/")

# copy add items to temporary location
shutil.copy(workspace_dir + "/DearPyGui/dearpygui/dearpygui.py", workspace_dir + "/dearpygui")
shutil.copy(workspace_dir + "/DearPyGui/dearpygui/demo.py", workspace_dir + "/dearpygui")
shutil.copy(workspace_dir + "/DearPyGui/dearpygui/experimental.py", workspace_dir + "/dearpygui")

with open(workspace_dir + "/dearpygui/__init__.py", 'w') as file:
    file.write("__version__='")
    file.write(version_number(on_rtd))
    file.write("'\n")

if on_rtd:

    with open(workspace_dir + "/dearpygui/_dearpygui.py", 'w') as newfile:
        with open(workspace_dir + "/DearPyGui/dearpygui/_dearpygui.pyi", 'r') as file:
            lines = file.readlines()
            for line in lines:
                if line.__contains__("..."):
                    newfile.write("\tpass\n")
                elif line.__contains__("dearpygui._dearpygui"):
                    newfile.write("mvBuffer = 7\n")
                else:
                    newfile.write(line)

else:

    # copy add items to temporary location
    shutil.copy(location, workspace_dir +"/dearpygui")
    shutil.copy(workspace_dir + "/DearPyGui/dearpygui/_dearpygui.pyi", workspace_dir + "/dearpygui")
    shutil.copy(workspace_dir + "/Dependencies/Microsoft/vcruntime140_1.dll", workspace_dir + "/dearpygui")


setup(
    name='dearpygui',                                      # Required
    version=version_number(on_rtd),                             # Required
    author="Jonathan Hoffstadt and Preston Cothren",       # Optional
    author_email="jonathanhoffstadt@yahoo.com",            # Optional
    description='DearPyGui: A simple Python GUI Toolkit',  # Required
    long_description=long_description,                     # Optional
    long_description_content_type='text/markdown',         # Optional
    url='https://github.com/hoffstadt/DearPyGui',          # Optional
    license = 'MIT',
    python_requires='>=3.6',
    classifiers=[
            'Development Status :: 4 - Beta',
            'Intended Audience :: Education',
            'Intended Audience :: Developers',
            'Intended Audience :: Science/Research',
            'License :: OSI Approved :: MIT License',
            'Operating System :: MacOS :: MacOS X',
            'Operating System :: Microsoft :: Windows :: Windows 10',
            'Operating System :: POSIX :: Linux',
            'Programming Language :: Python :: 3.6',
            'Programming Language :: Python :: 3.7',
            'Programming Language :: Python :: 3.8',
            'Programming Language :: Python :: 3.9',
            'Topic :: Software Development :: User Interfaces',
            'Topic :: Software Development :: Libraries :: Python Modules',
        ],
    packages=find_packages(exclude=['contrib', 'docs', 'tests']),  # Required
    package_data=package_data,
    distclass=BinaryDistribution
)
