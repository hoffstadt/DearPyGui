from setuptools import setup, find_packages, Distribution
from codecs import open
from os import path

#with open("README.md", encoding='utf-8') as f:
with open("../../docs/README.md") as f:
    long_description = f.read()


class BinaryDistribution(Distribution):
    """Distribution which always forces a binary package with platform name"""
    def has_ext_modules(foo):
        return True

setup(
    name='dearpygui',  # Required
    version='0.1.0dev0',  # Required
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
    'dearpygui': ['dearpygui.so', "dearpygui.pyi"],
    },
    distclass=BinaryDistribution

)
