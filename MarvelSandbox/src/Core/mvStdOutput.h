#pragma once
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "mvLogger.h"

static PyObject* aview_write(PyObject* self, PyObject* args)
{
    const char* what;
    if (!PyArg_ParseTuple(args, "s", &what))
        return NULL;
    Marvel::AppLog::getLogger()->AddLog("%0s", what);
    return Py_BuildValue("");
}

static PyMethodDef a_methods[] = {
    {"write", aview_write, METH_VARARGS, "Write something."},
    {NULL, NULL, 0, NULL}
};

PyModuleDef OutModule = {
    PyModuleDef_HEAD_INIT, "sandbox", NULL, -1, a_methods,
    NULL, NULL, NULL, NULL
};

PyObject* PyInit_embout(void)
{
    return PyModule_Create(&OutModule);
}