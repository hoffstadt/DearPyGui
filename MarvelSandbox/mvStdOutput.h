#pragma once
//-----------------------------------------------------------------------------
// mvStdOutput
//
//     - This file defines a python module to intercept console output and
//       reroute it to the marvel logger
//     
//-----------------------------------------------------------------------------

//#include "Core/StandardWindows/mvAppLog.h"
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <string>

namespace Marvel {

    static PyObject* writeToLogger(PyObject* self, PyObject* args)
    {
        PyGILState_STATE gstate = PyGILState_Ensure();
        const char* what;
        if (!PyArg_ParseTuple(args, "s", &what))
        {
            PyGILState_Release(gstate);
            Py_RETURN_NONE;
        }

        //Marvel::mvAppLog::getLogger()->AddLog("%0s", what);
        PyGILState_Release(gstate);
        Py_RETURN_NONE;
    }

    PyObject* flush(PyObject* self, PyObject* args)
    {
        // no-op
        return Py_BuildValue("");
    }

    static PyMethodDef outmethods[] = {
        {"write", writeToLogger, METH_VARARGS, "Write something."},
        {"flush", flush, METH_VARARGS, "Flush."},
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