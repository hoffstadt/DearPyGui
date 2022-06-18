/*[clinic input]
preserve
[clinic start generated code]*/

PyDoc_STRVAR(simplequeue_new__doc__,
"SimpleQueue()\n"
"--\n"
"\n"
"Simple, unbounded, reentrant FIFO queue.");

static PyObject *
simplequeue_new_impl(PyTypeObject *type);

static PyObject *
simplequeue_new(PyTypeObject *type, PyObject *args, PyObject *kwargs)
{
    PyObject *return_value = NULL;

    if ((type == &PySimpleQueueType) &&
        !_PyArg_NoPositional("SimpleQueue", args)) {
        goto exit;
    }
    if ((type == &PySimpleQueueType) &&
        !_PyArg_NoKeywords("SimpleQueue", kwargs)) {
        goto exit;
    }
    return_value = simplequeue_new_impl(type);

exit:
    return return_value;
}

PyDoc_STRVAR(_queue_SimpleQueue_put__doc__,
"put($self, /, item, block=True, timeout=None)\n"
"--\n"
"\n"
"Put the item on the queue.\n"
"\n"
"The optional \'block\' and \'timeout\' arguments are ignored, as this method\n"
"never blocks.  They are provided for compatibility with the Queue class.");

#define _QUEUE_SIMPLEQUEUE_PUT_METHODDEF    \
    {"put", (PyCFunction)(void(*)(void))_queue_SimpleQueue_put, METH_FASTCALL|METH_KEYWORDS, _queue_SimpleQueue_put__doc__},

static PyObject *
_queue_SimpleQueue_put_impl(simplequeueobject *self, PyObject *item,
                            int block, PyObject *timeout);

static PyObject *
_queue_SimpleQueue_put(simplequeueobject *self, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"item", "block", "timeout", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "put", 0};
    PyObject *argsbuf[3];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 1;
    PyObject *item;
    int block = 1;
    PyObject *timeout = Py_None;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 3, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    item = args[0];
    if (!noptargs) {
        goto skip_optional_pos;
    }
    if (args[1]) {
        block = PyObject_IsTrue(args[1]);
        if (block < 0) {
            goto exit;
        }
        if (!--noptargs) {
            goto skip_optional_pos;
        }
    }
    timeout = args[2];
skip_optional_pos:
    return_value = _queue_SimpleQueue_put_impl(self, item, block, timeout);

exit:
    return return_value;
}

PyDoc_STRVAR(_queue_SimpleQueue_put_nowait__doc__,
"put_nowait($self, /, item)\n"
"--\n"
"\n"
"Put an item into the queue without blocking.\n"
"\n"
"This is exactly equivalent to `put(item)` and is only provided\n"
"for compatibility with the Queue class.");

#define _QUEUE_SIMPLEQUEUE_PUT_NOWAIT_METHODDEF    \
    {"put_nowait", (PyCFunction)(void(*)(void))_queue_SimpleQueue_put_nowait, METH_FASTCALL|METH_KEYWORDS, _queue_SimpleQueue_put_nowait__doc__},

static PyObject *
_queue_SimpleQueue_put_nowait_impl(simplequeueobject *self, PyObject *item);

static PyObject *
_queue_SimpleQueue_put_nowait(simplequeueobject *self, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"item", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "put_nowait", 0};
    PyObject *argsbuf[1];
    PyObject *item;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 1, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    item = args[0];
    return_value = _queue_SimpleQueue_put_nowait_impl(self, item);

exit:
    return return_value;
}

PyDoc_STRVAR(_queue_SimpleQueue_get__doc__,
"get($self, /, block=True, timeout=None)\n"
"--\n"
"\n"
"Remove and return an item from the queue.\n"
"\n"
"If optional args \'block\' is true and \'timeout\' is None (the default),\n"
"block if necessary until an item is available. If \'timeout\' is\n"
"a non-negative number, it blocks at most \'timeout\' seconds and raises\n"
"the Empty exception if no item was available within that time.\n"
"Otherwise (\'block\' is false), return an item if one is immediately\n"
"available, else raise the Empty exception (\'timeout\' is ignored\n"
"in that case).");

