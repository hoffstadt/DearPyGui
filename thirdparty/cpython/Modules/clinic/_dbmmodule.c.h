/*[clinic input]
preserve
[clinic start generated code]*/

PyDoc_STRVAR(_dbm_dbm_close__doc__,
"close($self, /)\n"
"--\n"
"\n"
"Close the database.");

#define _DBM_DBM_CLOSE_METHODDEF    \
    {"close", (PyCFunction)_dbm_dbm_close, METH_NOARGS, _dbm_dbm_close__doc__},

static PyObject *
_dbm_dbm_close_impl(dbmobject *self);

static PyObject *
_dbm_dbm_close(dbmobject *self, PyObject *Py_UNUSED(ignored))
{
    return _dbm_dbm_close_impl(self);
}

PyDoc_STRVAR(_dbm_dbm_keys__doc__,
"keys($self, /)\n"
"--\n"
"\n"
"Return a list of all keys in the database.");

#define _DBM_DBM_KEYS_METHODDEF    \
    {"keys", (PyCFunction)_dbm_dbm_keys, METH_NOARGS, _dbm_dbm_keys__doc__},

static PyObject *
_dbm_dbm_keys_impl(dbmobject *self);

static PyObject *
_dbm_dbm_keys(dbmobject *self, PyObject *Py_UNUSED(ignored))
{
    return _dbm_dbm_keys_impl(self);
}

PyDoc_STRVAR(_dbm_dbm_get__doc__,
"get($self, key, default=None, /)\n"
"--\n"
"\n"
"Return the value for key if present, otherwise default.");

#define _DBM_DBM_GET_METHODDEF    \
    {"get", (PyCFunction)(void(*)(void))_dbm_dbm_get, METH_FASTCALL, _dbm_dbm_get__doc__},

static PyObject *
_dbm_dbm_get_impl(dbmobject *self, const char *key,
                  Py_ssize_clean_t key_length, PyObject *default_value);

static PyObject *
_dbm_dbm_get(dbmobject *self, PyObject *const *args, Py_ssize_t nargs)
{
    PyObject *return_value = NULL;
    const char *key;
    Py_ssize_clean_t key_length;
    PyObject *default_value = Py_None;

    if (!_PyArg_ParseStack(args, nargs, "s#|O:get",
        &key, &key_length, &default_value)) {
        goto exit;
    }
    return_value = _dbm_dbm_get_impl(self, key, key_length, default_value);

exit:
    return return_value;
}

PyDoc_STRVAR(_dbm_dbm_setdefault__doc__,
"setdefault($self, key, default=b\'\', /)\n"
"--\n"
"\n"
"Return the value for key if present, otherwise default.\n"
"\n"
"If key is not in the database, it is inserted with default as the value.");

#define _DBM_DBM_SETDEFAULT_METHODDEF    \
    {"setdefault", (PyCFunction)(void(*)(void))_dbm_dbm_setdefault, METH_FASTCALL, _dbm_dbm_setdefault__doc__},

static PyObject *
_dbm_dbm_setdefault_impl(dbmobject *self, const char *key,
                         Py_ssize_clean_t key_length,
                         PyObject *default_value);

static PyObject *
_dbm_dbm_setdefault(dbmobject *self, PyObject *const *args, Py_ssize_t nargs)
{
    PyObject *return_value = NULL;
    const char *key;
    Py_ssize_clean_t key_length;
    PyObject *default_value = NULL;

    if (!_PyArg_ParseStack(args, nargs, "s#|O:setdefault",
        &key, &key_length, &default_value)) {
        goto exit;
    }
    return_value = _dbm_dbm_setdefault_impl(self, key, key_length, default_value);

exit:
    return return_value;
}

PyDoc_STRVAR(dbmopen__doc__,
"open($module, filename, flags=\'r\', mode=0o666, /)\n"
"--\n"
"\n"
"Return a database object.\n"
"\n"
"  filename\n"
"    The filename to open.\n"
"  flags\n"
"    How to open the file.  \"r\" for reading, \"w\" for writing, etc.\n"
"  mode\n"
"    If creating a new file, the mode bits for the new file\n"
"    (e.g. os.O_RDWR).");

#define DBMOPEN_METHODDEF    \
    {"open", (PyCFunction)(void(*)(void))dbmopen, METH_FASTCALL, dbmopen__doc__},

static PyObject *
dbmopen_impl(PyObject *module, PyObject *filename, const char *flags,
             int mode);

static PyObject *
dbmopen(PyObject *module, PyObject *const *args, Py_ssize_t nargs)
{
    PyObject *return_value = NULL;
    PyObject *filename;
    const char *flags = "r";
    int mode = 438;

    if (!_PyArg_CheckPositional("open", nargs, 1, 3)) {
        goto exit;
    }
    if (!PyUnicode_Check(args[0])) {
        _PyArg_BadArgument("open", "argument 1", "str", args[0]);
        goto exit;
    }
    if (PyUnicode_READY(args[0]) == -1) {
        goto exit;
    }
    filename = args[0];
    if (nargs < 2) {
        goto skip_optional;
    }
    if (!PyUnicode_Check(args[1])) {
        _PyArg_BadArgument("open", "argument 2", "str", args[1]);
        goto exit;
    }
    Py_ssize_t flags_length;
    flags = PyUnicode_AsUTF8AndSize(args[1], &flags_length);
    if (flags == NULL) {
        goto exit;
    }
    if (strlen(flags) != (size_t)flags_length) {
        PyErr_SetString(PyExc_ValueError, "embedded null character");
        goto exit;
    }
    if (nargs < 3) {
        goto skip_optional;
    }
    if (PyFloat_Check(args[2])) {
        PyErr_SetString(PyExc_TypeError,
                        "integer argument expected, got float" );
        goto exit;
    }
    mode = _PyLong_AsInt(args[2]);
    if (mode == -1 && PyErr_Occurred()) {
        goto exit;
    }
skip_optional:
    return_value = dbmopen_impl(module, filename, flags, mode);

exit:
    return return_value;
}
/*[clinic end generated code: output=7ced103488cbca7a input=a9049054013a1b77]*/
