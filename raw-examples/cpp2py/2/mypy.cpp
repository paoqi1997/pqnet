#include "mypy.h"

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

PyObject* createObject(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    Object *self;
    self = reinterpret_cast<Object*>(type->tp_alloc(type, 0));
    self->init();
    return reinterpret_cast<PyObject*>(self);
}

void deleteObject(Object *self)
{
    self->release();
    Py_TYPE(self)->tp_free(reinterpret_cast<PyObject*>(self));
}

PyObject* getName(Object *self)
{
    return Py_BuildValue("s", self->getName().c_str());
}

PyObject* getSum(Object *self)
{
    return Py_BuildValue("i", self->getSum());
}

PyObject* setName(Object *self, PyObject *args)
{
    const char *name;
    if (!PyArg_ParseTuple(args, "s", &name)) {
        return nullptr;
    }

    self->setName(name);

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject* setSum(Object *self, PyObject *args)
{
    int sum = 0;
    if (!PyArg_ParseTuple(args, "i", &sum)) {
        return nullptr;
    }

    self->setSum(sum);

    Py_INCREF(Py_None);
    return Py_None;
}

PyMethodDef ObjectMethods[] = {
    {"getName", (PyCFunction) getName, METH_NOARGS, nullptr},
    {"getSum", (PyCFunction) getSum, METH_NOARGS, nullptr},
    {"setName", (PyCFunction) setName, METH_VARARGS, nullptr},
    {"setSum", (PyCFunction) setSum, METH_VARARGS, nullptr},
    {nullptr, nullptr, 0, nullptr}
};

PyTypeObject ObjectType = {
    PyVarObject_HEAD_INIT(nullptr, 0)
    "mypy.Object",                  /* tp_name */
    sizeof(Object),                 /* tp_basicsize */
    0,                              /* tp_itemsize */
    (destructor) deleteObject,      /* tp_dealloc */
    0,                              /* tp_vectorcall_offset */
    0,                              /* tp_getattr */
    0,                              /* tp_setattr */
    0,                              /* tp_as_async */
    0,                              /* tp_repr */
    0,                              /* tp_as_number */
    0,                              /* tp_as_sequence */
    0,                              /* tp_as_mapping */
    0,                              /* tp_hash */
    0,                              /* tp_call */
    0,                              /* tp_str */
    0,                              /* tp_getattro */
    0,                              /* tp_setattro */
    0,                              /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /* tp_flags */
    nullptr,                        /* tp_doc */
    0,                              /* tp_traverse */
    0,                              /* tp_clear */
    0,                              /* tp_richcompare */
    0,                              /* tp_weaklistoffset */
    0,                              /* tp_iter */
    0,                              /* tp_iternext */
    ObjectMethods,                  /* tp_methods */
    0,                              /* tp_members */
    0,                              /* tp_getset */
    0,                              /* tp_base */
    0,                              /* tp_dict */
    0,                              /* tp_descr_get */
    0,                              /* tp_descr_set */
    0,                              /* tp_dictoffset */
    0,                              /* tp_init */
    0,                              /* tp_alloc */
    createObject,                   /* tp_new */
};

PyModuleDef PyMod = {
    PyModuleDef_HEAD_INIT,
    "mypy",
    nullptr/* Module Doc */,
    -1
};

PyMODINIT_FUNC PyInit_mypy()
{
    PyObject *m;
    if (PyType_Ready(&ObjectType) < 0) {
        return nullptr;
    }

    m = PyModule_Create(&PyMod);
    if (m == nullptr) {
        return nullptr;
    }

    Py_INCREF(&ObjectType);
    if (PyModule_AddObject(m, "Object", reinterpret_cast<PyObject*>(&ObjectType)) < 0) {
        Py_DECREF(&ObjectType);
        Py_DECREF(m);
        return nullptr;
    }

    return m;
}
