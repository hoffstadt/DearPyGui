#include "mvPyUtils.h"
#include <utility>

#include <string>
#include "mvAppItem.h"
#include "mvAppItemCommons.h"
#include "mvContext.h"
#include "mvItemRegistry.h"
#include "dearpygui.h"
#include <ctime>
#include <frameobject.h>

mvGlobalIntepreterLock::mvGlobalIntepreterLock()
{
    _gstate = (int)PyGILState_Ensure();
}

mvGlobalIntepreterLock::~mvGlobalIntepreterLock()
{
    PyGILState_Release((PyGILState_STATE)_gstate);
}


mvPyObject::mvPyObject(PyObject* rawObject, bool borrowed) 
	:
	m_rawObject(rawObject),
	m_borrowed(borrowed),
	m_ok(rawObject != nullptr)
{

}

mvPyObject::mvPyObject(mvPyObject&& other)
	:
	m_rawObject(nullptr),
	m_borrowed(false),
	m_ok(false)
{
	std::swap(m_rawObject, other.m_rawObject);
	std::swap(m_borrowed, other.m_borrowed);
	std::swap(m_ok, other.m_ok);
}

mvPyObject& mvPyObject::operator=(mvPyObject&& other)
{
	if (this != &other)
	{
		if (m_rawObject != nullptr && !m_borrowed)
			Py_XDECREF(m_rawObject);

		std::swap(other.m_rawObject, m_rawObject);
		std::swap(other.m_borrowed, m_borrowed);
		std::swap(other.m_ok, m_ok);
	}

	return *this;
}

mvPyObject::~mvPyObject()
{
	if(!m_borrowed && !m_del)
		Py_XDECREF(m_rawObject);
}

mvPyObject::operator PyObject*()
{
	return m_rawObject;
}

void mvPyObject::addRef()
{
	Py_XINCREF(m_rawObject);
}

void mvPyObject::delRef()
{
	Py_XDECREF(m_rawObject);
	m_del = true;
}

void
mvThrowPythonError(mvErrorCode code, const std::string& message)
{
    std::string fullMessage = "Error: [%d] Message: \t" + message;
    PyErr_Format(PyExc_Exception, fullMessage.c_str(), (int)code);
}

void
mvThrowPythonError(mvErrorCode code, const std::string& command, const std::string& message, mvAppItem* item)
{
    if (item)
    {
        std::string fullMessage = "\nError:     [%d]\nCommand:   %s\nItem:      %d \nLabel:     %s\nItem Type: %s\nMessage:   %s";
        PyErr_Format(PyExc_Exception,
                     fullMessage.c_str(),
                     (int)code,
                     command.c_str(),
                     item->uuid,
                     item->config.specifiedLabel.c_str(),
                     DearPyGui::GetEntityTypeString(item->type),
                     message.c_str());
    }
    else
    {
        std::string fullMessage = "\nError:     [%d]\nCommand:   %s\nItem:      %d \nLabel:     %s\nItem Type: %s\nMessage:   %s";
        PyErr_Format(PyExc_Exception,
                     fullMessage.c_str(),
                     (int)code,
                     command.c_str(),
                     0,
                     "Not found",
                     "Unknown",
                     message.c_str());
    }
}

bool
isPyObject_Any(PyObject* obj)
{
    return obj != nullptr;
}

bool
isPyObject_String(PyObject* obj)
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

bool
isPyObject_Int(PyObject* obj)
{
    if (obj == nullptr)
        return false;

    if (!PyNumber_Check(obj))
        return false;
    return true;
}

bool
isPyObject_Float(PyObject* obj)
{
    if (obj == nullptr)
        return false;

    if (!PyNumber_Check(obj))
        return false;
    return true;
}

bool
isPyObject_Bool(PyObject* obj)
{
    if (obj == nullptr)
        return false;

    if (!PyBool_Check(obj))
        return false;
    return true;
}

bool
isPyObject_StringList(PyObject* obj)
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

bool
isPyObject_ListStringList(PyObject* obj)
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

bool
isPyObject_FloatList(PyObject* obj)
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

    else if (PyObject_CheckBuffer(obj))
    {
        return true;
    }

    return false;
}

bool
isPyObject_ListFloatList(PyObject* obj)
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

bool
isPyObject_IntList(PyObject* obj)
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

    else if (PyObject_CheckBuffer(obj))
    {
        return true;
    }

    return false;
}

bool
isPyObject_ListIntList(PyObject* obj)
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

bool
isPyObject_Double(PyObject* obj)
{
    if (obj == nullptr)
        return false;

    if (PyFloat_Check(obj))
        return true;

    return false;
}

bool
isPyObject_Callable(PyObject* obj)
{
    if (obj == nullptr)
        return false;

    if (PyCallable_Check(obj))
        return true;
    return false;
}

bool
isPyObject_Dict(PyObject* obj)
{
    if (obj == nullptr)
        return false;

    if (PyDict_Check(obj))
        return true;
    return false;
}


void
UpdatePyIntList(PyObject* pyvalue, const std::vector<int>& value)
{
    if (pyvalue == nullptr)
        return;


    if (!PyList_Check(pyvalue))
    {
        mvThrowPythonError(mvErrorCode::mvWrongType, "Python value error");
        return;
    }


    for (Py_ssize_t i = 0; i < PyList_Size(pyvalue); ++i)
    {
        if (static_cast<size_t>(i) == value.size())
            break;
        PyList_SetItem(pyvalue, i, PyLong_FromLong(value[i]));
    }
}

void
UpdatePyFloatList(PyObject* pyvalue, const std::vector<float>& value)
{
    if (pyvalue == nullptr)
        return;


    if (!PyList_Check(pyvalue))
    {
        mvThrowPythonError(mvErrorCode::mvWrongType, "Python value error");
        return;
    }


    for (Py_ssize_t i = 0; i < PyList_Size(pyvalue); ++i)
    {
        if (static_cast<size_t>(i) == value.size())
            break;
        PyList_SetItem(pyvalue, i, PyFloat_FromDouble(value[i]));
    }
}

