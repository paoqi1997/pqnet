#include <iostream>

#include <Python.h>

using std::cout;
using std::endl;

int main()
{
    Py_Initialize();
    if (!Py_IsInitialized()) {
        return 1;
    }

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('.')");

    PyObject *pyMod = PyImport_ImportModule("mod");
    if (pyMod) {
        PyObject *pyFunc;
        pyFunc = PyObject_GetAttrString(pyMod, "fn1");
        if (pyFunc) {
            PyObject *pyArgs = Py_BuildValue("ii", 2, 4);
            PyObject *pyRes = PyObject_CallObject(pyFunc, pyArgs);
            if (pyRes) {
                int myRes = 0;
                PyArg_Parse(pyRes, "i", &myRes);
                cout << myRes << endl;
                Py_DECREF(pyRes);
            }
            Py_DECREF(pyArgs);
            Py_DECREF(pyFunc);
        }
        pyFunc = PyObject_GetAttrString(pyMod, "fn2");
        if (pyFunc) {
            PyObject *pyArgs = Py_BuildValue("()");
            PyObject *pyRes = PyObject_CallObject(pyFunc, pyArgs);
            if (pyRes) {
                int Res1 = 0, Res2 = 0;
                PyArg_Parse(pyRes, "(ii)", &Res1, &Res2);
                cout << Res1 << " " << Res2 << endl;
                Py_DECREF(pyRes);
            }
            Py_DECREF(pyArgs);
            Py_DECREF(pyFunc);
        }
        Py_DECREF(pyMod);
    }

    Py_Finalize();

    return 0;
}
