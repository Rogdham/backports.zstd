#ifndef BACKPORTS_ZSTD_REDEF_H
#define BACKPORTS_ZSTD_REDEF_H

#include "backports_zstd_redef_orig.h"

#include "Python.h"

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

#if PY_VERSION_HEX < 0x030D0000 // Python 3.12 and below
#define _backportszstdredef_PyMutex PyThread_type_lock
static inline _backportszstdredef_PyMutex _backportszstdredef_PyMutex_Init()
{
    PyThread_type_lock lock = PyThread_allocate_lock();
    if (lock == NULL)
    {
        Py_FatalError("[backports.zstd] Could not allocate lock");
        // PyErr_NoMemory();
        // return -1;
    }
    return lock;
}
static inline void _backportszstdredef_PyMutex_Lock(PyThread_type_lock *mp)
{
    Py_BEGIN_ALLOW_THREADS
        PyThread_acquire_lock(*mp, WAIT_LOCK);
    Py_END_ALLOW_THREADS
}
static inline void _backportszstdredef_PyMutex_Unlock(PyThread_type_lock *mp)
{
    PyThread_release_lock(*mp);
}
#else
#define _backportszstdredef_PyMutex PyMutex
#define _backportszstdredef_PyMutex_Init() ((PyMutex){0})
#define _backportszstdredef_PyMutex_Lock PyMutex_Lock
#define _backportszstdredef_PyMutex_Unlock PyMutex_Unlock
#endif

#endif /* !BACKPORTS_ZSTD_REDEF_H */