void
UpdatePyStringStringList(PyObject* pyvalue, const std::vector<std::vector<std::string>>& value)
{
    if (pyvalue == nullptr)
        return;


    if (!PyList_Check(pyvalue))
    {
        mvThrowPythonError(mvErrorCode::mvWrongType, "Python value error");
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

PyObject*
GetPyNone()
{
    Py_RETURN_NONE;
}

PyObject*
ToPyString(const std::string& value)
{

    return PyUnicode_FromString(value.c_str());
}

PyObject*
ToPyFloat(float value)
{

    return PyFloat_FromDouble(value);
}

PyObject*
ToPyDouble(double value)
{

    return PyFloat_FromDouble(value);
}

PyObject*
ToPyInt(int value)
{

    return PyLong_FromLong(value);
}

PyObject*
ToPyUUID(mvAppItem* item)
{
    if (!item->config.alias.empty())
        return Py_BuildValue("K", item->uuid);

    return Py_BuildValue("K", item->uuid);
}

PyObject*
ToPyUUID(mvUUID value)
{
    mvAppItem* item = GetItem(*GContext->itemRegistry, value);
    if (item)
    {
        if (!item->config.alias.empty())
            return Py_BuildValue("K", item->uuid);
    }
    return Py_BuildValue("K", value);
}

PyObject*
ToPyLong(long value)
{

    return Py_BuildValue("K", value);
}

PyObject*
ToPyBool(bool value)
{

    return PyBool_FromLong(value);
}

PyObject*
ToPyMPair(int x, float y)
{

    return Py_BuildValue("[if]", x, y);
}

PyObject*
ToPyMTrip(int i, float x, float y)
{

    return Py_BuildValue("[iff]", i, x, y);
}

PyObject*
ToPyPair(float x, float y)
{

    return Py_BuildValue("[ff]", x, y);
}

PyObject*
ToPyPairII(int x, int y)
{

    return Py_BuildValue("[ii]", x, y);
}

PyObject*
ToPyPair(double x, double y)
{

    return Py_BuildValue("[dd]", x, y);
}

PyObject*
ToPyPair(const std::string& x, const std::string& y)
{

    return Py_BuildValue("[ss]", x.c_str(), y.c_str());
}

PyObject*
ToPyList(const std::vector<int>& value)
{


    PyObject* result = PyList_New(value.size());

    for (size_t i = 0; i < value.size(); ++i)
        PyList_SetItem(result, i, PyLong_FromLong(value[i]));

    return result;
}

PyObject*
ToPyList(const std::vector<mvUUID>& value)
{


    PyObject* result = PyList_New(value.size());

    for (size_t i = 0; i < value.size(); ++i)
        PyList_SetItem(result, i, Py_BuildValue("K", value[i]));

    return result;
}

PyObject*
ToPyList(const std::vector<mvVec2>& value)
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

PyObject*
ToPyList(const std::vector<mvVec4>& value)
{


    PyObject* result = PyList_New(value.size());

    for (size_t i = 0; i < value.size(); ++i)
    {
        PyObject* item = PyList_New(4);
        PyList_SetItem(item, 0, PyFloat_FromDouble(value[i].x));
        PyList_SetItem(item, 1, PyFloat_FromDouble(value[i].y));
        PyList_SetItem(item, 2, PyFloat_FromDouble(value[i].z));
        PyList_SetItem(item, 3, PyFloat_FromDouble(value[i].w));
        PyList_SetItem(item, i, item);
    }

    return result;
}

PyObject*
ToPyList(const std::vector<float>& value)
{

    PyObject* result = PyList_New(value.size());

    for (size_t i = 0; i < value.size(); ++i)
        PyList_SetItem(result, i, PyFloat_FromDouble(value[i]));

    return result;
}

PyObject*
ToPyList(const std::vector<double>& value)
{

    PyObject* result = PyList_New(value.size());

    if(result)
    {
        for (size_t i = 0; i < value.size(); ++i)
            PyList_SetItem(result, i, PyFloat_FromDouble(value[i]));
    }
    return result;
}

PyObject*
ToPyList(const std::vector<std::vector<float>>& value)
{
    PyObject* result = PyList_New(value.size());

    if(result)
    {
        for (size_t i = 0; i < value.size(); ++i)
            PyList_SetItem(result, i, ToPyList(value[i]));
    }

    return result;
}

PyObject*
ToPyList(const std::vector<std::vector<double>>& value)
{
    PyObject* result = PyList_New(value.size());

    if(result)
    {
        for (size_t i = 0; i < value.size(); ++i)
            PyList_SetItem(result, i, ToPyList(value[i]));
    }

    return result;
}

PyObject*
ToPyList(const std::vector<std::string>& value)
{


    PyObject* result = PyList_New(value.size());

    for (size_t i = 0; i < value.size(); ++i)
        PyList_SetItem(result, i, PyUnicode_FromString(value[i].c_str()));

    return result;
}

PyObject*
ToPyList(const std::vector<std::vector<std::string>>& value)
{


    PyObject* result = PyList_New(value.size());

    for (size_t i = 0; i < value.size(); ++i)
        PyList_SetItem(result, i, ToPyList(value[i]));

    return result;
}

PyObject*
ToPyList(const std::vector<std::pair<int, int>>& value)
{
    PyObject* result = PyList_New(value.size());

    for (size_t i = 0; i < value.size(); ++i)
        PyList_SetItem(result, i, ToPyPairII(value[i].first, value[i].second));

    return result;
}

PyObject*
ToPyList(const std::vector<std::pair<std::string, std::string>>& value)
{
    PyObject* result = PyList_New(value.size());

    for (size_t i = 0; i < value.size(); ++i)
        PyList_SetItem(result, i, ToPyPair(value[i].first, value[i].second));

    return result;
}

PyObject*
ToPyColor(const mvColor& color)
{


    PyObject* result = PyList_New(4);

    PyList_SetItem(result, 0, ToPyFloat((float)color.r));
    PyList_SetItem(result, 1, ToPyFloat((float)color.g));
    PyList_SetItem(result, 2, ToPyFloat((float)color.b));
    PyList_SetItem(result, 3, ToPyFloat((float)color.a));

    return result;
}

PyObject*
ToPyTime(const tm& time)
{

    PyObject* dict = PyDict_New();
    PyDict_SetItemString(dict, "sec",              mvPyObject(ToPyInt(time.tm_sec)));
    PyDict_SetItemString(dict, "min",              mvPyObject(ToPyInt(time.tm_min)));
    PyDict_SetItemString(dict, "hour",             mvPyObject(ToPyInt(time.tm_hour)));
    PyDict_SetItemString(dict, "month_day",        mvPyObject(ToPyInt(time.tm_mday)));
    PyDict_SetItemString(dict, "month",            mvPyObject(ToPyInt(time.tm_mon)));
    PyDict_SetItemString(dict, "year",             mvPyObject(ToPyInt(time.tm_year)));
    PyDict_SetItemString(dict, "week_day",         mvPyObject(ToPyInt(time.tm_wday)));
    PyDict_SetItemString(dict, "year_day",         mvPyObject(ToPyInt(time.tm_yday)));
    PyDict_SetItemString(dict, "daylight_savings", mvPyObject(ToPyInt(time.tm_isdst)));
    return dict;
}

PyObject*
ToPyIntList(const int* value, int count)
{


    PyObject* result = PyList_New(count);

    for (int i = 0; i < count; ++i)
        PyList_SetItem(result, i, PyLong_FromLong(value[i]));

    return result;
}

PyObject*
ToPyFloatList(const float* value, int count)
{


    PyObject* result = PyList_New(count);

    for (int i = 0; i < count; ++i)
        PyList_SetItem(result, i, PyFloat_FromDouble(value[i]));

    return result;
}

PyObject*
ToPyFloatList(const double* value, int count)
{


    PyObject* result = PyList_New(count);

    for (int i = 0; i < count; ++i)
        PyList_SetItem(result, i, PyFloat_FromDouble(value[i]));

    return result;
}

tm
ToTime(PyObject* value, const std::string& message)
{
    tm result = {};
    if (value == nullptr)
        return result;

    if (!PyDict_Check(value))
    {
        mvThrowPythonError(mvErrorCode::mvWrongType, "Python value error. Must be dict/time.");
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

int
ToInt(PyObject* value, const std::string& message)
{
    if (value == nullptr)
        return 0;


    if (PyLong_Check(value))
        return PyLong_AsLong(value);

    else if (PyFloat_Check(value))
        return (int)PyFloat_AsDouble(value);

    mvThrowPythonError(mvErrorCode::mvWrongType, "Python value error. Must be int.");
    return 0;
}

mvUUID
ToUUID(PyObject* value, const std::string& message)
{
    if (value == nullptr)
        return 0;

    if (PyUnicode_Check(value))
    {
        std::string result = _PyUnicode_AsString(value);
        mvUUID idfound = GetIdFromAlias(*GContext->itemRegistry, result);
        if (idfound == 0)
        {
            mvThrowPythonError(mvErrorCode::mvWrongType, "UUID not found.");
            return 0;
        }
        return idfound;
    }

    if (!PyLong_Check(value))
    {
        mvThrowPythonError(mvErrorCode::mvWrongType, "Python value error. Must be int.");
        return 0;
    }

    return PyLong_AsUnsignedLongLong(value);
}

float
ToFloat(PyObject* value, const std::string& message)
{
    if (value == nullptr)
        return 0.0f;


    if (!PyNumber_Check(value))
    {
        mvThrowPythonError(mvErrorCode::mvWrongType, "Python value error. Must be float.");
        return 0.0f;
    }

    if (PyLong_Check(value))
    {
        return (float)PyLong_AsLong(value);
    }

    return (float)PyFloat_AsDouble(value);
}

double
ToDouble(PyObject* value, const std::string& message)
{
    if (value == nullptr)
        return 0.0;


    if (!PyNumber_Check(value))
    {
        mvThrowPythonError(mvErrorCode::mvWrongType, "Python value error. Must be double.");
        return 0.0;
    }

    return PyFloat_AsDouble(value);
}

bool
ToBool(PyObject* value, const std::string& message)
{
    if (value == nullptr)
        return false;


    if (!PyBool_Check(value))
    {
        mvThrowPythonError(mvErrorCode::mvWrongType, "Python value error. Must be bool.");
        return false;
    }

    return PyLong_AsLong(value);
}

std::string
ToString(PyObject* value, const std::string& message)
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
            mvThrowPythonError(mvErrorCode::mvWrongType, "Python value error. Must be string.");
            return "";
        }
        result = _PyUnicode_AsString(str);
        Py_XDECREF(str);
    }

    return result;

}

static std::function<float(Py_buffer&, Py_ssize_t index)>
BufferViewFunctionsFloat(Py_buffer& bufferView)
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
        return [](Py_buffer& bufferView, Py_ssize_t index) {return (float)*((long long*)bufferView.buf + index); };

    else if (strcmp(bufferView.format, "k") == 0)
        return [](Py_buffer& bufferView, Py_ssize_t index) {return (float)*((unsigned long*)bufferView.buf + index); };

    else if (strcmp(bufferView.format, "K") == 0)
        return [](Py_buffer& bufferView, Py_ssize_t index) {return (float)*((unsigned long long*)bufferView.buf + index); };

    else if (strcmp(bufferView.format, "B") == 0)
        return [](Py_buffer& bufferView, Py_ssize_t index) {return (float)*((unsigned char*)bufferView.buf + index); };

    else if (strcmp(bufferView.format, "b") == 0)
        return [](Py_buffer& bufferView, Py_ssize_t index) {return (float)*((signed char*)bufferView.buf + index); };

    else if (strcmp(bufferView.format, "c") == 0)
        return [](Py_buffer& bufferView, Py_ssize_t index) {return (float)*((char*)bufferView.buf + index); };
    else
    {
        mvThrowPythonError(mvErrorCode::mvWrongType, "Unknown buffer type.");
        mvThrowPythonError(mvErrorCode::mvWrongType, bufferView.format);
        mvThrowPythonError(mvErrorCode::mvWrongType, "Currently supported buffer types f, d, l, B");
        return nullptr;
    }
}

