#include "mvPythonTypeChecker.h"
#define PY_SSIZE_T_CLEAN
#include <Python.h>

namespace Marvel {

	bool isPyObject_Any(PyObject* obj)
	{
		return obj != nullptr;
	}

	bool isPyObject_String(PyObject* obj)
	{
		if (obj == nullptr)
			return false;

		if (PyUnicode_Check(obj))
			return true;
		else
		{
			PyObject* str = PyObject_Str(obj);
			if (str == nullptr)
				return false;
			Py_XDECREF(str);
		}

		return true;
	}

	bool isPyObject_Int(PyObject* obj)
	{
		if (obj == nullptr)
			return false;

		if (!PyNumber_Check(obj))
			return false;
		return true;
	}

	bool isPyObject_Float(PyObject* obj)
	{
		if (obj == nullptr)
			return false;

		if (!PyNumber_Check(obj))
			return false;
		return true;
	}

	bool isPyObject_Bool(PyObject* obj)
	{
		if (obj == nullptr)
			return false;

		if (!PyBool_Check(obj))
			return false;
		return true;
	}

	bool isPyObject_StringList(PyObject* obj)
	{
		if (obj == nullptr)
			return false;

		if (PyTuple_Check(obj))
		{
			if (PyTuple_Size(obj) > 1)
			{
				PyObject* item = PyTuple_GetItem(obj, 0);
				if (PyUnicode_Check(item))
					return true;
				else
					return isPyObject_String(item);
			}

			return true;
		}
		else if (PyList_Check(obj))
		{
			if (PyList_Size(obj) > 1)
			{
				PyObject* item = PyList_GetItem(obj, 0);
				if (PyUnicode_Check(item))
					return true;
				else
					return isPyObject_String(item);
			}

			return true;
		}

		return false;
	}

	bool isPyObject_ListStringList(PyObject* obj)
	{
		if (obj == nullptr)
			return false;

		if (PyTuple_Check(obj))
		{
			if (PyTuple_Size(obj) > 1)
			{
				PyObject* item = PyTuple_GetItem(obj, 0);
				return isPyObject_StringList(item);
			}

			return true;
		}
		else if (PyList_Check(obj))
		{
			if (PyList_Size(obj) > 1)
			{
				PyObject* item = PyList_GetItem(obj, 0);
				return isPyObject_StringList(item);
			}

			return true;
		}

		return false;
	}

	bool isPyObject_FloatList(PyObject* obj)
	{
		if (obj == nullptr)
			return false;

		if (PyTuple_Check(obj))
		{
			if (PyTuple_Size(obj) > 1)
			{
				PyObject* item = PyTuple_GetItem(obj, 0);
				return isPyObject_Float(item);
			}

			return true;
		}
		else if (PyList_Check(obj))
		{
			if (PyList_Size(obj) > 1)
			{
				PyObject* item = PyList_GetItem(obj, 0);
				return isPyObject_Float(item);
			}

			return true;
		}

		return false;
	}

	bool isPyObject_ListFloatList(PyObject* obj)
	{
		if (obj == nullptr)
			return false;

		if (PyTuple_Check(obj))
		{
			if (PyTuple_Size(obj) > 1)
			{
				PyObject* item = PyTuple_GetItem(obj, 0);
				return isPyObject_FloatList(item);
			}

			return true;
		}
		else if (PyList_Check(obj))
		{
			if (PyList_Size(obj) > 1)
			{
				PyObject* item = PyList_GetItem(obj, 0);
				return isPyObject_FloatList(item);
			}

			return true;
		}

		return false;
	}

	bool isPyObject_IntList(PyObject* obj)
	{
		if (obj == nullptr)
			return false;

		if (PyTuple_Check(obj))
		{
			if (PyTuple_Size(obj) > 1)
			{
				PyObject* item = PyTuple_GetItem(obj, 0);
				return isPyObject_Int(item);
			}

			return true;
		}
		else if (PyList_Check(obj))
		{
			if (PyList_Size(obj) > 1)
			{
				PyObject* item = PyList_GetItem(obj, 0);
				return isPyObject_Int(item);
			}

			return true;
		}

		return false;
	}

	bool isPyObject_ListIntList(PyObject* obj)
	{
		if (obj == nullptr)
			return false;

		if (PyTuple_Check(obj))
		{
			if (PyTuple_Size(obj) > 1)
			{
				PyObject* item = PyTuple_GetItem(obj, 0);
				return isPyObject_IntList(item);
			}

			return true;
		}
		else if (PyList_Check(obj))
		{
			if (PyList_Size(obj) > 1)
			{
				PyObject* item = PyList_GetItem(obj, 0);
				return isPyObject_IntList(item);
			}

			return true;
		}

		return false;
	}

	bool isPyObject_Double(PyObject* obj)
	{
		if (obj == nullptr)
			return false;

		if (PyFloat_Check(obj))
			return true;

		return false;
	}

	bool isPyObject_Callable(PyObject* obj)
	{
		if (obj == nullptr)
			return false;

		if (PyCallable_Check(obj))
			return true;
		return false;
	}

	bool isPyObject_Dict(PyObject* obj)
	{
		if (obj == nullptr)
			return false;

		if (PyDict_Check(obj))
			return true;
		return false;
	}

}