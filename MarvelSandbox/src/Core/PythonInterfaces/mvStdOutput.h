#pragma once
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "Core/mvLogger.h"
#include "Core/mvThreadPool.h"

namespace Marvel {

    static PyObject* writeToLogger(PyObject* self, PyObject* args)
    {
        const char* what;
        if (!PyArg_ParseTuple(args, "s", &what))
            return NULL;

        Marvel::AppLog::getLogger()->AddLog("%0s", what);
        return Py_BuildValue("");
    }

    static PyMethodDef outmethods[] = {
        {"write", writeToLogger, METH_VARARGS, "Write something."},
        {NULL, NULL, 0, NULL}
    };

    PyModuleDef OutModule = {
        PyModuleDef_HEAD_INIT, "sandboxout", NULL, -1, outmethods,
        NULL, NULL, NULL, NULL
    };

    PyObject* PyInit_embOut(void)
    {
        return PyModule_Create(&OutModule);
    }
}