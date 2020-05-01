from c_object cimport add
cpdef int foo_add(int x, int y):
    return add(x, y)

from c_object cimport CppObject
cdef class Object:
    cdef CppObject c_ref

    def __cinit__(self):
        self.c_ref = CppObject()

    def getName(self):
        return self.c_ref.getName()

    def getMoney(self):
        return self.c_ref.getMoney()

    def setName(self, name):
        return self.c_ref.setName(name)

    def setMoney(self, money):
        return self.c_ref.setMoney(money)
