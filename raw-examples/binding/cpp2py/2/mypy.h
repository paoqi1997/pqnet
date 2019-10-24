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

class Object
{
    PyObject_HEAD
public:
    void init() { std::cout << "Object::Object" << std::endl; }
    void release() { std::cout << "Object::~Object" << std::endl; }
    std::string getName() const { return name; }
    int getSum() const { return sum; }
    void setName(const std::string& _name) { name = _name; }
    void setSum(int _sum) { sum = _sum; }
private:
    std::string name;
    int sum;
};

#endif // MYPY_H
