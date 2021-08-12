#include "mvPythonParser.h"
#include "mvAppItemCommons.h"
#include "mvModule_DearPyGui.h"
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

			case mvPyDataType::UUID:
			{
				if (isPyObject_Int(obj))
					break;
				else if (isPyObject_String(obj))
					break;
				return false;
			}

			case mvPyDataType::Long:
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
		//case mvPyDataType::UUID:    return 'K';
		case mvPyDataType::Long:    return 'l';
		case mvPyDataType::String:  return 's';
		case mvPyDataType::Integer: return 'i';
		case mvPyDataType::Float:   return 'f';
		case mvPyDataType::Double:  return 'd';
		case mvPyDataType::Bool:    return 'p';
		default:                    return 'O';
		}
	}

	static const char* PythonDataTypeString(mvPyDataType type)
	{
		switch (type)
		{
		case mvPyDataType::String:         return " : str";
		case mvPyDataType::UUID:           return " : Union[int, str]";
		case mvPyDataType::Integer:        return " : int";
		case mvPyDataType::Long:           return " : int";
		case mvPyDataType::Float:          return " : float";
		case mvPyDataType::Double:         return " : float";
		case mvPyDataType::Bool:           return " : bool";
		case mvPyDataType::StringList:     return " : Union[List[str], Tuple[str]]";
		case mvPyDataType::FloatList:      return " : Union[List[float], Tuple[float]]";
		case mvPyDataType::DoubleList:     return " : Union[List[float], Tuple[float]]";
		case mvPyDataType::IntList:        return " : Union[List[int], Tuple[int]]";
		case mvPyDataType::UUIDList:       return " : Union[List[int], Tuple[int]]";
		case mvPyDataType::Callable:       return " : Callable";
		case mvPyDataType::Dict:           return " : dict";
		case mvPyDataType::ListAny:        return " : List[Any]";
		case mvPyDataType::ListListInt:    return " : List[List[int]]";
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
		case mvPyDataType::UUID:          return "Union[int, str]";
		case mvPyDataType::Integer:       return "int";
		case mvPyDataType::Long:          return "int";
		case mvPyDataType::Float:         return "float";
		case mvPyDataType::Double:        return "float";
		case mvPyDataType::Bool:          return "bool";
		case mvPyDataType::StringList:    return "Union[List[str], Tuple[str]]";
		case mvPyDataType::FloatList:     return "Union[List[float], Tuple[float]]";
		case mvPyDataType::IntList:       return "Union[List[int], Tuple[int]]";
		case mvPyDataType::UUIDList:      return "Union[List[int], Tuple[int]]";
		case mvPyDataType::Callable:      return "Callable";
		case mvPyDataType::Dict:          return "dict";
		case mvPyDataType::ListFloatList: return "List[List[float]]";
		case mvPyDataType::ListStrList:   return "List[List[str]]";
		case mvPyDataType::None:          return "None";
		case mvPyDataType::Object:        return "Any";
		default:                              return "Any";
		}
	}

	mvPythonParser::mvPythonParser(mvPyDataType returnType, const char* about, const std::vector<std::string>& category, bool createContextManager)
		: m_about(about), m_return(returnType), m_category(category), m_createContextManager(createContextManager)
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
			mvThrowPythonError(mvErrorCode::mvNone, "Not enough arguments provided. Expected: " + 
				std::to_string(m_required_elements.size()) + " Recieved: " + std::to_string((size_t)PyTuple_Size(args)));
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
			return false;

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

			for (const auto& keyword : m_required_elements)
			{
				if (sitem == keyword.name)
				{
					found = true;
					break;
				}
			}

			if (found)
				continue;

			mvThrowPythonError(mvErrorCode::mvNone, sitem + " keyword does not exist.");
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
			mvThrowPythonError(mvErrorCode::mvNone, "This command has a minimum number of arguments of " + std::to_string(m_required_elements.size()));
			return false;
		}

		int possibleArgs = (int)m_required_elements.size() + (int)m_optional_elements.size();
		int minArgs = (int)m_required_elements.size();
		int numberOfArgs = (int)PyTuple_Size(args);

		if (numberOfArgs > possibleArgs)
		{
			mvThrowPythonError(mvErrorCode::mvNone, "This command has a maximum number of arguments of " + std::to_string(possibleArgs) +
				" but recieved " + std::to_string(numberOfArgs));
			return false;
		}
		if (numberOfArgs < minArgs)
		{
			mvThrowPythonError(mvErrorCode::mvNone, "This command has a minimum number of arguments of " + std::to_string(minArgs) +
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
			mvThrowPythonError(mvErrorCode::mvNone, "Error parsing Dear PyGui command: " + std::string(message));

		return check;
	}

	void mvPythonParser::GenerateStubFile(const std::string& file)
	{
		const auto& commands = mvModule_DearPyGui::GetModuleParsers();

		std::ofstream stub;
		stub.open(file + "/_dearpygui.pyi");

		stub << "from typing import List, Any, Callable, Union, Tuple\n";
		stub << "from dearpygui._dearpygui import *\n\n";
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

	void mvPythonParser::GenerateCoreFile(const std::string& file)
	{
		const auto& commands = mvModule_DearPyGui::GetModuleParsers();

		// current date/time based on current system
		time_t now = time(0);

		// convert now to string form
		char* dt = ctime(&now);

		std::ofstream stub;
		stub.open(file + "/_core.py");

		for (const auto& parser : commands)
		{
			if (parser.second.m_internal)
				continue;

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

			stub << "\n\t\"\"\"\n\t" << parser.second.m_about.c_str();

			stub << "\n\tArgs:";
			for (const auto& args : parser.second.m_required_elements)
			{
				stub << "\n\t\t" << args.name << " (" << PythonDataTypeActual(args.type) << "): " << args.description;
			}

			for (const auto& args : parser.second.m_optional_elements)
			{
				stub << "\n\t\t*" << args.name << " (" << PythonDataTypeActual(args.type) << "): " << args.description;
			}

			for (const auto& args : parser.second.m_keyword_elements)
			{
				stub << "\n\t\t**" << args.name << " (" << PythonDataTypeActual(args.type) << "): " << args.description;
			}

			stub << "\n\tReturns:";
			stub << "\n\t\t"<< PythonDataTypeActual(parser.second.m_return);
			stub << "\n\t\"\"\"";

			stub << "\n\n\treturn internal_dpg." << parser.first << "(";

			first_arg = true;
			for (const auto& args : parser.second.m_required_elements)
			{
				if (first_arg)
					first_arg = false;
				else
					stub << ", ";
				stub << args.name;
			}

			for (const auto& args : parser.second.m_optional_elements)
			{
				if (first_arg)
					first_arg = false;
				else
					stub << ", ";
				stub << args.name;
			}

			for (const auto& args : parser.second.m_keyword_elements)
			{
				if (first_arg)
					first_arg = false;
				else
					stub << ", ";
				stub << args.name << "=" << args.name;
			}

			stub << ")\n\n";
		}

		stub.close();
	}

	void mvPythonParser::GenerateContextsFile(const std::string& file)
	{
		const auto& commands = mvModule_DearPyGui::GetModuleParsers();

		// current date/time based on current system
		time_t now = time(0);

		// convert now to string form
		char* dt = ctime(&now);

		std::ofstream stub;
		stub.open(file + "/_contexts.py");

		for (const auto& parser : commands)
		{
			if (!parser.second.m_createContextManager)
				continue;

			stub << "@contextmanager\n";
			stub << "def " << parser.first.substr(4) << "(";

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

			stub << "\n\t\"\"\"\n\t" << parser.second.m_about.c_str();

			stub << "\n\tArgs:";
			for (const auto& args : parser.second.m_required_elements)
			{
				stub << "\n\t\t" << args.name << " (" << PythonDataTypeActual(args.type) << "): " << args.description;
			}

			for (const auto& args : parser.second.m_optional_elements)
			{
				stub << "\n\t\t*" << args.name << " (" << PythonDataTypeActual(args.type) << "): " << args.description;
			}

			for (const auto& args : parser.second.m_keyword_elements)
			{
				stub << "\n\t\t**" << args.name << " (" << PythonDataTypeActual(args.type) << "): " << args.description;
			}

			stub << "\n\tYields:";
			stub << "\n\t\t" << PythonDataTypeActual(parser.second.m_return);
			stub << "\n\t\"\"\"";

			stub << "\n\ttry:";
			stub << "\n\t\twidget = internal_dpg." << parser.first << "(";

			first_arg = true;
			for (const auto& args : parser.second.m_required_elements)
			{
				if (first_arg)
					first_arg = false;
				else
					stub << ", ";
				stub << args.name;
			}

			for (const auto& args : parser.second.m_optional_elements)
			{
				if (first_arg)
					first_arg = false;
				else
					stub << ", ";
				stub << args.name;
			}

			for (const auto& args : parser.second.m_keyword_elements)
			{
				if (first_arg)
					first_arg = false;
				else
					stub << ", ";
				stub << args.name << "=" << args.name;
			}

			stub << ")\n";
			stub << "\t\tinternal_dpg.push_container_stack(widget)\n";
			stub << "\t\tyield widget\n";
			stub << "\tfinally:\n";
			stub << "\t\tinternal_dpg.pop_container_stack()\n";

		}

		stub.close();
	}

	void mvPythonParser::GenerateDearPyGuiFile(const std::string& file)
	{
		GenerateStubFile("../../DearPyGui/dearpygui");
		GenerateCoreFile("../../DearPyGui/dearpygui");
		GenerateContextsFile("../../DearPyGui/dearpygui");

		std::ofstream stub;
		stub.open(file + "/dearpygui.py");

		stub << "\n##########################################################\n";
		stub << "# Dear PyGui User Interface\n";
		stub << "#   ~ Version: " << mvApp::GetVersion() << "\n";
		stub << "#\n";
		stub << "#   Notes:\n";
		stub << "#     * This file is automatically generated.\n#\n";
		stub << "#   Resources:\n";
		stub << "#     * FAQ:         https://github.com/hoffstadt/DearPyGui/discussions/categories/frequently-asked-questions-faq \n";
		stub << "#     * Homepage:    https://github.com/hoffstadt/DearPyGui \n";
		stub << "#     * Wiki:        https://github.com/hoffstadt/DearPyGui/wiki \n";
		stub << "#     * Issues:      https://github.com/hoffstadt/DearPyGui/issues\n";
		stub << "#     * Discussions: https://github.com/hoffstadt/DearPyGui/discussions\n";
		stub << "##########################################################\n\n";


		std::ifstream inputStream1(file + "/_header.py");

		for (std::string line; std::getline(inputStream1, line);)
			stub << line << "\n";

		stub << "\n##########################################################\n";
		stub << "# Container Context Managers\n";
		stub << "##########################################################\n\n";

		std::ifstream inputStream2(file + "/_contexts.py");

		for (std::string line; std::getline(inputStream2, line);)
			stub << line << "\n";

		stub << "\n##########################################################\n";
		stub << "# Core Wrappings\n";
		stub << "##########################################################\n\n";

		std::ifstream inputStream3(file + "/_core.py");

		for (std::string line; std::getline(inputStream3, line);)
			stub << line << "\n";

		stub << "\n##########################################################\n";
		stub << "# Constants #\n";
		stub << "##########################################################\n\n";

		auto& constants = mvModule_DearPyGui::GetSubModuleConstants();

		for(auto& item : constants)
			stub << item.first << "=internal_dpg." << item.first << "\n";

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
