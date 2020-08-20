#include <pybind11/pybind11.h>

#include "mypy.h"

namespace mypy
{

int add(int i, int j)
{
    return i + j;
}

} // namespace mypy

PYBIND11_MODULE(mypy, m) {
    m.doc() = "mypy module";
    m.def("add", mypy::add, "A function which adds two numbers");
    m.attr("DEEP_DARK_FANTASY") = 2077;
    pybind11::class_<Object>(m, "Object")
        .def(pybind11::init<>())
        .def("getName", &Object::getName)
        .def("getSum", &Object::getSum)
        .def("setName", &Object::setName)
        .def("setSum", &Object::setSum)
        .def_property("FLAG", &Object::getFlag, &Object::setFlag);
}
