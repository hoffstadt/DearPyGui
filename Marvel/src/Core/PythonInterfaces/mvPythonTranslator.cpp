#include "mvPythonTranslator.h"
#include "Core/mvApp.h"
#include "Core/StandardWindows/mvAppLog.h"

namespace Marvel {

	std::vector<std::string> mvPythonTranslator::getStringVec(PyObject* obj)
	{
		std::vector<std::string> items;

		if (PyTuple_Check(obj))
		{
			for (int i = 0; i < PyTuple_Size(obj); i++)
				items.emplace_back(PyUnicode_AsUTF8(PyTuple_GetItem(obj, i)));
		}
		else if (PyList_Check(obj))
		{
			for (int i = 0; i < PyList_Size(obj); i++)
				items.emplace_back(PyUnicode_AsUTF8(PyList_GetItem(obj, i)));
		}

		return items;
	}

	std::vector<std::pair<std::string, std::string>> mvPythonTranslator::getStringPairVec(PyObject* obj)
	{
		std::vector<std::pair<std::string, std::string>> items;

		if (PyTuple_Check(obj))
		{
			for (int i = 0; i < PyTuple_Size(obj); i++)
			{
				PyObject* item = PyTuple_GetItem(obj, i);
				if (PyTuple_Size(item) == 2)
					items.emplace_back(PyUnicode_AsUTF8(PyTuple_GetItem(item, 0)), PyUnicode_AsUTF8(PyTuple_GetItem(item, 1)));

			}

		}
		else if (PyList_Check(obj))
		{
			for (int i = 0; i < PyList_Size(obj); i++)
			{
				PyObject* item = PyList_GetItem(obj, i);
				if (PyList_Size(item) == 2)
					items.emplace_back(PyUnicode_AsUTF8(PyList_GetItem(item, 0)), PyUnicode_AsUTF8(PyList_GetItem(item, 1)));

			}
		}

		return items;
	}

	std::vector<float> mvPythonTranslator::getFloatVec(PyObject* obj)
	{
		std::vector<float> items;

		if (PyTuple_Check(obj))
		{
			for (int i = 0; i < PyTuple_Size(obj); i++)
				items.emplace_back(PyFloat_AsDouble(PyTuple_GetItem(obj, i)));
		}

		else if (PyList_Check(obj))
		{
			for (int i = 0; i < PyList_Size(obj); i++)
				items.emplace_back(PyFloat_AsDouble(PyList_GetItem(obj, i)));
		}

		return items;
	}

	std::vector<int> mvPythonTranslator::getIntVec(PyObject* obj)
	{
		std::vector<int> items;

		if (PyTuple_Check(obj))
		{
			for (int i = 0; i < PyTuple_Size(obj); i++)
				items.emplace_back(PyLong_AsLong(PyTuple_GetItem(obj, i)));
		}

		else if (PyList_Check(obj))
		{
			for (int i = 0; i < PyList_Size(obj); i++)
				items.emplace_back(PyLong_AsLong(PyList_GetItem(obj, i)));
		}

		return items;
	}

	mvVec2 mvPythonTranslator::getVec2(PyObject* obj)
	{
		std::vector<float> items;

		float x, y;

		if (PyTuple_Check(obj))
		{
			x = PyFloat_AsDouble(PyTuple_GetItem(obj, 0));
			y = PyFloat_AsDouble(PyTuple_GetItem(obj, 1));
		}
		else if (PyList_Check(obj))
		{
			x = PyFloat_AsDouble(PyList_GetItem(obj, 0));
			y = PyFloat_AsDouble(PyList_GetItem(obj, 1));
		}

		return mvVec2{ x, y };
	}

	mvColor mvPythonTranslator::getColor(PyObject* obj)
	{

		int color[4] = { 255, 255, 255, 255 };

		if (obj == nullptr)
			return mvColor{ color[0], color[1], color[2], color[3], false };

		if (PyTuple_Check(obj))
		{
			for (int i = 0; i < PyTuple_Size(obj); i++)
				color[i] = (int)PyFloat_AsDouble(PyTuple_GetItem(obj, i));
		}
		else if (PyList_Check(obj))
		{
			for (int i = 0; i < PyList_Size(obj); i++)
				color[i] = (int)PyFloat_AsDouble(PyList_GetItem(obj, i));
		}

		return mvColor{ color[0], color[1], color[2], color[3], true };
	}

	std::vector<mvVec2> mvPythonTranslator::getVectVec2(PyObject* obj)
	{
		std::vector<mvVec2> items;



		if (PyTuple_Check(obj))
		{
			for (int i = 0; i < PyTuple_Size(obj); i++)
			{
				PyObject* point = PyTuple_GetItem(obj, i);
				for (int j = 0; j < PyTuple_Size(point); j++)
				{
					float x = PyFloat_AsDouble(PyTuple_GetItem(point, 0));
					float y = PyFloat_AsDouble(PyTuple_GetItem(point, 1));
					items.push_back({ x, y });
				}
			}
		}
		else if (PyList_Check(obj))
		{
			for (int i = 0; i < PyList_Size(obj); i++)
			{
				PyObject* point = PyList_GetItem(obj, i);
				for (int j = 0; j < PyList_Size(point); j++)
				{
					float x = PyFloat_AsDouble(PyList_GetItem(point, 0));
					float y = PyFloat_AsDouble(PyList_GetItem(point, 1));
					items.push_back({ x, y });
				}
			}
		}

		return items;
	}

	std::vector<std::pair<int, int>> mvPythonTranslator::getVectInt2(PyObject* obj)
	{
		std::vector<std::pair<int, int>> items;

		for (int i = 0; i < PyTuple_Size(obj); i++)
		{
			PyObject* point = PyTuple_GetItem(obj, i);
			for (int j = 0; j < PyTuple_Size(point); j++)
			{
				int x = PyLong_AsLong(PyTuple_GetItem(point, 0));
				int y = PyLong_AsLong(PyTuple_GetItem(point, 1));
				items.emplace_back(x, y);
			}
		}

		return items;

	}
}