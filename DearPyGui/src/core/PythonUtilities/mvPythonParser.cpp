#include "mvPythonParser.h"
#include "mvModule_Core.h"
#include "mvApp.h"
#include <fstream>
#include <utility>
#include <ctime>
#include <frameobject.h>
#include "mvPythonTypeChecker.h"
#include "mvPythonExceptions.h"
#include "mvPythonTranslator.h"

namespace Marvel {

	static bool VerifyArguments(int start, PyObject* args, const std::vector<mvPythonDataElement>& elements)
	{

		if (start >= PyTuple_Size(args))
			return true;

		int end = (int)PyTuple_Size(args);
		if (end > (int)elements.size())
			end = (int)elements.size();

		for(int i = start; i < end; i++)
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

		return true;
	}

	static char PythonDataTypeSymbol(mvPyDataType type)
	{
		switch (type)
		{
		case mvPyDataType::UUID:       return 'K';
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
		case mvPyDataType::String:         return " : str";
		case mvPyDataType::UUID:           return " : int";
		case mvPyDataType::Integer:        return " : int";
		case mvPyDataType::Float:          return " : float";
		case mvPyDataType::Double:         return " : float";
		case mvPyDataType::Bool:           return " : bool";
		case mvPyDataType::StringList:     return " : List[str]";
		case mvPyDataType::FloatList:      return " : List[float]";
		case mvPyDataType::DoubleList:     return " : List[float]";
		case mvPyDataType::IntList:        return " : List[int]";
		case mvPyDataType::Callable:       return " : Callable";
		case mvPyDataType::Dict:           return " : dict";
		case mvPyDataType::ListAny:        return " : List[Any]";
		case mvPyDataType::ListFloatList:  return " : List[List[float]]";
		case mvPyDataType::ListDoubleList: return " : List[List[float]]";
		case mvPyDataType::ListStrList:    return " : List[List[str]]";
		case mvPyDataType::Object:         return " : Any";
		default:                              return " : unknown";
		}
	}

	const char* PythonDataTypeActual(mvPyDataType type)
	{
		switch (type)
		{
		case mvPyDataType::String:        return "str";
		case mvPyDataType::UUID:          return "int";
		case mvPyDataType::Integer:       return "int";
		case mvPyDataType::Float:         return "float";
		case mvPyDataType::Double:        return "float";
		case mvPyDataType::Bool:          return "bool";
		case mvPyDataType::StringList:    return "List[str]";
		case mvPyDataType::FloatList:     return "List[float]";
		case mvPyDataType::IntList:       return "List[int]";
		case mvPyDataType::UUIDList:       return "List[int]";
		case mvPyDataType::Callable:      return "Callable";
		case mvPyDataType::Dict:          return "dict";
		case mvPyDataType::ListFloatList: return "List[List[float]]";
		case mvPyDataType::ListStrList:   return "List[List[str]]";
		case mvPyDataType::None:          return "None";
		case mvPyDataType::Object:        return "Any";
		default:                              return "Any";
		}
	}

	mvPythonParser::mvPythonParser(mvPyDataType returnType, const char* about, const std::vector<std::string>& category)
		: m_about(about), m_return(returnType), m_category(category)
	{

	}

	void mvPythonParser::finalize()
	{
		for (auto& arg : m_staged_elements)
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

		buildDocumentation();
	}

	bool mvPythonParser::verifyRequiredArguments(PyObject* args)
	{
		// ensure enough args were provided
		if ((size_t)PyTuple_Size(args) < m_required_elements.size())
		{
			assert(false && "Not enough arguments provided");
			return false;
		}

		return VerifyArguments(0, args, m_required_elements);
	}

	bool mvPythonParser::verifyPositionalArguments(PyObject* args)
	{
		return VerifyArguments((int)m_optional_elements.size(), args, m_optional_elements);
	}

	bool mvPythonParser::verifyKeywordArguments(PyObject* args)
	{
		if (args == nullptr)
			return true;

		if (!PyArg_ValidateKeywordArguments(args))
			return false;

		PyObject* keys = PyDict_Keys(args);

		bool exists = false;
		for (int i = 0; i < PyList_Size(keys); i++)
		{
			PyObject* item = PyList_GetItem(keys, i);
			auto sitem = ToString(item);

			bool found = false;
			for (const auto& keyword : m_keyword_elements)
			{
				if (sitem == keyword.name)
				{
					found = true;
					break;
				}
			}

			if (found)
				continue;
			else
			{
				for (const auto& keyword : m_optional_elements)
				{
					if (sitem == keyword.name)
					{
						found = true;
						break;
					}
				}

				if (found)
					continue;
			}

			mvThrowPythonError(1000, sitem + " keyword does not exist.");
			assert(false);
			exists = false;
			break;
		}

		Py_XDECREF(keys);

		return exists;
	}

