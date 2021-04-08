#include "mvPythonParser.h"
#include "mvModule_Core.h"
#include "mvApp.h"
#include "mvAppLog.h"
#include <fstream>
#include <utility>
#include <ctime>

namespace Marvel {

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
		case mvPyDataType::Optional:      return "Optional Arguments\n____________________";
		case mvPyDataType::KeywordOnly:   return "Keyword Only Arguments\n____________________";
		case mvPyDataType::Callable:      return " : Callable";
		case mvPyDataType::Dict:          return " : dict";
		case mvPyDataType::ListFloatList: return " : List[List[float]]";
		case mvPyDataType::ListStrList:   return " : List[List[str]]";
		case mvPyDataType::Kwargs:        return "";
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
		case mvPyDataType::Kwargs:        return "";
		case mvPyDataType::Object:        return "Any";
		default:                              return "";
		}
	}

	mvPythonParser::mvPythonParser(mvPyDataType returnType, std::string about, std::string category)
		: m_about(std::move(about)), m_return(std::move(returnType)), m_category(std::move(category))
	{

	}

	void mvPythonParser::removeArg(std::string name)
	{
		for (auto& arg : m_staged_elements)
		{
			if (arg.name == name)
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
				case mvArgType::REQUIRED:
					m_positional_elements.push_back(arg);
					break;
				case mvArgType::OPTIONAL:
					m_optional_elements.push_back(arg);
					break;
				case mvArgType::KEYWORD:
					m_keyword_elements.push_back(arg);
					break;
				}
			}
		}
		m_staged_elements.clear();
	}

	//bool mvPythonParser::parse(PyObject* args, PyObject* kwargs, const char* message, ...)
	//{

	//	//bool check = true;

	//	//va_list arguments;
	//	//va_start(arguments, message);
	//	//if (!PyArg_VaParseTupleAndKeywords(args, kwargs, m_formatstring.data(),
	//	//	const_cast<char**>(m_keywords.data()), arguments))
	//	//{
	//	//	PyErr_Print();
	//	//	check = false;
	//	//	mvAppLog::Show();
	//	//	int line = PyFrame_GetLineNumber(PyEval_GetFrame());
	//	//	PyObject* ex = PyErr_Format(PyExc_Exception,
	//	//		"Error parsing DearPyGui %s command on line %d.", message, line);
	//	//	PyErr_Print();
	//	//	Py_XDECREF(ex);
	//	//}

	//	//va_end(arguments);

	//	//return check;
	//}

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
