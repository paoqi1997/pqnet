from c_object cimport _add
cpdef int add(int x, int y):
    return _add(x, y)

from c_object cimport CObject
cdef class Object:
    cdef CObject c_ref

    def __cinit__(self):
        self.c_ref = CObject()

    def getName(self):
        return self.c_ref.getName()

    def getSum(self):
        return self.c_ref.getSum()

    def setName(self, name):
        return self.c_ref.setName(name)

    def setSum(self, sum):
        return self.c_ref.setSum(sum)
