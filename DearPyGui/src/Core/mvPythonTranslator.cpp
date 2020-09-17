#include "mvPythonTranslator.h"
#include "mvApp.h"
#include "mvAppLog.h"
#include "Core/mvPythonExceptions.h"

namespace Marvel {

	mvGlobalIntepreterLock::mvGlobalIntepreterLock()
	{
		m_gstate = PyGILState_Ensure();
	}

	mvGlobalIntepreterLock::~mvGlobalIntepreterLock()
	{
		PyGILState_Release(m_gstate);
	}

	void UpdatePyIntList(PyObject* pyvalue, const std::vector<int>& value)
	{
		if (pyvalue == nullptr)
			return;
		mvGlobalIntepreterLock gil;

		if (!PyList_Check(pyvalue))
		{
			ThrowPythonException("Python value error");
			return;
		}
		

		for (size_t i = 0; i < PyList_Size(pyvalue); i++)
		{
			if (i == value.size())
				break;
			PyList_SetItem(pyvalue, i, PyLong_FromLong(value[i]));
		}
	}

	void UpdatePyFloatList(PyObject* pyvalue, const std::vector<float>& value)
	{
		if (pyvalue == nullptr)
			return;
		mvGlobalIntepreterLock gil;

		if (!PyList_Check(pyvalue))
		{
			ThrowPythonException("Python value error");
			return;
		}


		for (size_t i = 0; i < PyList_Size(pyvalue); i++)
		{
			if (i == value.size())
				break;
			PyList_SetItem(pyvalue, i, PyFloat_FromDouble(value[i]));
		}
	}

	void UpdatePyStringStringList(PyObject* pyvalue, const std::vector<std::vector<std::string>>& value)
	{
		if (pyvalue == nullptr)
			return;
		mvGlobalIntepreterLock gil;

		if (!PyList_Check(pyvalue))
		{
			ThrowPythonException("Python value error");
			return;
		}


		for (size_t i = 0; i < PyList_Size(pyvalue); i++)
		{
			if (i == value.size())
				break;
			PyObject* row = PyList_GetItem(pyvalue, i);
			for (size_t j = 0; j < PyList_Size(row); j++)
			{
				if (j == value[i].size())
					break;
				PyList_SetItem(row, i, PyUnicode_FromString(value[i][j].c_str()));
			}
		}
	}

	PyObject* GetPyNone()
	{
		mvGlobalIntepreterLock gil;
		Py_RETURN_NONE;
	}

	PyObject* ToPyString(const std::string& value)
	{
		mvGlobalIntepreterLock gil;
		return PyUnicode_FromString(value.c_str());
	}

	PyObject* ToPyFloat(float value)
	{
		mvGlobalIntepreterLock gil;
		return PyFloat_FromDouble(value);
	}

	PyObject* ToPyInt(int value)
	{
		mvGlobalIntepreterLock gil;
		return PyLong_FromLong(value);
	}

	PyObject* ToPyBool(bool value)
	{
		mvGlobalIntepreterLock gil;
		return PyBool_FromLong(value);
	}

	PyObject* ToPyMPair(int x, float y)
	{
		mvGlobalIntepreterLock gil;
		return Py_BuildValue("[if]", x, y);
	}

	PyObject* ToPyPair(float x, float y)
	{
		mvGlobalIntepreterLock gil;
		return Py_BuildValue("[ff]", x, y);
	}

	PyObject* ToPyPairII(int x, int y)
	{
		mvGlobalIntepreterLock gil;
		return Py_BuildValue("[ii]", x, y);
	}

	PyObject* ToPyPair(const std::string& x, const std::string& y)
	{
		mvGlobalIntepreterLock gil;
		return Py_BuildValue("[ss]", x.c_str(), y.c_str());
	}

	PyObject* ToPyList(const std::vector<int>& value)
	{
		mvGlobalIntepreterLock gil;
		
		PyObject* result = PyList_New(value.size());

		for (size_t i = 0; i < value.size(); i++)
			PyList_SetItem(result, i, PyLong_FromLong(value[i]));

		return result;
	}

	PyObject* ToPyList(const std::vector<float>& value)
	{
		mvGlobalIntepreterLock gil;

		PyObject* result = PyList_New(value.size());

		for (size_t i = 0; i < value.size(); i++)
			PyList_SetItem(result, i, PyFloat_FromDouble(value[i]));

		return result;
	}

