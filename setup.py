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

########################################################################################################################
# These functions read the version number and library location which is populated by
# github actions or the local build scripts.
########################################################################################################################

def version_number():

    if os.environ.get('READTHEDOCS') == 'True':
        return "latest"
    try:
        with open('version_number.txt', encoding='utf-8') as f:
            return f.readline().rstrip()

    except IOError:
        return 'latest'

def lib_location():

    if os.environ.get('READTHEDOCS') == 'True':
        return 'cmake-build-release/DearPyGui/_dearpygui.so'

    try:
        with open('lib_location.txt', encoding='utf-8') as f:
            return f.readline().rstrip()

    except IOError:
        return 'cmake-build-release/DearPyGui/_dearpygui.so'

def get_platform():
    platforms = {
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
    #command = ['/usr/bin/pylint']
    #self.announce('Running command: %s' % str(command),level=distutils.log.INFO)
    #subprocess.check_call(command)
    print(get_platform())

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

    # Commands that do more than print info, but also don't need Cython and
    # template parsing.
    other_commands = ['egg_info', 'install_egg_info', 'rotate', 'dist_info']
    for command in other_commands:
        if command in args:
            return False

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
    if not os.path.isdir(src_path +  "/dearpygui/"):
        os.mkdir(src_path + "/dearpygui/")

    # copy add items to temporary location
    shutil.copy(src_path + "/DearPyGui/dearpygui/dearpygui.py", src_path + "/dearpygui")
    shutil.copy(src_path + "/DearPyGui/dearpygui/demo.py", src_path + "/dearpygui")
    shutil.copy(src_path + "/DearPyGui/dearpygui/experimental.py", src_path + "/dearpygui")

    with open(src_path + "/dearpygui/__init__.py", 'w') as file:
        file.write("__version__='")
        file.write(version_number(on_rtd))
        file.write("'\n")

    if os.environ.get('READTHEDOCS') == 'True':

        with open(src_path + "/dearpygui/_dearpygui.py", 'w') as newfile:
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
        shutil.copy(lib_location(), src_path +"/dearpygui")
        shutil.copy(src_path + "/DearPyGui/dearpygui/_dearpygui.pyi", src_path + "/dearpygui")
        shutil.copy(src_path + "/Dependencies/Microsoft/vcruntime140_1.dll", src_path + "/dearpygui")

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
        package_data={},
        distclass=BinaryDistribution,
        cmdclass={
        'dpg_build': DPGBuildCommand,
        'build_py': BuildPyCommand,
        },
    )

    if on_rtd:
        metadata['package_data']['dearpygui'] = ["_dearpygui.py", "dearpygui.py", "demo.py", "experimental.py"]
    else:
        metadata['package_data']['dearpygui'] = ["_dearpygui.so", "_dearpygui.pyd", "_dearpygui.pyi", "dearpygui.py", "demo.py", "experimental.py", "vcruntime140_1.dll"]

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
