# cython: language_level=3
def add(x, y):
    return x + y

from libc.math cimport sin
cpdef double foo_sin(double x):
    return sin(x)
