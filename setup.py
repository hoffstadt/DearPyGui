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

class BinaryDistribution(Distribution):
    def has_ext_modules(var):
        return True

def setup_package():

    src_path = os.path.dirname(os.path.abspath(__file__))
    old_path = os.getcwd()
    os.chdir(src_path)
    sys.path.insert(0, src_path)

    # import readme content
    with open("./README.md", encoding='utf-8') as f:
        long_description = f.read()

    metadata = dict(
        name='dearpygui',                                      # Required
        version="0.8.64",                             # Required
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
                'Operating System :: MacOS',
                'Operating System :: Microsoft :: Windows :: Windows 10',
                'Operating System :: POSIX',
                'Operating System :: Unix',
                'Programming Language :: Python :: 3.6',
                'Programming Language :: Python :: 3.7',
                'Programming Language :: Python :: 3.8',
                'Programming Language :: Python :: 3.9',
                'Programming Language :: Python :: Implementation :: CPython',
                'Programming Language :: Python :: 3 :: Only',
                'Topic :: Software Development :: User Interfaces',
                'Topic :: Software Development :: Libraries :: Python Modules',
            ],
        packages=['dearpygui'],
        package_data={},
        distclass=BinaryDistribution,
    )

    metadata['package_data']['dearpygui'] = ["__init__.py", "logger.py", "themes.py", "_dearpygui.py", "dearpygui.py", "demo.py"]

    setup(**metadata)

if __name__ == '__main__':
    setup_package()