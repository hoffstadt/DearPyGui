#pragma once

//-----------------------------------------------------------------------------
// mvPythonTranslator
//
//     - This class handles conversion between python/c++ types while also
//       ensuring thread safety on the python interpreter
//     
//-----------------------------------------------------------------------------

#include <string>
#include <vector>
#include <map>
#include "mvCore.h"
#define PY_SSIZE_T_CLEAN
#include <Python.h>

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvGlobalIntepreterLock
	//     - A convience class to automatically handle aqcuiring and releasing
	//       python's global intpreter lock between python c api calls.
	//-----------------------------------------------------------------------------
	class mvGlobalIntepreterLock
	{
	
	public:

		mvGlobalIntepreterLock();
		~mvGlobalIntepreterLock();

	private:

		PyGILState_STATE m_gstate;

	};

	//-----------------------------------------------------------------------------
	// mvPythonTranslator
	//-----------------------------------------------------------------------------
	class mvPythonTranslator
	{

	public:

		// conversion to python
		static PyObject* GetPyNone ();
		static PyObject* ToPyString(const std::string& value);
		static PyObject* ToPyFloat (float value);
		static PyObject* ToPyInt   (int value);
		static PyObject* ToPyBool  (bool value);
		static PyObject* ToPyMPair (int x, float y);
		static PyObject* ToPyPair  (float x, float y);
		static PyObject* ToPyPairII(int x, int y);
		static PyObject* ToPyPair  (const std::string& x, const std::string& y);
		static PyObject* ToPyList  (const std::vector<int>& value);
		static PyObject* ToPyList  (const std::vector<float>& value);
		static PyObject* ToPyList  (const std::vector<std::string>& value);
		static PyObject* ToPyList  (const std::vector<std::vector<std::string>>& value);
		static PyObject* ToPyList  (const std::vector<std::pair<int, int>>& value);

		// conversion to c++
		static int         ToInt   (PyObject* value, const std::string& message = "Type must be an integer.");
		static float       ToFloat (PyObject* value, const std::string& message = "Type must be a float.");
		static bool        ToBool  (PyObject* value, const std::string& message = "Type must be a bool.");
		static mvColor     ToColor (PyObject* value, const std::string& message = "Type must be a list or tuple of ints.");
		static mvVec2      ToVec2  (PyObject* value, const std::string& message = "Type must be a list or tuple of floats.");
		static std::string ToString(PyObject* value, const std::string& message = "Type must be a string.");

		static std::vector<mvVec2>                              ToVectVec2      (PyObject* value, const std::string& message = "Type must be a list/tuple of list/tuple.");
		static std::vector<int>                                 ToIntVect       (PyObject* value, const std::string& message = "Type must be a list or tuple of integers.");
		static std::vector<float>                               ToFloatVect     (PyObject* value, const std::string& message = "Type must be a list or tuple of floats.");
		static std::vector<std::string>                         ToStringVect    (PyObject* value, const std::string& message = "Type must be a list or tuple of strings.");
		static std::vector<std::pair<int, int>>                 ToVectInt2      (PyObject* value, const std::string& message = "Type must be an list/tuple of integer.");
		static std::vector<std::pair<std::string, std::string>> ToVectPairString(PyObject* value, const std::string& message = "Type must be an list/tuple of string pairs.");
		static std::vector<std::vector<std::string>>            ToVectVectString(PyObject* value, const std::string& message = "Type must be an list/tuple of list/tuple of strings.");

	private:

		mvPythonTranslator() = default;

	};
}