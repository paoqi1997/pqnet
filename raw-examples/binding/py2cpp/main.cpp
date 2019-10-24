#include <iostream>

#include <Python.h>

using std::cout;
using std::endl;

int main()
{
    PyObject *pyMod;
    PyObject *pyFunc;
    PyObject *pyArgs, *pyRes;

    Py_Initialize();
    if (!Py_IsInitialized()) {
        return 1;
    }

    if (PyRun_SimpleString("import sys") != 0) {
        return 1;
    }
    if (PyRun_SimpleString("sys.path.append('.')") != 0) {
        return 1;
    }

    pyMod = PyImport_ImportModule("mod");
    if (pyMod) {
        pyFunc = PyObject_GetAttrString(pyMod, "fn1");
        if (pyFunc) {
            pyArgs = Py_BuildValue("ii", 2, 4);
            pyRes = PyObject_CallObject(pyFunc, pyArgs);
            Py_DECREF(pyFunc);
            Py_DECREF(pyArgs);
            if (pyRes) {
                int myRes = 0;
                PyArg_Parse(pyRes, "i", &myRes);
                Py_DECREF(pyRes);
                cout << myRes << endl;
            }
        }
        pyFunc = PyObject_GetAttrString(pyMod, "fn2");
        if (pyFunc) {
            pyArgs = Py_BuildValue("()");
            pyRes = PyObject_CallObject(pyFunc, pyArgs);
            Py_DECREF(pyFunc);
            Py_DECREF(pyArgs);
            if (pyRes) {
                int Res1 = 0, Res2 = 0;
                PyArg_Parse(pyRes, "(ii)", &Res1, &Res2);
                Py_DECREF(pyRes);
                cout << Res1 << " " << Res2 << endl;
            }
        }
        Py_DECREF(pyMod);
    }

    Py_Finalize();

    return 0;
}
