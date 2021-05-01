#include "mvPythonTranslator.h"
#include "mvPythonExceptions.h"

#define PY_SSIZE_T_CLEAN
#include <Python.h>

namespace Marvel {

	void UpdatePyIntList(PyObject* pyvalue, const std::vector<int>& value)
	{
		if (pyvalue == nullptr)
			return;
		 

		if (!PyList_Check(pyvalue))
		{
			mvThrowPythonError(1000, "Python value error");
			return;
		}
		

		for (Py_ssize_t i = 0; i < PyList_Size(pyvalue); ++i)
		{
			if (static_cast<size_t>(i) == value.size())
				break;
			PyList_SetItem(pyvalue, i, PyLong_FromLong(value[i]));
		}
	}

	void UpdatePyFloatList(PyObject* pyvalue, const std::vector<float>& value)
	{
		if (pyvalue == nullptr)
			return;
		 

		if (!PyList_Check(pyvalue))
		{
			mvThrowPythonError(1000, "Python value error");
			return;
		}


		for (Py_ssize_t i = 0; i < PyList_Size(pyvalue); ++i)
		{
			if (static_cast<size_t>(i) == value.size())
				break;
			PyList_SetItem(pyvalue, i, PyFloat_FromDouble(value[i]));
		}
	}

	void UpdatePyStringStringList(PyObject* pyvalue, const std::vector<std::vector<std::string>>& value)
	{
		if (pyvalue == nullptr)
			return;
		 

		if (!PyList_Check(pyvalue))
		{
			mvThrowPythonError(1000, "Python value error");
			return;
		}


		for (Py_ssize_t i = 0; i < PyList_Size(pyvalue); ++i)
		{
			if (static_cast<size_t>(i) == value.size())
				break;
			PyObject* row = PyList_GetItem(pyvalue, i);
			for (Py_ssize_t j = 0; j < PyList_Size(row); ++j)
			{
				if (static_cast<size_t>(j) == value[i].size())
					break;
				PyList_SetItem(row, i, PyUnicode_FromString(value[i][j].c_str()));
			}
		}
	}

	PyObject* GetPyNone()
	{
		 
		Py_RETURN_NONE;
	}

	PyObject* ToPyString(const std::string& value)
	{
		 
		return PyUnicode_FromString(value.c_str());
	}

	PyObject* ToPyFloat(float value)
	{
		 
		return PyFloat_FromDouble(value);
	}

	PyObject* ToPyDouble(double value)
	{

		return PyFloat_FromDouble(value);
	}

	PyObject* ToPyInt(int value)
	{
		 
		return PyLong_FromLong(value);
	}

	PyObject* ToPyBool(bool value)
	{
		 
		return PyBool_FromLong(value);
	}

	PyObject* ToPyMPair(int x, float y)
	{
		 
		return Py_BuildValue("[if]", x, y);
	}

	PyObject* ToPyMTrip(int i, float x, float y)
	{
		 
		return Py_BuildValue("[iff]", i, x, y);
	}

	PyObject* ToPyPair(float x, float y)
	{
		 
		return Py_BuildValue("[ff]", x, y);
	}

	PyObject* ToPyPairII(int x, int y)
	{
		 
		return Py_BuildValue("[ii]", x, y);
	}

	PyObject* ToPyPair(double x, double y)
	{

		return Py_BuildValue("[dd]", x, y);
	}

	PyObject* ToPyPair(const std::string& x, const std::string& y)
	{
		 
		return Py_BuildValue("[ss]", x.c_str(), y.c_str());
	}

	PyObject* ToPyList(const std::vector<int>& value)
	{
		 
		
		PyObject* result = PyList_New(value.size());

		for (size_t i = 0; i < value.size(); ++i)
			PyList_SetItem(result, i, PyLong_FromLong(value[i]));

		return result;
	}

