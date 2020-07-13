#include "mvPythonTranslator.h"
#include "Core/mvApp.h"
#include "Core/StandardWindows/mvAppLog.h"

namespace Marvel {

	static const char PythonDataTypeSymbol(mvPythonDataType type)
	{
		switch (type)
		{
		case mvPythonDataType::String:     return 's';
		case mvPythonDataType::Integer:    return 'i';
		case mvPythonDataType::Float:      return 'f';
		case mvPythonDataType::Bool:       return 'p';
		case mvPythonDataType::StringList: return 'O';
		case mvPythonDataType::FloatList:  return 'O';
		case mvPythonDataType::IntList:    return 'O';
		case mvPythonDataType::Optional:   return '|';
		case mvPythonDataType::KeywordOnly:return '$';
		case mvPythonDataType::Object:     return 'O';
		default:                           return 'O';
		}
	}

	static const char* PythonDataTypeString(mvPythonDataType type)
	{
		switch (type)
		{
		case mvPythonDataType::String:     return " : str";
		case mvPythonDataType::Integer:    return " : int";
		case mvPythonDataType::Float:      return " : float";
		case mvPythonDataType::Bool:       return " : bool";
		case mvPythonDataType::StringList: return " : List(str)";
		case mvPythonDataType::FloatList:  return " : List(float)";
		case mvPythonDataType::IntList:    return " : List(int)";
		case mvPythonDataType::Optional:   return "Optional Arguments\n____________________";
		case mvPythonDataType::KeywordOnly:return "Keyword Only Arguments\n____________________";
		case mvPythonDataType::Object:     return " : object";
		default:                           return " : unknown";
		}
	}


	mvPythonDataElement::mvPythonDataElement(mvPythonDataType type, const char* name, const std::string& description)
		: name(name), type(type), description(description)
	{}

	const char mvPythonDataElement::getSymbol() const 
	{ 
		return PythonDataTypeSymbol(type); 
	}

	mvPythonParser::mvPythonParser(const std::initializer_list<mvPythonDataElement>& elements, 
		const std::string& about, const std::string& returnType)
		: m_elements(elements), m_about(about), m_return(returnType)
	{

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

			if (element.type == mvPythonDataType::KeywordOnly && !m_optional)
			{
				m_formatstring.push_back('|');
			}

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

	bool mvPythonParser::parse(PyObject* args, PyObject* kwargs, const char* message, ...)
	{

		bool check = true;

		va_list arguments;
		va_start(arguments, message);
		if (!PyArg_VaParseTupleAndKeywords(args, kwargs, m_formatstring.data(),
			const_cast<char**>(m_keywords.data()), arguments))
		{
			PyErr_Print();
			PyErr_SetString(PyExc_Exception, message);
			PyErr_Print();
			check = false;
			mvApp::GetApp()->showStandardWindow("logger");
		}

		va_end(arguments);

		return check;
	}

	std::vector<std::string> mvPythonParser::getStringVec(PyObject* obj)
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

	std::vector<std::pair<std::string, std::string>> mvPythonParser::getStringPairVec(PyObject* obj)
	{
		std::vector<std::pair<std::string, std::string>> items;

		if (PyTuple_Check(obj))
		{
			for (int i = 0; i < PyTuple_Size(obj); i++)
			{
				PyObject* item = PyTuple_GetItem(obj, i);
				if (PyTuple_Size(item) == 2)
					items.emplace_back(PyUnicode_AsUTF8(PyTuple_GetItem(item, 0)),PyUnicode_AsUTF8(PyTuple_GetItem(item, 1)));

			}
				
		}
		else if (PyList_Check(obj))
		{
			for (int i = 0; i < PyList_Size(obj); i++)
			{
				PyObject* item = PyList_GetItem(obj, i);
				if (PyList_Size(item) == 2)
					items.emplace_back(PyUnicode_AsUTF8(PyList_GetItem(item, 0)), PyUnicode_AsUTF8(PyList_GetItem(item, 1)));

			}
		}

		return items;
	}

	std::vector<float> mvPythonParser::getFloatVec(PyObject* obj)
	{
		std::vector<float> items;

		if (PyTuple_Check(obj))
		{
			for (int i = 0; i < PyTuple_Size(obj); i++)
				items.emplace_back(PyFloat_AsDouble(PyTuple_GetItem(obj, i)));
		}

		else if (PyList_Check(obj))
		{
			for (int i = 0; i < PyList_Size(obj); i++)
				items.emplace_back(PyFloat_AsDouble(PyList_GetItem(obj, i)));
		}

		return items;
	}

	std::vector<int> mvPythonParser::getIntVec(PyObject* obj)
	{
		std::vector<int> items;

		if (PyTuple_Check(obj))
		{
			for (int i = 0; i < PyTuple_Size(obj); i++)
				items.emplace_back(PyLong_AsLong(PyTuple_GetItem(obj, i)));
		}

		else if (PyList_Check(obj))
		{
			for (int i = 0; i < PyList_Size(obj); i++)
				items.emplace_back(PyLong_AsLong(PyList_GetItem(obj, i)));
		}

		return items;
	}

	mvVec2 mvPythonParser::getVec2(PyObject* obj)
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

	mvColor mvPythonParser::getColor(PyObject* obj)
	{

		int color[4] = { 255, 255, 255, 255 };

		if(obj == nullptr)
			return mvColor{ color[0], color[1], color[2], color[3], false };

		if (PyTuple_Check(obj))
		{
			for (int i = 0; i < PyTuple_Size(obj); i++)
				color[i] = (int)PyFloat_AsDouble(PyTuple_GetItem(obj, i));
		}
		else if (PyList_Check(obj))
		{
			for (int i = 0; i < PyList_Size(obj); i++)
				color[i] = (int)PyFloat_AsDouble(PyList_GetItem(obj, i));
		}

		return mvColor{ color[0], color[1], color[2], color[3], true };
	}

	std::vector<mvVec2> mvPythonParser::getVectVec2(PyObject* obj)
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

	std::vector<std::pair<int, int>> mvPythonParser::getVectInt2(PyObject* obj)
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

	void mvPythonParser::buildDocumentation()
	{
		std::string documentation = m_about + "\n\nReturn Type: " + m_return + "\n";

		if (!m_elements.empty())
			documentation = documentation + "\n\nParameters\n__________\n\n";

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