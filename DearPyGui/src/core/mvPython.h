#pragma once
#include "mvCore.h"

// extremely nasty TEMPORARY code until
// the python c API can be properly
// seperated from the architecture.
// May the Clean Code Gods forgive me.

#ifdef MV_CPP
#include <iostream>
#define PyObject void
#define PyGILState_STATE int
#define PyExc_Exception -1
#define PyMODINIT_FUNC int
#define Py_None nullptr
#define GetPyNone() nullptr
#define Py_XINCREF(x) x
#define Py_XDECREF(x) x
#define Py_BEGIN_ALLOW_THREADS //
#define Py_END_ALLOW_THREADS //
#define ThrowPythonException(x) std::cout<<x<<std::endl

inline int       PyEval_GetFrame() { return -1; }
inline int       PyFrame_GetLineNumber(int) { return -1; }
inline void      PyErr_Format(int, const char*, int, int line = 0) {}
inline void      PyErr_Format(int, const char*, const char*, int line = 0) {}
inline void      PyErr_Print() { std::cout << "Error" << std::endl; };
inline PyObject* PyTuple_New(int) { return nullptr; }
inline PyObject* PyDict_New() { return nullptr; }
inline PyObject* PyTuple_GetItem(PyObject*, int) { return nullptr; }
inline void      PyTuple_SetItem(PyObject*, int, PyObject*) {}
inline PyObject* PyFloat_FromDouble(double x) { return nullptr; }
inline PyObject* PyLong_FromLong(int x) { return nullptr; }
inline bool      PyList_Check(PyObject*) { return true; }

namespace Marvel {

	//-----------------------------------------------------------------------------
	// Enums and Helper Functions
	//-----------------------------------------------------------------------------
	enum class mvPythonDataType
	{
		None, String, Integer, Float, Bool, StringList, FloatList, Optional,
		Object, IntList, KeywordOnly, Double, Callable, Dict, ListFloatList,
		ListStrList, Kwargs
	};

	//-----------------------------------------------------------------------------
	// mvPythonDataElement
	//-----------------------------------------------------------------------------
		struct mvPythonDataElement
		{
			mvPythonDataType type = mvPythonDataType::None;
			std::string      name = "";
			std::string      description;
			std::string      default_value = "...";

			[[nodiscard]] char getSymbol() const { return 'a'; }
		};

	class mvPythonParser
	{
	public:


		mvPythonParser() = default;

		mvPythonParser(const std::initializer_list<mvPythonDataElement>& elements,
			std::string about = "", std::string returnType = "None",
			std::string category = "App") {}

		bool parse(PyObject* args, PyObject* kwargs, const char* message, ...) { return true; }

		const char* getDocumentation() const { return ""; }
		const char* getCategory() const { return ""; }

	};

	inline PyObject* PyDict_GetItemString(PyObject*, const char*) { return nullptr; }
	inline void  PyDict_SetItemString(PyObject*, const char*, PyObject*) {}
	inline PyObject* ToPyString(const std::string& value) { return nullptr; }
	inline PyObject* ToPyFloat(float value) { return nullptr; }
	inline PyObject* ToPyInt(int value) { return nullptr; }
	inline PyObject* ToPyBool(bool value) { return nullptr; }
	inline PyObject* ToPyMPair(int x, float y) { return nullptr; }
	inline PyObject* ToPyMTrip(int i, float x, float y) { return nullptr; }
	inline PyObject* ToPyPair(float x, float y) { return nullptr; }
	inline PyObject* ToPyPairII(int x, int y) { return nullptr; }
	inline PyObject* ToPyPair(const std::string& x, const std::string& y) { return nullptr; }
	inline PyObject* ToPyList(const std::vector<mvVec2>& value) { return nullptr; }
	inline PyObject* ToPyList(const std::vector<int>& value) { return nullptr; }
	inline PyObject* ToPyList(const std::vector<float>& value) { return nullptr; }
	inline PyObject* ToPyList(const std::vector<std::string>& value) { return nullptr; }
	inline PyObject* ToPyList(const std::vector<std::vector<std::string>>& value) { return nullptr; }
	inline PyObject* ToPyList(const std::vector<std::pair<int, int>>& value) { return nullptr; }
	inline PyObject* ToPyColor(const mvColor& color) { return nullptr; }
	inline PyObject* ToPyTime(const tm& time) { return nullptr; }
	inline PyObject* ToPyIntList(int* value, int count) { return nullptr; }
	inline PyObject* ToPyFloatList(float* value, int count) { return nullptr; }

	inline int         ToInt(PyObject* value, const std::string& message = "Type must be an integer.") { return 0; }
	inline float       ToFloat(PyObject* value, const std::string& message = "Type must be a float.") { return 0.0f; }
	inline bool        ToBool(PyObject* value, const std::string& message = "Type must be a bool.") { return false; }
	inline mvColor     ToColor(PyObject* value, const std::string& message = "Type must be a list or tuple of ints.") { return {}; }
	inline mvVec2      ToVec2(PyObject* value, const std::string& message = "Type must be a list or tuple of floats.") { return {}; }
	inline mvVec4      ToVec4(PyObject* value, const std::string& message = "Type must be a list or tuple of floats.") { return {}; }
	inline std::string ToString(PyObject* value, const std::string& message = "Type must be a string.") { return ""; }
	inline tm          ToTime(PyObject* value, const std::string& message = "Type must be a dict") { return {}; }

	inline std::vector<mvVec2>                              ToVectVec2(PyObject* value, const std::string& message = "Type must be a list/tuple of list/tuple.") { return {}; }
	inline std::pair<std::vector<float>, std::vector<float>>ToPairVec(PyObject* value, const std::string& message = "Type must be a list/tuple of two list/tuple.") { return {}; }
	inline std::vector<mvVec4>                              ToVectVec4(PyObject* value, const std::string& message = "Type must be a list/tuple of list/tuple.") { return {}; }
	inline std::vector<int>                                 ToIntVect(PyObject* value, const std::string& message = "Type must be a list or tuple of integers.") { return {}; }
	inline std::vector<float>                               ToFloatVect(PyObject* value, const std::string& message = "Type must be a list or tuple of floats.") { return {}; }
	inline std::vector<std::string>                         ToStringVect(PyObject* value, const std::string& message = "Type must be a list or tuple of strings.") { return {}; }
	inline std::vector<std::pair<int, int>>                 ToVectInt2(PyObject* value, const std::string& message = "Type must be an list/tuple of integer.") { return {}; }
	inline std::vector<std::pair<std::string, std::string>> ToVectPairString(PyObject* value, const std::string& message = "Type must be an list/tuple of string pairs.") { return {}; }
	inline std::vector<std::vector<std::string>>            ToVectVectString(PyObject* value, const std::string& message = "Type must be an list/tuple of list/tuple of strings.") { return {}; }
	inline std::vector<std::vector<float>>                  ToVectVectFloat(PyObject* value, const std::string& message = "Type must be an list/tuple of list/tuple of strings.") { return {}; }
	inline std::vector<std::pair<std::string, float>>       ToVectPairStringFloat(PyObject* value, const std::string& message = "Type must be an list/tuple of str,float pairs.") { return {}; }

}
#else
	#define PY_SSIZE_T_CLEAN
	#include <Python.h>
	#include <frameobject.h>
	#include "mvPyObject.h"
	#include "mvDataStorage.h"
	#include "mvPythonExceptions.h"
	#include "mvPythonParser.h"
	#include "mvPythonTranslator.h"
	#include "mvGlobalIntepreterLock.h"
#endif
