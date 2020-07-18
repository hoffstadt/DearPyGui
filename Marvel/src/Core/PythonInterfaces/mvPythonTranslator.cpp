#include "mvPythonTranslator.h"
#include "Core/mvApp.h"
#include "Core/StandardWindows/mvAppLog.h"

namespace Marvel {

	mvGlobalIntepreterLock::mvGlobalIntepreterLock()
	{
		m_gstate = PyGILState_Ensure();
	}

	mvGlobalIntepreterLock::~mvGlobalIntepreterLock()
	{
		PyGILState_Release(m_gstate);
	}

	PyObject* mvPythonTranslator::GetPyNone()
	{
		mvGlobalIntepreterLock gil;
		Py_RETURN_NONE;
	}

	PyObject* mvPythonTranslator::ToPyString(const std::string& value)
	{
		mvGlobalIntepreterLock gil;
		return PyUnicode_FromString(value.c_str());
	}

	PyObject* mvPythonTranslator::ToPyFloat(float value)
	{
		mvGlobalIntepreterLock gil;
		return PyFloat_FromDouble(value);
	}

	PyObject* mvPythonTranslator::ToPyInt(int value)
	{
		mvGlobalIntepreterLock gil;
		return PyLong_FromLong(value);
	}

	PyObject* mvPythonTranslator::ToPyBool(bool value)
	{
		mvGlobalIntepreterLock gil;
		return PyBool_FromLong(value);
	}

	PyObject* mvPythonTranslator::ToPyMPair(int x, float y)
	{
		mvGlobalIntepreterLock gil;
		return Py_BuildValue("[if]", x, y);
	}

	PyObject* mvPythonTranslator::ToPyPair(float x, float y)
	{
		mvGlobalIntepreterLock gil;
		return Py_BuildValue("[ff]", x, y);
	}

	PyObject* mvPythonTranslator::ToPyPair(const std::string& x, const std::string& y)
	{
		mvGlobalIntepreterLock gil;
		return Py_BuildValue("[ss]", x, y);
	}

	PyObject* mvPythonTranslator::ToPyList(const std::vector<int>& value)
	{
		mvGlobalIntepreterLock gil;
		
		PyObject* result = PyList_New(value.size());

		for (int i = 0; i < value.size(); i++)
			PyList_SetItem(result, i, PyLong_FromLong(value[i]));

		return result;
	}

	PyObject* mvPythonTranslator::ToPyList(const std::vector<float>& value)
	{
		mvGlobalIntepreterLock gil;

		PyObject* result = PyList_New(value.size());

		for (int i = 0; i < value.size(); i++)
			PyList_SetItem(result, i, PyFloat_FromDouble(value[i]));

		return result;
	}

	int mvPythonTranslator::ToInt(PyObject* value, const std::string& message)
	{
		mvGlobalIntepreterLock gil;

		if (!PyLong_Check(value))
		{
			mvAppLog::getLogger()->LogError(message);
			return 0;
		}

		return PyLong_AsLong(value);
	}

	float mvPythonTranslator::ToFloat(PyObject* value, const std::string& message)
	{
		mvGlobalIntepreterLock gil;

		if (!PyNumber_Check(value))
		{
			mvAppLog::getLogger()->LogError(message);
			return 0.0f;
		}

		return PyFloat_AsDouble(value);
	}

	bool mvPythonTranslator::ToBool(PyObject* value, const std::string& message)
	{
		mvGlobalIntepreterLock gil;

		if (!PyBool_Check(value))
		{
			mvAppLog::getLogger()->LogError(message);
			return false;
		}

		return PyLong_AsLong(value);
	}

	std::string mvPythonTranslator::ToString(PyObject* value, const std::string& message)
	{
		mvGlobalIntepreterLock gil;

		if (!PyUnicode_Check(value))
		{
			mvAppLog::getLogger()->LogError(message);
			return "";
		}

		return _PyUnicode_AsString(value);

	}

	std::vector<int> mvPythonTranslator::ToIntVect(PyObject* value, const std::string& message)
	{

		std::vector<int> items;
		mvGlobalIntepreterLock gil;

		if (PyTuple_Check(value))
		{
			for (int i = 0; i < PyTuple_Size(value); i++)
			{
				PyObject* item = PyTuple_GetItem(value, i);
				if(PyLong_Check(item))
					items.emplace_back(PyLong_AsLong(item));
			}
		}

		else if (PyList_Check(value))
		{
			for (int i = 0; i < PyList_Size(value); i++)
			{
				PyObject* item = PyList_GetItem(value, i);
				if (PyLong_Check(item))
					items.emplace_back(PyLong_AsLong(item));
			}
		}

		else
			mvAppLog::getLogger()->LogError(message);


		return items;
	}

