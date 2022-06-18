#! /usr/bin/env python3
"""nm2def.py

Helpers to extract symbols from Unix libs and auto-generate
Windows definition files from them. Depends on nm(1). Tested
on Linux and Solaris only (-p option to nm is for Solaris only).

By Marc-Andre Lemburg, Aug 1998.

Additional notes: the output of nm is supposed to look like this:

acceler.o:
000001fd T PyGrammar_AddAccelerators
         U PyGrammar_FindDFA
00000237 T PyGrammar_RemoveAccelerators
         U _IO_stderr_
         U exit
         U fprintf
         U free
         U malloc
         U printf

grammar1.o:
00000000 T PyGrammar_FindDFA
00000034 T PyGrammar_LabelRepr
         U _PyParser_TokenNames
         U abort
         U printf
         U sprintf

...

Even if this isn't the default output of your nm, there is generally an
option to produce this format (since it is the original v7 Unix format).

"""
import os, sys

PYTHONLIB = 'libpython%d.%d.a' % sys.version_info[:2]
PC_PYTHONLIB = 'Python%d%d.dll' % sys.version_info[:2]
NM = 'nm -p -g %s'                      # For Linux, use "nm -g %s"

def symbols(lib=PYTHONLIB,types=('T','C','D')):

    with os.popen(NM % lib) as pipe:
        lines = pipe.readlines()
    lines = [s.strip() for s in lines]
    symbols = {}
    for line in lines:
        if len(line) == 0 or ':' in line:
            continue
        items = line.split()
        if len(items) != 3:
            continue
        address, type, name = items
        if type not in types:
            continue
        symbols[name] = address,type
    return symbols

def export_list(symbols):

    data = []
    code = []
    for name,(addr,type) in symbols.items():
        if type in ('C','D'):
            data.append('\t'+name)
        else:
            code.append('\t'+name)
    data.sort()
    data.append('')
    code.sort()
    return ' DATA\n'.join(data)+'\n'+'\n'.join(code)

# Definition file template
DEF_TEMPLATE = """\
EXPORTS
%s
"""

# Special symbols that have to be included even though they don't
# pass the filter
SPECIALS = (
    )

def filter_Python(symbols,specials=SPECIALS):

    for name in list(symbols.keys()):
        if name[:2] == 'Py' or name[:3] == '_Py':
            pass
        elif name not in specials:
            del symbols[name]

def main():

    s = symbols(PYTHONLIB)
    filter_Python(s)
    exports = export_list(s)
    f = sys.stdout # open('PC/python_nt.def','w')
    f.write(DEF_TEMPLATE % (exports))
    # f.close()

if __name__ == '__main__':
    main()
