#!/usr/bin/python3

import sys
from platform import system

from setuptools import setup
from Cython.Build import cythonize

# Separator
Sep = '\\' if system() == 'Windows' else '/'

if __name__ == '__main__':
    # python3 setup.py build_ext --inplace
    setup(ext_modules=cythonize('%s%s%s'%(sys.path[0], Sep, 'mypy.pyx')))
