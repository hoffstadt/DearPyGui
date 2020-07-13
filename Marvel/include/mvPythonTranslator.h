#pragma once

#include <string>
#include <vector>
#include <map>
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "Core/mvCore.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvPythonTranslator
	//-----------------------------------------------------------------------------
	class mvPythonTranslator
	{

	public:

		static std::vector<std::string>                         getStringVec(PyObject* obj);
		static std::vector<float>                               getFloatVec(PyObject* obj);
		static std::vector<int>                                 getIntVec(PyObject* obj);
		static mvVec2                                           getVec2(PyObject* obj);
		static mvColor                                          getColor(PyObject* obj);
		static std::vector<mvVec2>                              getVectVec2(PyObject* obj);
		static std::vector<std::pair<int, int>>                 getVectInt2(PyObject* obj);
		static std::vector<std::pair<std::string, std::string>> getStringPairVec(PyObject* obj);


	private:

		mvPythonTranslator() = default;

	};
}