#define _QUEUE_SIMPLEQUEUE_GET_METHODDEF    \
    {"get", (PyCFunction)(void(*)(void))_queue_SimpleQueue_get, METH_FASTCALL|METH_KEYWORDS, _queue_SimpleQueue_get__doc__},

static PyObject *
_queue_SimpleQueue_get_impl(simplequeueobject *self, int block,
                            PyObject *timeout);

static PyObject *
_queue_SimpleQueue_get(simplequeueobject *self, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"block", "timeout", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "get", 0};
    PyObject *argsbuf[2];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 0;
    int block = 1;
    PyObject *timeout = Py_None;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 0, 2, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!noptargs) {
        goto skip_optional_pos;
    }
    if (args[0]) {
        block = PyObject_IsTrue(args[0]);
        if (block < 0) {
            goto exit;
        }
        if (!--noptargs) {
            goto skip_optional_pos;
        }
    }
    timeout = args[1];
skip_optional_pos:
    return_value = _queue_SimpleQueue_get_impl(self, block, timeout);

exit:
    return return_value;
}

PyDoc_STRVAR(_queue_SimpleQueue_get_nowait__doc__,
"get_nowait($self, /)\n"
"--\n"
"\n"
"Remove and return an item from the queue without blocking.\n"
"\n"
"Only get an item if one is immediately available. Otherwise\n"
"raise the Empty exception.");

#define _QUEUE_SIMPLEQUEUE_GET_NOWAIT_METHODDEF    \
    {"get_nowait", (PyCFunction)_queue_SimpleQueue_get_nowait, METH_NOARGS, _queue_SimpleQueue_get_nowait__doc__},

static PyObject *
_queue_SimpleQueue_get_nowait_impl(simplequeueobject *self);

static PyObject *
_queue_SimpleQueue_get_nowait(simplequeueobject *self, PyObject *Py_UNUSED(ignored))
{
    return _queue_SimpleQueue_get_nowait_impl(self);
}

PyDoc_STRVAR(_queue_SimpleQueue_empty__doc__,
"empty($self, /)\n"
"--\n"
"\n"
"Return True if the queue is empty, False otherwise (not reliable!).");

#define _QUEUE_SIMPLEQUEUE_EMPTY_METHODDEF    \
    {"empty", (PyCFunction)_queue_SimpleQueue_empty, METH_NOARGS, _queue_SimpleQueue_empty__doc__},

static int
_queue_SimpleQueue_empty_impl(simplequeueobject *self);

static PyObject *
_queue_SimpleQueue_empty(simplequeueobject *self, PyObject *Py_UNUSED(ignored))
{
    PyObject *return_value = NULL;
    int _return_value;

    _return_value = _queue_SimpleQueue_empty_impl(self);
    if ((_return_value == -1) && PyErr_Occurred()) {
        goto exit;
    }
    return_value = PyBool_FromLong((long)_return_value);

exit:
    return return_value;
}

PyDoc_STRVAR(_queue_SimpleQueue_qsize__doc__,
"qsize($self, /)\n"
"--\n"
"\n"
"Return the approximate size of the queue (not reliable!).");

#define _QUEUE_SIMPLEQUEUE_QSIZE_METHODDEF    \
    {"qsize", (PyCFunction)_queue_SimpleQueue_qsize, METH_NOARGS, _queue_SimpleQueue_qsize__doc__},

static Py_ssize_t
_queue_SimpleQueue_qsize_impl(simplequeueobject *self);

static PyObject *
_queue_SimpleQueue_qsize(simplequeueobject *self, PyObject *Py_UNUSED(ignored))
{
    PyObject *return_value = NULL;
    Py_ssize_t _return_value;

    _return_value = _queue_SimpleQueue_qsize_impl(self);
    if ((_return_value == -1) && PyErr_Occurred()) {
        goto exit;
    }
    return_value = PyLong_FromSsize_t(_return_value);

exit:
    return return_value;
}
/*[clinic end generated code: output=b4717e2974cbc909 input=a9049054013a1b77]*/
