#include "Python.h"
#if PY_VERSION_HEX < 0x030B0000 // Python 3.10 and below

typedef struct
{
    PyObject_HEAD PyObject *md_dict;
    PyModuleDef *md_def;
    void *md_state;
    PyObject *md_weaklist;
    // for logging purposes after md_dict is cleared
    PyObject *md_name;
} PyModuleObject;

static inline PyModuleDef *_PyModule_GetDef(PyObject *mod)
{
    assert(PyModule_Check(mod));
    return ((PyModuleObject *)mod)->md_def;
}

static inline int
_PyType_HasFeature(PyTypeObject *type, unsigned long feature)
{
    return ((type->tp_flags & feature) != 0);
}

PyObject *
PyType_GetModuleByDef(PyTypeObject *type, PyModuleDef *def)
{
    assert(PyType_Check(type));

    PyObject *mro = type->tp_mro;
    // The type must be ready
    assert(mro != NULL);
    assert(PyTuple_Check(mro));
    // mro_invoke() ensures that the type MRO cannot be empty, so we don't have
    // to check i < PyTuple_GET_SIZE(mro) at the first loop iteration.
    assert(PyTuple_GET_SIZE(mro) >= 1);

    Py_ssize_t n = PyTuple_GET_SIZE(mro);
    for (Py_ssize_t i = 0; i < n; i++)
    {
        PyObject *super = PyTuple_GET_ITEM(mro, i);
        if (!_PyType_HasFeature((PyTypeObject *)super, Py_TPFLAGS_HEAPTYPE))
        {
            // Static types in the MRO need to be skipped
            continue;
        }

        PyHeapTypeObject *ht = (PyHeapTypeObject *)super;
        PyObject *module = ht->ht_module;
        if (module && _PyModule_GetDef(module) == def)
        {
            return module;
        }
    }

    PyErr_Format(
        PyExc_TypeError,
        "PyType_GetModuleByDef: No superclass of '%s' has the given module",
        type->tp_name);
    return NULL;
}

#endif
