#ifndef BACKPORTS_ZSTD_REDEF_ORIG_H
#define BACKPORTS_ZSTD_REDEF_ORIG_H

PyAPI_FUNC(void) _PyArg_BadArgument(
    const char *fname,
    const char *displayname,
    const char *expected,
    PyObject *arg);

PyAPI_FUNC(int) _PyArg_CheckPositional(const char *, Py_ssize_t,
                                       Py_ssize_t, Py_ssize_t);
#define _Py_ANY_VARARGS(n) ((n) == PY_SSIZE_T_MAX)
#define _PyArg_CheckPositional(funcname, nargs, min, max) \
    ((!_Py_ANY_VARARGS(max) && (min) <= (nargs) && (nargs) <= (max)) || _PyArg_CheckPositional((funcname), (nargs), (min), (max)))

PyAPI_FUNC(PyObject *const *) _PyArg_UnpackKeywords(
    PyObject *const *args,
    Py_ssize_t nargs,
    PyObject *kwargs,
    PyObject *kwnames,
    struct _PyArg_Parser *parser,
    int minpos,
    int maxpos,
    int minkw,
    PyObject **buf);
#define _PyArg_UnpackKeywords(args, nargs, kwargs, kwnames, parser, minpos, maxpos, minkw, buf) \
    (((minkw) == 0 && (kwargs) == NULL && (kwnames) == NULL &&                                  \
      (minpos) <= (nargs) && (nargs) <= (maxpos) && (args) != NULL)                             \
         ? (args)                                                                               \
         : _PyArg_UnpackKeywords((args), (nargs), (kwargs), (kwnames), (parser),                \
                                 (minpos), (maxpos), (minkw), (buf)))

PyAPI_FUNC(PyObject *) _PyNumber_Index(PyObject *o);

#endif /* !BACKPORTS_ZSTD_REDEF_ORIG_H */
