#!/usr/bin/env python3

"""List all those Python files that require a coding directive

Usage: findnocoding.py dir1 [dir2...]
"""

__author__ = "Oleg Broytmann, Georg Brandl"

import sys, os, re, getopt

# our pysource module finds Python source files
try:
    import pysource
except ImportError:
    # emulate the module with a simple os.walk
    class pysource:
        has_python_ext = looks_like_python = can_be_compiled = None
        def walk_python_files(self, paths, *args, **kwargs):
            for path in paths:
                if os.path.isfile(path):
                    yield path.endswith(".py")
                elif os.path.isdir(path):
                    for root, dirs, files in os.walk(path):
                        for filename in files:
                            if filename.endswith(".py"):
                                yield os.path.join(root, filename)
    pysource = pysource()


    print("The pysource module is not available; "
                         "no sophisticated Python source file search will be done.", file=sys.stderr)


decl_re = re.compile(rb'^[ \t\f]*#.*?coding[:=][ \t]*([-\w.]+)')
blank_re = re.compile(rb'^[ \t\f]*(?:[#\r\n]|$)')

def get_declaration(line):
    match = decl_re.match(line)
    if match:
        return match.group(1)
    return b''

def has_correct_encoding(text, codec):
    try:
        str(text, codec)
    except UnicodeDecodeError:
        return False
    else:
        return True

def needs_declaration(fullpath):
    try:
        infile = open(fullpath, 'rb')
    except IOError: # Oops, the file was removed - ignore it
        return None

    with infile:
        line1 = infile.readline()
        line2 = infile.readline()

        if (get_declaration(line1) or
            blank_re.match(line1) and get_declaration(line2)):
            # the file does have an encoding declaration, so trust it
            return False

        # check the whole file for non utf-8 characters
        rest = infile.read()

    if has_correct_encoding(line1+line2+rest, "utf-8"):
        return False

    return True


usage = """Usage: %s [-cd] paths...
    -c: recognize Python source files trying to compile them
    -d: debug output""" % sys.argv[0]

if __name__ == '__main__':

    try:
        opts, args = getopt.getopt(sys.argv[1:], 'cd')
    except getopt.error as msg:
        print(msg, file=sys.stderr)
        print(usage, file=sys.stderr)
        sys.exit(1)

    is_python = pysource.looks_like_python
    debug = False

    for o, a in opts:
        if o == '-c':
            is_python = pysource.can_be_compiled
        elif o == '-d':
            debug = True

    if not args:
        print(usage, file=sys.stderr)
        sys.exit(1)

    for fullpath in pysource.walk_python_files(args, is_python):
        if debug:
            print("Testing for coding: %s" % fullpath)
        result = needs_declaration(fullpath)
        if result:
            print(fullpath)
