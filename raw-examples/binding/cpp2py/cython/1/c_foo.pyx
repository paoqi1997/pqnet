# cython: language_level=3
def add(x, y):
    return x + y

from libc cimport math
cpdef double sin(double x):
    return math.sin(x)
