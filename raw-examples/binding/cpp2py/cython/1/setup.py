#!/usr/bin/python3

import sys
from platform import system

from setuptools import setup
from Cython.Build import cythonize

# Separator
Sep = '\\' if system() == 'Windows' else '/'

if __name__ == '__main__':
    sFilePath = '{Dir}{Sep}{FileName}'.format(
        Dir=sys.path[0], Sep=Sep, FileName='mypy.pyx'
    )
    setup(ext_modules=cythonize(sFilePath))
