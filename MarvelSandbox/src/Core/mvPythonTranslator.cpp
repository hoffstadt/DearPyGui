#include "mvPythonTranslator.h"
#include <iostream>

namespace Marvel {

	void mvPythonTranslator::parse(int num, ...)
	{

		va_list arguments;
		va_start(arguments, num);
		if (!PyArg_VaParseTupleAndKeywords(m_args, m_kwargs, m_formatstring.data(),
			const_cast<char**>(m_keywords.data()), arguments))
		{
			PyErr_Print();
			//__debugbreak();
		}
		va_end(arguments);
	}

	std::vector<std::string> mvPythonTranslator::getStringVec(PyObject* obj)
	{
		std::vector<std::string> sitems;

		for (int i = 0; i < PyTuple_Size(obj); i++)
			sitems.emplace_back(PyUnicode_AsUTF8(PyTuple_GetItem(obj, i)));

		return sitems;
	}

	std::vector<float> mvPythonTranslator::getFloatVec(PyObject* obj)
	{
		std::vector<float> items;

		for (int i = 0; i < PyTuple_Size(obj); i++)
			items.emplace_back(PyFloat_AsDouble(PyTuple_GetItem(obj, i)));

		return items;
	}

}