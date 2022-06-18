
/* Traceback implementation */

#include "Python.h"

#include "code.h"
#include "frameobject.h"          // PyFrame_GetBack()
#include "structmember.h"         // PyMemberDef
#include "osdefs.h"               // SEP
#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif

#define OFF(x) offsetof(PyTracebackObject, x)

#define PUTS(fd, str) _Py_write_noraise(fd, str, (int)strlen(str))
#define MAX_STRING_LENGTH 500
#define MAX_FRAME_DEPTH 100
#define MAX_NTHREADS 100

/* Function from Parser/tokenizer.c */
extern char * PyTokenizer_FindEncodingFilename(int, PyObject *);

_Py_IDENTIFIER(TextIOWrapper);
_Py_IDENTIFIER(close);
_Py_IDENTIFIER(open);
_Py_IDENTIFIER(path);

/*[clinic input]
class TracebackType "PyTracebackObject *" "&PyTraceback_Type"
[clinic start generated code]*/
/*[clinic end generated code: output=da39a3ee5e6b4b0d input=928fa06c10151120]*/

#include "clinic/traceback.c.h"

static PyObject *
tb_create_raw(PyTracebackObject *next, PyFrameObject *frame, int lasti,
              int lineno)
{
    PyTracebackObject *tb;
    if ((next != NULL && !PyTraceBack_Check(next)) ||
                    frame == NULL || !PyFrame_Check(frame)) {
        PyErr_BadInternalCall();
        return NULL;
    }
    tb = PyObject_GC_New(PyTracebackObject, &PyTraceBack_Type);
    if (tb != NULL) {
        Py_XINCREF(next);
        tb->tb_next = next;
        Py_XINCREF(frame);
        tb->tb_frame = frame;
        tb->tb_lasti = lasti;
        tb->tb_lineno = lineno;
        PyObject_GC_Track(tb);
    }
    return (PyObject *)tb;
}

/*[clinic input]
@classmethod
TracebackType.__new__ as tb_new

  tb_next: object
  tb_frame: object(type='PyFrameObject *', subclass_of='&PyFrame_Type')
  tb_lasti: int
  tb_lineno: int

Create a new traceback object.
[clinic start generated code]*/

static PyObject *
tb_new_impl(PyTypeObject *type, PyObject *tb_next, PyFrameObject *tb_frame,
            int tb_lasti, int tb_lineno)
/*[clinic end generated code: output=fa077debd72d861a input=01cbe8ec8783fca7]*/
{
    if (tb_next == Py_None) {
        tb_next = NULL;
    } else if (!PyTraceBack_Check(tb_next)) {
        return PyErr_Format(PyExc_TypeError,
                            "expected traceback object or None, got '%s'",
                            Py_TYPE(tb_next)->tp_name);
    }

    return tb_create_raw((PyTracebackObject *)tb_next, tb_frame, tb_lasti,
                         tb_lineno);
}

static PyObject *
tb_dir(PyTracebackObject *self, PyObject *Py_UNUSED(ignored))
{
    return Py_BuildValue("[ssss]", "tb_frame", "tb_next",
                                   "tb_lasti", "tb_lineno");
}

static PyObject *
tb_next_get(PyTracebackObject *self, void *Py_UNUSED(_))
{
    PyObject* ret = (PyObject*)self->tb_next;
    if (!ret) {
        ret = Py_None;
    }
    Py_INCREF(ret);
    return ret;
}

static int
tb_next_set(PyTracebackObject *self, PyObject *new_next, void *Py_UNUSED(_))
{
    if (!new_next) {
        PyErr_Format(PyExc_TypeError, "can't delete tb_next attribute");
        return -1;
    }

    /* We accept None or a traceback object, and map None -> NULL (inverse of
       tb_next_get) */
    if (new_next == Py_None) {
        new_next = NULL;
    } else if (!PyTraceBack_Check(new_next)) {
        PyErr_Format(PyExc_TypeError,
                     "expected traceback object, got '%s'",
                     Py_TYPE(new_next)->tp_name);
        return -1;
    }

    /* Check for loops */
    PyTracebackObject *cursor = (PyTracebackObject *)new_next;
    while (cursor) {
        if (cursor == self) {
            PyErr_Format(PyExc_ValueError, "traceback loop detected");
            return -1;
        }
        cursor = cursor->tb_next;
    }

    PyObject *old_next = (PyObject*)self->tb_next;
    Py_XINCREF(new_next);
    self->tb_next = (PyTracebackObject *)new_next;
    Py_XDECREF(old_next);

    return 0;
}


