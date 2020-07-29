python3 "BuildPythonWheel.py"
python3 -m setup bdist_wheel --plat-name macosx-10.6-x86_64 --dist-dir ../../dist
#python3 -m twine upload --repository testpypi dist/*