static std::function<int(Py_buffer&, Py_ssize_t index)>
BufferViewFunctionsInt(Py_buffer& bufferView)
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
        return [](Py_buffer& bufferView, Py_ssize_t index) {return (int)*((long long*)bufferView.buf + index); };

    else if (strcmp(bufferView.format, "k") == 0)
        return [](Py_buffer& bufferView, Py_ssize_t index) {return (int)*((unsigned long*)bufferView.buf + index); };

    else if (strcmp(bufferView.format, "K") == 0)
        return [](Py_buffer& bufferView, Py_ssize_t index) {return (int)*((unsigned long long*)bufferView.buf + index); };

    else if (strcmp(bufferView.format, "B") == 0)
        return [](Py_buffer& bufferView, Py_ssize_t index) {return (int)*((unsigned char*)bufferView.buf + index); };

    else if (strcmp(bufferView.format, "b") == 0)
        return [](Py_buffer& bufferView, Py_ssize_t index) {return (int)*((signed char*)bufferView.buf + index); };

    else if (strcmp(bufferView.format, "c") == 0)
        return [](Py_buffer& bufferView, Py_ssize_t index) {return (int)*((char*)bufferView.buf + index); };
    else
    {
        mvThrowPythonError(mvErrorCode::mvWrongType, "Unknown buffer type.");
        mvThrowPythonError(mvErrorCode::mvWrongType, bufferView.format);
        mvThrowPythonError(mvErrorCode::mvWrongType, "Currently supported buffer types f, d, l, B");
        return nullptr;
    }
}

std::vector<unsigned char>
ToUCharVect(PyObject* value, const std::string& message)
{
    std::vector<unsigned char> items;
    if (value == nullptr)
        return items;

    if (PyTuple_Check(value))
    {
        items.resize(PyTuple_Size(value));
        for (Py_ssize_t i = 0; i < PyTuple_Size(value); ++i)
        {
            items[i] = (unsigned char)PyLong_AsLong(PyTuple_GetItem(value, i));
        }
    }

    else if (PyList_Check(value))
    {
        items.resize(PyList_Size(value));
        for (Py_ssize_t i = 0; i < PyList_Size(value); ++i)
        {
            items[i] = (unsigned char)PyLong_AsLong(PyList_GetItem(value, i));
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
                items.emplace_back((unsigned char)BufferViewer(buffer_info, i));
            }
        }

        PyBuffer_Release(&buffer_info);
    }

    else
        mvThrowPythonError(mvErrorCode::mvWrongType, "Python value error. Must be List[int].");


    return items;
}

std::vector<int>
ToIntVect(PyObject* value, const std::string& message)
{

    std::vector<int> items;
    if (value == nullptr)
        return items;

    if (PyTuple_Check(value))
    {
        items.resize(PyTuple_Size(value));
        for (Py_ssize_t i = 0; i < PyTuple_Size(value); ++i)
        {
            items[i] = PyLong_AsLong(PyTuple_GetItem(value, i));
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
        mvThrowPythonError(mvErrorCode::mvWrongType, "Python value error. Must be List[int].");


    return items;
}

std::vector<mvUUID>
ToUUIDVect(PyObject* value, const std::string& message)
{

    std::vector<mvUUID> items;
    if (value == nullptr)
        return items;


    if (PyTuple_Check(value))
    {
        items.resize(PyTuple_Size(value));
        for (Py_ssize_t i = 0; i < PyTuple_Size(value); ++i)
        {
            PyObject* item = PyTuple_GetItem(value, i);
            if (isPyObject_Int(item))
                items[i] = PyLong_AsUnsignedLongLong(item);
            else if (isPyObject_String(item))
                items[i] = GetIdFromAlias(*GContext->itemRegistry, ToString(item));
        }
    }

    else if (PyList_Check(value))
    {
        items.resize(PyList_Size(value));
        for (Py_ssize_t i = 0; i < PyList_Size(value); ++i)
        {
            PyObject* item = PyList_GetItem(value, i);
            if (isPyObject_Int(item))
                items[i] = PyLong_AsUnsignedLongLong(item);
            else if (isPyObject_String(item))
                items[i] = GetIdFromAlias(*GContext->itemRegistry, ToString(item));
        }
    }

    else
        mvThrowPythonError(mvErrorCode::mvWrongType, "Python value error. Must be List[int].");


    return items;
}

template<typename T>
Py_ssize_t GetSizeAndReserveMemoryVectVect(PyObject* value, std::vector<std::vector<T>>& vec, const std::string mode)
{
    assert((mode == "tuple" || mode == "list") && "mode parameter must be 'tuple' or 'list'");
    Py_ssize_t size = 0;
    if (mode == "tuple")
        size = PyTuple_Size(value);
    else if (mode == "list")
        size = PyList_Size(value);

    vec.reserve(size);
    return size;
}

template<typename T>
Py_ssize_t GetSizeAndReserveMemoryVect(PyObject* value, std::vector<T>& vec, const std::string mode)
{
    assert((mode == "tuple" || mode == "list") && "mode parameter must be 'tuple' or 'list'");
    Py_ssize_t size = 0;
    if (mode == "tuple")
        size = PyTuple_Size(value);
    else if (mode == "list")
        size = PyList_Size(value);

    vec.reserve(size);
    return size;
}

std::vector<float>
ToFloatVect(PyObject* value, const std::string& message)
{

    std::vector<float> items;
    if (value == nullptr)
        return items;


    if (PyTuple_Check(value))
    {
        for (Py_ssize_t i = 0; i < GetSizeAndReserveMemoryVect(value, items, "tuple"); ++i)
        {
            items.emplace_back((float)PyFloat_AsDouble(PyTuple_GetItem(value, i)));
        }
    }

    else if (PyList_Check(value))
    {
        for (Py_ssize_t i = 0; i < GetSizeAndReserveMemoryVect(value, items, "list"); ++i)
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
            items.reserve(buffer_info.len / buffer_info.itemsize);

            for (Py_ssize_t i = 0; i < buffer_info.len / buffer_info.itemsize; ++i)
            {
                items.emplace_back(BufferViewer(buffer_info, i));
            }
        }
        PyBuffer_Release(&buffer_info);
    }

    else
        mvThrowPythonError(mvErrorCode::mvWrongType, "Python value error. Must be List[float].");


    return items;
}

std::vector<double>
ToDoubleVect(PyObject* value, const std::string& message)
{

    std::vector<double> items;
    if (value == nullptr)
        return items;


    if (PyTuple_Check(value))
    {
        for (Py_ssize_t i = 0; i < GetSizeAndReserveMemoryVect(value, items, "tuple"); ++i)
        {
            items.emplace_back(PyFloat_AsDouble(PyTuple_GetItem(value, i)));
        }
    }

    else if (PyList_Check(value))
    {
        for (Py_ssize_t i = 0; i < GetSizeAndReserveMemoryVect(value, items, "list"); ++i)
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
            const Py_ssize_t size = buffer_info.len / buffer_info.itemsize;
            items.reserve(size);
            for (Py_ssize_t i = 0; i < size; ++i)
            {
                items.emplace_back(BufferViewer(buffer_info, i));
            }
        }
        PyBuffer_Release(&buffer_info);
    }

    else
        mvThrowPythonError(mvErrorCode::mvWrongType, "Python value error. Must be List[float].");


    return items;
}

std::vector<std::string>
ToStringVect(PyObject* value, const std::string& message)
{

    std::vector<std::string> items;
    if (value == nullptr)
        return items;


    if (PyTuple_Check(value))
    {
        for (Py_ssize_t i = 0; i < GetSizeAndReserveMemoryVect(value, items, "tuple"); ++i)
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
        for (Py_ssize_t i = 0; i < GetSizeAndReserveMemoryVect(value, items, "list"); ++i)
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
        mvThrowPythonError(mvErrorCode::mvWrongType, "Python value error. Must be List[str].");


    return items;
}

mvColor
ToColor(PyObject* value, const std::string& message)
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

ImPlotPoint
ToPoint(PyObject* value, const std::string& message)
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

mvVec2
ToVec2(PyObject* value, const std::string& message)
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

mvVec4
ToVec4(PyObject* value, const std::string& message)
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

std::vector<std::pair<std::string, std::string>>
ToVectPairString(PyObject* value, const std::string& message)
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
        mvThrowPythonError(mvErrorCode::mvWrongType, "Python value error. Must be List[List[str, str]].");

    return items;
}