static PyMethodDef tb_methods[] = {
   {"__dir__", (PyCFunction)tb_dir, METH_NOARGS},
   {NULL, NULL, 0, NULL},
};

static PyMemberDef tb_memberlist[] = {
    {"tb_frame",        T_OBJECT,       OFF(tb_frame),  READONLY|READ_RESTRICTED},
    {"tb_lasti",        T_INT,          OFF(tb_lasti),  READONLY},
    {"tb_lineno",       T_INT,          OFF(tb_lineno), READONLY},
    {NULL}      /* Sentinel */
};

static PyGetSetDef tb_getsetters[] = {
    {"tb_next", (getter)tb_next_get, (setter)tb_next_set, NULL, NULL},
    {NULL}      /* Sentinel */
};

static void
tb_dealloc(PyTracebackObject *tb)
{
    PyObject_GC_UnTrack(tb);
    Py_TRASHCAN_BEGIN(tb, tb_dealloc)
    Py_XDECREF(tb->tb_next);
    Py_XDECREF(tb->tb_frame);
    PyObject_GC_Del(tb);
    Py_TRASHCAN_END
}

static int
tb_traverse(PyTracebackObject *tb, visitproc visit, void *arg)
{
    Py_VISIT(tb->tb_next);
    Py_VISIT(tb->tb_frame);
    return 0;
}

static int
tb_clear(PyTracebackObject *tb)
{
    Py_CLEAR(tb->tb_next);
    Py_CLEAR(tb->tb_frame);
    return 0;
}

PyTypeObject PyTraceBack_Type = {
    PyVarObject_HEAD_INIT(&PyType_Type, 0)
    "traceback",
    sizeof(PyTracebackObject),
    0,
    (destructor)tb_dealloc, /*tp_dealloc*/
    0,                  /*tp_vectorcall_offset*/
    0,    /*tp_getattr*/
    0,                  /*tp_setattr*/
    0,                  /*tp_as_async*/
    0,                  /*tp_repr*/
    0,                  /*tp_as_number*/
    0,                  /*tp_as_sequence*/
    0,                  /*tp_as_mapping*/
    0,                  /* tp_hash */
    0,                  /* tp_call */
    0,                  /* tp_str */
    PyObject_GenericGetAttr,                    /* tp_getattro */
    0,                  /* tp_setattro */
    0,                                          /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_GC,/* tp_flags */
    tb_new__doc__,                              /* tp_doc */
    (traverseproc)tb_traverse,                  /* tp_traverse */
    (inquiry)tb_clear,                          /* tp_clear */
    0,                                          /* tp_richcompare */
    0,                                          /* tp_weaklistoffset */
    0,                                          /* tp_iter */
    0,                                          /* tp_iternext */
    tb_methods,         /* tp_methods */
    tb_memberlist,      /* tp_members */
    tb_getsetters,                              /* tp_getset */
    0,                                          /* tp_base */
    0,                                          /* tp_dict */
    0,                                          /* tp_descr_get */
    0,                                          /* tp_descr_set */
    0,                                          /* tp_dictoffset */
    0,                                          /* tp_init */
    0,                                          /* tp_alloc */
    tb_new,                                     /* tp_new */
};


PyObject*
_PyTraceBack_FromFrame(PyObject *tb_next, PyFrameObject *frame)
{
    assert(tb_next == NULL || PyTraceBack_Check(tb_next));
    assert(frame != NULL);

    return tb_create_raw((PyTracebackObject *)tb_next, frame, frame->f_lasti,
                         PyFrame_GetLineNumber(frame));
}


int
PyTraceBack_Here(PyFrameObject *frame)
{
    PyObject *exc, *val, *tb, *newtb;
    PyErr_Fetch(&exc, &val, &tb);
    newtb = _PyTraceBack_FromFrame(tb, frame);
    if (newtb == NULL) {
        _PyErr_ChainExceptions(exc, val, tb);
        return -1;
    }
    PyErr_Restore(exc, val, newtb);
    Py_XDECREF(tb);
    return 0;
}

