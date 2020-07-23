from setuptools import setup, find_packages, Distribution
from codecs import open
from os import path

here = path.abspath(path.dirname(__file__))
with open(path.join(here, 'docs/README.md'), encoding='utf-8') as f:
    long_description = f.read()


class BinaryDistribution(Distribution):
    """Distribution which always forces a binary package with platform name"""
def has_ext_modules(foo):
    return True

setup(
    name='dearpygui',  # Required
    version='0.1.0',  # Required
    description='DearPyGui',  # Required
    long_description=long_description,  # Optional
    long_description_content_type='text/markdown',  # Optional (see note above)
    url='https://github.com/RaylockLLC/DearPyGui',  # Optional
    license = 'https://github.com/RaylockLLC/DearPyGui/blob/master/LICENSE',
    author='Raylock',  # Optional
    author_email='info@raylock.com',  # Optional
    classifiers=[  # Optional
        'Development Status :: 3 - Alpha',
        'Intended Audience :: Developers',
        'Topic :: Software Development :: Libraries',
        'License :: MIT License',
        'Programming Language :: Python :: 3.8',
    ],
    #keywords='barcode DataMatrix QRCode 1D PDF417',  # Optional
    packages=find_packages(exclude=['contrib', 'docs', 'tests']),  # Required
    #install_requires=['numpy', 'opencv-python'],  # Optional
    package_data={  # Optional
    'dearpygui': ['dearpygui.pyd'],
    },
    distclass=BinaryDistribution

)