	PyObject* ToPyList(const std::vector<mvVec2>& value)
	{
		 

		PyObject* result = PyList_New(value.size());

		for (size_t i = 0; i < value.size(); ++i)
		{
			PyObject* item = PyList_New(2);
			PyList_SetItem(item, 0, PyFloat_FromDouble (value[i].x));
			PyList_SetItem(item, 1, PyFloat_FromDouble (value[i].y));
			PyList_SetItem(item, i, item);
		}

		return result;
	}

	PyObject* ToPyList(const std::vector<float>& value)
	{
		 
		PyObject* result = PyList_New(value.size());

		for (size_t i = 0; i < value.size(); ++i)
			PyList_SetItem(result, i, PyFloat_FromDouble(value[i]));

		return result;
	}

	PyObject* ToPyList(const std::vector<double>& value)
	{

		PyObject* result = PyList_New(value.size());

		for (size_t i = 0; i < value.size(); ++i)
			PyList_SetItem(result, i, PyFloat_FromDouble(value[i]));

		return result;
	}

	PyObject* ToPyList(const std::vector<std::vector<float>>& value)
	{
		PyObject* result = PyList_New(value.size());

		for (size_t i = 0; i < value.size(); ++i)
			PyList_SetItem(result, i, ToPyList(value[i]));

		return result;
	}

	PyObject* ToPyList(const std::vector<std::vector<double>>& value)
	{
		PyObject* result = PyList_New(value.size());

		for (size_t i = 0; i < value.size(); ++i)
			PyList_SetItem(result, i, ToPyList(value[i]));

		return result;
	}

	PyObject* ToPyList(const std::vector<std::string>& value)
	{
		 

		PyObject* result = PyList_New(value.size());

		for (size_t i = 0; i < value.size(); ++i)
			PyList_SetItem(result, i, PyUnicode_FromString(value[i].c_str()));

		return result;
	}

	PyObject* ToPyList(const std::vector<std::vector<std::string>>& value)
	{
		 

		PyObject* result = PyList_New(value.size());

		for (size_t i = 0; i < value.size(); ++i)
			PyList_SetItem(result, i, ToPyList(value[i]));

		return result;
	}

	PyObject* ToPyList(const std::vector<std::pair<int, int>>& value)
	{
		PyObject* result = PyList_New(value.size());

		for (size_t i = 0; i < value.size(); ++i)
			PyList_SetItem(result, i, ToPyPairII(value[i].first, value[i].second));

		return result;
	}

	PyObject* ToPyList(const std::vector<std::pair<std::string, std::string>>& value)
	{
		PyObject* result = PyList_New(value.size());

		for (size_t i = 0; i < value.size(); ++i)
			PyList_SetItem(result, i, ToPyPair(value[i].first, value[i].second));

		return result;
	}

	PyObject* ToPyColor(const mvColor& color)
	{
		 

		PyObject* result = PyList_New(4);

		PyList_SetItem(result, 0, ToPyFloat((float)color.r));
		PyList_SetItem(result, 1, ToPyFloat((float)color.g));
		PyList_SetItem(result, 2, ToPyFloat((float)color.b));
		PyList_SetItem(result, 3, ToPyFloat((float)color.a));

		return result;
	}

	PyObject* ToPyTime(const tm& time)
	{
		 
		PyObject* dict = PyDict_New();
		PyDict_SetItemString(dict, "sec",              ToPyInt(time.tm_sec));
		PyDict_SetItemString(dict, "min",              ToPyInt(time.tm_min));
		PyDict_SetItemString(dict, "hour",             ToPyInt(time.tm_hour));
		PyDict_SetItemString(dict, "month_day",        ToPyInt(time.tm_mday));
		PyDict_SetItemString(dict, "month",            ToPyInt(time.tm_mon));
		PyDict_SetItemString(dict, "year",             ToPyInt(time.tm_year));
		PyDict_SetItemString(dict, "week_day",         ToPyInt(time.tm_wday));
		PyDict_SetItemString(dict, "year_day",         ToPyInt(time.tm_yday));
		PyDict_SetItemString(dict, "daylight_savings", ToPyInt(time.tm_isdst));
		return dict;
	}

