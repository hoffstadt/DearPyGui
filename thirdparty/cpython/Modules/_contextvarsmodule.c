#include "Python.h"

#include "clinic/_contextvarsmodule.c.h"

/*[clinic input]
module _contextvars
[clinic start generated code]*/
/*[clinic end generated code: output=da39a3ee5e6b4b0d input=a0955718c8b8cea6]*/


/*[clinic input]
_contextvars.copy_context
[clinic start generated code]*/

static PyObject *
_contextvars_copy_context_impl(PyObject *module)
/*[clinic end generated code: output=1fcd5da7225c4fa9 input=89bb9ae485888440]*/
{
    return PyContext_CopyCurrent();
}


PyDoc_STRVAR(module_doc, "Context Variables");

static PyMethodDef _contextvars_methods[] = {
    _CONTEXTVARS_COPY_CONTEXT_METHODDEF
    {NULL, NULL}
};

static int
_contextvars_exec(PyObject *m)
{
    Py_INCREF(&PyContext_Type);
    if (PyModule_AddObject(m, "Context",
                           (PyObject *)&PyContext_Type) < 0)
    {
        Py_DECREF(&PyContext_Type);
        return -1;
    }

    Py_INCREF(&PyContextVar_Type);
    if (PyModule_AddObject(m, "ContextVar",
                           (PyObject *)&PyContextVar_Type) < 0)
    {
        Py_DECREF(&PyContextVar_Type);
        return -1;
    }

    Py_INCREF(&PyContextToken_Type);
    if (PyModule_AddObject(m, "Token",
                           (PyObject *)&PyContextToken_Type) < 0)
    {
        Py_DECREF(&PyContextToken_Type);
        return -1;
    }

    return 0;
}

static struct PyModuleDef_Slot _contextvars_slots[] = {
    {Py_mod_exec, _contextvars_exec},
    {0, NULL}
};

static struct PyModuleDef _contextvarsmodule = {
    PyModuleDef_HEAD_INIT,      /* m_base */
    "_contextvars",             /* m_name */
    module_doc,                 /* m_doc */
    0,                          /* m_size */
    _contextvars_methods,       /* m_methods */
    _contextvars_slots,         /* m_slots */
    NULL,                       /* m_traverse */
    NULL,                       /* m_clear */
    NULL,                       /* m_free */
};

PyMODINIT_FUNC
PyInit__contextvars(void)
{
    return PyModuleDef_Init(&_contextvarsmodule);
}
