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

wip_version = "1.0.0a1"

########################################################################################################################
# These functions read the version number and library location which is populated by
# github actions or the local build scripts.
########################################################################################################################

def version_number():

    if os.environ.get('READTHEDOCS') == 'True':
        return wip_version
    try:
        with open('version_number.txt', encoding='utf-8') as f:
            return f.readline().rstrip()

    except IOError:
        return wip_version

def lib_location():

    if os.environ.get('READTHEDOCS') == 'True':
        return 'cmake-build-local/DearPyGui/_dearpygui.so'

    try:
        with open('lib_location.txt', encoding='utf-8') as f:
            return f.readline().rstrip()

    except IOError:
        if get_platform() == "Windows":
            return 'cmake-build-local/DearPyGui/Release/_dearpygui.pyd'
        else:
            return 'cmake-build-local/DearPyGui/Release/_dearpygui.so'

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
    """Distribution which always forces a binary package with platform name"""
    def has_ext_modules(var):
        return True

class DPGBuildCommand(distutils.cmd.Command):
  """A custom command to run Pylint on all Python source files."""

  description = 'DPG Build Command'
  user_options = []

  def initialize_options(self):
    pass

  def finalize_options(self):
    pass

  def run(self):
    """Run command."""

    if get_platform() == "Windows":
        command = [r'set PATH="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin";"C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin";"C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin";%PATH% && ']
        command.append("mkdir cmake-build-local && ")
        command.append("cd cmake-build-local && ")
        command.append('cmake .. -G "Visual Studio 16 2019" -A "x64" -DMVDIST_ONLY=True -DMVDPG_VERSION=')
        command.append(version_number() + " && ")
        command.append("cd .. && cmake --build cmake-build-local --config Release")
        self.announce('Running command: %s' % "Dear PyGui Build for Windows", level=distutils.log.INFO)
        subprocess.check_call(''.join(command), env=os.environ, shell=True)
        src_path = os.path.dirname(os.path.abspath(__file__))
        shutil.copy(lib_location(), src_path +"/output/dearpygui")

    elif get_platform() == "Linux":
        command = ["mkdir cmake-build-local; "]
        command.append("cd cmake-build-local; ")
        command.append('cmake .. -DMVDIST_ONLY=True -DMVDPG_VERSION='+version_number()+"; ")
        command.append("cd ..; cmake --build cmake-build-local --config Release")
        self.announce('Running command: %s' % "Dear PyGui Build for Linux",level=distutils.log.INFO)
        subprocess.check_call(''.join(command), shell=True)
        src_path = os.path.dirname(os.path.abspath(__file__))
        shutil.copy(lib_location(), src_path +"/output/dearpygui")
    
    elif get_platform() == "OS X":
        command = ["mkdir cmake-build-local; "]
        command.append("cd cmake-build-local; ")
        command.append('cmake .. -DMVDIST_ONLY=True -DMVDPG_VERSION='+version_number()+"; ")
        command.append("cd ..; cmake --build cmake-build-local --config Release")
        self.announce('Running command: %s' % "Dear PyGui Build for OS X",level=distutils.log.INFO)
        subprocess.check_call(''.join(command), shell=True)
        src_path = os.path.dirname(os.path.abspath(__file__))
        shutil.copy(lib_location(), src_path +"/output/dearpygui")

    else:
        self.announce('Command not ready.',level=distutils.log.INFO)

    

class BuildPyCommand(build_py.build_py):
  """Custom build command."""

  def run(self):
    self.run_command('dpg_build')
    build_py.build_py.run(self)