	PyObject* ToPyIntList(int* value, int count)
	{
		 

		PyObject* result = PyList_New(count);

		for (int i = 0; i < count; ++i)
			PyList_SetItem(result, i, PyLong_FromLong(value[i]));

		return result;
	}

	PyObject* ToPyFloatList(float* value, int count)
	{
		 

		PyObject* result = PyList_New(count);

		for (int i = 0; i < count; ++i)
			PyList_SetItem(result, i, PyFloat_FromDouble(value[i]));

		return result;
	}

	PyObject* ToPyFloatList(double* value, int count)
	{


		PyObject* result = PyList_New(count);

		for (int i = 0; i < count; ++i)
			PyList_SetItem(result, i, PyFloat_FromDouble(value[i]));

		return result;
	}

	tm ToTime(PyObject* value, const std::string& message)
	{
		tm result = {};
		if (value == nullptr)
			return result;

		if (!PyDict_Check(value))
		{
			mvThrowPythonError(1000, message);
			return result;
		}

		if (PyObject* item = PyDict_GetItemString(value, "sec"))              result.tm_sec = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(value, "min"))              result.tm_min = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(value, "hour"))             result.tm_hour = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(value, "month_day"))        result.tm_mday = ToInt(item);
		else result.tm_mday = 1;
		if (PyObject* item = PyDict_GetItemString(value, "month"))            result.tm_mon = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(value, "year"))             result.tm_year = ToInt(item);
		else result.tm_year = 70;
		if (PyObject* item = PyDict_GetItemString(value, "week_day"))         result.tm_wday = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(value, "year_day"))         result.tm_yday = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(value, "daylight_savings")) result.tm_isdst = ToInt(item);

		return result;
	}

	int ToInt(PyObject* value, const std::string& message)
	{
		if (value == nullptr)
			return 0;
		 

		if (!PyLong_Check(value))
		{
			mvThrowPythonError(1000, message);
			return 0;
		}

		return PyLong_AsLong(value);
	}

	float ToFloat(PyObject* value, const std::string& message)
	{
		if (value == nullptr)
			return 0.0f;
		 

		if (!PyNumber_Check(value))
		{
			mvThrowPythonError(1000, message);
			return 0.0f;
		}

		return (float)PyFloat_AsDouble(value);
	}

	double ToDouble(PyObject* value, const std::string& message)
	{
		if (value == nullptr)
			return 0.0;


		if (!PyNumber_Check(value))
		{
			mvThrowPythonError(1000, message);
			return 0.0;
		}

		return PyFloat_AsDouble(value);
	}

	bool ToBool(PyObject* value, const std::string& message)
	{
		if (value == nullptr)
			return false;
		 

		if (!PyBool_Check(value))
		{
			mvThrowPythonError(1000, message);
			return false;
		}

		return PyLong_AsLong(value);
	}

	std::string ToString(PyObject* value, const std::string& message)
	{
		std::string result;
		if (value == nullptr)
			return result;

		if (PyUnicode_Check(value))
		{
			result = _PyUnicode_AsString(value);
		}
		else 
		{
			PyObject* str = PyObject_Str(value);
			if (str == nullptr)
			{
				mvThrowPythonError(1000, message);
				return "";
			}
			result = _PyUnicode_AsString(str);
			Py_XDECREF(str);
		}

		return result;

	}

	std::function<float(Py_buffer&, Py_ssize_t index)> BufferViewFunctionsFloat(Py_buffer& bufferView)
	{
		if (strcmp(bufferView.format, "f") == 0)
			return [](Py_buffer& bufferView, Py_ssize_t index) {return *((float*)bufferView.buf + index); };

		else if (strcmp(bufferView.format, "d") == 0)
			return [](Py_buffer& bufferView, Py_ssize_t index) {return (float)*((double*)bufferView.buf + index); };

		else if (strcmp(bufferView.format, "i") == 0)
			return [](Py_buffer& bufferView, Py_ssize_t index) {return (float)*((int*)bufferView.buf + index); };

		else if (strcmp(bufferView.format, "I") == 0)
			return [](Py_buffer& bufferView, Py_ssize_t index) {return (float)*((unsigned int*)bufferView.buf + index); };

		else if (strcmp(bufferView.format, "l") == 0)
			return [](Py_buffer& bufferView, Py_ssize_t index) {return (float)*((long*)bufferView.buf + index); };

		else if (strcmp(bufferView.format, "L") == 0)
			return [](Py_buffer& bufferView, Py_ssize_t index) {return (float)*((unsigned long*)bufferView.buf + index); };

		else if (strcmp(bufferView.format, "B") == 0)
			return [](Py_buffer& bufferView, Py_ssize_t index) {return (float)*((unsigned char*)bufferView.buf + index); };

		else if (strcmp(bufferView.format, "b") == 0)
			return [](Py_buffer& bufferView, Py_ssize_t index) {return (float)*((signed char*)bufferView.buf + index); };

		else if (strcmp(bufferView.format, "c") == 0)
			return [](Py_buffer& bufferView, Py_ssize_t index) {return (float)*((char*)bufferView.buf + index); };
		else
		{
			mvThrowPythonError(1000, "Unknown buffer type.");
			mvThrowPythonError(1000, bufferView.format);
			mvThrowPythonError(1000, "Currently supported buffer types f, d, l, B");
			return nullptr;
		}
	}

	std::function<int(Py_buffer&, Py_ssize_t index)> BufferViewFunctionsInt(Py_buffer& bufferView)
	{
		if (strcmp(bufferView.format, "f") == 0)
			return [](Py_buffer& bufferView, Py_ssize_t index) {return (int)*((float*)bufferView.buf + index); };

		else if (strcmp(bufferView.format, "d") == 0)
			return [](Py_buffer& bufferView, Py_ssize_t index) {return (int)*((double*)bufferView.buf + index); };

		else if (strcmp(bufferView.format, "i") == 0)
			return [](Py_buffer& bufferView, Py_ssize_t index) {return (int)*((int*)bufferView.buf + index); };

		else if (strcmp(bufferView.format, "I") == 0)
			return [](Py_buffer& bufferView, Py_ssize_t index) {return (int)*((unsigned int*)bufferView.buf + index); };

		else if (strcmp(bufferView.format, "l") == 0)
			return [](Py_buffer& bufferView, Py_ssize_t index) {return (int)*((long*)bufferView.buf + index); };

		else if (strcmp(bufferView.format, "L") == 0)
			return [](Py_buffer& bufferView, Py_ssize_t index) {return (int)*((unsigned long*)bufferView.buf + index); };

		else if (strcmp(bufferView.format, "B") == 0)
			return [](Py_buffer& bufferView, Py_ssize_t index) {return (int)*((unsigned char*)bufferView.buf + index); };

		else if (strcmp(bufferView.format, "b") == 0)
			return [](Py_buffer& bufferView, Py_ssize_t index) {return (int)*((signed char*)bufferView.buf + index); };

		else if (strcmp(bufferView.format, "c") == 0)
			return [](Py_buffer& bufferView, Py_ssize_t index) {return (int)*((char*)bufferView.buf + index); };
		else
		{
			mvThrowPythonError(1000, "Unknown buffer type.");
			mvThrowPythonError(1000, bufferView.format);
			mvThrowPythonError(1000, "Currently supported buffer types f, d, l, B");
			return nullptr;
		}
	}

	std::vector<int> ToIntVect(PyObject* value, const std::string& message)
	{

		std::vector<int> items;
		if (value == nullptr)
			return items;
		 

		if (PyTuple_Check(value))
		{
			items.resize(PyTuple_Size(value));
			for (Py_ssize_t i = 0; i < PyTuple_Size(value); ++i)
			{
				items.emplace_back(PyLong_AsLong(PyTuple_GetItem(value, i)));
			}
		}

		else if (PyList_Check(value))
		{
			items.resize(PyList_Size(value));
			for (Py_ssize_t i = 0; i < PyList_Size(value); ++i)
			{
				items[i] = PyLong_AsLong(PyList_GetItem(value, i));
			}
		}

		else if (PyObject_CheckBuffer(value))
		{
			

			Py_buffer buffer_info;

			if (!PyObject_GetBuffer(value, &buffer_info,
				PyBUF_CONTIG_RO | PyBUF_FORMAT))
			{
				auto BufferViewer = BufferViewFunctionsInt(buffer_info);

				for (Py_ssize_t i = 0; i < buffer_info.len / buffer_info.itemsize; ++i)
				{
					items.emplace_back(BufferViewer(buffer_info, i));
				}
			}

			PyBuffer_Release(&buffer_info);
		}

		else
			mvThrowPythonError(1000, message);


		return items;
	}

	std::vector<float> ToFloatVect(PyObject* value, const std::string& message)
	{

		std::vector<float> items;
		if (value == nullptr)
			return items;
		 

		if (PyTuple_Check(value))
		{
			for (Py_ssize_t i = 0; i < PyTuple_Size(value); ++i)
			{
				items.emplace_back((float)PyFloat_AsDouble(PyTuple_GetItem(value, i)));
			}
		}

		else if (PyList_Check(value))
		{
			for (Py_ssize_t i = 0; i < PyList_Size(value); ++i)
			{
				items.emplace_back((float)PyFloat_AsDouble(PyList_GetItem(value, i)));
			}
		}

		else if (PyObject_CheckBuffer(value))
		{
			Py_buffer buffer_info;

			if (!PyObject_GetBuffer(value, &buffer_info,
				PyBUF_CONTIG_RO | PyBUF_FORMAT)) 
			{

				auto BufferViewer = BufferViewFunctionsFloat(buffer_info);

				for (Py_ssize_t i = 0; i < buffer_info.len / buffer_info.itemsize; ++i)
				{
					items.emplace_back(BufferViewer(buffer_info, i));
				}
			}
			PyBuffer_Release(&buffer_info);
		}

		else
			mvThrowPythonError(1000, message);


		return items;
	}

	std::vector<double> ToDoubleVect(PyObject* value, const std::string& message)
	{

		std::vector<double> items;
		if (value == nullptr)
			return items;


		if (PyTuple_Check(value))
		{
			for (Py_ssize_t i = 0; i < PyTuple_Size(value); ++i)
			{
				items.emplace_back(PyFloat_AsDouble(PyTuple_GetItem(value, i)));
			}
		}

		else if (PyList_Check(value))
		{
			for (Py_ssize_t i = 0; i < PyList_Size(value); ++i)
			{
				items.emplace_back(PyFloat_AsDouble(PyList_GetItem(value, i)));
			}
		}

		else if (PyObject_CheckBuffer(value))
		{
			Py_buffer buffer_info;

			if (!PyObject_GetBuffer(value, &buffer_info,
				PyBUF_CONTIG_RO | PyBUF_FORMAT))
			{

				auto BufferViewer = BufferViewFunctionsFloat(buffer_info);

				for (Py_ssize_t i = 0; i < buffer_info.len / buffer_info.itemsize; ++i)
				{
					items.emplace_back(BufferViewer(buffer_info, i));
				}
			}
			PyBuffer_Release(&buffer_info);
		}

		else
			mvThrowPythonError(1000, message);


		return items;
	}

	std::vector<std::string> ToStringVect(PyObject* value, const std::string& message)
	{

		std::vector<std::string> items;
		if (value == nullptr)
			return items;
		 

		if (PyTuple_Check(value))
		{
			for (Py_ssize_t i = 0; i < PyTuple_Size(value); ++i)
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
			for (Py_ssize_t i = 0; i < PyList_Size(value); ++i)
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
			mvThrowPythonError(1000, message);


		return items;
	}

	mvColor ToColor(PyObject* value, const std::string& message)
	{
		float color[4] = { -1.0f, 0.0f, 0.0f, 1.0f };

		if (value == nullptr)
			return mvColor{ color[0], color[1], color[2], color[3] };

		if (PyTuple_Check(value))
		{
			for (Py_ssize_t i = 0; i < PyTuple_Size(value); ++i)
			{
				if (i >= 4)
					break;
				PyObject* item = PyTuple_GetItem(value, i);
				if(PyNumber_Check(item))
					color[i] = (float)PyFloat_AsDouble(item)/255.0f;
			}

		}
		else if (PyList_Check(value))
		{
			for (Py_ssize_t i = 0; i < PyList_Size(value); ++i)
			{
				if (i >= 4)
					break;
				PyObject* item = PyList_GetItem(value, i);
				if (PyNumber_Check(item))
					color[i] = (float)PyFloat_AsDouble(item)/255.0f;
			}
		}

		return mvColor{ color[0], color[1], color[2], color[3] };
	}

	mvPlotPoint ToPoint(PyObject* value, const std::string& message)
	{
		if (value == nullptr)
			return { 0.0, 0.0 };

		std::vector<double> result = ToDoubleVect(value, message);

		if (result.size() > 1)
			return { result[0], result[1] };
		else if (result.size() == 1)
			return { result[0], 0.0 };
		else
			return { 0.0, 0.0 };
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
		 

		if (PyTuple_Check(value))
		{
			for (Py_ssize_t i = 0; i < PyTuple_Size(value); ++i)
			{
				PyObject* item = PyTuple_GetItem(value, i);
				if (PyTuple_Size(item) == 2)
					items.emplace_back(PyUnicode_AsUTF8(PyTuple_GetItem(item, 0)), PyUnicode_AsUTF8(PyTuple_GetItem(item, 1)));

			}

		}
		else if (PyList_Check(value))
		{
			for (Py_ssize_t i = 0; i < PyList_Size(value); ++i)
			{
				PyObject* item = PyList_GetItem(value, i);
				if (PyList_Size(item) == 2)
					items.emplace_back(PyUnicode_AsUTF8(PyList_GetItem(item, 0)), PyUnicode_AsUTF8(PyList_GetItem(item, 1)));

			}
		}

		else
			mvThrowPythonError(1000, message);

		return items;
	}

	std::vector<mvVec2> ToVectVec2(PyObject* value, const std::string& message)
	{
		std::vector<mvVec2> items;
		if (value == nullptr)
			return items;
		 

		if (PyTuple_Check(value))
		{
			for (Py_ssize_t i = 0; i < PyTuple_Size(value); ++i)
				items.push_back(ToVec2(PyTuple_GetItem(value, i)));
		}
		else if (PyList_Check(value))
		{
			for (Py_ssize_t i = 0; i < PyList_Size(value); ++i)
				items.push_back(ToVec2(PyList_GetItem(value, i)));
		}

		else
			mvThrowPythonError(1000, message);

		return items;
	}

	std::pair<std::vector<float>, std::vector<float>> ToPairVec(PyObject* value, const std::string& message)
	{
		std::pair<std::vector<float>, std::vector<float>> items;
		if (value == nullptr)
			return items;
		 

		if (PyTuple_Check(value))
		{
			if (PyTuple_Size(value) != 2) mvThrowPythonError(1000, message);
			items.first = ToFloatVect(PyTuple_GetItem(value, 0), message);
			items.second = ToFloatVect(PyTuple_GetItem(value, 1), message);
		}
		else if (PyList_Check(value))
		{
			if (PyList_Size(value) != 2) mvThrowPythonError(1000, message);
			items.first = ToFloatVect(PyList_GetItem(value, 0), message);
			items.second = ToFloatVect(PyList_GetItem(value, 1), message);
		}

		else
			mvThrowPythonError(1000, message);

		return items;
	}

	std::vector<mvVec4> ToVectVec4(PyObject* value, const std::string& message)
	{
		std::vector<mvVec4> items;
		if (value == nullptr)
			return items;
		 

		if (PyTuple_Check(value))
		{
			for (Py_ssize_t i = 0; i < PyTuple_Size(value); ++i)
				items.push_back(ToVec4(PyTuple_GetItem(value, i)));
		}
		else if (PyList_Check(value))
		{
			for (Py_ssize_t i = 0; i < PyList_Size(value); ++i)
				items.push_back(ToVec4(PyList_GetItem(value, i)));
		}

		else
			mvThrowPythonError(1000, message);

		return items;
	}

	std::vector<std::pair<int, int>> ToVectInt2(PyObject* value, const std::string& message)
	{
		std::vector<std::pair<int, int>> items;
		if (value == nullptr)
			return items;
		 

		if (PyTuple_Check(value))
		{
			for (Py_ssize_t i = 0; i < PyTuple_Size(value); ++i)
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
			for (Py_ssize_t i = 0; i < PyList_Size(value); ++i)
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
			mvThrowPythonError(1000, message);

		return items;

	}

	std::vector<std::vector<std::string>> ToVectVectString(PyObject* value, const std::string& message)
	{
		std::vector<std::vector<std::string>> items;
		if (value == nullptr)
			return items;
		 

		if (PyTuple_Check(value))
		{
			for (Py_ssize_t i = 0; i < PyTuple_Size(value); ++i)
				items.emplace_back(ToStringVect(PyTuple_GetItem(value, i), message));
		}

		else if (PyList_Check(value))
		{
			for (Py_ssize_t i = 0; i < PyList_Size(value); ++i)
				items.emplace_back(ToStringVect(PyList_GetItem(value, i), message));
		}

		return items;
	}

	std::vector<std::pair<std::string, float>> ToVectPairStringFloat(PyObject* value, const std::string& message)
	{
		std::vector<std::pair<std::string, float>> items;
		if (value == nullptr)
			return items;
		 

		if (PyTuple_Check(value))
		{
			for (Py_ssize_t i = 0; i < PyTuple_Size(value); ++i)
			{
				PyObject* item = PyTuple_GetItem(value, i);
				if (PyTuple_Size(item) == 2 && PyNumber_Check(PyTuple_GetItem(item, 1)))
					items.emplace_back(PyUnicode_AsUTF8(PyTuple_GetItem(item, 0)), (float)PyFloat_AsDouble(PyTuple_GetItem(item, 1)));

			}

		}
		else if (PyList_Check(value))
		{
			for (Py_ssize_t i = 0; i < PyList_Size(value); ++i)
			{
				PyObject* item = PyList_GetItem(value, i);
				if (PyList_Size(item) == 2 && PyNumber_Check(PyList_GetItem(item, 1)))
					items.emplace_back(PyUnicode_AsUTF8(PyList_GetItem(item, 0)), (float)PyFloat_AsDouble(PyList_GetItem(item, 1)));

			}
		}

		else
			mvThrowPythonError(1000, message);

		return items;
	}

	std::vector<std::vector<float>> ToVectVectFloat(PyObject* value, const std::string& message)
	{
		std::vector<std::vector<float>> items;
		if (value == nullptr)
			return items;
		 

		if (PyTuple_Check(value))
		{
			for (Py_ssize_t i = 0; i < PyTuple_Size(value); ++i)
				items.emplace_back(ToFloatVect(PyTuple_GetItem(value, i), message));
		}

		else if (PyList_Check(value))
		{
			for (Py_ssize_t i = 0; i < PyList_Size(value); ++i)
				items.emplace_back(ToFloatVect(PyList_GetItem(value, i), message));
		}

		return items;
	}

	std::vector<std::vector<double>> ToVectVectDouble(PyObject* value, const std::string& message)
	{
		std::vector<std::vector<double>> items;
		if (value == nullptr)
			return items;


		if (PyTuple_Check(value))
		{
			for (Py_ssize_t i = 0; i < PyTuple_Size(value); ++i)
				items.emplace_back(ToDoubleVect(PyTuple_GetItem(value, i), message));
		}

		else if (PyList_Check(value))
		{
			for (Py_ssize_t i = 0; i < PyList_Size(value); ++i)
				items.emplace_back(ToDoubleVect(PyList_GetItem(value, i), message));
		}

		return items;
	}
}