	bool mvPythonParser::verifyArgumentCount(PyObject* args)
	{
		if (args == nullptr && m_required_elements.size() == 0)
			return true;
		if (args == nullptr)
		{
			mvThrowPythonError(1000, "This command has a minimum number of arguments of " + std::to_string(m_required_elements.size()));
			return false;
		}

		int possibleArgs = (int)m_required_elements.size() + (int)m_optional_elements.size();
		int minArgs = (int)m_required_elements.size();
		int numberOfArgs = (int)PyTuple_Size(args);

		if (numberOfArgs > possibleArgs)
		{
			mvThrowPythonError(1000, "This command has a maximum number of arguments of " + std::to_string(possibleArgs) +
				" but recieved " + std::to_string(numberOfArgs));
			return false;
		}
		if (numberOfArgs < minArgs)
		{
			mvThrowPythonError(1000, "This command has a minimum number of arguments of " + std::to_string(minArgs) +
				" but only recieved " + std::to_string(numberOfArgs));
			return false;
		}
		return true;
	}

	bool mvPythonParser::parse(PyObject* args, PyObject* kwargs, const char* message, ...)
	{

		bool check = true;

		va_list arguments;
		va_start(arguments, message);
		if (!PyArg_VaParseTupleAndKeywords(args, kwargs, m_formatstring.data(),
			const_cast<char**>(m_keywords.data()), arguments))
		{
			check = false;
		}

		va_end(arguments);

		if (!check)
			mvThrowPythonError(1000, "Error parsing Dear PyGui command: " + std::string(message));

		return check;
	}

	void mvPythonParser::GenerateStubFile(const std::string& file)
	{
		const auto& commands = mvModule_Core::GetModuleParsers();

		// current date/time based on current system
		time_t now = time(0);

		// convert now to string form
		char* dt = ctime(&now);

		std::ofstream stub;
		stub.open(file);

		stub << "from typing import List, Any, Callable\n";
		stub << "from dearpygui.core import *\n\n";
		stub << "##########################################################\n";
		stub << "# This file is generated automatically by mvPythonParser #\n";
		stub << "##########################################################\n\n";
		stub << "# ~ Dear PyGui Version: " << mvApp::GetVersion() << "\n";

		for (const auto& parser : commands)
		{
			stub << "def " << parser.first << "(";

			bool first_arg = true;
			for (const auto& args : parser.second.m_required_elements)
			{
				if (first_arg)
					first_arg = false;
				else
					stub << ", ";
				stub << args.name << PythonDataTypeString(args.type);
			}

			for (const auto& args : parser.second.m_optional_elements)
			{
				if (first_arg)
					first_arg = false;
				else
					stub << ", ";
				stub << args.name << PythonDataTypeString(args.type) << " =" << args.default_value;
			}

			if (!parser.second.m_keyword_elements.empty())
			{
				if (first_arg)
					first_arg = false;
				else
					stub << ", ";

				stub << "*";
			}

			for (const auto& args : parser.second.m_keyword_elements)
				stub << ", " << args.name << ": " << PythonDataTypeActual(args.type) << " =" << args.default_value;

			if (parser.second.m_unspecifiedKwargs)
				stub << ", **kwargs";

			stub << ") -> " << PythonDataTypeActual(parser.second.m_return) << ":";

			stub << "\n\t\"\"\""<<parser.second.m_about.c_str() << "\"\"\"";

			stub << "\n\t...\n\n";
		}

		stub.close();
	}

	void mvPythonParser::buildDocumentation()
	{
		std::string documentation = m_about + "\n\nReturn Type: " + PythonDataTypeActual(m_return) + "\n";

		if (!m_required_elements.empty())
			documentation += "\n\nRequired Arguments\n_______________\n\n";

		for (const auto& element : m_required_elements)
		{
			documentation += "\n* ";
			documentation += element.name + std::string(PythonDataTypeString(element.type));
			documentation += "\n\t\t" + std::string(element.description);
		}

		if (!m_optional_elements.empty())
			documentation += "\n\nOptional Arguments\n_______________\n\n";

		for (const auto& element : m_optional_elements)
		{
			documentation += "\n* ";
			documentation += element.name + std::string(PythonDataTypeString(element.type));
			documentation += " = " + std::string(element.default_value);
			documentation += "\n\t\t" + std::string(element.description);
		}

		if (!m_keyword_elements.empty())
			documentation += "\n\nKeyword Arguments\n_______________\n\n";

		for (const auto& element : m_keyword_elements)
		{
			documentation += "\n* ";
			documentation += element.name + std::string(PythonDataTypeString(element.type));
			documentation += " = " + std::string(element.default_value);
			documentation += "\n\t\t" + std::string(element.description);
		}

		m_documentation = std::move(documentation);
	}

}