std::vector<mvVec2>
ToVectVec2(PyObject* value, const std::string& message)
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
        mvThrowPythonError(mvErrorCode::mvWrongType, "Python value error. Must be List[List[int, int]].");

    return items;
}

std::pair<std::vector<float>, std::vector<float>>
ToPairVec(PyObject* value, const std::string& message)
{
    std::pair<std::vector<float>, std::vector<float>> items;
    if (value == nullptr)
        return items;


    if (PyTuple_Check(value))
    {
        if (PyTuple_Size(value) != 2) mvThrowPythonError(mvErrorCode::mvNone, message);
        items.first = ToFloatVect(PyTuple_GetItem(value, 0), message);
        items.second = ToFloatVect(PyTuple_GetItem(value, 1), message);
    }
    else if (PyList_Check(value))
    {
        if (PyList_Size(value) != 2) mvThrowPythonError(mvErrorCode::mvNone, message);
        items.first = ToFloatVect(PyList_GetItem(value, 0), message);
        items.second = ToFloatVect(PyList_GetItem(value, 1), message);
    }

    else
        mvThrowPythonError(mvErrorCode::mvWrongType, message);

    return items;
}

std::vector<mvVec4>
ToVectVec4(PyObject* value, const std::string& message)
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
        mvThrowPythonError(mvErrorCode::mvWrongType, message);

    return items;
}

std::vector<std::pair<int, int>>
ToVectInt2(PyObject* value, const std::string& message)
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
        mvThrowPythonError(mvErrorCode::mvWrongType, message);

    return items;

}

std::vector<std::vector<std::string>>
ToVectVectString(PyObject* value, const std::string& message)
{
    std::vector<std::vector<std::string>> items;
    if (value == nullptr)
        return items;


    if (PyTuple_Check(value))
    {
        for (Py_ssize_t i = 0; i < GetSizeAndReserveMemoryVectVect(value, items, "tuple"); ++i)
            items.emplace_back(ToStringVect(PyTuple_GetItem(value, i), message));
    }

    else if (PyList_Check(value))
    {
        for (Py_ssize_t i = 0; i < GetSizeAndReserveMemoryVectVect(value, items, "list"); ++i)
            items.emplace_back(ToStringVect(PyList_GetItem(value, i), message));
    }

    return items;
}

std::vector<std::pair<std::string, float>>
ToVectPairStringFloat(PyObject* value, const std::string& message)
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
        mvThrowPythonError(mvErrorCode::mvWrongType, message);

    return items;
}

std::vector<std::vector<float>>
ToVectVectFloat(PyObject* value, const std::string& message)
{
    std::vector<std::vector<float>> items;
    if (value == nullptr)
        return items;


    if (PyTuple_Check(value))
    {
        for (Py_ssize_t i = 0; i < GetSizeAndReserveMemoryVectVect(value, items, "tuple"); ++i)
            items.emplace_back(ToFloatVect(PyTuple_GetItem(value, i), message));
    }

    else if (PyList_Check(value))
    {
        for (Py_ssize_t i = 0; i < GetSizeAndReserveMemoryVectVect(value, items, "list"); ++i)
            items.emplace_back(ToFloatVect(PyList_GetItem(value, i), message));
    }

    return items;
}

std::vector<std::vector<int>>
ToVectVectInt(PyObject* value, const std::string& message)
{
    std::vector<std::vector<int>> items;
    if (value == nullptr)
        return items;


    if (PyTuple_Check(value))
    {
        for (Py_ssize_t i = 0; i < GetSizeAndReserveMemoryVectVect(value, items, "tuple"); ++i)
            items.emplace_back(ToIntVect(PyTuple_GetItem(value, i), message));
    }

    else if (PyList_Check(value))
    {
        for (Py_ssize_t i = 0; i < GetSizeAndReserveMemoryVectVect(value, items, "list"); ++i)
            items.emplace_back(ToIntVect(PyList_GetItem(value, i), message));
    }

    return items;
}

std::vector<std::vector<double>>
ToVectVectDouble(PyObject* value, const std::string& message)
{
    std::vector<std::vector<double>> items;
    if (value == nullptr)
        return items;


    if (PyTuple_Check(value))
    {
        for (Py_ssize_t i = 0; i < GetSizeAndReserveMemoryVectVect(value, items, "tuple"); ++i)
            items.emplace_back(ToDoubleVect(PyTuple_GetItem(value, i), message));
    }

    else if (PyList_Check(value))
    {
        for (Py_ssize_t i = 0; i < GetSizeAndReserveMemoryVectVect(value, items, "list"); ++i)
            items.emplace_back(ToDoubleVect(PyList_GetItem(value, i), message));
    }

    return items;
}



static bool
VerifyArguments(int start, PyObject* args, const std::vector<mvPythonDataElement>& elements)
{

    if (start >= PyTuple_Size(args))
        return true;

    int end = (int)PyTuple_Size(args);
    if (end > (int)elements.size())
        end = (int)elements.size();

    for (int i = start; i < end; i++)
    {
        const auto& item = elements[i];
        PyObject* obj = nullptr;
        obj = PyTuple_GetItem(args, i);

        switch (item.type)
        {
            case mvPyDataType::String:
                if (!isPyObject_String(obj))
                    return false;
                break;

            case mvPyDataType::UUID:
            {
                if (isPyObject_Int(obj))
                    break;
                else if (isPyObject_String(obj))
                    break;
                return false;
            }

            case mvPyDataType::Long:
            case mvPyDataType::Integer:
                if (!isPyObject_Int(obj))
                    return false;
                break;

            case mvPyDataType::Float:
                if (!isPyObject_Float(obj))
                    return false;
                break;

            case mvPyDataType::Bool:
                if (!isPyObject_Bool(obj))
                    return false;
                break;

            case mvPyDataType::StringList:
                if (!isPyObject_StringList(obj))
                    return false;
                break;

            case mvPyDataType::FloatList:
                if (!isPyObject_FloatList(obj))
                    return false;
                break;

            case mvPyDataType::IntList:
                if (!isPyObject_IntList(obj))
                    return false;
                break;

            case mvPyDataType::Double:
                if (!isPyObject_Double(obj))
                    return false;
                break;

            case mvPyDataType::Callable:
                if (!isPyObject_Callable(obj))
                    return false;
                break;

            case mvPyDataType::Dict:
                if (!isPyObject_Dict(obj))
                    return false;
                break;

            case mvPyDataType::ListFloatList:
                if (!isPyObject_ListFloatList(obj))
                    return false;
                break;

            case mvPyDataType::ListStrList:
                if (!isPyObject_ListStringList(obj))
                    return false;
                break;

            case mvPyDataType::ListListInt:
                if (!isPyObject_ListIntList(obj))
                    return false;
                break;

            default:
                if (!isPyObject_Any(obj))
                    return false;
        }
    }

    return true;
}

static char
PythonDataTypeSymbol(mvPyDataType type)
{
    switch (type)
    {
        //case mvPyDataType::UUID:    return 'K';
        case mvPyDataType::Long:    return 'l';
        case mvPyDataType::String:  return 's';
        case mvPyDataType::Integer: return 'i';
        case mvPyDataType::Float:   return 'f';
        case mvPyDataType::Double:  return 'd';
        case mvPyDataType::Bool:    return 'p';
        default:                    return 'O';
    }
}

static const char*
PythonDataTypeString(mvPyDataType type)
{
    switch (type)
    {
        case mvPyDataType::String:         return " : str";
        case mvPyDataType::UUID:           return " : Union[int, str]";
        case mvPyDataType::Integer:        return " : int";
        case mvPyDataType::Long:           return " : int";
        case mvPyDataType::Float:          return " : float";
        case mvPyDataType::Double:         return " : float";
        case mvPyDataType::Bool:           return " : bool";
        case mvPyDataType::StringList:     return " : Union[List[str], Tuple[str, ...]]";
        case mvPyDataType::FloatList:      return " : Union[List[float], Tuple[float, ...]]";
        case mvPyDataType::DoubleList:     return " : Union[List[float], Tuple[float, ...]]";
        case mvPyDataType::IntList:        return " : Union[List[int], Tuple[int, ...]]";
        case mvPyDataType::UUIDList:       return " : Union[List[int], Tuple[int, ...]]";
        case mvPyDataType::Callable:       return " : Callable";
        case mvPyDataType::Dict:           return " : dict";
        case mvPyDataType::ListAny:        return " : List[Any]";
        case mvPyDataType::ListListInt:    return " : List[Union[List[int], Tuple[int, ...]]]";
        case mvPyDataType::ListFloatList:  return " : List[List[float]]";
        case mvPyDataType::ListDoubleList: return " : List[List[float]]";
        case mvPyDataType::ListStrList:    return " : List[List[str]]";
        case mvPyDataType::Object:         return " : Any";
        default:                              return " : unknown";
    }
}

