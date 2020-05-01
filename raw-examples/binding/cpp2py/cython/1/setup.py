#!/usr/bin/python3

from setuptools import Extension, setup
from Cython.Build import cythonize

lstModule = [
    Extension(
        name="c_foo",
        sources=["c_foo.pyx"]
    ),
    Extension(
        name="c_math",
        sources=["c_math.pyx"]
    )
]

if __name__ == '__main__':
    setup(ext_modules=cythonize(lstModule))
