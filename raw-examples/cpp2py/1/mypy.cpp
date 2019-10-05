#include "mypy.h"

namespace mypy {

PyObject* py_add(PyObject *self, PyObject *args)
{
    int x, y;
    if (!PyArg_ParseTuple(args, "ii", &x, &y)) {
        return nullptr;
    }

    return Py_BuildValue("i", x + y);
}

} // namespace mypy

PyMethodDef PyMethods[] = {
    {"add", mypy::py_add, METH_VARARGS, nullptr/* Method Doc */},
    {nullptr, nullptr, 0, nullptr}
};

PyModuleDef PyMod = {
    PyModuleDef_HEAD_INIT,
    "mypy",
    nullptr/* Module Doc */,
    -1,
    PyMethods
};

PyObject* PyInit_mypy()
{
    return PyModule_Create(&PyMod);
}
