#pragma once

#include "mvCustomTypes.h"

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
