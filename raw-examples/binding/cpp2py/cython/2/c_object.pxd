# cython: language_level=3
from libcpp.string cimport string

cdef extern from "object.h" namespace "mypy":
    int _add(int x, int y)
    cdef cppclass CObject:
        CObject() except +
        string getName()
        int getSum()
        void setName(const string& name)
        void setSum(int sum)