/* Insert a frame into the traceback for (funcname, filename, lineno). */
void _PyTraceback_Add(const char *funcname, const char *filename, int lineno)
{
    PyObject *globals;
    PyCodeObject *code;
    PyFrameObject *frame;
    PyObject *exc, *val, *tb;

    /* Save and clear the current exception. Python functions must not be
       called with an exception set. Calling Python functions happens when
       the codec of the filesystem encoding is implemented in pure Python. */
    PyErr_Fetch(&exc, &val, &tb);

    globals = PyDict_New();
    if (!globals)
        goto error;
    code = PyCode_NewEmpty(filename, funcname, lineno);
    if (!code) {
        Py_DECREF(globals);
        goto error;
    }
    frame = PyFrame_New(PyThreadState_Get(), code, globals, NULL);
    Py_DECREF(globals);
    Py_DECREF(code);
    if (!frame)
        goto error;
    frame->f_lineno = lineno;

    PyErr_Restore(exc, val, tb);
    PyTraceBack_Here(frame);
    Py_DECREF(frame);
    return;

error:
    _PyErr_ChainExceptions(exc, val, tb);
}

static PyObject *
_Py_FindSourceFile(PyObject *filename, char* namebuf, size_t namelen, PyObject *io)
{
    Py_ssize_t i;
    PyObject *binary;
    PyObject *v;
    Py_ssize_t npath;
    size_t taillen;
    PyObject *syspath;
    PyObject *path;
    const char* tail;
    PyObject *filebytes;
    const char* filepath;
    Py_ssize_t len;
    PyObject* result;

    filebytes = PyUnicode_EncodeFSDefault(filename);
    if (filebytes == NULL) {
        PyErr_Clear();
        return NULL;
    }
    filepath = PyBytes_AS_STRING(filebytes);

    /* Search tail of filename in sys.path before giving up */
    tail = strrchr(filepath, SEP);
    if (tail == NULL)
        tail = filepath;
    else
        tail++;
    taillen = strlen(tail);

    syspath = _PySys_GetObjectId(&PyId_path);
    if (syspath == NULL || !PyList_Check(syspath))
        goto error;
    npath = PyList_Size(syspath);

    for (i = 0; i < npath; i++) {
        v = PyList_GetItem(syspath, i);
        if (v == NULL) {
            PyErr_Clear();
            break;
        }
        if (!PyUnicode_Check(v))
            continue;
        path = PyUnicode_EncodeFSDefault(v);
        if (path == NULL) {
            PyErr_Clear();
            continue;
        }
        len = PyBytes_GET_SIZE(path);
        if (len + 1 + (Py_ssize_t)taillen >= (Py_ssize_t)namelen - 1) {
            Py_DECREF(path);
            continue; /* Too long */
        }
        strcpy(namebuf, PyBytes_AS_STRING(path));
        Py_DECREF(path);
        if (strlen(namebuf) != (size_t)len)
            continue; /* v contains '\0' */
        if (len > 0 && namebuf[len-1] != SEP)
            namebuf[len++] = SEP;
        strcpy(namebuf+len, tail);

        binary = _PyObject_CallMethodId(io, &PyId_open, "ss", namebuf, "rb");
        if (binary != NULL) {
            result = binary;
            goto finally;
        }
        PyErr_Clear();
    }
    goto error;

error:
    result = NULL;
finally:
    Py_DECREF(filebytes);
    return result;
}

