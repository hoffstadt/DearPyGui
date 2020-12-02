#include <core/NodeAppItems/mvNodeEditor.h>
#include "mvPythonTranslator.h"
#include "mvApp.h"
#include "mvAppLog.h"
#include "mvPythonExceptions.h"

namespace Marvel {

mvGlobalIntepreterLock::mvGlobalIntepreterLock() {
  m_gstate = PyGILState_Ensure();
}

mvGlobalIntepreterLock::~mvGlobalIntepreterLock() {
  PyGILState_Release(m_gstate);
}

void UpdatePyIntList(PyObject* pyvalue, const std::vector<int>& value) {
  if (pyvalue == nullptr)
	return;
  mvGlobalIntepreterLock gil;

  if (!PyList_Check(pyvalue)) {
	ThrowPythonException("Python value error");
	return;
  }

  for (Py_ssize_t i = 0; i < PyList_Size(pyvalue); i++) {
	if (static_cast<size_t>(i) == value.size())
	  break;
	PyList_SetItem(pyvalue, i, PyLong_FromLong(value[i]));
  }
}

void UpdatePyFloatList(PyObject* pyvalue, const std::vector<float>& value) {
  if (pyvalue == nullptr)
	return;
  mvGlobalIntepreterLock gil;

  if (!PyList_Check(pyvalue)) {
	ThrowPythonException("Python value error");
	return;
  }

  for (Py_ssize_t i = 0; i < PyList_Size(pyvalue); i++) {
	if (static_cast<size_t>(i) == value.size())
	  break;
	PyList_SetItem(pyvalue, i, PyFloat_FromDouble(value[i]));
  }
}

void UpdatePyStringStringList(PyObject* pyvalue, const std::vector<std::vector<std::string>>& value) {
  if (pyvalue == nullptr)
	return;
  mvGlobalIntepreterLock gil;

  if (!PyList_Check(pyvalue)) {
	ThrowPythonException("Python value error");
	return;
  }

  for (Py_ssize_t i = 0; i < PyList_Size(pyvalue); i++) {
	if (static_cast<size_t>(i) == value.size())
	  break;
	PyObject* row = PyList_GetItem(pyvalue, i);
	for (Py_ssize_t j = 0; j < PyList_Size(row); j++) {
	  if (static_cast<size_t>(j) == value[i].size())
		break;
	  PyList_SetItem(row, i, PyUnicode_FromString(value[i][j].c_str()));
	}
  }
}

PyObject* GetPyNone() {
  mvGlobalIntepreterLock gil;
  Py_RETURN_NONE;
}

PyObject* ToPyString(const std::string& value) {
  mvGlobalIntepreterLock gil;
  return PyUnicode_FromString(value.c_str());
}

PyObject* ToPyFloat(float value) {
  mvGlobalIntepreterLock gil;
  return PyFloat_FromDouble(value);
}

PyObject* ToPyInt(int value) {
  mvGlobalIntepreterLock gil;
  return PyLong_FromLong(value);
}

PyObject* ToPyBool(bool value) {
  mvGlobalIntepreterLock gil;
  return PyBool_FromLong(value);
}

PyObject* ToPyMPair(int x, float y) {
  mvGlobalIntepreterLock gil;
  return Py_BuildValue("[if]", x, y);
}

PyObject* ToPyMTrip(int i, float x, float y) {
  mvGlobalIntepreterLock gil;
  return Py_BuildValue("[iff]", i, x, y);
}

PyObject* ToPyPair(float x, float y) {
  mvGlobalIntepreterLock gil;
  return Py_BuildValue("[ff]", x, y);
}

PyObject *ToPyPairII(int x, int y) {
  mvGlobalIntepreterLock gil;
  return Py_BuildValue("[ii]", x, y);
}

PyObject *ToPyPairIII(int x, int y, int z) {
  mvGlobalIntepreterLock gil;
  return Py_BuildValue("[iii]", x, y, z);
}

PyObject *ToPyPair(const std::string &x, const std::string &y) {
  mvGlobalIntepreterLock gil;
  return Py_BuildValue("[ss]", x.c_str(), y.c_str());
}

PyObject *ToPyList(const std::vector<int> &value) {
  mvGlobalIntepreterLock gil;

  PyObject* result = PyList_New(value.size());

  for (size_t i = 0; i < value.size(); i++)
	PyList_SetItem(result, i, PyLong_FromLong(value[i]));

  return result;
}

PyObject* ToPyList(const std::vector<mvVec2>& value) {
  mvGlobalIntepreterLock gil;

  PyObject* result = PyList_New(value.size());

  for (size_t i = 0; i < value.size(); i++) {
	PyObject* item = PyList_New(2);
	PyList_SetItem(item, 0, PyFloat_FromDouble (value[i].x));
	PyList_SetItem(item, 1, PyFloat_FromDouble (value[i].y));
	PyList_SetItem(item, i, item);
  }

  return result;
}

PyObject* ToPyList(const std::vector<float>& value) {
  mvGlobalIntepreterLock gil;

  PyObject* result = PyList_New(value.size());

  for (size_t i = 0; i < value.size(); i++)
	PyList_SetItem(result, i, PyFloat_FromDouble(value[i]));

  return result;
}

PyObject* ToPyList(const std::vector<std::string>& value) {
  mvGlobalIntepreterLock gil;

  PyObject* result = PyList_New(value.size());

  for (size_t i = 0; i < value.size(); i++)
	PyList_SetItem(result, i, PyUnicode_FromString(value[i].c_str()));

  return result;
}

PyObject* ToPyList(const std::vector<std::vector<std::string>>& value) {
  mvGlobalIntepreterLock gil;

  PyObject* result = PyList_New(value.size());

  for (size_t i = 0; i < value.size(); i++)
	PyList_SetItem(result, i, ToPyList(value[i]));

  return result;
}

PyObject* ToPyList(const std::vector<std::pair<int, int>>& value) {
  mvGlobalIntepreterLock gil;

  PyObject *result = PyList_New(value.size());

  for (size_t i = 0; i < value.size(); i++)
	PyList_SetItem(result, i, ToPyPairII(value[i].first, value[i].second));

  return result;
}

PyObject *ToPyListLinks(const std::vector<mvLink> &value) {
  mvGlobalIntepreterLock gil;

  PyObject *result = PyList_New(value.size());
  for (size_t i = 0; i < value.size(); i++)
	PyList_SetItem(result, i, ToPyPairIII(value[i].id, value[i].start_attr, value[i].end_attr));
  return result;
}

PyObject *ToPyColor(const mvColor &color) {
  mvGlobalIntepreterLock gil;

  PyObject *result = PyList_New(4);

  PyList_SetItem(result, 0, ToPyFloat((float)color.r));
  PyList_SetItem(result, 1, ToPyFloat((float)color.g));
  PyList_SetItem(result, 2, ToPyFloat((float)color.b));
  PyList_SetItem(result, 3, ToPyFloat((float)color.a));

  return result;
}

PyObject* ToPyTime(const tm& time) {
  mvGlobalIntepreterLock gil;
  PyObject* dict = PyDict_New();
  PyDict_SetItemString(dict, "sec", ToPyInt(time.tm_sec));
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

PyObject* ToPyIntList(int* value, int count) {
  mvGlobalIntepreterLock gil;

  PyObject* result = PyList_New(count);

  for (int i = 0; i < count; i++)
	PyList_SetItem(result, i, PyLong_FromLong(value[i]));

  return result;
}

PyObject* ToPyFloatList(float* value, int count) {
  mvGlobalIntepreterLock gil;

  PyObject* result = PyList_New(count);

  for (int i = 0; i < count; i++)
	PyList_SetItem(result, i, PyFloat_FromDouble(value[i]));

  return result;
}

tm ToTime(PyObject* value, const std::string& message) {
  tm result = {};
  if (value == nullptr)
	return result;

  if (!PyDict_Check(value)) {
	ThrowPythonException(message);
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

int ToInt(PyObject* value, const std::string& message) {
  if (value == nullptr)
	return 0;
  mvGlobalIntepreterLock gil;

  if (!PyLong_Check(value)) {
	ThrowPythonException(message);
	return 0;
  }

  return PyLong_AsLong(value);
}

float ToFloat(PyObject* value, const std::string& message) {
  if (value == nullptr)
	return 0.0f;
  mvGlobalIntepreterLock gil;

  if (!PyNumber_Check(value)) {
	ThrowPythonException(message);
	return 0.0f;
  }

  return (float)PyFloat_AsDouble(value);
}

bool ToBool(PyObject* value, const std::string& message) {
  if (value == nullptr)
	return false;
  mvGlobalIntepreterLock gil;

  if (!PyBool_Check(value)) {
	ThrowPythonException(message);
	return false;
  }

  return PyLong_AsLong(value);
}

std::string ToString(PyObject* value, const std::string& message) {
  std::string result;
  if (value == nullptr)
	return result;

  mvGlobalIntepreterLock gil;

  if (PyUnicode_Check(value)) {
	result = _PyUnicode_AsString(value);
  } else {
	PyObject* str = PyObject_Str(value);
	if (str == nullptr) {
	  ThrowPythonException(message);
	  return "";
	}
	result = _PyUnicode_AsString(str);
	Py_XDECREF(str);
  }

  return result;

}

std::vector<int> ToIntVect(PyObject* value, const std::string& message) {

  std::vector<int> items;
  if (value == nullptr)
	return items;
  mvGlobalIntepreterLock gil;

  if (PyTuple_Check(value)) {
	for (Py_ssize_t i = 0; i < PyTuple_Size(value); i++) {
	  PyObject* item = PyTuple_GetItem(value, i);
	  if(PyLong_Check(item))
		items.emplace_back(PyLong_AsLong(item));
	}
  } else if (PyList_Check(value)) {
	for (Py_ssize_t i = 0; i < PyList_Size(value); i++) {
	  PyObject* item = PyList_GetItem(value, i);
	  if (PyLong_Check(item))
		items.emplace_back(PyLong_AsLong(item));
	}
  } else
	ThrowPythonException(message);

  return items;
}

std::vector<float> ToFloatVect(PyObject* value, const std::string& message) {

  std::vector<float> items;
  if (value == nullptr)
	return items;
  mvGlobalIntepreterLock gil;

  if (PyTuple_Check(value)) {
	for (Py_ssize_t i = 0; i < PyTuple_Size(value); i++) {
	  PyObject* item = PyTuple_GetItem(value, i);
	  if (PyNumber_Check(item))
		items.emplace_back(PyFloat_AsDouble(item));
	}
  } else if (PyList_Check(value)) {
	for (Py_ssize_t i = 0; i < PyList_Size(value); i++) {
	  PyObject* item = PyList_GetItem(value, i);
	  if (PyNumber_Check(item))
		items.emplace_back(PyFloat_AsDouble(item));
	}
  } else
	ThrowPythonException(message);

  return items;
}

std::vector<std::string> ToStringVect(PyObject* value, const std::string& message) {

  std::vector<std::string> items;
  if (value == nullptr)
	return items;
  mvGlobalIntepreterLock gil;

  if (PyTuple_Check(value)) {
	for (Py_ssize_t i = 0; i < PyTuple_Size(value); i++) {
	  PyObject* item = PyTuple_GetItem(value, i);
	  if (PyUnicode_Check(item))
		items.emplace_back(_PyUnicode_AsString(item));
	  else {
		PyObject* str = PyObject_Str(item);
		items.emplace_back(_PyUnicode_AsString(str));
		Py_XDECREF(str);
	  }
	}
  } else if (PyList_Check(value)) {
	for (Py_ssize_t i = 0; i < PyList_Size(value); i++) {
	  PyObject* item = PyList_GetItem(value, i);
	  if (PyUnicode_Check(item))
		items.emplace_back(_PyUnicode_AsString(item));
	  else {
		PyObject* str = PyObject_Str(item);
		items.emplace_back(_PyUnicode_AsString(str));
		Py_XDECREF(str);
	  }
	}
  } else
	ThrowPythonException(message);

  return items;
}

mvColor ToColor(PyObject* value, const std::string& message) {
  int color[4] = { 255, 255, 255, 255 };

  if (value == nullptr)
	return mvColor{ color[0], color[1], color[2], color[3], false };

  mvGlobalIntepreterLock gil;

  if (PyTuple_Check(value)) {
	for (Py_ssize_t i = 0; i < PyTuple_Size(value); i++) {
	  if (i >= 4)
		break;
	  PyObject* item = PyTuple_GetItem(value, i);
	  if(PyNumber_Check(item))
		color[i] = (int)PyFloat_AsDouble(item);
	}

  } else if (PyList_Check(value)) {
	for (Py_ssize_t i = 0; i < PyList_Size(value); i++) {
	  if (i >= 4)
		break;
	  PyObject* item = PyList_GetItem(value, i);
	  if (PyNumber_Check(item))
		color[i] = (int)PyFloat_AsDouble(item);
	}
  }

  return mvColor{ color[0], color[1], color[2], color[3], true };
}

mvVec2 ToVec2(PyObject* value, const std::string& message) {
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

mvVec4 ToVec4(PyObject* value, const std::string& message) {
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

std::vector<std::pair<std::string, std::string>> ToVectPairString(PyObject* value, const std::string& message) {
  std::vector<std::pair<std::string, std::string>> items;
  if (value == nullptr)
	return items;
  mvGlobalIntepreterLock gil;

  if (PyTuple_Check(value)) {
	for (Py_ssize_t i = 0; i < PyTuple_Size(value); i++) {
	  PyObject* item = PyTuple_GetItem(value, i);
	  if (PyTuple_Size(item) == 2)
		items.emplace_back(PyUnicode_AsUTF8(PyTuple_GetItem(item, 0)), PyUnicode_AsUTF8(PyTuple_GetItem(item, 1)));

	}

  } else if (PyList_Check(value)) {
	for (Py_ssize_t i = 0; i < PyList_Size(value); i++) {
	  PyObject* item = PyList_GetItem(value, i);
	  if (PyList_Size(item) == 2)
		items.emplace_back(PyUnicode_AsUTF8(PyList_GetItem(item, 0)), PyUnicode_AsUTF8(PyList_GetItem(item, 1)));

	}
  } else
	ThrowPythonException(message);

  return items;
}

std::vector<mvVec2> ToVectVec2(PyObject* value, const std::string& message) {
  std::vector<mvVec2> items;
  if (value == nullptr)
	return items;
  mvGlobalIntepreterLock gil;

  if (PyTuple_Check(value)) {
	for (Py_ssize_t i = 0; i < PyTuple_Size(value); i++)
	  items.push_back(ToVec2(PyTuple_GetItem(value, i)));
  } else if (PyList_Check(value)) {
	for (Py_ssize_t i = 0; i < PyList_Size(value); i++)
	  items.push_back(ToVec2(PyList_GetItem(value, i)));
  } else
	ThrowPythonException(message);

  return items;
}

std::pair<std::vector<float>, std::vector<float>> ToPairVec(PyObject* value, const std::string& message) {
  std::pair<std::vector<float>, std::vector<float>> items;
  if (value == nullptr)
	return items;
  mvGlobalIntepreterLock gil;

  if (PyTuple_Check(value)) {
	if (PyTuple_Size(value) != 2) ThrowPythonException(message);
	items.first = ToFloatVect(PyTuple_GetItem(value, 0), message);
	items.second = ToFloatVect(PyTuple_GetItem(value, 1), message);
  } else if (PyList_Check(value)) {
	if (PyList_Size(value) != 2) ThrowPythonException(message);
	items.first = ToFloatVect(PyList_GetItem(value, 0), message);
	items.second = ToFloatVect(PyList_GetItem(value, 1), message);
  } else
	ThrowPythonException(message);

  return items;
}

std::vector<mvVec4> ToVectVec4(PyObject* value, const std::string& message) {
  std::vector<mvVec4> items;
  if (value == nullptr)
	return items;
  mvGlobalIntepreterLock gil;

  if (PyTuple_Check(value)) {
	for (Py_ssize_t i = 0; i < PyTuple_Size(value); i++)
	  items.push_back(ToVec4(PyTuple_GetItem(value, i)));
  } else if (PyList_Check(value)) {
	for (Py_ssize_t i = 0; i < PyList_Size(value); i++)
	  items.push_back(ToVec4(PyList_GetItem(value, i)));
  } else
	ThrowPythonException(message);

  return items;
}

std::vector<std::pair<int, int>> ToVectInt2(PyObject* value, const std::string& message) {
  std::vector<std::pair<int, int>> items;
  if (value == nullptr)
	return items;
  mvGlobalIntepreterLock gil;

  if (PyTuple_Check(value)) {
	for (Py_ssize_t i = 0; i < PyTuple_Size(value); i++) {
	  PyObject* point = PyTuple_GetItem(value, i);
	  if(PyTuple_Check(point)) {
		if (PyTuple_Size(point) >= 2) {
		  int x = PyLong_AsLong(PyTuple_GetItem(point, 0));
		  int y = PyLong_AsLong(PyTuple_GetItem(point, 1));
		  items.emplace_back(x, y);
		}
	  } else if(PyList_Check(point)) {
		if (PyList_Size(point) >= 2) {
		  int x = PyLong_AsLong(PyList_GetItem(point, 0));
		  int y = PyLong_AsLong(PyList_GetItem(point, 1));
		  items.emplace_back(x, y);
		}
	  } else
		items.emplace_back(0, 0);
	}
  } else if (PyList_Check(value)) {
	for (Py_ssize_t i = 0; i < PyList_Size(value); i++) {
	  PyObject* point = PyList_GetItem(value, i);
	  if(PyTuple_Check(point)) {
		if (PyTuple_Size(point) >= 2) {
		  int x = PyLong_AsLong(PyTuple_GetItem(point, 0));
		  int y = PyLong_AsLong(PyTuple_GetItem(point, 1));
		  items.emplace_back(x, y);
		}
	  } else if(PyList_Check(point)) {
		if (PyList_Size(point) >= 2) {
		  int x = PyLong_AsLong(PyList_GetItem(point, 0));
		  int y = PyLong_AsLong(PyList_GetItem(point, 1));
		  items.emplace_back(x, y);
		}
	  } else
		items.emplace_back(0, 0);
	}
  } else
	ThrowPythonException(message);

  return items;

}

std::vector<std::vector<std::string>> ToVectVectString(PyObject* value, const std::string& message) {
  std::vector<std::vector<std::string>> items;
  if (value == nullptr)
	return items;
  mvGlobalIntepreterLock gil;

  if (PyTuple_Check(value)) {
	for (Py_ssize_t i = 0; i < PyTuple_Size(value); i++)
	  items.emplace_back(ToStringVect(PyTuple_GetItem(value, i), message));
  } else if (PyList_Check(value)) {
	for (Py_ssize_t i = 0; i < PyList_Size(value); i++)
	  items.emplace_back(ToStringVect(PyList_GetItem(value, i), message));
  }

  return items;
}

std::vector<std::pair<std::string, float>> ToVectPairStringFloat(PyObject* value, const std::string& message) {
  std::vector<std::pair<std::string, float>> items;
  if (value == nullptr)
	return items;
  mvGlobalIntepreterLock gil;

  if (PyTuple_Check(value)) {
	for (Py_ssize_t i = 0; i < PyTuple_Size(value); i++) {
	  PyObject* item = PyTuple_GetItem(value, i);
	  if (PyTuple_Size(item) == 2 && PyNumber_Check(PyTuple_GetItem(item, 1)))
		items.emplace_back(PyUnicode_AsUTF8(PyTuple_GetItem(item, 0)), (float)PyFloat_AsDouble(PyTuple_GetItem(item, 1)));

	}

  } else if (PyList_Check(value)) {
	for (Py_ssize_t i = 0; i < PyList_Size(value); i++) {
	  PyObject* item = PyList_GetItem(value, i);
	  if (PyList_Size(item) == 2 && PyNumber_Check(PyList_GetItem(item, 1)))
		items.emplace_back(PyUnicode_AsUTF8(PyList_GetItem(item, 0)), (float)PyFloat_AsDouble(PyList_GetItem(item, 1)));

	}
  } else
	ThrowPythonException(message);

  return items;
}

std::vector<std::vector<float>> ToVectVectFloat(PyObject* value, const std::string& message) {
  std::vector<std::vector<float>> items;
  if (value == nullptr)
	return items;
  mvGlobalIntepreterLock gil;

  if (PyTuple_Check(value)) {
	for (Py_ssize_t i = 0; i < PyTuple_Size(value); i++)
	  items.emplace_back(ToFloatVect(PyTuple_GetItem(value, i), message));
  } else if (PyList_Check(value)) {
	for (Py_ssize_t i = 0; i < PyList_Size(value); i++)
	  items.emplace_back(ToFloatVect(PyList_GetItem(value, i), message));
  }

  return items;
}
}
