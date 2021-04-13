#pragma once

// forward declare PyObject
// as suggested on the python mailing list
// http://mail.python.org/pipermail/python-dev/2003-August/037601.html
#ifndef PyObject_HEAD
struct _object;
typedef _object PyObject;
#endif

namespace Marvel{

    bool isPyObject_String(PyObject* obj);
    bool isPyObject_Int(PyObject* obj);
    bool isPyObject_Float(PyObject* obj);
    bool isPyObject_Bool(PyObject* obj);
    bool isPyObject_StringList(PyObject* obj);
    bool isPyObject_ListStringList(PyObject* obj);
    bool isPyObject_FloatList(PyObject* obj);
    bool isPyObject_ListFloatList(PyObject* obj);
    bool isPyObject_IntList(PyObject* obj);
    bool isPyObject_ListIntList(PyObject* obj);
    bool isPyObject_Double(PyObject* obj);
    bool isPyObject_Callable(PyObject* obj);
    bool isPyObject_Dict(PyObject* obj);
    bool isPyObject_Any(PyObject* obj);

}