const char*
PythonDataTypeActual(mvPyDataType type)
{
    switch (type)
    {
        case mvPyDataType::String:        return "str";
        case mvPyDataType::UUID:          return "Union[int, str]";
        case mvPyDataType::Integer:       return "int";
        case mvPyDataType::Long:          return "int";
        case mvPyDataType::Float:         return "float";
        case mvPyDataType::Double:        return "float";
        case mvPyDataType::Bool:          return "bool";
        case mvPyDataType::StringList:    return "Union[List[str], Tuple[str, ...]]";
        case mvPyDataType::FloatList:     return "Union[List[float], Tuple[float, ...]]";
        case mvPyDataType::IntList:       return "Union[List[int], Tuple[int, ...]]";
        case mvPyDataType::UUIDList:      return "Union[List[int], Tuple[int, ...]]";
        case mvPyDataType::Callable:      return "Callable";
        case mvPyDataType::Dict:          return "dict";
        case mvPyDataType::ListFloatList: return "List[List[float]]";
        case mvPyDataType::ListStrList:   return "List[List[str]]";
        case mvPyDataType::None:          return "None";
        case mvPyDataType::Object:        return "Any";
        default:                              return "Any";
    }
}

mvPythonParser
FinalizeParser(const mvPythonParserSetup& setup, const std::vector<mvPythonDataElement>& args)
{

    mvPythonParser parser;

    // separate args into category
    for (auto& arg : args)
    {
        switch (arg.arg_type)
        {
            case mvArgType::REQUIRED_ARG:
                parser.required_elements.push_back(arg);
                break;
            case mvArgType::POSITIONAL_ARG:
                parser.optional_elements.push_back(arg);
                break;
            case mvArgType::KEYWORD_ARG:
                parser.keyword_elements.push_back(arg);
                break;
            default:
                parser.deprecated_elements.push_back(arg);
        }
    }

    // build format string and keywords
    if (!parser.required_elements.empty())
    {
        for (auto& element : parser.required_elements)
        {
            parser.formatstring.push_back(PythonDataTypeSymbol(element.type));
            parser.keywords.push_back(element.name);
        }
    }

    parser.formatstring.push_back('|');

    if (!parser.optional_elements.empty())
    {

        for (auto& element : parser.optional_elements)
        {
            parser.formatstring.push_back(PythonDataTypeSymbol(element.type));
            parser.keywords.push_back(element.name);
        }
    }

    if (!parser.keyword_elements.empty())
    {
        parser.formatstring.push_back('$');
        for (auto& element : parser.keyword_elements)
        {
            parser.formatstring.push_back(PythonDataTypeSymbol(element.type));
            parser.keywords.push_back(element.name);
        }
    }
    parser.formatstring.push_back(0);
    parser.keywords.push_back(NULL);

    parser.about = setup.about;
    parser.returnType = setup.returnType;
    parser.category = setup.category;
    parser.createContextManager = setup.createContextManager;
    parser.unspecifiedKwargs = setup.unspecifiedKwargs;
    parser.internal = setup.internal;

    // build documentation
    std::string documentation = parser.about + "\n\nReturn Type: " + PythonDataTypeActual(parser.returnType) + "\n";

    if (!parser.required_elements.empty())
        documentation += "\n\nRequired Arguments\n_______________\n\n";

    for (const auto& element : parser.required_elements)
    {
        documentation += "\n* ";
        documentation += element.name + std::string(PythonDataTypeString(element.type));
        documentation += "\n\t\t" + std::string(element.description);
    }

    if (!parser.optional_elements.empty())
        documentation += "\n\nOptional Arguments\n_______________\n\n";

    for (const auto& element : parser.optional_elements)
    {
        documentation += "\n* ";
        documentation += element.name + std::string(PythonDataTypeString(element.type));
        documentation += " = " + std::string(element.default_value);
        documentation += "\n\t\t" + std::string(element.description);
    }

    if (!parser.keyword_elements.empty())
        documentation += "\n\nKeyword Arguments\n_______________\n\n";

    for (const auto& element : parser.keyword_elements)
    {
        documentation += "\n* ";
        documentation += element.name + std::string(PythonDataTypeString(element.type));
        documentation += " = " + std::string(element.default_value);
        documentation += "\n\t\t" + std::string(element.description);
    }

    if (!parser.keyword_elements.empty())
        documentation += "\n\nDeprecated Keyword Arguments\n_______________\n\n";

    for (const auto& element : parser.deprecated_elements)
    {
        documentation += "\n* ";
        documentation += element.name + std::string(PythonDataTypeString(element.type));
        documentation += " = " + std::string(element.default_value);
        documentation += "\n\t\t" + std::string(element.description);
    }

    parser.documentation = std::move(documentation);

    return parser;
}

bool
VerifyRequiredArguments(const mvPythonParser& parser, PyObject* args)
{

    // ensure enough args were provided
    if ((size_t)PyTuple_Size(args) < parser.required_elements.size())
    {
        assert(false && "Not enough arguments provided");
        mvThrowPythonError(mvErrorCode::mvNone, "Not enough arguments provided. Expected: " +
                                                std::to_string(parser.required_elements.size()) + " Recieved: " + std::to_string((size_t)PyTuple_Size(args)));
        return false;
    }

    return VerifyArguments(0, args, parser.required_elements);
}

bool
VerifyPositionalArguments(const mvPythonParser& parser, PyObject* args)
{
    return VerifyArguments((int)parser.optional_elements.size(), args, parser.optional_elements);
}

bool
VerifyKeywordArguments(const mvPythonParser& parser, PyObject* args)
{
    if (args == nullptr)
        return false;

    if (!PyArg_ValidateKeywordArguments(args))
        return false;

    PyObject* keys = PyDict_Keys(args);

    bool exists = false;
    for (int i = 0; i < PyList_Size(keys); i++)
    {
        PyObject* item = PyList_GetItem(keys, i);
        auto sitem = ToString(item);

        bool found = false;
        for (const auto& keyword : parser.keyword_elements)
        {
            if (sitem == keyword.name)
            {
                found = true;
                break;
            }
        }

        if (found)
            continue;
        else
        {
            for (const auto& keyword : parser.optional_elements)
            {
                if (sitem == keyword.name)
                {
                    found = true;
                    break;
                }
            }

            if (found)
                continue;
        }

        for (const auto& keyword : parser.required_elements)
        {
            if (sitem == keyword.name)
            {
                found = true;
                break;
            }
        }

        if (found)
            continue;

        for (const auto& keyword : parser.deprecated_elements)
        {
            if (sitem == keyword.name)
            {
                found = true;
                break;
            }
        }

        if (found)
            continue;

        mvThrowPythonError(mvErrorCode::mvNone, sitem + " keyword does not exist.");
        assert(false);
        exists = false;
        break;
    }

    Py_XDECREF(keys);

    return exists;
}

bool
VerifyArgumentCount(const mvPythonParser& parser, PyObject* args)
{
    if (args == nullptr && parser.required_elements.size() == 0)
        return true;
    if (args == nullptr)
    {
        mvThrowPythonError(mvErrorCode::mvNone, "This command has a minimum number of arguments of " + std::to_string(parser.required_elements.size()));
        return false;
    }

    int possibleArgs = (int)parser.required_elements.size() + (int)parser.optional_elements.size();
    int minArgs = (int)parser.required_elements.size();
    int numberOfArgs = (int)PyTuple_Size(args);

    if (numberOfArgs > possibleArgs)
    {
        mvThrowPythonError(mvErrorCode::mvNone, "This command has a maximum number of arguments of " + std::to_string(possibleArgs) +
                                                " but recieved " + std::to_string(numberOfArgs));
        return false;
    }
    if (numberOfArgs < minArgs)
    {
        mvThrowPythonError(mvErrorCode::mvNone, "This command has a minimum number of arguments of " + std::to_string(minArgs) +
                                                " but only recieved " + std::to_string(numberOfArgs));
        return false;
    }
    return true;
}