	PyObject* ToPyList(const std::vector<std::string>& value)
	{
		mvGlobalIntepreterLock gil;

		PyObject* result = PyList_New(value.size());

		for (size_t i = 0; i < value.size(); i++)
			PyList_SetItem(result, i, PyUnicode_FromString(value[i].c_str()));

		return result;
	}

	PyObject* ToPyList(const std::vector<std::vector<std::string>>& value)
	{
		mvGlobalIntepreterLock gil;

		PyObject* result = PyList_New(value.size());

		for (size_t i = 0; i < value.size(); i++)
			PyList_SetItem(result, i, ToPyList(value[i]));

		return result;
	}

	PyObject* ToPyList(const std::vector<std::pair<int, int>>& value)
	{
		mvGlobalIntepreterLock gil;

		PyObject* result = PyList_New(value.size());

		for (size_t i = 0; i < value.size(); i++)
			PyList_SetItem(result, i, ToPyPairII(value[i].first, value[i].second));

		return result;
	}

	int ToInt(PyObject* value, const std::string& message)
	{
		if (value == nullptr)
			return 0;
		mvGlobalIntepreterLock gil;

		if (!PyLong_Check(value))
		{
			ThrowPythonException(message);
			return 0;
		}

		return PyLong_AsLong(value);
	}

	float ToFloat(PyObject* value, const std::string& message)
	{
		if (value == nullptr)
			return 0.0f;
		mvGlobalIntepreterLock gil;

		if (!PyNumber_Check(value))
		{
			ThrowPythonException(message);
			return 0.0f;
		}

		return (float)PyFloat_AsDouble(value);
	}

	bool ToBool(PyObject* value, const std::string& message)
	{
		if (value == nullptr)
			return false;
		mvGlobalIntepreterLock gil;

		if (!PyBool_Check(value))
		{
			ThrowPythonException(message);
			return false;
		}

		return PyLong_AsLong(value);
	}

	std::string ToString(PyObject* value, const std::string& message)
	{
		std::string result;
		if (value == nullptr)
			return result;

		mvGlobalIntepreterLock gil;

		if (PyUnicode_Check(value))
		{
			result = _PyUnicode_AsString(value);
		}
		else 
		{
			PyObject* str = PyObject_Str(value);
			if (str == nullptr)
			{
				ThrowPythonException(message);
				return "";
			}
			result = _PyUnicode_AsString(str);
			Py_XDECREF(str);
		}

		return result;

	}

	std::vector<int> ToIntVect(PyObject* value, const std::string& message)
	{

		std::vector<int> items;
		if (value == nullptr)
			return items;
		mvGlobalIntepreterLock gil;

		if (PyTuple_Check(value))
		{
			for (size_t i = 0; i < PyTuple_Size(value); i++)
			{
				PyObject* item = PyTuple_GetItem(value, i);
				if(PyLong_Check(item))
					items.emplace_back(PyLong_AsLong(item));
			}
		}

		else if (PyList_Check(value))
		{
			for (size_t i = 0; i < PyList_Size(value); i++)
			{
				PyObject* item = PyList_GetItem(value, i);
				if (PyLong_Check(item))
					items.emplace_back(PyLong_AsLong(item));
			}
		}

		else
			ThrowPythonException(message);


		return items;
	}

	std::vector<float> ToFloatVect(PyObject* value, const std::string& message)
	{

		std::vector<float> items;
		if (value == nullptr)
			return items;
		mvGlobalIntepreterLock gil;

		if (PyTuple_Check(value))
		{
			for (size_t i = 0; i < PyTuple_Size(value); i++)
			{
				PyObject* item = PyTuple_GetItem(value, i);
				if (PyNumber_Check(item))
					items.emplace_back(PyFloat_AsDouble(item));
			}
		}

		else if (PyList_Check(value))
		{
			for (size_t i = 0; i < PyList_Size(value); i++)
			{
				PyObject* item = PyList_GetItem(value, i);
				if (PyNumber_Check(item))
					items.emplace_back(PyFloat_AsDouble(item));
			}
		}

		else
			ThrowPythonException(message);


		return items;
	}

