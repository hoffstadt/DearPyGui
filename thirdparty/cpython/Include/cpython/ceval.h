#ifndef Py_CPYTHON_CEVAL_H
#  error "this header file must not be included directly"
#endif

#ifdef __cplusplus
extern "C" {
#endif

PyAPI_FUNC(void) PyEval_SetProfile(Py_tracefunc, PyObject *);
PyAPI_DATA(int) _PyEval_SetProfile(PyThreadState *tstate, Py_tracefunc func, PyObject *arg);
PyAPI_FUNC(void) PyEval_SetTrace(Py_tracefunc, PyObject *);
PyAPI_FUNC(int) _PyEval_SetTrace(PyThreadState *tstate, Py_tracefunc func, PyObject *arg);
PyAPI_FUNC(int) _PyEval_GetCoroutineOriginTrackingDepth(void);
PyAPI_FUNC(int) _PyEval_SetAsyncGenFirstiter(PyObject *);
PyAPI_FUNC(PyObject *) _PyEval_GetAsyncGenFirstiter(void);
PyAPI_FUNC(int) _PyEval_SetAsyncGenFinalizer(PyObject *);
PyAPI_FUNC(PyObject *) _PyEval_GetAsyncGenFinalizer(void);

/* Helper to look up a builtin object */
PyAPI_FUNC(PyObject *) _PyEval_GetBuiltinId(_Py_Identifier *);
/* Look at the current frame's (if any) code's co_flags, and turn on
   the corresponding compiler flags in cf->cf_flags.  Return 1 if any
   flag was set, else return 0. */
PyAPI_FUNC(int) PyEval_MergeCompilerFlags(PyCompilerFlags *cf);

PyAPI_FUNC(PyObject *) _PyEval_EvalFrameDefault(PyThreadState *tstate, PyFrameObject *f, int exc);

PyAPI_FUNC(void) _PyEval_SetSwitchInterval(unsigned long microseconds);
PyAPI_FUNC(unsigned long) _PyEval_GetSwitchInterval(void);

PyAPI_FUNC(Py_ssize_t) _PyEval_RequestCodeExtraIndex(freefunc);

PyAPI_FUNC(int) _PyEval_SliceIndex(PyObject *, Py_ssize_t *);
PyAPI_FUNC(int) _PyEval_SliceIndexNotNone(PyObject *, Py_ssize_t *);

#ifdef __cplusplus
}
#endif