bool
Parse(const mvPythonParser& parser, PyObject* args, PyObject* kwargs, const char* message, ...)
{

    bool check = true;

    va_list arguments;
    va_start(arguments, message);
    if (!PyArg_VaParseTupleAndKeywords(args, kwargs, parser.formatstring.data(),
                                       const_cast<char**>(parser.keywords.data()), arguments))
    {
        check = false;
    }

    va_end(arguments);

    if (!check)
        mvThrowPythonError(mvErrorCode::mvNone, "Error parsing Dear PyGui command: " + std::string(message));

    return check;
}

void
GenerateStubFile(const std::string& directory)
{
    // TODO: Check here on how to do this for documentation
    const auto& commands = GetModuleParsers();

    std::ofstream stub;
    stub.open(directory + "/_dearpygui.pyi");

    stub << "from typing import List, Any, Callable, Union, Tuple\n";
    stub << "from dearpygui._dearpygui import *\n\n";
    stub << "##########################################################\n";
    stub << "# This file is generated automatically by mvPythonParser #\n";
    stub << "##########################################################\n\n";
    stub << "# ~ Dear PyGui Version: " << MV_SANDBOX_VERSION << "\n";

    for (const auto& parser : commands)
    {
        stub << "def " << parser.first << "(";

        bool first_arg = true;
        for (const auto& args : parser.second.required_elements)
        {
            if (first_arg)
                first_arg = false;
            else
                stub << ", ";
            stub << args.name << PythonDataTypeString(args.type);
        }

        for (const auto& args : parser.second.optional_elements)
        {
            if (first_arg)
                first_arg = false;
            else
                stub << ", ";
            stub << args.name << PythonDataTypeString(args.type) << " =''";
        }

        if (!parser.second.keyword_elements.empty())
        {
            if (first_arg)
                first_arg = false;
            else
                stub << ", ";

            stub << "*";
        }

        for (const auto& args : parser.second.keyword_elements)
            stub << ", " << args.name << ": " << PythonDataTypeActual(args.type) << " =''";

        if (parser.second.unspecifiedKwargs)
            stub << ", **kwargs";

        stub << ") -> " << PythonDataTypeActual(parser.second.returnType) << ":";

        stub << "\n\t\"\"\"" << parser.second.about.c_str() << "\"\"\"";

        stub << "\n\t...\n\n";
    }

    auto& constants = GetModuleConstants();
    auto& keysConstants = GetKeysConstants();

    for (auto& item : constants)
        stub << item.first << "=0\n";

    for (auto& item : keysConstants)
        stub << item.first << "=0\n";

    stub.close();
}

void
GenerateCoreFile(std::ofstream& stream)
{
    const auto& commands = GetModuleParsers();

    // current date/time based on current system
    time_t now = time(0);

    // convert now to string form
    char* dt = ctime(&now);

    for (const auto& parser : commands)
    {
        if (parser.second.internal)
            continue;

        stream << "def " << parser.first << "(";

        bool first_arg = true;
        for (const auto& args : parser.second.required_elements)
        {
            if (first_arg)
                first_arg = false;
            else
                stream << ", ";
            stream << args.name << PythonDataTypeString(args.type);
        }

        for (const auto& args : parser.second.optional_elements)
        {
            if (first_arg)
                first_arg = false;
            else
                stream << ", ";
            stream << args.name << PythonDataTypeString(args.type) << " =" << args.default_value;
        }

        if (!parser.second.keyword_elements.empty())
        {
            if (first_arg)
                first_arg = false;
            else
                stream << ", ";

            stream << "*";
        }

        for (const auto& args : parser.second.keyword_elements)
            stream << ", " << args.name << ": " << PythonDataTypeActual(args.type) << " =" << args.default_value;

        if (first_arg)
            stream << "**kwargs) -> ";
        else
            stream << ", **kwargs) -> ";
        stream << PythonDataTypeActual(parser.second.returnType) << ":";

        stream << "\n\t\"\"\"\t " << parser.second.about.c_str();

        stream << "\n\n\tArgs:";
        for (const auto& args : parser.second.required_elements)
        {
            stream << "\n\t\t" << args.name << " (" << PythonDataTypeActual(args.type) << "): " << args.description;
        }

        for (const auto& args : parser.second.optional_elements)
        {
            stream << "\n\t\t" << args.name << " (" << PythonDataTypeActual(args.type) << ", optional): " << args.description;
        }

        for (const auto& args : parser.second.keyword_elements)
        {
            stream << "\n\t\t" << args.name << " (" << PythonDataTypeActual(args.type) << ", optional): " << args.description;
        }

        for (const auto& args : parser.second.deprecated_elements)
        {
            stream << "\n\t\t" << args.name << " (" << PythonDataTypeActual(args.type) << ", optional): (deprecated) " << args.description;
        }

        stream << "\n\tReturns:";
        stream << "\n\t\t" << PythonDataTypeActual(parser.second.returnType);
        stream << "\n\t\"\"\"";


        for (const auto& args : parser.second.deprecated_elements)
        {
            if (args.arg_type == mvArgType::DEPRECATED_REMOVE_KEYWORD_ARG)
            {
                stream << "\n\n\tif '" << args.name << "' in kwargs.keys():";
                stream << "\n\n\t\twarnings.warn('" << args.name << " keyword removed', DeprecationWarning, 2)";
                stream << "\n\n\t\tkwargs.pop('" << args.name << "', None)";
            }

            else if (args.arg_type == mvArgType::DEPRECATED_RENAME_KEYWORD_ARG)
            {
                stream << "\n\n\tif '" << args.name << "' in kwargs.keys():";
                stream << "\n\t\twarnings.warn('" << args.name << " keyword renamed to " << args.new_name << "', DeprecationWarning, 2)";
                stream << "\n\t\t" << args.new_name << "=kwargs['" << args.name << "']";
            }
        }

        stream << "\n\n\treturn internal_dpg." << parser.first << "(";

        first_arg = true;
        for (const auto& args : parser.second.required_elements)
        {
            if (first_arg)
                first_arg = false;
            else
                stream << ", ";
            stream << args.name;
        }

        for (const auto& args : parser.second.optional_elements)
        {
            if (first_arg)
                first_arg = false;
            else
                stream << ", ";
            stream << args.name;
        }

        for (const auto& args : parser.second.keyword_elements)
        {
            if (first_arg)
                first_arg = false;
            else
                stream << ", ";
            stream << args.name << "=" << args.name;
        }

        if (first_arg)
            stream << "**kwargs)\n\n";
        else
            stream << ", **kwargs)\n\n";
    }
}

void
GenerateContextsFile(std::ofstream& stream)
{
    const auto& commands = GetModuleParsers();

    // current date/time based on current system
    time_t now = time(0);

    // convert now to string form
    char* dt = ctime(&now);

    for (const auto& parser : commands)
    {
        if (!parser.second.createContextManager)
            continue;

        stream << "\n@contextmanager\n";
        stream << "def " << parser.first.substr(4) << "(";

        bool first_arg = true;
        for (const auto& args : parser.second.required_elements)
        {
            if (first_arg)
                first_arg = false;
            else
                stream << ", ";
            stream << args.name << PythonDataTypeString(args.type);
        }

        for (const auto& args : parser.second.optional_elements)
        {
            if (first_arg)
                first_arg = false;
            else
                stream << ", ";
            stream << args.name << PythonDataTypeString(args.type) << " =" << args.default_value;
        }

        if (!parser.second.keyword_elements.empty())
        {
            if (first_arg)
                first_arg = false;
            else
                stream << ", ";

            stream << "*";
        }

        for (const auto& args : parser.second.keyword_elements)
            stream << ", " << args.name << ": " << PythonDataTypeActual(args.type) << " =" << args.default_value;

        if (first_arg)
            stream << "**kwargs) -> ";
        else
            stream << ", **kwargs) -> ";
        stream << PythonDataTypeActual(parser.second.returnType) << ":";

        stream << "\n\t\"\"\"\t " << parser.second.about.c_str();

        stream << "\n\n\tArgs:";
        for (const auto& args : parser.second.required_elements)
        {
            stream << "\n\t\t" << args.name << " (" << PythonDataTypeActual(args.type) << "): " << args.description;
        }

        for (const auto& args : parser.second.optional_elements)
        {
            stream << "\n\t\t" << args.name << " (" << PythonDataTypeActual(args.type) << ", optional): " << args.description;
        }

        for (const auto& args : parser.second.keyword_elements)
        {
            stream << "\n\t\t" << args.name << " (" << PythonDataTypeActual(args.type) << ", optional): " << args.description;
        }

        for (const auto& args : parser.second.deprecated_elements)
        {
            stream << "\n\t\t" << args.name << " (" << PythonDataTypeActual(args.type) << ", optional): (deprecated) " << args.description;
        }

        stream << "\n\tYields:";
        stream << "\n\t\t" << PythonDataTypeActual(parser.second.returnType);
        stream << "\n\t\"\"\"";

        stream << "\n\ttry:";

        for (const auto& args : parser.second.deprecated_elements)
        {
            if (args.arg_type == mvArgType::DEPRECATED_REMOVE_KEYWORD_ARG)
            {
                stream << "\n\n\t\tif '" << args.name << "' in kwargs.keys():";
                stream << "\n\t\t\twarnings.warn('" << args.name << " keyword removed', DeprecationWarning, 2)";
                stream << "\n\t\t\tkwargs.pop('" << args.name << "', None)";
            }

            else if (args.arg_type == mvArgType::DEPRECATED_RENAME_KEYWORD_ARG)
            {
                stream << "\n\n\t\tif '" << args.name << "' in kwargs.keys():";
                stream << "\n\t\t\twarnings.warn('" << args.name << " keyword renamed to " << args.new_name << "', DeprecationWarning, 2)";
                stream << "\n\t\t\t" << args.new_name << "=kwargs['" << args.name << "']";
            }
        }

        stream << "\n\t\twidget = internal_dpg." << parser.first << "(";

        first_arg = true;
        for (const auto& args : parser.second.required_elements)
        {
            if (first_arg)
                first_arg = false;
            else
                stream << ", ";
            stream << args.name;
        }

        for (const auto& args : parser.second.optional_elements)
        {
            if (first_arg)
                first_arg = false;
            else
                stream << ", ";
            stream << args.name;
        }

        for (const auto& args : parser.second.keyword_elements)
        {
            if (first_arg)
                first_arg = false;
            else
                stream << ", ";
            stream << args.name << "=" << args.name;
        }

        if (first_arg)
            stream << "**kwargs)\n";
        else
            stream << ", **kwargs)\n";

        stream << "\t\tinternal_dpg.push_container_stack(widget)\n";
        stream << "\t\tyield widget\n";
        stream << "\tfinally:\n";
        stream << "\t\tinternal_dpg.pop_container_stack()\n";

    }

}

