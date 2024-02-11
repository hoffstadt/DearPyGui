cd /d "%~dp0\.."
py -m pip install --upgrade pip twine wheel setuptools
py -m setup bdist_wheel --plat-name win_amd64 --dist-dir dist