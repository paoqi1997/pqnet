#ifndef MYPY_H
#define MYPY_H

#include <iostream>
#include <string>

#ifdef __cplusplus
extern "C" {
#endif

#include <Python.h>
#include <structmember.h>

PyMODINIT_FUNC PyInit_mypy();

#ifdef __cplusplus
}
#endif

class Object
{
public:
    PyObject_HEAD
    // Methods
    void init() { flag = true; std::cout << "Object::Object" << std::endl; }
    void release() { std::cout << "Object::~Object" << std::endl; }
    std::string getName() const { return name; }
    int getSum() const { return sum; }
    void setName(const std::string& _name) { name = _name; }
    void setSum(int _sum) { sum = _sum; }
    // Members
    std::string name;
    int sum;
    bool flag;
};

#endif // MYPY_H
