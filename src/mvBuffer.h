#pragma once

#include "mvCustomTypes.h"

// methods
int        PymvBuffer_init       (PymvBuffer* self, PyObject* args, PyObject* kwds); // __init__ method
PyObject*  PymvBuffer_clear_value(PymvBuffer* self, PyObject* args, PyObject* kwds); // clear method
PyObject*  PymvBuffer_get_width  (PymvBuffer* self, PyObject* args, PyObject* kwds);
PyObject*  PymvBuffer_get_height (PymvBuffer* self, PyObject* args, PyObject* kwds);

void       PymvBuffer_dealloc    (PymvBuffer* self); // called when deallocated
PyObject*  PymvBuffer_str        (PymvBuffer* self); // string interface
int        PymvBuffer_getbuffer  (PyObject* obj, Py_buffer* view, int flags); // buffer interface function
Py_ssize_t PymvBuffer_getLength  (PyObject* obj);
PyObject*  PymvBuffer_getItem    (PyObject* obj, Py_ssize_t index);
int        PymvBuffer_setItem    (PyObject* obj, Py_ssize_t index, PyObject* value);

static PyBufferProcs PymvBuffer_as_buffer = {
    (getbufferproc)PymvBuffer_getbuffer,
    (releasebufferproc)0,
};

static PySequenceMethods PymvBuffer_as_sequence = {
    (lenfunc)PymvBuffer_getLength,       // length
    (binaryfunc)0,                       // concat
    (ssizeargfunc)0,                     // repeat
    (ssizeargfunc)PymvBuffer_getItem,    // item
    (void*)0,                            // slice
    (ssizeobjargproc)PymvBuffer_setItem, // assignment
    (void*)0,                            // assign slice
    (objobjproc)0,                       // contains
    (binaryfunc)0,                       // inplace concat
    (ssizeargfunc)0                      // inplace repeat
};

static PyMethodDef PymvBuffer_methods[] = {
    {"clear_value", (PyCFunction)PymvBuffer_clear_value, METH_VARARGS | METH_KEYWORDS,
        "clears buffer with specific value."
    },
    {"get_width", (PyCFunction)PymvBuffer_get_width, METH_VARARGS | METH_KEYWORDS,
        "returns buffer width."
    },
    {"get_height", (PyCFunction)PymvBuffer_get_height, METH_VARARGS | METH_KEYWORDS,
        "returns buffer height."
    },

    {NULL}  /* Sentinel */
};

static PyTypeObject PymvBufferType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "_dearpygui.mvBuffer",         /* tp_name */
    sizeof(PymvBuffer),            /* tp_basicsize */
    0,                             /* tp_itemsize */
    (destructor)PymvBuffer_dealloc,/* tp_dealloc */
    0,                             /* tp_vectorcall_offset, tp_print in 3.6, 3.7 */
    0,                             /* tp_getattr */
    0,                             /* tp_setattr */
    0,                             /* tp_as_async */
    (reprfunc)PymvBuffer_str,      /* tp_repr */
    0,                             /* tp_as_number */
    &PymvBuffer_as_sequence,       /* tp_as_sequence */
    0,                             /* tp_as_mapping */
    0,                             /* tp_hash  */
    0,                             /* tp_call */
    (reprfunc)PymvBuffer_str,      /* tp_str */
    0,                             /* tp_getattro */
    0,                             /* tp_setattro */
    &PymvBuffer_as_buffer,         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,            /* tp_flags */
    "mvBuffer object",             /* tp_doc */
    0,                             /* tp_traverse */
    0,                             /* tp_clear */
    0,                             /* tp_richcompare */
    0,                             /* tp_weaklistoffset */
    0,                             /* tp_iter */
    0,                             /* tp_iternext */
    PymvBuffer_methods,            /* tp_methods */
    0,                             /* tp_members */
    0,                             /* tp_getset */
    0,                             /* tp_base */
    0,                             /* tp_dict */
    0,                             /* tp_descr_get */
    0,                             /* tp_descr_set */
    0,                             /* tp_dictoffset */
    (initproc)PymvBuffer_init,     /* tp_init */
    0,                             /* tp_alloc */
    PyType_GenericNew,             /* tp_new */
    PyObject_Free,                 /* tp_free */
    0,                             /* tp_is_gc */
    0,                             /* tp_bases */
    0,                             /* tp_mro */
    0,                             /* tp_cache */
    0,                             /* tp_subclasses */
    0,                             /* tp_weeklist */
    0,                             /* tp_del */
    0,                             /* tp_version_tag */
    0                              /* tp_finalize */
    //0,                           /* tp_vectorcall */
};