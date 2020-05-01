# cython: language_level=3
from libcpp.string cimport string

cdef extern from "object.h" namespace "mypy":
    int add(int x, int y)
    cdef cppclass CppObject:
        CppObject() except +
        string getName()
        int getMoney()
        void setName(const string& name)
        void setMoney(int money)
