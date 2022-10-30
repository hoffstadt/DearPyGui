#pragma once

#include <Python.h>
#include "mvMath.h"

//-----------------------------------------------------------------------------
// mvBuffer
//-----------------------------------------------------------------------------
struct mvBuffer
{
    float* data;
    long length;
    int width;
    int height;
};

struct PymvBuffer
{
    PyObject_HEAD
    mvBuffer arr;
};

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

//-----------------------------------------------------------------------------
// mvVec4
//-----------------------------------------------------------------------------

struct PymvVec4
{
    PyObject_HEAD
    mvVec4 vec4;
};

// methods
int        PymvVec4_init       (PymvVec4* self, PyObject* args, PyObject* kwds); // __init__ method

void       PymvVec4_dealloc    (PymvVec4* self); // called when deallocated
PyObject*  PymvVec4_str        (PymvVec4* self); // string interface
Py_ssize_t PymvVec4_getLength  (PyObject* obj);
PyObject*  PymvVec4_getItem    (PyObject* obj, Py_ssize_t index);
int        PymvVec4_setItem    (PyObject* obj, Py_ssize_t index, PyObject* value);

// math operators
PyObject* PymvVec4_add     (PyObject* left, PyObject* right);
PyObject* PymvVec4_subtract(PyObject* left, PyObject* right);
PyObject* PymvVec4_multiply(PyObject* left, PyObject* right);

static PyNumberMethods PymvVec4_as_number = {
        (binaryfunc)PymvVec4_add,      // nb_add;
        (binaryfunc)PymvVec4_subtract, // nb_subtract;
        (binaryfunc)PymvVec4_multiply, // nb_multiply;
        (binaryfunc)0,                 // nb_remainder;
        (binaryfunc)0,                 // nb_divmod;
        (ternaryfunc)0,                // nb_power;
        (unaryfunc)0,                  // nb_negative;
        (unaryfunc)0,                  // nb_positive;
        (unaryfunc)0,                  // nb_absolute;
        (inquiry)0,                    // nb_bool;
        (unaryfunc)0,                  // nb_invert;
        (binaryfunc)0,                 // nb_lshift;
        (binaryfunc)0,                 // nb_rshift;
        (binaryfunc)0,                 // nb_and;
        (binaryfunc)0,                 // nb_xor;
        (binaryfunc)0,                 // nb_or;
        (unaryfunc)0,                  // nb_int;
        (void*)0,                      // nb_reserved;
        (unaryfunc)0,                  // nb_float;
        (binaryfunc)0,                 // nb_inplace_add;
        (binaryfunc)0,                 // nb_inplace_subtract;
        (binaryfunc)0,                 // nb_inplace_multiply;
        (binaryfunc)0,                 // nb_inplace_remainder;
        (ternaryfunc)0,                // nb_inplace_power;
        (binaryfunc)0,                 // nb_inplace_lshift;
        (binaryfunc)0,                 // nb_inplace_rshift;
        (binaryfunc)0,                 // nb_inplace_and;
        (binaryfunc)0,                 // nb_inplace_xor;
        (binaryfunc)0,                 // nb_inplace_or;
        (binaryfunc)0,                 // nb_floor_divide;
        (binaryfunc)0,                 // nb_true_divide;
        (binaryfunc)0,                 // nb_inplace_floor_divide;
        (binaryfunc)0,                 // nb_inplace_true_divide;
        (unaryfunc)0,                  // nb_index;
        (binaryfunc)0,                 // nb_matrix_multiply;
        (binaryfunc)0                  // nb_inplace_matrix_multiply;
};

static PySequenceMethods PymvVec4_as_sequence = {
        (lenfunc)PymvVec4_getLength,       // length
        (binaryfunc)0,                     // concat
        (ssizeargfunc)0,                   // repeat
        (ssizeargfunc)PymvVec4_getItem,    // item
        (void*)0,                          // slice
        (ssizeobjargproc)PymvVec4_setItem, // assignment
        (void*)0,                          // assign slice
        (objobjproc)0,                     // contains
        (binaryfunc)0,                     // inplace concat
        (ssizeargfunc)0                    // inplace repeat
};