void
GenerateCoreFileRTD(std::ofstream& stream)
{
    const auto& commands = GetModuleParsers();

    // current date/time based on current system
    time_t now = time(0);

    // convert now to string form
    char* dt = ctime(&now);

    for (const auto& parser : commands)
    {
        if (parser.second.internal)
            continue;

        stream << "def " << parser.first << "(";

        bool first_arg = true;
        for (const auto& args : parser.second.required_elements)
        {
            if (first_arg)
                first_arg = false;
            else
                stream << ", ";
            stream << args.name;
        }

        for (const auto& args : parser.second.optional_elements)
        {
            if (first_arg)
                first_arg = false;
            else
                stream << ", ";
            stream << args.name << "=" << args.default_value;
        }

        if (parser.second.keyword_elements.empty())
            stream << "):";
        else
        {
            if(parser.second.required_elements.empty() && parser.second.optional_elements.empty())
                stream << "**kwargs):";
            else
                stream << ", **kwargs):";
        }

        stream << "\n\t\"\"\"\t " << parser.second.about.c_str();

        stream << "\n\n\tArgs:";
        for (const auto& args : parser.second.required_elements)
        {
            stream << "\n\t\t" << args.name << " (" << PythonDataTypeActual(args.type) << "): " << args.description;
        }

        for (const auto& args : parser.second.optional_elements)
        {
            stream << "\n\t\t" << args.name << " (" << PythonDataTypeActual(args.type) << ", optional): " << args.description;
        }

        for (const auto& args : parser.second.keyword_elements)
        {
            stream << "\n\t\t" << args.name << " (" << PythonDataTypeActual(args.type) << ", optional): " << args.description;
        }

        for (const auto& args : parser.second.deprecated_elements)
        {
            stream << "\n\t\t" << args.name << " (" << PythonDataTypeActual(args.type) << ", optional): (deprecated)" << args.description;
        }

        stream << "\n\tReturns:";
        stream << "\n\t\t" << PythonDataTypeActual(parser.second.returnType);
        stream << "\n\t\"\"\"";

        stream << "\n\n\treturn internal_dpg." << parser.first << "(";

        first_arg = true;
        for (const auto& args : parser.second.required_elements)
        {
            if (first_arg)
                first_arg = false;
            else
                stream << ", ";
            stream << args.name;
        }

        for (const auto& args : parser.second.optional_elements)
        {
            if (first_arg)
                first_arg = false;
            else
                stream << ", ";
            stream << args.name;
        }

        if (!parser.second.keyword_elements.empty())
        {
            if (parser.second.required_elements.empty() && parser.second.optional_elements.empty())
                stream << "**kwargs";
            else
                stream << ", **kwargs";
        }

        stream << ")\n\n";
    }
}

void
GenerateContextsFileRTD(std::ofstream& stream)
{
    const auto& commands = GetModuleParsers();

    // current date/time based on current system
    time_t now = time(0);

    // convert now to string form
    char* dt = ctime(&now);

    for (const auto& parser : commands)
    {
        if (!parser.second.createContextManager)
            continue;

        stream << "\n@contextmanager\n";
        stream << "def " << parser.first.substr(4) << "(";

        bool first_arg = true;
        for (const auto& args : parser.second.required_elements)
        {
            if (first_arg)
                first_arg = false;
            else
                stream << ", ";
            stream << args.name;
        }

        for (const auto& args : parser.second.optional_elements)
        {
            if (first_arg)
                first_arg = false;
            else
                stream << ", ";
            stream << args.name << "=" << args.default_value;
        }

        if (parser.second.keyword_elements.empty())
            stream << "):";
        else
        {
            if (parser.second.required_elements.empty() && parser.second.optional_elements.empty())
                stream << "**kwargs):";
            else
                stream << ", **kwargs):";
        }

        stream << "\n\t\"\"\"\t " << parser.second.about.c_str();

        stream << "\n\n\tArgs:";
        for (const auto& args : parser.second.required_elements)
        {
            stream << "\n\t\t" << args.name << " (" << PythonDataTypeActual(args.type) << "): " << args.description;
        }

        for (const auto& args : parser.second.optional_elements)
        {
            stream << "\n\t\t" << args.name << " (" << PythonDataTypeActual(args.type) << ", optional): " << args.description;
        }

        for (const auto& args : parser.second.keyword_elements)
        {
            stream << "\n\t\t" << args.name << " (" << PythonDataTypeActual(args.type) << ", optional): " << args.description;
        }

        for (const auto& args : parser.second.deprecated_elements)
        {
            stream << "\n\t\t" << args.name << " (" << PythonDataTypeActual(args.type) << ", optional): (deprecated)" << args.description;
        }

        stream << "\n\tYields:";
        stream << "\n\t\t" << PythonDataTypeActual(parser.second.returnType);
        stream << "\n\t\"\"\"";

        stream << "\n\ttry:";
        stream << "\n\t\twidget = internal_dpg." << parser.first << "(";

        first_arg = true;
        for (const auto& args : parser.second.required_elements)
        {
            if (first_arg)
                first_arg = false;
            else
                stream << ", ";
            stream << args.name;
        }

        for (const auto& args : parser.second.optional_elements)
        {
            if (first_arg)
                first_arg = false;
            else
                stream << ", ";
            stream << args.name;
        }

        if (!parser.second.keyword_elements.empty())
        {
            if (parser.second.required_elements.empty() && parser.second.optional_elements.empty())
                stream << "**kwargs";
            else
                stream << ", **kwargs";
        }

        stream << ")\n";
        stream << "\t\tinternal_dpg.push_container_stack(widget)\n";
        stream << "\t\tyield widget\n";
        stream << "\tfinally:\n";
        stream << "\t\tinternal_dpg.pop_container_stack()\n";

    }

}