int
_Py_DisplaySourceLine(PyObject *f, PyObject *filename, int lineno, int indent)
{
    int err = 0;
    int fd;
    int i;
    char *found_encoding;
    const char *encoding;
    PyObject *io;
    PyObject *binary;
    PyObject *fob = NULL;
    PyObject *lineobj = NULL;
    PyObject *res;
    char buf[MAXPATHLEN+1];
    int kind;
    const void *data;

    /* open the file */
    if (filename == NULL)
        return 0;

    io = PyImport_ImportModuleNoBlock("io");
    if (io == NULL)
        return -1;
    binary = _PyObject_CallMethodId(io, &PyId_open, "Os", filename, "rb");

    if (binary == NULL) {
        PyErr_Clear();

        binary = _Py_FindSourceFile(filename, buf, sizeof(buf), io);
        if (binary == NULL) {
            Py_DECREF(io);
            return -1;
        }
    }

    /* use the right encoding to decode the file as unicode */
    fd = PyObject_AsFileDescriptor(binary);
    if (fd < 0) {
        Py_DECREF(io);
        Py_DECREF(binary);
        return 0;
    }
    found_encoding = PyTokenizer_FindEncodingFilename(fd, filename);
    if (found_encoding == NULL)
        PyErr_Clear();
    encoding = (found_encoding != NULL) ? found_encoding : "utf-8";
    /* Reset position */
    if (lseek(fd, 0, SEEK_SET) == (off_t)-1) {
        Py_DECREF(io);
        Py_DECREF(binary);
        PyMem_FREE(found_encoding);
        return 0;
    }
    fob = _PyObject_CallMethodId(io, &PyId_TextIOWrapper, "Os", binary, encoding);
    Py_DECREF(io);
    PyMem_FREE(found_encoding);

    if (fob == NULL) {
        PyErr_Clear();

        res = _PyObject_CallMethodIdNoArgs(binary, &PyId_close);
        Py_DECREF(binary);
        if (res)
            Py_DECREF(res);
        else
            PyErr_Clear();
        return 0;
    }
    Py_DECREF(binary);

    /* get the line number lineno */
    for (i = 0; i < lineno; i++) {
        Py_XDECREF(lineobj);
        lineobj = PyFile_GetLine(fob, -1);
        if (!lineobj) {
            PyErr_Clear();
            err = -1;
            break;
        }
    }
    res = _PyObject_CallMethodIdNoArgs(fob, &PyId_close);
    if (res)
        Py_DECREF(res);
    else
        PyErr_Clear();
    Py_DECREF(fob);
    if (!lineobj || !PyUnicode_Check(lineobj)) {
        Py_XDECREF(lineobj);
        return err;
    }

    /* remove the indentation of the line */
    kind = PyUnicode_KIND(lineobj);
    data = PyUnicode_DATA(lineobj);
    for (i=0; i < PyUnicode_GET_LENGTH(lineobj); i++) {
        Py_UCS4 ch = PyUnicode_READ(kind, data, i);
        if (ch != ' ' && ch != '\t' && ch != '\014')
            break;
    }
    if (i) {
        PyObject *truncated;
        truncated = PyUnicode_Substring(lineobj, i, PyUnicode_GET_LENGTH(lineobj));
        if (truncated) {
            Py_DECREF(lineobj);
            lineobj = truncated;
        } else {
            PyErr_Clear();
        }
    }

    /* Write some spaces before the line */
    strcpy(buf, "          ");
    assert (strlen(buf) == 10);
    while (indent > 0) {
        if (indent < 10)
            buf[indent] = '\0';
        err = PyFile_WriteString(buf, f);
        if (err != 0)
            break;
        indent -= 10;
    }

    /* finally display the line */
    if (err == 0)
        err = PyFile_WriteObject(lineobj, f, Py_PRINT_RAW);
    Py_DECREF(lineobj);
    if  (err == 0)
        err = PyFile_WriteString("\n", f);
    return err;
}

static int
tb_displayline(PyObject *f, PyObject *filename, int lineno, PyObject *name)
{
    int err;
    PyObject *line;

    if (filename == NULL || name == NULL)
        return -1;
    line = PyUnicode_FromFormat("  File \"%U\", line %d, in %U\n",
                                filename, lineno, name);
    if (line == NULL)
        return -1;
    err = PyFile_WriteObject(line, f, Py_PRINT_RAW);
    Py_DECREF(line);
    if (err != 0)
        return err;
    /* ignore errors since we can't report them, can we? */
    if (_Py_DisplaySourceLine(f, filename, lineno, 4))
        PyErr_Clear();
    return err;
}

static const int TB_RECURSIVE_CUTOFF = 3; // Also hardcoded in traceback.py.

