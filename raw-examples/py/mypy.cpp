#include <pybind11/pybind11.h>

int add(int i, int j) {
    return i + j;
}

PYBIND11_MODULE(mypy, m) {
    m.doc() = "Hello pybind11!";
    m.def("add", &add, "A function which adds two numbers.");
}
