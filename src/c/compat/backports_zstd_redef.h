#ifndef BACKPORTS_ZSTD_REDEF_H
#define BACKPORTS_ZSTD_REDEF_H

#include "backports_zstd_redef_orig.h"

#define _backportszstdredef__PyArg_BadArgument(fname, displayname, expected, args) \
    _PyArg_BadArgument(fname, displayname, expected, args)

#define _backportszstdredef__PyArg_CheckPositional(funcname, nargs, min, max) \
    _PyArg_CheckPositional(funcname, nargs, min, max)

PyAPI_FUNC(PyObject *const *) _backportszstdredef__PyArg_UnpackKeywords(
    PyObject *const *args,
    Py_ssize_t nargs,
    PyObject *kwargs,
    PyObject *kwnames,
    struct _PyArg_Parser *parser,
    int minpos,
    int maxpos,
    int minkw,
    int varpos,
    PyObject **buf);

#define _backportszstdredef__PyNumber_Index(o) \
    _PyNumber_Index(o)

#endif /* !BACKPORTS_ZSTD_REDEF_H */