static int
tb_print_line_repeated(PyObject *f, long cnt)
{
    cnt -= TB_RECURSIVE_CUTOFF;
    PyObject *line = PyUnicode_FromFormat(
        (cnt > 1)
          ? "  [Previous line repeated %ld more times]\n"
          : "  [Previous line repeated %ld more time]\n",
        cnt);
    if (line == NULL) {
        return -1;
    }
    int err = PyFile_WriteObject(line, f, Py_PRINT_RAW);
    Py_DECREF(line);
    return err;
}

static int
tb_printinternal(PyTracebackObject *tb, PyObject *f, long limit)
{
    int err = 0;
    Py_ssize_t depth = 0;
    PyObject *last_file = NULL;
    int last_line = -1;
    PyObject *last_name = NULL;
    long cnt = 0;
    PyTracebackObject *tb1 = tb;
    while (tb1 != NULL) {
        depth++;
        tb1 = tb1->tb_next;
    }
    while (tb != NULL && depth > limit) {
        depth--;
        tb = tb->tb_next;
    }
    while (tb != NULL && err == 0) {
        PyCodeObject *code = PyFrame_GetCode(tb->tb_frame);
        if (last_file == NULL ||
            code->co_filename != last_file ||
            last_line == -1 || tb->tb_lineno != last_line ||
            last_name == NULL || code->co_name != last_name) {
            if (cnt > TB_RECURSIVE_CUTOFF) {
                err = tb_print_line_repeated(f, cnt);
            }
            last_file = code->co_filename;
            last_line = tb->tb_lineno;
            last_name = code->co_name;
            cnt = 0;
        }
        cnt++;
        if (err == 0 && cnt <= TB_RECURSIVE_CUTOFF) {
            err = tb_displayline(f, code->co_filename, tb->tb_lineno,
                                 code->co_name);
            if (err == 0) {
                err = PyErr_CheckSignals();
            }
        }
        Py_DECREF(code);
        tb = tb->tb_next;
    }
    if (err == 0 && cnt > TB_RECURSIVE_CUTOFF) {
        err = tb_print_line_repeated(f, cnt);
    }
    return err;
}

#define PyTraceBack_LIMIT 1000

int
PyTraceBack_Print(PyObject *v, PyObject *f)
{
    int err;
    PyObject *limitv;
    long limit = PyTraceBack_LIMIT;

    if (v == NULL)
        return 0;
    if (!PyTraceBack_Check(v)) {
        PyErr_BadInternalCall();
        return -1;
    }
    limitv = PySys_GetObject("tracebacklimit");
    if (limitv && PyLong_Check(limitv)) {
        int overflow;
        limit = PyLong_AsLongAndOverflow(limitv, &overflow);
        if (overflow > 0) {
            limit = LONG_MAX;
        }
        else if (limit <= 0) {
            return 0;
        }
    }
    err = PyFile_WriteString("Traceback (most recent call last):\n", f);
    if (!err)
        err = tb_printinternal((PyTracebackObject *)v, f, limit);
    return err;
}

/* Format an integer in range [0; 0xffffffff] to decimal and write it
   into the file fd.

   This function is signal safe. */

void
_Py_DumpDecimal(int fd, unsigned long value)
{
    /* maximum number of characters required for output of %lld or %p.
       We need at most ceil(log10(256)*SIZEOF_LONG_LONG) digits,
       plus 1 for the null byte.  53/22 is an upper bound for log10(256). */
    char buffer[1 + (sizeof(unsigned long)*53-1) / 22 + 1];
    char *ptr, *end;

    end = &buffer[Py_ARRAY_LENGTH(buffer) - 1];
    ptr = end;
    *ptr = '\0';
    do {
        --ptr;
        assert(ptr >= buffer);
        *ptr = '0' + (value % 10);
        value /= 10;
    } while (value);

    _Py_write_noraise(fd, ptr, end - ptr);
}

/* Format an integer in range [0; 0xffffffff] to hexadecimal of 'width' digits,
   and write it into the file fd.

   This function is signal safe. */

