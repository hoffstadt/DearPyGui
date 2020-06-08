#include "mvPythonTranslator.h"
#include "Core/mvApp.h"

namespace Marvel {

	bool mvPythonTranslator::parse(const char* message, ...)
	{

		bool check = true;

		va_list arguments;
		va_start(arguments, message);
		if (!PyArg_VaParseTupleAndKeywords(m_args, m_kwargs, m_formatstring.data(),
			const_cast<char**>(m_keywords.data()), arguments))
		{
			mvApp::GetApp()->LogError(message);
			PyErr_Print();
			check = false;
		}

		va_end(arguments);

		return check;
	}

	std::vector<std::string> mvPythonTranslator::getStringVec(PyObject* obj)
	{
		std::vector<std::string> items;

		for (int i = 0; i < PyTuple_Size(obj); i++)
			items.emplace_back(PyUnicode_AsUTF8(PyTuple_GetItem(obj, i)));

		return items;
	}

	std::vector<float> mvPythonTranslator::getFloatVec(PyObject* obj)
	{
		std::vector<float> items;

		for (int i = 0; i < PyTuple_Size(obj); i++)
			items.emplace_back(PyFloat_AsDouble(PyTuple_GetItem(obj, i)));

		return items;
	}

}