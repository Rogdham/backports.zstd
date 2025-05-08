#ifndef BACKPORTS_ZSTD_COMPAT_H
#define BACKPORTS_ZSTD_COMPAT_H

#include "Python.h"

#if PY_VERSION_HEX < 0x030D0000 // Python 3.12 and below
#define Py_mod_gil 0
#define Py_MOD_GIL_NOT_USED NULL
#define PyLong_AsInt _PyLong_AsInt
#define Py_BEGIN_CRITICAL_SECTION(op) {
#define Py_END_CRITICAL_SECTION() }
#define Py_BEGIN_CRITICAL_SECTION2(a, b) {
#define Py_END_CRITICAL_SECTION2() }
#endif

#if PY_VERSION_HEX < 0x030C0000 // Python 3.11 and below
#include "structmember.h"
#define Py_T_INT T_INT
#define Py_T_UINT T_UINT
#define Py_T_BOOL T_BOOL
#define Py_T_OBJECT_EX T_OBJECT_EX
#define Py_READONLY READONLY
#endif

#endif /* !BACKPORTS_ZSTD_COMPAT_H */
