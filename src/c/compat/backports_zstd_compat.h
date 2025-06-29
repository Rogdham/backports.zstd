#ifndef BACKPORTS_ZSTD_COMPAT_H
#define BACKPORTS_ZSTD_COMPAT_H

#include "Python.h"

#include "pythoncapi_compat.h"

#if PY_VERSION_HEX < 0x030E0000 // Python 3.13 and below
static inline int PyType_Freeze(PyTypeObject *type)
{
    // this will not perform the freeze
    // but it is acceptable in the scope of this library
    return -1;
}
#endif

#if PY_VERSION_HEX < 0x030D0000 // Python 3.12 and below
#define Py_mod_gil 0
#define Py_MOD_GIL_NOT_USED NULL
#endif

#if PY_VERSION_HEX < 0x030C0000 // Python 3.11 and below
#define Py_mod_multiple_interpreters 0
#define Py_MOD_PER_INTERPRETER_GIL_SUPPORTED NULL
#endif

#if PY_VERSION_HEX < 0x030B0000 // Python 3.10 and below
#define _PyCFunction_CAST(func) _Py_CAST(PyCFunction, _Py_CAST(void (*)(void), (func)))
#endif

#if PY_VERSION_HEX < 0x030A0000 // Python 3.9 and below
// this will not mark objects as immutable
// but it is acceptable in the scope of this library
#define Py_TPFLAGS_IMMUTABLETYPE 0
#endif

#endif /* !BACKPORTS_ZSTD_COMPAT_H */
