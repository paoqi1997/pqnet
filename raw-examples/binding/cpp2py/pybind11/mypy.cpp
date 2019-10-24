#include <pybind11/pybind11.h>

namespace mypy {

int py_add(int i, int j) {
    return i + j;
}

} // namespace mypy

PYBIND11_MODULE(mypy, m) {
    m.doc() = "mypy module";
    m.def("add", mypy::py_add, "A function which adds two numbers");
}