void
_Py_DumpHexadecimal(int fd, unsigned long value, Py_ssize_t width)
{
    char buffer[sizeof(unsigned long) * 2 + 1], *ptr, *end;
    const Py_ssize_t size = Py_ARRAY_LENGTH(buffer) - 1;

    if (width > size)
        width = size;
    /* it's ok if width is negative */

    end = &buffer[size];
    ptr = end;
    *ptr = '\0';
    do {
        --ptr;
        assert(ptr >= buffer);
        *ptr = Py_hexdigits[value & 15];
        value >>= 4;
    } while ((end - ptr) < width || value);

    _Py_write_noraise(fd, ptr, end - ptr);
}

void
_Py_DumpASCII(int fd, PyObject *text)
{
    PyASCIIObject *ascii = (PyASCIIObject *)text;
    Py_ssize_t i, size;
    int truncated;
    int kind;
    void *data = NULL;
    wchar_t *wstr = NULL;
    Py_UCS4 ch;

    if (!PyUnicode_Check(text))
        return;

    size = ascii->length;
    kind = ascii->state.kind;
    if (kind == PyUnicode_WCHAR_KIND) {
        wstr = ((PyASCIIObject *)text)->wstr;
        if (wstr == NULL)
            return;
        size = ((PyCompactUnicodeObject *)text)->wstr_length;
    }
    else if (ascii->state.compact) {
        if (ascii->state.ascii)
            data = ((PyASCIIObject*)text) + 1;
        else
            data = ((PyCompactUnicodeObject*)text) + 1;
    }
    else {
        data = ((PyUnicodeObject *)text)->data.any;
        if (data == NULL)
            return;
    }

    if (MAX_STRING_LENGTH < size) {
        size = MAX_STRING_LENGTH;
        truncated = 1;
    }
    else {
        truncated = 0;
    }

    // Is an ASCII string?
    if (ascii->state.ascii) {
        assert(kind == PyUnicode_1BYTE_KIND);
        char *str = data;

        int need_escape = 0;
        for (i=0; i < size; i++) {
            ch = str[i];
            if (!(' ' <= ch && ch <= 126)) {
                need_escape = 1;
                break;
            }
        }
        if (!need_escape) {
            // The string can be written with a single write() syscall
            _Py_write_noraise(fd, str, size);
            goto done;
        }
    }

    for (i=0; i < size; i++) {
        if (kind != PyUnicode_WCHAR_KIND)
            ch = PyUnicode_READ(kind, data, i);
        else
            ch = wstr[i];
        if (' ' <= ch && ch <= 126) {
            /* printable ASCII character */
            char c = (char)ch;
            _Py_write_noraise(fd, &c, 1);
        }
        else if (ch <= 0xff) {
            PUTS(fd, "\\x");
            _Py_DumpHexadecimal(fd, ch, 2);
        }
        else if (ch <= 0xffff) {
            PUTS(fd, "\\u");
            _Py_DumpHexadecimal(fd, ch, 4);
        }
        else {
            PUTS(fd, "\\U");
            _Py_DumpHexadecimal(fd, ch, 8);
        }
    }

done:
    if (truncated) {
        PUTS(fd, "...");
    }
}

/* Write a frame into the file fd: "File "xxx", line xxx in xxx".

   This function is signal safe. */

static void
dump_frame(int fd, PyFrameObject *frame)
{
    PyCodeObject *code = PyFrame_GetCode(frame);
    PUTS(fd, "  File ");
    if (code->co_filename != NULL
        && PyUnicode_Check(code->co_filename))
    {
        PUTS(fd, "\"");
        _Py_DumpASCII(fd, code->co_filename);
        PUTS(fd, "\"");
    } else {
        PUTS(fd, "???");
    }

    /* PyFrame_GetLineNumber() was introduced in Python 2.7.0 and 3.2.0 */
    int lineno = PyCode_Addr2Line(code, frame->f_lasti);
    PUTS(fd, ", line ");
    if (lineno >= 0) {
        _Py_DumpDecimal(fd, (unsigned long)lineno);
    }
    else {
        PUTS(fd, "???");
    }
    PUTS(fd, " in ");

    if (code->co_name != NULL
       && PyUnicode_Check(code->co_name)) {
        _Py_DumpASCII(fd, code->co_name);
    }
    else {
        PUTS(fd, "???");
    }

    PUTS(fd, "\n");
    Py_DECREF(code);
}

