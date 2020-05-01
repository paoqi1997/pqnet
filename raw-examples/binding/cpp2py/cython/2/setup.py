#!/usr/bin/python3

from setuptools import Extension, setup
from Cython.Build import cythonize

lstModule = [
    Extension(
        name="c_object",
        sources=["c_object.pyx"],
        language="c++"
    )
]

if __name__ == '__main__':
    setup(ext_modules=cythonize(lstModule))
