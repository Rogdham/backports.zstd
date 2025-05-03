#include "Python.h"

#include "backports_zstd_redef_orig.h"

PyObject *const *
_backportszstdredef__PyArg_UnpackKeywords(
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
        All calls of _backportszstdredef__PyArg_UnpackKeywords have varpos set to 0
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