static PyTypeObject PymvVec4Type = {
        PyVarObject_HEAD_INIT(NULL, 0)
        "_dearpygui.mvVec4",           /* tp_name */
        sizeof(PymvVec4),              /* tp_basicsize */
        0,                             /* tp_itemsize */
        (destructor)PymvVec4_dealloc,  /* tp_dealloc */
        0,                             /* tp_vectorcall_offset, tp_print in 3.6, 3.7 */
        0,                             /* tp_getattr */
        0,                             /* tp_setattr */
        0,                             /* tp_as_async */
        (reprfunc)PymvVec4_str,        /* tp_repr */
        &PymvVec4_as_number,           /* tp_as_number */
        &PymvVec4_as_sequence,         /* tp_as_sequence */
        0,                             /* tp_as_mapping */
        0,                             /* tp_hash  */
        0,                             /* tp_call */
        (reprfunc)PymvVec4_str,        /* tp_str */
        0,                             /* tp_getattro */
        0,                             /* tp_setattro */
        0,                             /* tp_as_buffer */
        Py_TPFLAGS_DEFAULT,            /* tp_flags */
        "mvVec4 object",               /* tp_doc */
        0,                             /* tp_traverse */
        0,                             /* tp_clear */
        0,                             /* tp_richcompare */
        0,                             /* tp_weaklistoffset */
        0,                             /* tp_iter */
        0,                             /* tp_iternext */
        0,                             /* tp_methods */
        0,                             /* tp_members */
        0,                             /* tp_getset */
        0,                             /* tp_base */
        0,                             /* tp_dict */
        0,                             /* tp_descr_get */
        0,                             /* tp_descr_set */
        0,                             /* tp_dictoffset */
        (initproc)PymvVec4_init,       /* tp_init */
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

//-----------------------------------------------------------------------------
// mvMat4
//-----------------------------------------------------------------------------

struct PymvMat4
{
    PyObject_HEAD
    mvMat4 m;
};

// methods
int        PymvMat4_init       (PymvMat4* self, PyObject* args, PyObject* kwds); // __init__ method

void       PymvMat4_dealloc    (PymvMat4* self); // called when deallocated
PyObject*  PymvMat4_str        (PymvMat4* self); // string interface
Py_ssize_t PymvMat4_getLength  (PyObject* obj);
PyObject*  PymvMat4_getItem    (PyObject* obj, Py_ssize_t index);
int        PymvMat4_setItem    (PyObject* obj, Py_ssize_t index, PyObject* value);

// math operators
PyObject* PymvMat4_add(PyObject* left, PyObject* right);
PyObject* PymvMat4_subtract(PyObject* left, PyObject* right);
PyObject* PymvMat4_multiply(PyObject* left, PyObject* right);

static PyNumberMethods PymvMat4_as_number = {
        (binaryfunc)PymvMat4_add,      // nb_add;
        (binaryfunc)PymvMat4_subtract, // nb_subtract;
        (binaryfunc)PymvMat4_multiply, // nb_multiply;
        (binaryfunc)0,                 // nb_remainder;
        (binaryfunc)0,                 // nb_divmod;
        (ternaryfunc)0,                // nb_power;
        (unaryfunc)0,                  // nb_negative;
        (unaryfunc)0,                  // nb_positive;
        (unaryfunc)0,                  // nb_absolute;
        (inquiry)0,                    // nb_bool;
        (unaryfunc)0,                  // nb_invert;
        (binaryfunc)0,                 // nb_lshift;
        (binaryfunc)0,                 // nb_rshift;
        (binaryfunc)0,                 // nb_and;
        (binaryfunc)0,                 // nb_xor;
        (binaryfunc)0,                 // nb_or;
        (unaryfunc)0,                  // nb_int;
        (void*)0,                      // nb_reserved;
        (unaryfunc)0,                  // nb_float;
        (binaryfunc)0,                 // nb_inplace_add;
        (binaryfunc)0,                 // nb_inplace_subtract;
        (binaryfunc)0,                 // nb_inplace_multiply;
        (binaryfunc)0,                 // nb_inplace_remainder;
        (ternaryfunc)0,                // nb_inplace_power;
        (binaryfunc)0,                 // nb_inplace_lshift;
        (binaryfunc)0,                 // nb_inplace_rshift;
        (binaryfunc)0,                 // nb_inplace_and;
        (binaryfunc)0,                 // nb_inplace_xor;
        (binaryfunc)0,                 // nb_inplace_or;
        (binaryfunc)0,                 // nb_floor_divide;
        (binaryfunc)0,                 // nb_true_divide;
        (binaryfunc)0,                 // nb_inplace_floor_divide;
        (binaryfunc)0,                 // nb_inplace_true_divide;
        (unaryfunc)0,                  // nb_index;
        (binaryfunc)0,                 // nb_matrix_multiply;
        (binaryfunc)0                  // nb_inplace_matrix_multiply;
};

static PySequenceMethods PymvMat4_as_sequence = {
        (lenfunc)PymvMat4_getLength,       // length
        (binaryfunc)0,                     // concat
        (ssizeargfunc)0,                   // repeat
        (ssizeargfunc)PymvMat4_getItem,    // item
        (void*)0,                          // slice
        (ssizeobjargproc)PymvMat4_setItem, // assignment
        (void*)0,                          // assign slice
        (objobjproc)0,                     // contains
        (binaryfunc)0,                     // inplace concat
        (ssizeargfunc)0                    // inplace repeat
};

static PyTypeObject PymvMat4Type = {
        PyVarObject_HEAD_INIT(NULL, 0)
        "_dearpygui.mvMat4",           /* tp_name */
        sizeof(PymvMat4),              /* tp_basicsize */
        0,                             /* tp_itemsize */
        (destructor)PymvMat4_dealloc,  /* tp_dealloc */
        0,                             /* tp_vectorcall_offset, tp_print in 3.6, 3.7 */
        0,                             /* tp_getattr */
        0,                             /* tp_setattr */
        0,                             /* tp_as_async */
        (reprfunc)PymvMat4_str,        /* tp_repr */
        &PymvMat4_as_number,           /* tp_as_number */
        &PymvMat4_as_sequence,         /* tp_as_sequence */
        0,                             /* tp_as_mapping */
        0,                             /* tp_hash  */
        0,                             /* tp_call */
        (reprfunc)PymvMat4_str,        /* tp_str */
        0,                             /* tp_getattro */
        0,                             /* tp_setattro */
        0,                             /* tp_as_buffer */
        Py_TPFLAGS_DEFAULT,            /* tp_flags */
        "mvMat4 object",               /* tp_doc */
        0,                             /* tp_traverse */
        0,                             /* tp_clear */
        0,                             /* tp_richcompare */
        0,                             /* tp_weaklistoffset */
        0,                             /* tp_iter */
        0,                             /* tp_iternext */
        0,                             /* tp_methods */
        0,                             /* tp_members */
        0,                             /* tp_getset */
        0,                             /* tp_base */
        0,                             /* tp_dict */
        0,                             /* tp_descr_get */
        0,                             /* tp_descr_set */
        0,                             /* tp_dictoffset */
        (initproc)PymvMat4_init,       /* tp_init */
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

