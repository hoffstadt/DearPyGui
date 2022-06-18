#ifndef Py_CPYTHON_ERRORS_H
#  error "this header file must not be included directly"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Error objects */

/* PyException_HEAD defines the initial segment of every exception class. */
#define PyException_HEAD PyObject_HEAD PyObject *dict;\
             PyObject *args; PyObject *traceback;\
             PyObject *context; PyObject *cause;\
             char suppress_context;

typedef struct {
    PyException_HEAD
} PyBaseExceptionObject;

typedef struct {
    PyException_HEAD
    PyObject *msg;
    PyObject *filename;
    PyObject *lineno;
    PyObject *offset;
    PyObject *text;
    PyObject *print_file_and_line;
} PySyntaxErrorObject;

typedef struct {
    PyException_HEAD
    PyObject *msg;
    PyObject *name;
    PyObject *path;
} PyImportErrorObject;

typedef struct {
    PyException_HEAD
    PyObject *encoding;
    PyObject *object;
    Py_ssize_t start;
    Py_ssize_t end;
    PyObject *reason;
} PyUnicodeErrorObject;

typedef struct {
    PyException_HEAD
    PyObject *code;
} PySystemExitObject;

typedef struct {
    PyException_HEAD
    PyObject *myerrno;
    PyObject *strerror;
    PyObject *filename;
    PyObject *filename2;
#ifdef MS_WINDOWS
    PyObject *winerror;
#endif
    Py_ssize_t written;   /* only for BlockingIOError, -1 otherwise */
} PyOSErrorObject;

typedef struct {
    PyException_HEAD
    PyObject *value;
} PyStopIterationObject;

/* Compatibility typedefs */
typedef PyOSErrorObject PyEnvironmentErrorObject;
#ifdef MS_WINDOWS
typedef PyOSErrorObject PyWindowsErrorObject;
#endif

/* Error handling definitions */

PyAPI_FUNC(void) _PyErr_SetKeyError(PyObject *);
PyAPI_FUNC(_PyErr_StackItem*) _PyErr_GetTopmostException(PyThreadState *tstate);
PyAPI_FUNC(void) _PyErr_GetExcInfo(PyThreadState *, PyObject **, PyObject **, PyObject **);

/* Context manipulation (PEP 3134) */

PyAPI_FUNC(void) _PyErr_ChainExceptions(PyObject *, PyObject *, PyObject *);

/* */

#define PyExceptionClass_Name(x)  (((PyTypeObject*)(x))->tp_name)

/* Convenience functions */

#ifdef MS_WINDOWS
Py_DEPRECATED(3.3)
PyAPI_FUNC(PyObject *) PyErr_SetFromErrnoWithUnicodeFilename(
    PyObject *, const Py_UNICODE *);
#endif /* MS_WINDOWS */

/* Like PyErr_Format(), but saves current exception as __context__ and
   __cause__.
 */
PyAPI_FUNC(PyObject *) _PyErr_FormatFromCause(
    PyObject *exception,
    const char *format,   /* ASCII-encoded string  */
    ...
    );

#ifdef MS_WINDOWS
/* XXX redeclare to use WSTRING */
Py_DEPRECATED(3.3)
PyAPI_FUNC(PyObject *) PyErr_SetFromWindowsErrWithUnicodeFilename(
    int, const Py_UNICODE *);
Py_DEPRECATED(3.3)
PyAPI_FUNC(PyObject *) PyErr_SetExcFromWindowsErrWithUnicodeFilename(
    PyObject *,int, const Py_UNICODE *);
#endif

/* In exceptions.c */

/* Helper that attempts to replace the current exception with one of the
 * same type but with a prefix added to the exception text. The resulting
 * exception description looks like:
 *
 *     prefix (exc_type: original_exc_str)
 *
 * Only some exceptions can be safely replaced. If the function determines
 * it isn't safe to perform the replacement, it will leave the original
 * unmodified exception in place.
 *
 * Returns a borrowed reference to the new exception (if any), NULL if the
 * existing exception was left in place.
 */
PyAPI_FUNC(PyObject *) _PyErr_TrySetFromCause(
    const char *prefix_format,   /* ASCII-encoded string  */
    ...
    );

/* In signalmodule.c */

int PySignal_SetWakeupFd(int fd);
PyAPI_FUNC(int) _PyErr_CheckSignals(void);

/* Support for adding program text to SyntaxErrors */

PyAPI_FUNC(void) PyErr_SyntaxLocationObject(
    PyObject *filename,
    int lineno,
    int col_offset);

PyAPI_FUNC(PyObject *) PyErr_ProgramTextObject(
    PyObject *filename,
    int lineno);

/* Create a UnicodeEncodeError object.
 *
 * TODO: This API will be removed in Python 3.11.
 */
Py_DEPRECATED(3.3) PyAPI_FUNC(PyObject *) PyUnicodeEncodeError_Create(
    const char *encoding,       /* UTF-8 encoded string */
    const Py_UNICODE *object,
    Py_ssize_t length,
    Py_ssize_t start,
    Py_ssize_t end,
    const char *reason          /* UTF-8 encoded string */
    );

/* Create a UnicodeTranslateError object.
 *
 * TODO: This API will be removed in Python 3.11.
 */
Py_DEPRECATED(3.3) PyAPI_FUNC(PyObject *) PyUnicodeTranslateError_Create(
    const Py_UNICODE *object,
    Py_ssize_t length,
    Py_ssize_t start,
    Py_ssize_t end,
    const char *reason          /* UTF-8 encoded string */
    );
PyAPI_FUNC(PyObject *) _PyUnicodeTranslateError_Create(
    PyObject *object,
    Py_ssize_t start,
    Py_ssize_t end,
    const char *reason          /* UTF-8 encoded string */
    );

PyAPI_FUNC(void) _PyErr_WriteUnraisableMsg(
    const char *err_msg,
    PyObject *obj);

PyAPI_FUNC(void) _Py_NO_RETURN _Py_FatalErrorFunc(
    const char *func,
    const char *message);

PyAPI_FUNC(void) _Py_NO_RETURN _Py_FatalErrorFormat(
    const char *func,
    const char *format,
    ...);

#define Py_FatalError(message) _Py_FatalErrorFunc(__func__, message)

#ifdef __cplusplus
}
#endif
