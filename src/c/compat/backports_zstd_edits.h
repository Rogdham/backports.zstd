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

#if PY_VERSION_HEX < 0x030D0000 // Python 3.12 and below
#define BACKPORTSZSTD_PyMutex PyThread_type_lock
#define BACKPORTSZSTD_PyMutex_IsNull(l) (l == NULL)
#define BACKPORTSZSTD_PyMutex_allocate PyThread_allocate_lock
static inline void BACKPORTSZSTD_PyMutex_Lock(PyThread_type_lock *mp)
{
    Py_BEGIN_ALLOW_THREADS
    PyThread_acquire_lock(*mp, WAIT_LOCK);
    Py_END_ALLOW_THREADS
}
static inline void BACKPORTSZSTD_PyMutex_Unlock(PyThread_type_lock *mp)
{
    PyThread_release_lock(*mp);
}
static inline void BACKPORTSZSTD_PyMutex_free(PyThread_type_lock mp)
{
    if (mp)
    {
        PyThread_free_lock(mp);
    }
}
static inline int BACKPORTSZSTD_PyMutex_IsLocked(PyThread_type_lock *mp)
{
    // note: this function is only used in asserts
    PyLockStatus status;
    Py_BEGIN_ALLOW_THREADS
    status = PyThread_acquire_lock_timed(*mp, 0, 0);
    Py_END_ALLOW_THREADS
    if (status == PY_LOCK_ACQUIRED)
    {
        PyThread_release_lock(*mp);
        return 0;
    }
    return 1;
}
#else
#define BACKPORTSZSTD_PyMutex PyMutex
#define BACKPORTSZSTD_PyMutex_IsNull(l) (0)
#define BACKPORTSZSTD_PyMutex_allocate() ((PyMutex){0})
#define BACKPORTSZSTD_PyMutex_Lock PyMutex_Lock
#define BACKPORTSZSTD_PyMutex_Unlock PyMutex_Unlock
#define BACKPORTSZSTD_PyMutex_free(l)
static inline int BACKPORTSZSTD_PyMutex_IsLocked(PyMutex *lp)
{
    // note: this function is only used in asserts
    // PyMutex_IsLocked is not exposed publicly https://github.com/python/cpython/issues/134009
    Py_FatalError("Not implemented");
}
#endif

#endif /* !BACKPORTS_ZSTD_EDITS_H */
