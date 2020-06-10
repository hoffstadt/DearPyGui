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

	mvVec2 mvPythonTranslator::getVec2(PyObject* obj)
	{
		std::vector<float> items;


		float x = PyFloat_AsDouble(PyTuple_GetItem(obj, 0));
		float y = PyFloat_AsDouble(PyTuple_GetItem(obj, 1));

		return mvVec2{ x, y };
	}

	mvColor mvPythonTranslator::getColor(PyObject* obj)
	{

		float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

		if(obj == nullptr)
			return mvColor{ color[0], color[1], color[2], color[3], false };

		for (int i = 0; i < PyTuple_Size(obj); i++)
			color[i] = PyFloat_AsDouble(PyTuple_GetItem(obj, i));

		return mvColor{ color[0], color[1], color[2], color[3], true };
	}

	std::vector<mvVec2> mvPythonTranslator::getVectVec2(PyObject* obj)
	{
		std::vector<mvVec2> items;

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

		return items;
	}

}