void
GenerateDearPyGuiFile(const std::string& directory)
{
    std::ofstream stub;
    stub.open(directory + "/dearpygui.py");

    stub << "\n##########################################################\n";
    stub << "# Dear PyGui User Interface\n";
    stub << "#   ~ Version: " << MV_SANDBOX_VERSION << "\n";
    stub << "#\n";
    stub << "#   Notes:\n";
    stub << "#     * This file is automatically generated.\n#\n";
    stub << "#   Resources:\n";
    stub << "#     * FAQ:         https://github.com/hoffstadt/DearPyGui/discussions/categories/frequently-asked-questions-faq \n";
    stub << "#     * Homepage:    https://github.com/hoffstadt/DearPyGui \n";
    stub << "#     * Wiki:        https://github.com/hoffstadt/DearPyGui/wiki \n";
    stub << "#     * Issues:      https://github.com/hoffstadt/DearPyGui/issues\n";
    stub << "#     * Discussions: https://github.com/hoffstadt/DearPyGui/discussions\n";
    stub << "##########################################################\n\n";

    std::ifstream inputStream0(directory + "/_header.py");

    for (std::string line; std::getline(inputStream0, line);)
        stub << line << "\n";

    stub << "\n##########################################################\n";
    stub << "# Deprecated Commands\n";
    stub << "##########################################################\n";

    std::ifstream inputStream1(directory + "/_deprecated.py");

    for (std::string line; std::getline(inputStream1, line);)
        stub << line << "\n";

    stub << "\n##########################################################\n";
    stub << "# Container Context Managers\n";
    stub << "##########################################################\n\n";

    GenerateContextsFile(stub);

    stub << "\n##########################################################\n";
    stub << "# Core Wrappings\n";
    stub << "##########################################################\n\n";

    GenerateCoreFile(stub);

    stub << "\n##########################################################\n";
    stub << "# Constants #\n";
    stub << "##########################################################\n\n";

    auto& constants = GetModuleConstants();
    auto& keysConstants = GetKeysConstants();

    for (auto& item : constants)
        stub << item.first << "=internal_dpg." << item.first << "\n";
    
    for (auto& item : keysConstants)
        stub << item.first << "=internal_dpg." << item.first << "\n";

    stub.close();

    std::ofstream redirect;
    redirect.open(directory + "/_dearpygui.py");
    redirect << "from _dearpygui import *\n";
    redirect.close();
}

void
GenerateDearPyGuiFileRTD(const std::string& directory)
{
    std::ofstream stub;
    stub.open(directory + "/_dearpygui_RTD.py");

    stub << "\n##########################################################\n";
    stub << "# Dear PyGui User Interface (MODIFIED FOR READTHEDOCS)\n";
    stub << "#   ~ Version: " << MV_SANDBOX_VERSION << "\n";
    stub << "#\n";
    stub << "#   Notes:\n";
    stub << "#     * This file is automatically generated.\n#\n";
    stub << "#   Resources:\n";
    stub << "#     * FAQ:         https://github.com/hoffstadt/DearPyGui/discussions/categories/frequently-asked-questions-faq \n";
    stub << "#     * Homepage:    https://github.com/hoffstadt/DearPyGui \n";
    stub << "#     * Wiki:        https://github.com/hoffstadt/DearPyGui/wiki \n";
    stub << "#     * Issues:      https://github.com/hoffstadt/DearPyGui/issues\n";
    stub << "#     * Discussions: https://github.com/hoffstadt/DearPyGui/discussions\n";
    stub << "##########################################################\n\n";

    std::ifstream inputStream0(directory + "/_header.py");

    for (std::string line; std::getline(inputStream0, line);)
        stub << line << "\n";

    stub << "\n##########################################################\n";
    stub << "# Deprecated Commands\n";
    stub << "##########################################################\n";

    std::ifstream inputStream1(directory + "/_deprecated.py");

    for (std::string line; std::getline(inputStream1, line);)
        stub << line << "\n";

    stub << "\n##########################################################\n";
    stub << "# Container Context Managers\n";
    stub << "##########################################################\n\n";

    GenerateContextsFileRTD(stub);

    stub << "\n##########################################################\n";
    stub << "# Core Wrappings\n";
    stub << "##########################################################\n\n";

    GenerateCoreFileRTD(stub);

    stub << "\n##########################################################\n";
    stub << "# Constants #\n";
    stub << "##########################################################\n\n";

    auto& constants = GetModuleConstants();
    auto& keysConstants = GetKeysConstants();

    for (auto& item : constants)
        stub << item.first << "=internal_dpg." << item.first << "\n";

    for (auto& item : keysConstants)
        stub << item.first << "=internal_dpg." << item.first << "\n";

    stub.close();

    std::ofstream redirect;
    redirect.open(directory + "/_dearpygui.py");
    redirect << "from _dearpygui import *\n";
    redirect.close();
}

void
AddCommonArgs(std::vector<mvPythonDataElement>& args, CommonParserArgs argsFlags)
{

    args.push_back({ mvPyDataType::UUID, "id", mvArgType::DEPRECATED_RENAME_KEYWORD_ARG, "0", "", "tag" });
    args.push_back({ mvPyDataType::String, "label", mvArgType::KEYWORD_ARG, "None", "Overrides 'name' as label." });
    args.push_back({ mvPyDataType::Object, "user_data", mvArgType::KEYWORD_ARG, "None", "User data for callbacks" });
    args.push_back({ mvPyDataType::Bool, "use_internal_label", mvArgType::KEYWORD_ARG, "True", "Use generated internal label instead of user specified (appends ### uuid)." });

    if (argsFlags & MV_PARSER_ARG_ID)           args.push_back({ mvPyDataType::UUID, "tag", mvArgType::KEYWORD_ARG, "0", "Unique id used to programmatically refer to the item.If label is unused this will be the label." });
    if (argsFlags & MV_PARSER_ARG_WIDTH)        args.push_back({ mvPyDataType::Integer, "width", mvArgType::KEYWORD_ARG, "0", "Width of the item." });
    if (argsFlags & MV_PARSER_ARG_HEIGHT)       args.push_back({ mvPyDataType::Integer, "height", mvArgType::KEYWORD_ARG, "0", "Height of the item." });
    if (argsFlags & MV_PARSER_ARG_INDENT)       args.push_back({ mvPyDataType::Integer, "indent", mvArgType::KEYWORD_ARG, "-1", "Offsets the widget to the right the specified number multiplied by the indent style." });
    if (argsFlags & MV_PARSER_ARG_PARENT)       args.push_back({ mvPyDataType::UUID, "parent", mvArgType::KEYWORD_ARG, "0", "Parent to add this item to. (runtime adding)" });
    if (argsFlags & MV_PARSER_ARG_BEFORE)       args.push_back({ mvPyDataType::UUID, "before", mvArgType::KEYWORD_ARG, "0", "This item will be displayed before the specified item in the parent." });
    if (argsFlags & MV_PARSER_ARG_SOURCE)       args.push_back({ mvPyDataType::UUID, "source", mvArgType::KEYWORD_ARG, "0", "Overrides 'id' as value storage key." });
    if (argsFlags & MV_PARSER_ARG_PAYLOAD_TYPE) args.push_back({ mvPyDataType::String, "payload_type", mvArgType::KEYWORD_ARG, "'$$DPG_PAYLOAD'", "Sender string type must be the same as the target for the target to run the payload_callback." });
    if (argsFlags & MV_PARSER_ARG_CALLBACK)     args.push_back({ mvPyDataType::Callable, "callback", mvArgType::KEYWORD_ARG, "None", "Registers a callback." });
    if (argsFlags & MV_PARSER_ARG_DRAG_CALLBACK)args.push_back({ mvPyDataType::Callable, "drag_callback", mvArgType::KEYWORD_ARG, "None", "Registers a drag callback for drag and drop." });
    if (argsFlags & MV_PARSER_ARG_DROP_CALLBACK)args.push_back({ mvPyDataType::Callable, "drop_callback", mvArgType::KEYWORD_ARG, "None", "Registers a drop callback for drag and drop." });
    if (argsFlags & MV_PARSER_ARG_SHOW)         args.push_back({ mvPyDataType::Bool, "show", mvArgType::KEYWORD_ARG, "True", "Attempt to render widget." });
    if (argsFlags & MV_PARSER_ARG_ENABLED)      args.push_back({ mvPyDataType::Bool, "enabled", mvArgType::KEYWORD_ARG, "True", "Turns off functionality of widget and applies the disabled theme." });
    if (argsFlags & MV_PARSER_ARG_POS)		    args.push_back({ mvPyDataType::IntList, "pos", mvArgType::KEYWORD_ARG, "[]", "Places the item relative to window coordinates, [0,0] is top left." });
    if (argsFlags & MV_PARSER_ARG_FILTER)		args.push_back({ mvPyDataType::String, "filter_key", mvArgType::KEYWORD_ARG, "''", "Used by filter widget." });
    if (argsFlags & MV_PARSER_ARG_SEARCH_DELAY) args.push_back({ mvPyDataType::Bool, "delay_search", mvArgType::KEYWORD_ARG, "False", "Delays searching container for specified items until the end of the app. Possible optimization when a container has many children that are not accessed often." });

    if (argsFlags & MV_PARSER_ARG_TRACKED)
    {
        args.push_back({ mvPyDataType::Bool, "tracked", mvArgType::KEYWORD_ARG, "False", "Scroll tracking" });
        args.push_back({ mvPyDataType::Float, "track_offset", mvArgType::KEYWORD_ARG, "0.5", "0.0f:top, 0.5f:center, 1.0f:bottom" });
    }

}