	std::vector<std::string> ToStringVect(PyObject* value, const std::string& message)
	{

		std::vector<std::string> items;
		if (value == nullptr)
			return items;
		mvGlobalIntepreterLock gil;

		if (PyTuple_Check(value))
		{
			for (size_t i = 0; i < PyTuple_Size(value); i++)
			{
				PyObject* item = PyTuple_GetItem(value, i);
				if (PyUnicode_Check(item))
					items.emplace_back(_PyUnicode_AsString(item));
				else
				{
					PyObject* str = PyObject_Str(item);
					items.emplace_back(_PyUnicode_AsString(str));
					Py_XDECREF(str);
				}
			}
		}

		else if (PyList_Check(value))
		{
			for (size_t i = 0; i < PyList_Size(value); i++)
			{
				PyObject* item = PyList_GetItem(value, i);
				if (PyUnicode_Check(item))
					items.emplace_back(_PyUnicode_AsString(item));
				else
				{
					PyObject* str = PyObject_Str(item);
					items.emplace_back(_PyUnicode_AsString(str));
					Py_XDECREF(str);
				}
			}
		}

		else
			ThrowPythonException(message);


		return items;
	}

	mvColor ToColor(PyObject* value, const std::string& message)
	{
		int color[4] = { 255, 255, 255, 255 };

		if (value == nullptr)
			return mvColor{ color[0], color[1], color[2], color[3], false };

		mvGlobalIntepreterLock gil;

		if (PyTuple_Check(value))
		{
			for (size_t i = 0; i < PyTuple_Size(value); i++)
			{
				if (i >= 4)
					break;
				PyObject* item = PyTuple_GetItem(value, i);
				if(PyNumber_Check(item))
					color[i] = (int)PyFloat_AsDouble(item);
			}

		}
		else if (PyList_Check(value))
		{
			for (size_t i = 0; i < PyList_Size(value); i++)
			{
				if (i >= 4)
					break;
				PyObject* item = PyList_GetItem(value, i);
				if (PyNumber_Check(item))
					color[i] = (int)PyFloat_AsDouble(item);
			}
		}

		return mvColor{ color[0], color[1], color[2], color[3], true };
	}

	mvVec2 ToVec2(PyObject* value, const std::string& message)
	{
		if (value == nullptr)
			return { 0.0f, 0.0f };

		std::vector<float> result = ToFloatVect(value, message);

		if (result.size() > 1)
			return { result[0], result[1] };
		else if (result.size() == 1)
			return { result[0], 0.0f };
		else
			return { 0.0f, 0.0f };
	}

	mvVec4 ToVec4(PyObject* value, const std::string& message)
	{
		if (value == nullptr)
			return { 0.0f, 0.0f, 0.0f, 0.0f };

		std::vector<float> result = ToFloatVect(value, message);

		if (result.size() > 3)
			return { result[0], result[1], result[2], result[3] };
		else if (result.size() > 2)
			return { result[0], result[1], result[2], 0.0f };
		else if (result.size() > 1)
			return { result[0], result[1], 0.0f, 0.0f };
		else if (result.size() == 1)
			return { result[0], 0.0f, 0.0f, 0.0f };
		else
			return { 0.0f, 0.0f, 0.0f, 0.0f };
	}

	std::vector<std::pair<std::string, std::string>> ToVectPairString(PyObject* value, const std::string& message)
	{
		std::vector<std::pair<std::string, std::string>> items;
		if (value == nullptr)
			return items;
		mvGlobalIntepreterLock gil;

		if (PyTuple_Check(value))
		{
			for (size_t i = 0; i < PyTuple_Size(value); i++)
			{
				PyObject* item = PyTuple_GetItem(value, i);
				if (PyTuple_Size(item) == 2)
					items.emplace_back(PyUnicode_AsUTF8(PyTuple_GetItem(item, 0)), PyUnicode_AsUTF8(PyTuple_GetItem(item, 1)));

			}

		}
		else if (PyList_Check(value))
		{
			for (size_t i = 0; i < PyList_Size(value); i++)
			{
				PyObject* item = PyList_GetItem(value, i);
				if (PyList_Size(item) == 2)
					items.emplace_back(PyUnicode_AsUTF8(PyList_GetItem(item, 0)), PyUnicode_AsUTF8(PyList_GetItem(item, 1)));

			}
		}

		else
			ThrowPythonException(message);

		return items;
	}

	std::vector<mvVec2> ToVectVec2(PyObject* value, const std::string& message)
	{
		std::vector<mvVec2> items;
		if (value == nullptr)
			return items;
		mvGlobalIntepreterLock gil;

		if (PyTuple_Check(value))
		{
			for (size_t i = 0; i < PyTuple_Size(value); i++)
				items.push_back(ToVec2(PyTuple_GetItem(value, i)));
		}
		else if (PyList_Check(value))
		{
			for (size_t i = 0; i < PyList_Size(value); i++)
				items.push_back(ToVec2(PyList_GetItem(value, i)));
		}

		else
			ThrowPythonException(message);

		return items;
	}

