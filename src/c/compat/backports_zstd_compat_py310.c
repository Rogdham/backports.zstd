#include "Python.h"
#if PY_VERSION_HEX < 0x030B0000 // Python 3.10 and below

PyObject *
PyType_GetModuleByDef(PyTypeObject *type, PyModuleDef *def)
{
    // no worries, it's actually not really used
    // and the calls will be removed in future version of the module
    // FIXME to remove (3.14.0b2)
    Py_FatalError("Not implemented");
    return NULL;
}

#endif
