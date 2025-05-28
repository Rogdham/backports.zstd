#ifndef BACKPORTS_ZSTD_COMPAT_H
#define BACKPORTS_ZSTD_COMPAT_H

#include "Python.h"

#include "pythoncapi_compat.h"

#if PY_VERSION_HEX < 0x030D0000 // Python 3.12 and below
#define Py_mod_gil 0
#define Py_MOD_GIL_NOT_USED NULL
#endif

#if PY_VERSION_HEX < 0x030B0000 // Python 3.10 and below
#define _PyCFunction_CAST(func) _Py_CAST(PyCFunction, _Py_CAST(void (*)(void), (func)))
PyAPI_FUNC(PyObject *) PyType_GetModuleByDef(PyTypeObject *, PyModuleDef *);
#endif

#endif /* !BACKPORTS_ZSTD_COMPAT_H */
