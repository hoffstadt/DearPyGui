#include "mvPythonTranslator.h"
#include "Core/mvApp.h"
#include "Core/mvLogger.h"

namespace Marvel {

	mvPythonTranslator::mvPythonTranslator(const std::initializer_list<mvPythonDataElement>& elements, 
		bool standardKeywords, const std::string& about)
		: m_elements(elements), m_about(about)
	{

		if (standardKeywords)
		{
			m_elements.push_back(mvPythonDataElement(mvPythonDataType::Optional));
			m_elements.push_back(mvPythonDataElement(mvPythonDataType::KeywordOnly));
			m_elements.push_back(mvPythonDataElement(mvPythonDataType::String, "callback", "Registers a callback if supported"));
			m_elements.push_back(mvPythonDataElement(mvPythonDataType::String, "tip", "Adds a simple tooltip"));
			m_elements.push_back(mvPythonDataElement(mvPythonDataType::String, "popup", "Registers a popup if supported"));
			m_elements.push_back(mvPythonDataElement(mvPythonDataType::Integer, "width", "Sets the width of the widget"));
			m_elements.push_back(mvPythonDataElement(mvPythonDataType::Integer, "height", "Sets the height of the widget if supported"));
		}

		for (const auto& element : m_elements)
		{
			// ignore name types for optional and keyword only
			if (element.type != mvPythonDataType::Optional && element.type != mvPythonDataType::KeywordOnly)
				m_keywords.push_back(element.name);

			// ignore additional optionals
			if (m_optional && element.type == mvPythonDataType::Optional)
				continue;
			else if (element.type == mvPythonDataType::Optional)
				m_optional = true;

			// ignore additional keywords
			if (m_keyword && element.type == mvPythonDataType::KeywordOnly)
				continue;
			else if (element.type == mvPythonDataType::KeywordOnly)
				m_keyword = true;

			m_formatstring.push_back(element.getSymbol());
		}

		m_formatstring.push_back(0);
		m_keywords.push_back(NULL);

		buildDocumentation();
	}

	bool mvPythonTranslator::parse(PyObject* args, PyObject* kwargs, const char* message, ...)
	{

		bool check = true;

		va_list arguments;
		va_start(arguments, message);
		if (!PyArg_VaParseTupleAndKeywords(args, kwargs, m_formatstring.data(),
			const_cast<char**>(m_keywords.data()), arguments))
		{
			AppLog::getLogger()->LogError(message);
			PyErr_Print();
			check = false;
		}

		va_end(arguments);

		return check;
	}

	std::vector<std::string> mvPythonTranslator::getStringVec(PyObject* obj)
	{
		std::vector<std::string> items;

		if (PyTuple_Check(obj))
		{
			for (int i = 0; i < PyTuple_Size(obj); i++)
				items.emplace_back(PyUnicode_AsUTF8(PyTuple_GetItem(obj, i)));
		}
		else if (PyList_Check(obj))
		{
			for (int i = 0; i < PyList_Size(obj); i++)
				items.emplace_back(PyUnicode_AsUTF8(PyList_GetItem(obj, i)));
		}

		return items;
	}

	std::vector<float> mvPythonTranslator::getFloatVec(PyObject* obj)
	{
		std::vector<float> items;

		for (int i = 0; i < PyTuple_Size(obj); i++)
			items.emplace_back(PyFloat_AsDouble(PyTuple_GetItem(obj, i)));

		return items;
	}

	std::vector<int> mvPythonTranslator::getIntVec(PyObject* obj)
	{
		std::vector<int> items;

		for (int i = 0; i < PyTuple_Size(obj); i++)
			items.emplace_back(PyLong_AsLong(PyTuple_GetItem(obj, i)));

		return items;
	}

	mvVec2 mvPythonTranslator::getVec2(PyObject* obj)
	{
		std::vector<float> items;

		float x, y;

		if (PyTuple_Check(obj))
		{
			x = PyFloat_AsDouble(PyTuple_GetItem(obj, 0));
			y = PyFloat_AsDouble(PyTuple_GetItem(obj, 1));
		}
		else if (PyList_Check(obj))
		{
			x = PyFloat_AsDouble(PyList_GetItem(obj, 0));
			y = PyFloat_AsDouble(PyList_GetItem(obj, 1));
		}

		return mvVec2{ x, y };
	}

	mvColor mvPythonTranslator::getColor(PyObject* obj)
	{

		int color[4] = { 255, 255, 255, 255 };

		if(obj == nullptr)
			return mvColor{ color[0], color[1], color[2], color[3], false };

		if (PyTuple_Check(obj))
		{
			for (int i = 0; i < PyTuple_Size(obj); i++)
				color[i] = PyLong_AsLong(PyTuple_GetItem(obj, i));
		}
		else if (PyList_Check(obj))
		{
			for (int i = 0; i < PyList_Size(obj); i++)
				color[i] = PyLong_AsLong(PyList_GetItem(obj, i));
		}

		return mvColor{ color[0], color[1], color[2], color[3], true };
	}

	std::vector<mvVec2> mvPythonTranslator::getVectVec2(PyObject* obj)
	{
		std::vector<mvVec2> items;



		if (PyTuple_Check(obj))
		{
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
		}
		else if (PyList_Check(obj))
		{
			for (int i = 0; i < PyList_Size(obj); i++)
			{
				PyObject* point = PyList_GetItem(obj, i);
				for (int j = 0; j < PyList_Size(point); j++)
				{
					float x = PyFloat_AsDouble(PyList_GetItem(point, 0));
					float y = PyFloat_AsDouble(PyList_GetItem(point, 1));
					items.push_back({ x, y });
				}
			}
		}

		return items;
	}

	std::vector<std::pair<int, int>> mvPythonTranslator::getVectInt2(PyObject* obj)
	{
		std::vector<std::pair<int, int>> items;

		for (int i = 0; i < PyTuple_Size(obj); i++)
		{
			PyObject* point = PyTuple_GetItem(obj, i);
			for (int j = 0; j < PyTuple_Size(point); j++)
			{
				int x = PyLong_AsLong(PyTuple_GetItem(point, 0));
				int y = PyLong_AsLong(PyTuple_GetItem(point, 1));
				items.emplace_back( x, y);
			}
		}

		return items;

	}

	void mvPythonTranslator::buildDocumentation()
	{
		std::string documentation = m_about + "\n\nParameters\n__________\n\n";

		bool opfound = false;
		bool keyfound = false;

		for (auto& element : m_elements)
		{
			if (element.type == mvPythonDataType::Optional)
			{
				if (opfound)
					continue;
				opfound = true;
			}

			if (element.type == mvPythonDataType::KeywordOnly)
			{
				if (keyfound)
					continue;
				keyfound = true;
			}

			documentation = documentation + "* " + element.name + PythonDataTypeString(element.type) + 
				"\n\t" + element.description + "\n\n";
		}

		m_documentation = documentation;
	}

}