static void
dump_traceback(int fd, PyThreadState *tstate, int write_header)
{
    PyFrameObject *frame;
    unsigned int depth;

    if (write_header) {
        PUTS(fd, "Stack (most recent call first):\n");
    }

    // Use a borrowed reference. Avoid Py_INCREF/Py_DECREF, since this function
    // can be called in a signal handler by the faulthandler module which must
    // not modify Python objects.
    frame = tstate->frame;
    if (frame == NULL) {
        PUTS(fd, "<no Python frame>\n");
        return;
    }

    depth = 0;
    while (1) {
        if (MAX_FRAME_DEPTH <= depth) {
            PUTS(fd, "  ...\n");
            break;
        }
        if (!PyFrame_Check(frame)) {
            break;
        }
        dump_frame(fd, frame);
        PyFrameObject *back = frame->f_back;

        if (back == NULL) {
            break;
        }
        frame = back;
        depth++;
    }
}

/* Dump the traceback of a Python thread into fd. Use write() to write the
   traceback and retry if write() is interrupted by a signal (failed with
   EINTR), but don't call the Python signal handler.

   The caller is responsible to call PyErr_CheckSignals() to call Python signal
   handlers if signals were received. */
void
_Py_DumpTraceback(int fd, PyThreadState *tstate)
{
    dump_traceback(fd, tstate, 1);
}

/* Write the thread identifier into the file 'fd': "Current thread 0xHHHH:\" if
   is_current is true, "Thread 0xHHHH:\n" otherwise.

   This function is signal safe. */

static void
write_thread_id(int fd, PyThreadState *tstate, int is_current)
{
    if (is_current)
        PUTS(fd, "Current thread 0x");
    else
        PUTS(fd, "Thread 0x");
    _Py_DumpHexadecimal(fd,
                        tstate->thread_id,
                        sizeof(unsigned long) * 2);
    PUTS(fd, " (most recent call first):\n");
}

/* Dump the traceback of all Python threads into fd. Use write() to write the
   traceback and retry if write() is interrupted by a signal (failed with
   EINTR), but don't call the Python signal handler.

   The caller is responsible to call PyErr_CheckSignals() to call Python signal
   handlers if signals were received. */
const char*
_Py_DumpTracebackThreads(int fd, PyInterpreterState *interp,
                         PyThreadState *current_tstate)
{
    PyThreadState *tstate;
    unsigned int nthreads;

    if (current_tstate == NULL) {
        /* _Py_DumpTracebackThreads() is called from signal handlers by
           faulthandler.

           SIGSEGV, SIGFPE, SIGABRT, SIGBUS and SIGILL are synchronous signals
           and are thus delivered to the thread that caused the fault. Get the
           Python thread state of the current thread.

           PyThreadState_Get() doesn't give the state of the thread that caused
           the fault if the thread released the GIL, and so
           _PyThreadState_GET() cannot be used. Read the thread specific
           storage (TSS) instead: call PyGILState_GetThisThreadState(). */
        current_tstate = PyGILState_GetThisThreadState();
    }

    if (interp == NULL) {
        if (current_tstate == NULL) {
            interp = _PyGILState_GetInterpreterStateUnsafe();
            if (interp == NULL) {
                /* We need the interpreter state to get Python threads */
                return "unable to get the interpreter state";
            }
        }
        else {
            interp = current_tstate->interp;
        }
    }
    assert(interp != NULL);

    /* Get the current interpreter from the current thread */
    tstate = PyInterpreterState_ThreadHead(interp);
    if (tstate == NULL)
        return "unable to get the thread head state";

    /* Dump the traceback of each thread */
    tstate = PyInterpreterState_ThreadHead(interp);
    nthreads = 0;
    _Py_BEGIN_SUPPRESS_IPH
    do
    {
        if (nthreads != 0)
            PUTS(fd, "\n");
        if (nthreads >= MAX_NTHREADS) {
            PUTS(fd, "...\n");
            break;
        }
        write_thread_id(fd, tstate, tstate == current_tstate);
        dump_traceback(fd, tstate, 0);
        tstate = PyThreadState_Next(tstate);
        nthreads++;
    } while (tstate != NULL);
    _Py_END_SUPPRESS_IPH

    return NULL;
}

