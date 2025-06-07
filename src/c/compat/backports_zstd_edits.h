#ifndef BACKPORTS_ZSTD_EDITS_H
#define BACKPORTS_ZSTD_EDITS_H

#include "backports_zstd_edits_orig.h"

#define BACKPORTSZSTD__PyArg_BadArgument(fname, displayname, expected, args) \
    _PyArg_BadArgument(fname, displayname, expected, args)

#define BACKPORTSZSTD__PyArg_CheckPositional(funcname, nargs, min, max) \
    _PyArg_CheckPositional(funcname, nargs, min, max)

static inline PyObject *const *
BACKPORTSZSTD__PyArg_UnpackKeywords(
    PyObject *const *args,
    Py_ssize_t nargs,
    PyObject *kwargs,
    PyObject *kwnames,
    struct _PyArg_Parser *parser,
    int minpos,
    int maxpos,
    int minkw,
    int varpos, // introduced in Python 3.14
    PyObject **buf)
{
    if (varpos)
    {
        /*
        All calls of BACKPORTSZSTD__PyArg_UnpackKeywords have varpos set to 0
        This will catch future code evolutions that may change this assumption
        */
        Py_FatalError("Not implemented");
    }
    return _PyArg_UnpackKeywords(
        args,
        nargs,
        kwargs,
        kwnames,
        parser,
        minpos,
        maxpos,
        minkw,
        buf);
}

#define BACKPORTSZSTD__PyNumber_Index(o) \
    _PyNumber_Index(o)

#endif /* !BACKPORTS_ZSTD_EDITS_H */
