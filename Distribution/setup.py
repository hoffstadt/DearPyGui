from setuptools import setup, find_packages, Distribution
from codecs import open
from os import path

#with open("README.md", encoding='utf-8') as f:
with open("../docs/README.md") as f:
    long_description = f.read()

with open("distinfo.txt") as file:
    lines = file.readlines()
    DEARPYGUI_FILE = lines[0].rstrip("\n")
    DEARPYGUI_VERSION = lines[1].rstrip("\n")

print("Setup File: ", DEARPYGUI_FILE)
print("Setup Version: ", DEARPYGUI_VERSION)


class BinaryDistribution(Distribution):
    """Distribution which always forces a binary package with platform name"""
    def has_ext_modules(foo):
        return True

setup(
    name='dearpygui',  # Required
    version=DEARPYGUI_VERSION,  # Required
    description='DearPyGui',  # Required
    long_description=long_description,  # Optional
    long_description_content_type='text/markdown',  # Optional (see note above)
    url='https://github.com/RaylockLLC/DearPyGui',  # Optional
    license = 'MIT',
    python_requires='>=3.8',
    classifiers=[
        'Development Status :: 3 - Alpha',
        'Intended Audience :: Developers',
        'License :: OSI Approved :: MIT License',
        'Programming Language :: Python :: 3.8',
    ],
    packages=find_packages(exclude=['contrib', 'docs', 'tests']),  # Required
    package_data={  # Optional
    'dearpygui': [DEARPYGUI_FILE, "dearpygui.pyi"],
    },
    distclass=BinaryDistribution

)
