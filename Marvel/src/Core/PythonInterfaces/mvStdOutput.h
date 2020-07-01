#pragma once
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "Core/StandardWindows/mvAppLog.h"
#include "Core/Concurrency/mvThreadPool.h"

namespace Marvel {

    static PyObject* writeToLogger(PyObject* self, PyObject* args)
    {
        PyGILState_STATE gstate = PyGILState_Ensure();
        const char* what;
        if (!PyArg_ParseTuple(args, "s", &what))
        {
            Py_XINCREF(Py_None);
            PyGILState_Release(gstate);
            return Py_None;
        }

        
        Marvel::mvAppLog::getLogger()->AddLog("%0s", what);
        Py_XINCREF(Py_None);
        PyGILState_Release(gstate);
        
        return Py_None;
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