	std::vector<float> mvPythonTranslator::ToFloatVect(PyObject* value, const std::string& message)
	{

		std::vector<float> items;
		mvGlobalIntepreterLock gil;

		if (PyTuple_Check(value))
		{
			for (int i = 0; i < PyTuple_Size(value); i++)
			{
				PyObject* item = PyTuple_GetItem(value, i);
				if (PyNumber_Check(item))
					items.emplace_back(PyFloat_AsDouble(item));
			}
		}

		else if (PyList_Check(value))
		{
			for (int i = 0; i < PyList_Size(value); i++)
			{
				PyObject* item = PyList_GetItem(value, i);
				if (PyNumber_Check(item))
					items.emplace_back(PyFloat_AsDouble(item));
			}
		}

		else
			mvAppLog::getLogger()->LogError(message);


		return items;
	}

	std::vector<std::string> mvPythonTranslator::ToStringVect(PyObject* value, const std::string& message)
	{

		std::vector<std::string> items;
		mvGlobalIntepreterLock gil;

		if (PyTuple_Check(value))
		{
			for (int i = 0; i < PyTuple_Size(value); i++)
			{
				PyObject* item = PyTuple_GetItem(value, i);
				if (PyUnicode_Check(item))
					items.emplace_back(_PyUnicode_AsString(item));
			}
		}

		else if (PyList_Check(value))
		{
			for (int i = 0; i < PyList_Size(value); i++)
			{
				PyObject* item = PyList_GetItem(value, i);
				if (PyUnicode_Check(item))
					items.emplace_back(_PyUnicode_AsString(item));
			}
		}

		else
			mvAppLog::getLogger()->LogError(message);


		return items;
	}

	mvColor mvPythonTranslator::ToColor(PyObject* value, const std::string& message)
	{
		int color[4] = { 255, 255, 255, 255 };

		if (value == nullptr)
			return mvColor{ color[0], color[1], color[2], color[3], false };

		mvGlobalIntepreterLock gil;

		if (PyTuple_Check(value))
		{
			for (int i = 0; i < PyTuple_Size(value); i++)
			{
				PyObject* item = PyTuple_GetItem(value, i);
				if(PyNumber_Check(item))
					color[i] = (int)PyFloat_AsDouble(item);
			}

		}
		else if (PyList_Check(value))
		{
			for (int i = 0; i < PyList_Size(value); i++)
			{
				PyObject* item = PyList_GetItem(value, i);
				if (PyNumber_Check(item))
					color[i] = (int)PyFloat_AsDouble(item);
			}
		}

		return mvColor{ color[0], color[1], color[2], color[3], true };
	}

	mvVec2 mvPythonTranslator::ToVec2(PyObject* value, const std::string& message)
	{
		std::vector<float> result = ToFloatVect(value, message);

		if (result.size() > 1)
			return { result[0], result[1] };
		else if (result.size() == 1)
			return { result[0], 0.0f };
		else
			return { 0.0f, 0.0f };
	}

	std::vector<std::pair<std::string, std::string>> mvPythonTranslator::ToVectPairString(PyObject* value, const std::string& message)
	{
		std::vector<std::pair<std::string, std::string>> items;
		mvGlobalIntepreterLock gil;

		if (PyTuple_Check(value))
		{
			for (int i = 0; i < PyTuple_Size(value); i++)
			{
				PyObject* item = PyTuple_GetItem(value, i);
				if (PyTuple_Size(item) == 2)
					items.emplace_back(PyUnicode_AsUTF8(PyTuple_GetItem(item, 0)), PyUnicode_AsUTF8(PyTuple_GetItem(item, 1)));

			}

		}
		else if (PyList_Check(value))
		{
			for (int i = 0; i < PyList_Size(value); i++)
			{
				PyObject* item = PyList_GetItem(value, i);
				if (PyList_Size(item) == 2)
					items.emplace_back(PyUnicode_AsUTF8(PyList_GetItem(item, 0)), PyUnicode_AsUTF8(PyList_GetItem(item, 1)));

			}
		}

		else
			mvAppLog::getLogger()->LogError(message);

		return items;
	}

	std::vector<mvVec2> mvPythonTranslator::ToVectVec2(PyObject* value, const std::string& message)
	{
		std::vector<mvVec2> items;
		mvGlobalIntepreterLock gil;

		if (PyTuple_Check(value))
		{
			for (int i = 0; i < PyTuple_Size(value); i++)
				items.push_back(ToVec2(PyTuple_GetItem(value, i)));
		}
		else if (PyList_Check(value))
		{
			for (int i = 0; i < PyList_Size(value); i++)
				items.push_back(ToVec2(PyList_GetItem(value, i)));
		}

		else
			mvAppLog::getLogger()->LogError(message);

		return items;
	}

	std::vector<std::pair<int, int>> mvPythonTranslator::ToVectInt2(PyObject* value, const std::string& message)
	{
		std::vector<std::pair<int, int>> items;
		mvGlobalIntepreterLock gil;

		if (PyTuple_Check(value))
		{
			for (int i = 0; i < PyTuple_Size(value); i++)
			{
				PyObject* point = PyTuple_GetItem(value, i);
				for (int j = 0; j < PyTuple_Size(point); j++)
				{
					int x = PyLong_AsLong(PyTuple_GetItem(point, 0));
					int y = PyLong_AsLong(PyTuple_GetItem(point, 1));
					items.emplace_back(x, y);
				}
			}
		}

		else
			mvAppLog::getLogger()->LogError(message);

		return items;

	}
}