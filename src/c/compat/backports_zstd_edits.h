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
#define BACKPORTSZSTD_PyErr_Format_AppendPT(t, s, o) PyErr_Format(t, (s "%s"), Py_TYPE(o)->tp_name)
#else
#define BACKPORTSZSTD_PyErr_Format_AppendPT(t, s, o) PyErr_Format(t, (s "%T"), o)
#endif

/*
Backporting PyMutex is a lot of work
Instead, we fallback on PyThread_type_lock for Python 3.12 and below
We introduced some functions of our own to compensate API differences
*/
#if PY_VERSION_HEX < 0x030D0000 // Python 3.12 and below

#define BACKPORTSZSTD_LOCK PyThread_type_lock
#define BACKPORTSZSTD_LOCK_allocate PyThread_allocate_lock
#define BACKPORTSZSTD_LOCK_isError(l) (l == NULL)
static inline void BACKPORTSZSTD_LOCK_lock(PyThread_type_lock *mp)
{
    Py_BEGIN_ALLOW_THREADS
    PyThread_acquire_lock(*mp, WAIT_LOCK);
    Py_END_ALLOW_THREADS
}
static inline void BACKPORTSZSTD_LOCK_unlock(PyThread_type_lock *mp)
{
    PyThread_release_lock(*mp);
}
static inline void BACKPORTSZSTD_LOCK_free(PyThread_type_lock mp)
{
    if (mp)
    {
        PyThread_free_lock(mp);
    }
}
static inline int BACKPORTSZSTD_LOCK_isLocked(PyThread_type_lock *mp)
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

#else // Python 3.13 and above

#define BACKPORTSZSTD_LOCK PyMutex
#define BACKPORTSZSTD_LOCK_allocate() ((PyMutex){0})
#define BACKPORTSZSTD_LOCK_isError(l) (0)
#define BACKPORTSZSTD_LOCK_lock PyMutex_Lock
#define BACKPORTSZSTD_LOCK_unlock PyMutex_Unlock
#define BACKPORTSZSTD_LOCK_free(l)
static inline int BACKPORTSZSTD_LOCK_isLocked(PyMutex *lp)
{
    // note: this function is only used in asserts
    // PyMutex_IsLocked is not exposed publicly https://github.com/python/cpython/issues/134009
    Py_FatalError("Not implemented");
}

#endif /* !BACKPORTSZSTD_LOCK */

#endif /* !BACKPORTS_ZSTD_EDITS_H */