	std::vector<mvVec4> ToVectVec4(PyObject* value, const std::string& message)
	{
		std::vector<mvVec4> items;
		if (value == nullptr)
			return items;
		mvGlobalIntepreterLock gil;

		if (PyTuple_Check(value))
		{
			for (size_t i = 0; i < PyTuple_Size(value); i++)
				items.push_back(ToVec4(PyTuple_GetItem(value, i)));
		}
		else if (PyList_Check(value))
		{
			for (size_t i = 0; i < PyList_Size(value); i++)
				items.push_back(ToVec4(PyList_GetItem(value, i)));
		}

		else
			ThrowPythonException(message);

		return items;
	}

	std::vector<std::pair<int, int>> ToVectInt2(PyObject* value, const std::string& message)
	{
		std::vector<std::pair<int, int>> items;
		if (value == nullptr)
			return items;
		mvGlobalIntepreterLock gil;

		if (PyTuple_Check(value))
		{
			for (size_t i = 0; i < PyTuple_Size(value); i++)
			{
				PyObject* point = PyTuple_GetItem(value, i);
				if(PyTuple_Check(point))
				{
                    if (PyTuple_Size(point) >= 2) {
                        int x = PyLong_AsLong(PyTuple_GetItem(point, 0));
                        int y = PyLong_AsLong(PyTuple_GetItem(point, 1));
                        items.emplace_back(x, y);
                    }
                }
                else if(PyList_Check(point))
                {
                    if (PyList_Size(point) >= 2) {
                        int x = PyLong_AsLong(PyList_GetItem(point, 0));
                        int y = PyLong_AsLong(PyList_GetItem(point, 1));
                        items.emplace_back(x, y);
                    }
                }
				else
					items.emplace_back(0, 0);
			}
		}

		else if (PyList_Check(value))
		{
			for (size_t i = 0; i < PyList_Size(value); i++)
			{
				PyObject* point = PyList_GetItem(value, i);
                if(PyTuple_Check(point))
                {
                    if (PyTuple_Size(point) >= 2) {
                        int x = PyLong_AsLong(PyTuple_GetItem(point, 0));
                        int y = PyLong_AsLong(PyTuple_GetItem(point, 1));
                        items.emplace_back(x, y);
                    }
                }
                else if(PyList_Check(point))
                {
                    if (PyList_Size(point) >= 2) {
                        int x = PyLong_AsLong(PyList_GetItem(point, 0));
                        int y = PyLong_AsLong(PyList_GetItem(point, 1));
                        items.emplace_back(x, y);
                    }
                }
				else
					items.emplace_back(0, 0);
			}
		}

		else
			ThrowPythonException(message);

		return items;

	}

	std::vector<std::vector<std::string>> ToVectVectString(PyObject* value, const std::string& message)
	{
		std::vector<std::vector<std::string>> items;
		if (value == nullptr)
			return items;
		mvGlobalIntepreterLock gil;

		if (PyTuple_Check(value))
		{
			for (size_t i = 0; i < PyTuple_Size(value); i++)
				items.emplace_back(ToStringVect(PyTuple_GetItem(value, i), message));
		}

		else if (PyList_Check(value))
		{
			for (size_t i = 0; i < PyList_Size(value); i++)
				items.emplace_back(ToStringVect(PyList_GetItem(value, i), message));
		}

		return items;
	}

	std::vector<std::pair<std::string, float>> ToVectPairStringFloat(PyObject* value, const std::string& message)
	{
		std::vector<std::pair<std::string, float>> items;
		if (value == nullptr)
			return items;
		mvGlobalIntepreterLock gil;

		if (PyTuple_Check(value))
		{
			for (size_t i = 0; i < PyTuple_Size(value); i++)
			{
				PyObject* item = PyTuple_GetItem(value, i);
				if (PyTuple_Size(item) == 2)
					items.emplace_back(PyUnicode_AsUTF8(PyTuple_GetItem(item, 0)), (float)PyFloat_AsDouble(PyTuple_GetItem(item, 1)));

			}

		}
		else if (PyList_Check(value))
		{
			for (size_t i = 0; i < PyList_Size(value); i++)
			{
				PyObject* item = PyList_GetItem(value, i);
				if (PyList_Size(item) == 2)
					items.emplace_back(PyUnicode_AsUTF8(PyList_GetItem(item, 0)), (float)PyFloat_AsDouble(PyList_GetItem(item, 1)));

			}
		}

		else
			ThrowPythonException(message);

		return items;
	}
}