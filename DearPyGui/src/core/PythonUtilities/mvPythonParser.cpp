#include "mvPythonParser.h"
#include "mvModule_Core.h"
#include "mvApp.h"
#include "mvAppLog.h"
#include <fstream>
#include <utility>
#include <ctime>
#include <frameobject.h>
#include "mvPythonTypeChecker.h"

namespace Marvel {

	static bool VerifyArguments(size_t start, PyObject* args, const std::vector<mvPythonDataElement>& elements)
	{

		if (start >= PyTuple_Size(args))
			return true;

		for(size_t i = start; i< PyTuple_Size(args); i++)
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
	}

	static char PythonDataTypeSymbol(mvPyDataType type)
	{
		switch (type)
		{
		case mvPyDataType::String:     return 's';
		case mvPyDataType::Integer:    return 'i';
		case mvPyDataType::Float:      return 'f';
		case mvPyDataType::Double:     return 'd';
		case mvPyDataType::Bool:       return 'p';
		default:                       return 'O';
		}
	}

	static const char* PythonDataTypeString(mvPyDataType type)
	{
		switch (type)
		{
		case mvPyDataType::String:        return " : str";
		case mvPyDataType::Integer:       return " : int";
		case mvPyDataType::Float:         return " : float";
		case mvPyDataType::Double:        return " : float";
		case mvPyDataType::Bool:          return " : bool";
		case mvPyDataType::StringList:    return " : List[str]";
		case mvPyDataType::FloatList:     return " : List[float]";
		case mvPyDataType::IntList:       return " : List[int]";
		case mvPyDataType::Callable:      return " : Callable";
		case mvPyDataType::Dict:          return " : dict";
		case mvPyDataType::ListFloatList: return " : List[List[float]]";
		case mvPyDataType::ListStrList:   return " : List[List[str]]";
		case mvPyDataType::Object:        return " : Any";
		default:                              return " : unknown";
		}
	}

	const char* PythonDataTypeActual(mvPyDataType type)
	{
		switch (type)
		{
		case mvPyDataType::String:        return "str";
		case mvPyDataType::Integer:       return "int";
		case mvPyDataType::Float:         return "float";
		case mvPyDataType::Double:        return "float";
		case mvPyDataType::Bool:          return "bool";
		case mvPyDataType::StringList:    return "List[str]";
		case mvPyDataType::FloatList:     return "List[float]";
		case mvPyDataType::IntList:       return "List[int]";
		case mvPyDataType::Callable:      return "Callable";
		case mvPyDataType::Dict:          return "dict";
		case mvPyDataType::ListFloatList: return "List[List[float]]";
		case mvPyDataType::ListStrList:   return "List[List[str]]";
		case mvPyDataType::Object:        return "Any";
		default:                              return "";
		}
	}

	mvPythonParser::mvPythonParser(mvPyDataType returnType, const char* about, const char* category)
		: m_about(about), m_return(returnType), m_category(category)
	{

	}

	void mvPythonParser::removeArg(const char* name)
	{
		for (auto& arg : m_staged_elements)
		{
			if (strcmp(arg.name, name) != 0)
			{
				arg.active = false;
				return;
			}
		}
		assert(false);
	}

	void mvPythonParser::finalize()
	{
		for (auto& arg : m_staged_elements)
		{
			if (arg.active)
			{
				switch (arg.arg_type)
				{
				case mvArgType::REQUIRED_ARG:
					m_required_elements.push_back(arg);
					break;
				case mvArgType::POSITIONAL_ARG:
					m_optional_elements.push_back(arg);
					break;
				case mvArgType::KEYWORD_ARG:
					m_keyword_elements.push_back(arg);
					break;
				}
			}
		}
		m_staged_elements.clear();

		// build format string and keywords
		if (!m_required_elements.empty())
		{
			for (auto& element : m_required_elements)
			{
				m_formatstring.push_back(PythonDataTypeSymbol(element.type));
				m_keywords.push_back(element.name);
			}
		}

		m_formatstring.push_back('|');

		if (!m_optional_elements.empty())
		{
			
			for (auto& element : m_optional_elements)
			{
				m_formatstring.push_back(PythonDataTypeSymbol(element.type));
				m_keywords.push_back(element.name);
			}
		}

		if (!m_keyword_elements.empty())
		{
			m_formatstring.push_back('$');
			for (auto& element : m_keyword_elements)
			{
				m_formatstring.push_back(PythonDataTypeSymbol(element.type));
				m_keywords.push_back(element.name);
			}
		}
		m_formatstring.push_back(0);
		m_keywords.push_back(NULL);
	}

	bool mvPythonParser::verifyRequiredArguments(PyObject* args)
	{
		// ensure enough args were provided
		if (PyTuple_Size(args) < m_required_elements.size())
		{
			assert(false && "Not enough arguments provided");
			return false;
		}

		return VerifyArguments(0, args, m_required_elements);
	}

	bool mvPythonParser::verifyPositionalArguments(PyObject* args)
	{
		return VerifyArguments(m_required_elements.size(), args, m_optional_elements);
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
			check = false;
			mvAppLog::Show();
			int line = PyFrame_GetLineNumber(PyEval_GetFrame());
			PyObject* ex = PyErr_Format(PyExc_Exception,
				"Error parsing DearPyGui %s command on line %d.", message, line);
			PyErr_Print();
			Py_XDECREF(ex);
		}

		va_end(arguments);

		return check;
	}

	//void mvPythonParser::buildDocumentation()
	//{
	//	//std::string documentation = m_about + "\n\nReturn Type: " + m_return + "\n";

	//	//if (!m_elements.empty())
	//	//	documentation = documentation + "\n\nParameters\n__________\n\n";

	//	//bool opfound = false;
	//	//bool keyfound = false;

	//	//for (auto& element : m_elements)
	//	//{
	//	//	if (element.type == mvPythonDataType::Optional)
	//	//	{
	//	//		if (opfound)
	//	//			continue;
	//	//		opfound = true;
	//	//	}

	//	//	if (element.type == mvPythonDataType::KeywordOnly)
	//	//	{
	//	//		if (keyfound)
	//	//			continue;
	//	//		keyfound = true;
	//	//	}

	//	//	if(element.type != mvPythonDataType::KeywordOnly && element.type != mvPythonDataType::Optional && (opfound || keyfound))
	//	//		documentation = documentation + "* " + element.name + PythonDataTypeString(element.type) + " = " + element.default_value +
	//	//			"\n\t" + element.description + "\n\n";
	//	//	else
	//	//		documentation = documentation + "* " + element.name + PythonDataTypeString(element.type) + 
	//	//		"\n\t" + element.description + "\n\n";
	//	//}

	//	//m_documentation = std::move(documentation);
	//}

}
