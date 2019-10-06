#ifndef MYPY_H
#define MYPY_H

#include <iostream>
#include <string>

#ifdef __cplusplus
extern "C" {
#endif

#include <Python.h>

PyMODINIT_FUNC PyInit_mypy();

#ifdef __cplusplus
}
#endif

#endif // MYPY_H
