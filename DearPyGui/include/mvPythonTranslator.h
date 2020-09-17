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
	// conversion to python
	PyObject*   GetPyNone ();
	PyObject*   ToPyString(const std::string& value);
	PyObject*   ToPyFloat (float value);
	PyObject*   ToPyInt   (int value);
	PyObject*   ToPyBool  (bool value);
	PyObject*   ToPyMPair (int x, float y);
	PyObject*   ToPyPair  (float x, float y);
	PyObject*   ToPyPairII(int x, int y);
	PyObject*   ToPyPair  (const std::string& x, const std::string& y);
	PyObject*   ToPyList  (const std::vector<int>& value);
	PyObject*   ToPyList  (const std::vector<float>& value);
	PyObject*   ToPyList  (const std::vector<std::string>& value);
	PyObject*   ToPyList  (const std::vector<std::vector<std::string>>& value);
	PyObject*   ToPyList  (const std::vector<std::pair<int, int>>& value);

	// updates
	void        UpdatePyIntList         (PyObject* pyvalue, const std::vector<int>& value);
	void        UpdatePyFloatList       (PyObject* pyvalue, const std::vector<float>& value);
	void        UpdatePyStringStringList(PyObject* pyvalue, const std::vector<std::vector<std::string>>& value);

	// conversion to c++
	int         ToInt   (PyObject* value, const std::string& message = "Type must be an integer.");
	float       ToFloat (PyObject* value, const std::string& message = "Type must be a float.");
	bool        ToBool  (PyObject* value, const std::string& message = "Type must be a bool.");
	mvColor     ToColor (PyObject* value, const std::string& message = "Type must be a list or tuple of ints.");
	mvVec2      ToVec2  (PyObject* value, const std::string& message = "Type must be a list or tuple of floats.");
	mvVec4      ToVec4  (PyObject* value, const std::string& message = "Type must be a list or tuple of floats.");
	std::string ToString(PyObject* value, const std::string& message = "Type must be a string.");

	std::vector<mvVec2>                              ToVectVec2           (PyObject* value, const std::string& message = "Type must be a list/tuple of list/tuple.");
	std::vector<mvVec4>                              ToVectVec4           (PyObject* value, const std::string& message = "Type must be a list/tuple of list/tuple.");
	std::vector<int>                                 ToIntVect            (PyObject* value, const std::string& message = "Type must be a list or tuple of integers.");
	std::vector<float>                               ToFloatVect          (PyObject* value, const std::string& message = "Type must be a list or tuple of floats.");
	std::vector<std::string>                         ToStringVect         (PyObject* value, const std::string& message = "Type must be a list or tuple of strings.");
	std::vector<std::pair<int, int>>                 ToVectInt2           (PyObject* value, const std::string& message = "Type must be an list/tuple of integer.");
	std::vector<std::pair<std::string, std::string>> ToVectPairString     (PyObject* value, const std::string& message = "Type must be an list/tuple of string pairs.");
	std::vector<std::vector<std::string>>            ToVectVectString     (PyObject* value, const std::string& message = "Type must be an list/tuple of list/tuple of strings.");
	std::vector<std::vector<float>>                  ToVectVectFloat      (PyObject* value, const std::string& message = "Type must be an list/tuple of list/tuple of strings.");
	std::vector<std::pair<std::string, float>>       ToVectPairStringFloat(PyObject* value, const std::string& message = "Type must be an list/tuple of str,float pairs.");
}