def parse_setuppy_commands():
    """Check the commands and respond appropriately.  Disable broken commands.
    Return a boolean value for whether or not to run the build..
    """
    args = sys.argv[1:]

    if not args:
        # User forgot to give an argument probably, let setuptools handle that.
        return True

    info_commands = ['--help-commands', '--name', '--version', '-V',
                     '--fullname', '--author', '--author-email',
                     '--maintainer', '--maintainer-email', '--contact',
                     '--contact-email', '--url', '--license', '--description',
                     '--long-description', '--platforms', '--classifiers',
                     '--keywords', '--provides', '--requires', '--obsoletes',
                     'version',]

    for command in info_commands:
        if command in args:
            return False

    # Note that 'alias', 'saveopts' and 'setopt' commands also seem to work
    # fine as they are, but are usually used together with one of the commands
    # below and not standalone.  Hence they're not added to good_commands.
    good_commands = ('develop', 'sdist', 'build', 'build_ext', 'build_py',
                     'build_clib', 'build_scripts', 'bdist_wheel', 'bdist_rpm',
                     'bdist_wininst', 'bdist_msi', 'bdist_mpkg', 'build_src',
                     'bdist_egg')

    for command in good_commands:
        if command in args:
            return True

    # The following commands are supported, but we need to show more
    # useful messages to the user
    if 'install' in args:
        print(textwrap.dedent("""
            Note: if you need reliable uninstall behavior, then install
            with pip instead of using `setup.py install`:
              - `pip install .`       (from a git repo or downloaded source
                                       release)
              - `pip install dearpygui`   (last Dear PyGui release on PyPi)
            """))
        return True

    if '--help' in args or '-h' in sys.argv[1]:
        print(textwrap.dedent("""
            Dear PyGui-specific help
            -------------------
            To install Dear PyGui from here with reliable uninstall, we recommend
            that you use `pip install .`. To install the latest Dear PyGui release
            from PyPi, use `pip install dearpygui`.
            Setuptools commands help
            ------------------------
            """))
        return False

    # The following commands aren't supported.  They can only be executed when
    # the user explicitly adds a --force command-line argument.
    bad_commands = dict(
        test="""
            `setup.py test` is not supported.
            """,
        upload="""
            `setup.py upload` is not supported, because it's insecure.
            Instead, build what you want to upload and upload those files
            with `twine upload -s <filenames>` instead.
            """,
        clean="""
            `setup.py clean` is not supported, use one of the following instead:
              - `git clean -xdf` (cleans all files)
              - `git clean -Xdf` (cleans all versioned files, doesn't touch
                                  files that aren't checked into the git repo)
            """,
        build_sphinx="""
            `setup.py build_sphinx` is not supported, use the
            Makefile under docs/""",
        flake8="`setup.py flake8` is not supported, use flake8 standalone",
        )
    bad_commands['nosetests'] = bad_commands['test']
    for command in ('upload_docs', 'easy_install', 'bdist', 'bdist_dumb',
                    'register', 'check', 'install_data', 'install_headers',
                    'install_lib', 'install_scripts', ):
        bad_commands[command] = "`setup.py %s` is not supported" % command

    for command in bad_commands.keys():
        if command in args:
            print(textwrap.dedent(bad_commands[command]) +
                  "\nAdd `--force` to your command to use it anyway if you "
                  "must (unsupported).\n")
            sys.exit(1)

    # If we got here, we didn't detect what setup.py command was given
    raise RuntimeError("Unrecognized setuptools command: {}".format(args))

def setup_package():

    src_path = os.path.dirname(os.path.abspath(__file__))
    old_path = os.getcwd()
    os.chdir(src_path)
    sys.path.insert(0, src_path)

    # import readme content
    with open("./docs/README.md", encoding='utf-8') as f:
        long_description = f.read()

    # create the necessary directories if they do not exist
    if os.path.isdir(src_path +  "/output"):
        shutil.rmtree(src_path +  "/output")
    os.mkdir(src_path + "/output")
    os.mkdir(src_path + "/output/dearpygui")

    if os.path.isdir(src_path +  "/cmake-build-local"):
        shutil.rmtree(src_path +  "/cmake-build-local")

    # copy add items to temporary location
    shutil.copy(src_path + "/DearPyGui/dearpygui/dearpygui.py", src_path + "/output/dearpygui")
    shutil.copy(src_path + "/DearPyGui/dearpygui/demo.py", src_path + "/output/dearpygui")
    shutil.copy(src_path + "/DearPyGui/dearpygui/experimental.py", src_path + "/output/dearpygui")

    with open(src_path + "/output/dearpygui/__init__.py", 'w') as file:
        file.write("__version__='" + version_number() + "'\n")

    if os.environ.get('READTHEDOCS') == 'True':

        with open(src_path + "/output/dearpygui/_dearpygui.py", 'w') as newfile:
            with open(src_path + "/DearPyGui/dearpygui/_dearpygui.pyi", 'r') as file:
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
        shutil.copy(src_path + "/DearPyGui/dearpygui/_dearpygui.pyi", src_path + "/output/dearpygui")
        if get_platform() == "Windows":
            shutil.copy(src_path + "/Dependencies/Microsoft/vcruntime140_1.dll", src_path + "/output/dearpygui")

    metadata = dict(
        name='dearpygui',                                      # Required
        version=version_number(),                             # Required
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
        packages=find_packages(where='output'),
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
        run_build = True
        sys.argv.remove('--force')
    else:
        # Raise errors for unsupported commands, improve help output, etc.
        run_build = parse_setuppy_commands()

    if run_build:
        pass

    try:
        setup(**metadata)
    finally:
        del sys.path[0]
        os.chdir(old_path)
    return

if __name__ == '__main__':
    setup_package()
