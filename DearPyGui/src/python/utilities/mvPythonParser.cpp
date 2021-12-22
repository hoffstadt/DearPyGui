#include "mvPythonParser.h"
#include "mvAppItemCommons.h"
#include "dearpygui.h"
#include "mvContext.h"
#include <utility>
#include <ctime>
#include <frameobject.h>
#include "mvPythonTypeChecker.h"
#include "mvPythonExceptions.h"
#include "mvPythonTranslator.h"

namespace Marvel {

	mv_internal bool 
	VerifyArguments(int start, PyObject* args, const std::vector<mvPythonDataElement>& elements)
	{

		if (start >= PyTuple_Size(args))
			return true;

		int end = (int)PyTuple_Size(args);
		if (end > (int)elements.size())
			end = (int)elements.size();

		for (int i = start; i < end; i++)
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

	mv_internal char
	PythonDataTypeSymbol(mvPyDataType type)
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

	mv_internal const char*
	PythonDataTypeString(mvPyDataType type)
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
		case mvPyDataType::StringList:     return " : Union[List[str], Tuple[str, ...]]";
		case mvPyDataType::FloatList:      return " : Union[List[float], Tuple[float, ...]]";
		case mvPyDataType::DoubleList:     return " : Union[List[float], Tuple[float, ...]]";
		case mvPyDataType::IntList:        return " : Union[List[int], Tuple[int, ...]]";
		case mvPyDataType::UUIDList:       return " : Union[List[int], Tuple[int, ...]]";
		case mvPyDataType::Callable:       return " : Callable";
		case mvPyDataType::Dict:           return " : dict";
		case mvPyDataType::ListAny:        return " : List[Any]";
		case mvPyDataType::ListListInt:    return " : List[Union[List[int], Tuple[int, ...]]]";
		case mvPyDataType::ListFloatList:  return " : List[List[float]]";
		case mvPyDataType::ListDoubleList: return " : List[List[float]]";
		case mvPyDataType::ListStrList:    return " : List[List[str]]";
		case mvPyDataType::Object:         return " : Any";
		default:                              return " : unknown";
		}
	}

	const char* 
	PythonDataTypeActual(mvPyDataType type)
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
		case mvPyDataType::StringList:    return "Union[List[str], Tuple[str, ...]]";
		case mvPyDataType::FloatList:     return "Union[List[float], Tuple[float, ...]]";
		case mvPyDataType::IntList:       return "Union[List[int], Tuple[int, ...]]";
		case mvPyDataType::UUIDList:      return "Union[List[int], Tuple[int, ...]]";
		case mvPyDataType::Callable:      return "Callable";
		case mvPyDataType::Dict:          return "dict";
		case mvPyDataType::ListFloatList: return "List[List[float]]";
		case mvPyDataType::ListStrList:   return "List[List[str]]";
		case mvPyDataType::None:          return "None";
		case mvPyDataType::Object:        return "Any";
		default:                              return "Any";
		}
	}

	mvPythonParser 
	FinalizeParser(const mvPythonParserSetup& setup, const std::vector<mvPythonDataElement>& args)
	{

		mvPythonParser parser;

		// separate args into category
		for (auto& arg : args)
		{
			switch (arg.arg_type)
			{
			case mvArgType::REQUIRED_ARG:
				parser.required_elements.push_back(arg);
				break;
			case mvArgType::POSITIONAL_ARG:
				parser.optional_elements.push_back(arg);
				break;
			case mvArgType::KEYWORD_ARG:
				parser.keyword_elements.push_back(arg);
				break;
			default:
				parser.deprecated_elements.push_back(arg);
			}
		}

		// build format string and keywords
		if (!parser.required_elements.empty())
		{
			for (auto& element : parser.required_elements)
			{
				parser.formatstring.push_back(PythonDataTypeSymbol(element.type));
				parser.keywords.push_back(element.name);
			}
		}

		parser.formatstring.push_back('|');

		if (!parser.optional_elements.empty())
		{

			for (auto& element : parser.optional_elements)
			{
				parser.formatstring.push_back(PythonDataTypeSymbol(element.type));
				parser.keywords.push_back(element.name);
			}
		}

		if (!parser.keyword_elements.empty())
		{
			parser.formatstring.push_back('$');
			for (auto& element : parser.keyword_elements)
			{
				parser.formatstring.push_back(PythonDataTypeSymbol(element.type));
				parser.keywords.push_back(element.name);
			}
		}
		parser.formatstring.push_back(0);
		parser.keywords.push_back(NULL);

		parser.about = setup.about;
		parser.returnType = setup.returnType;
		parser.category = setup.category;
		parser.createContextManager = setup.createContextManager;
		parser.unspecifiedKwargs = setup.unspecifiedKwargs;
		parser.internal = setup.internal;

		// build documentation
		std::string documentation = parser.about + "\n\nReturn Type: " + PythonDataTypeActual(parser.returnType) + "\n";

		if (!parser.required_elements.empty())
			documentation += "\n\nRequired Arguments\n_______________\n\n";

		for (const auto& element : parser.required_elements)
		{
			documentation += "\n* ";
			documentation += element.name + std::string(PythonDataTypeString(element.type));
			documentation += "\n\t\t" + std::string(element.description);
		}

		if (!parser.optional_elements.empty())
			documentation += "\n\nOptional Arguments\n_______________\n\n";

		for (const auto& element : parser.optional_elements)
		{
			documentation += "\n* ";
			documentation += element.name + std::string(PythonDataTypeString(element.type));
			documentation += " = " + std::string(element.default_value);
			documentation += "\n\t\t" + std::string(element.description);
		}

		if (!parser.keyword_elements.empty())
			documentation += "\n\nKeyword Arguments\n_______________\n\n";

		for (const auto& element : parser.keyword_elements)
		{
			documentation += "\n* ";
			documentation += element.name + std::string(PythonDataTypeString(element.type));
			documentation += " = " + std::string(element.default_value);
			documentation += "\n\t\t" + std::string(element.description);
		}

		if (!parser.keyword_elements.empty())
			documentation += "\n\nDeprecated Keyword Arguments\n_______________\n\n";

		for (const auto& element : parser.deprecated_elements)
		{
			documentation += "\n* ";
			documentation += element.name + std::string(PythonDataTypeString(element.type));
			documentation += " = " + std::string(element.default_value);
			documentation += "\n\t\t" + std::string(element.description);
		}

		parser.documentation = std::move(documentation);

		return parser;
	}

	bool 
	VerifyRequiredArguments(const mvPythonParser& parser, PyObject* args)
	{

		// ensure enough args were provided
		if ((size_t)PyTuple_Size(args) < parser.required_elements.size())
		{
			assert(false && "Not enough arguments provided");
			mvThrowPythonError(mvErrorCode::mvNone, "Not enough arguments provided. Expected: " +
				std::to_string(parser.required_elements.size()) + " Recieved: " + std::to_string((size_t)PyTuple_Size(args)));
			return false;
		}

		return VerifyArguments(0, args, parser.required_elements);
	}

	bool 
	VerifyPositionalArguments(const mvPythonParser& parser, PyObject* args)
	{
		return VerifyArguments((int)parser.optional_elements.size(), args, parser.optional_elements);
	}

	bool 
	VerifyKeywordArguments(const mvPythonParser& parser, PyObject* args)
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
			for (const auto& keyword : parser.keyword_elements)
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
				for (const auto& keyword : parser.optional_elements)
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

			for (const auto& keyword : parser.required_elements)
			{
				if (sitem == keyword.name)
				{
					found = true;
					break;
				}
			}

			if (found)
				continue;

			for (const auto& keyword : parser.deprecated_elements)
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

	bool 
	VerifyArgumentCount(const mvPythonParser& parser, PyObject* args)
	{
		if (args == nullptr && parser.required_elements.size() == 0)
			return true;
		if (args == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvNone, "This command has a minimum number of arguments of " + std::to_string(parser.required_elements.size()));
			return false;
		}

		int possibleArgs = (int)parser.required_elements.size() + (int)parser.optional_elements.size();
		int minArgs = (int)parser.required_elements.size();
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

	bool 
	Parse(const mvPythonParser& parser, PyObject* args, PyObject* kwargs, const char* message, ...)
	{

		bool check = true;

		va_list arguments;
		va_start(arguments, message);
		if (!PyArg_VaParseTupleAndKeywords(args, kwargs, parser.formatstring.data(),
			const_cast<char**>(parser.keywords.data()), arguments))
		{
			check = false;
		}

		va_end(arguments);

		if (!check)
			mvThrowPythonError(mvErrorCode::mvNone, "Error parsing Dear PyGui command: " + std::string(message));

		return check;
	}

	void 
	GenerateStubFile(const std::string& directory)
	{
		const auto& commands = GetModuleParsers();

		std::ofstream stub;
		stub.open(directory + "/_dearpygui.pyi");

		stub << "from typing import List, Any, Callable, Union, Tuple\n";
		stub << "from dearpygui._dearpygui import *\n\n";
		stub << "##########################################################\n";
		stub << "# This file is generated automatically by mvPythonParser #\n";
		stub << "##########################################################\n\n";
		stub << "# ~ Dear PyGui Version: " << MV_SANDBOX_VERSION << "\n";

		for (const auto& parser : commands)
		{
			stub << "def " << parser.first << "(";

			bool first_arg = true;
			for (const auto& args : parser.second.required_elements)
			{
				if (first_arg)
					first_arg = false;
				else
					stub << ", ";
				stub << args.name << PythonDataTypeString(args.type);
			}

			for (const auto& args : parser.second.optional_elements)
			{
				if (first_arg)
					first_arg = false;
				else
					stub << ", ";
				stub << args.name << PythonDataTypeString(args.type) << " =''";
			}

			if (!parser.second.keyword_elements.empty())
			{
				if (first_arg)
					first_arg = false;
				else
					stub << ", ";

				stub << "*";
			}

			for (const auto& args : parser.second.keyword_elements)
				stub << ", " << args.name << ": " << PythonDataTypeActual(args.type) << " =''";

			if (parser.second.unspecifiedKwargs)
				stub << ", **kwargs";

			stub << ") -> " << PythonDataTypeActual(parser.second.returnType) << ":";

			stub << "\n\t\"\"\"" << parser.second.about.c_str() << "\"\"\"";

			stub << "\n\t...\n\n";
		}

		auto& constants = GetModuleConstants();

		for (auto& item : constants)
			stub << item.first << "=0\n";

		stub.close();
	}

	void 
	GenerateCoreFile(std::ofstream& stream)
	{
		const auto& commands = GetModuleParsers();

		// current date/time based on current system
		time_t now = time(0);

		// convert now to string form
		char* dt = ctime(&now);

		for (const auto& parser : commands)
		{
			if (parser.second.internal)
				continue;

			stream << "def " << parser.first << "(";

			bool first_arg = true;
			for (const auto& args : parser.second.required_elements)
			{
				if (first_arg)
					first_arg = false;
				else
					stream << ", ";
				stream << args.name << PythonDataTypeString(args.type);
			}

			for (const auto& args : parser.second.optional_elements)
			{
				if (first_arg)
					first_arg = false;
				else
					stream << ", ";
				stream << args.name << PythonDataTypeString(args.type) << " =" << args.default_value;
			}

			if (!parser.second.keyword_elements.empty())
			{
				if (first_arg)
					first_arg = false;
				else
					stream << ", ";

				stream << "*";
			}

			for (const auto& args : parser.second.keyword_elements)
				stream << ", " << args.name << ": " << PythonDataTypeActual(args.type) << " =" << args.default_value;

			if (first_arg)
				stream << "**kwargs) -> ";
			else
				stream << ", **kwargs) -> ";
			stream << PythonDataTypeActual(parser.second.returnType) << ":";

			stream << "\n\t\"\"\"\t " << parser.second.about.c_str();

			stream << "\n\n\tArgs:";
			for (const auto& args : parser.second.required_elements)
			{
				stream << "\n\t\t" << args.name << " (" << PythonDataTypeActual(args.type) << "): " << args.description;
			}

			for (const auto& args : parser.second.optional_elements)
			{
				stream << "\n\t\t" << args.name << " (" << PythonDataTypeActual(args.type) << ", optional): " << args.description;
			}

			for (const auto& args : parser.second.keyword_elements)
			{
				stream << "\n\t\t" << args.name << " (" << PythonDataTypeActual(args.type) << ", optional): " << args.description;
			}

			for (const auto& args : parser.second.deprecated_elements)
			{
				stream << "\n\t\t" << args.name << " (" << PythonDataTypeActual(args.type) << ", optional): (deprecated) " << args.description;
			}

			stream << "\n\tReturns:";
			stream << "\n\t\t" << PythonDataTypeActual(parser.second.returnType);
			stream << "\n\t\"\"\"";


			for (const auto& args : parser.second.deprecated_elements)
			{
				if (args.arg_type == mvArgType::DEPRECATED_REMOVE_KEYWORD_ARG)
				{
					stream << "\n\n\tif '" << args.name << "' in kwargs.keys():";
					stream << "\n\n\t\twarnings.warn('" << args.name << " keyword removed', DeprecationWarning, 2)";
					stream << "\n\n\t\tkwargs.pop('" << args.name << "', None)";
				}

				else if (args.arg_type == mvArgType::DEPRECATED_RENAME_KEYWORD_ARG)
				{
					stream << "\n\n\tif '" << args.name << "' in kwargs.keys():";
					stream << "\n\t\twarnings.warn('" << args.name << " keyword renamed to " << args.new_name << "', DeprecationWarning, 2)";
					stream << "\n\t\t" << args.new_name << "=kwargs['" << args.name << "']";
				}
			}

			stream << "\n\n\treturn internal_dpg." << parser.first << "(";

			first_arg = true;
			for (const auto& args : parser.second.required_elements)
			{
				if (first_arg)
					first_arg = false;
				else
					stream << ", ";
				stream << args.name;
			}

			for (const auto& args : parser.second.optional_elements)
			{
				if (first_arg)
					first_arg = false;
				else
					stream << ", ";
				stream << args.name;
			}

			for (const auto& args : parser.second.keyword_elements)
			{
				if (first_arg)
					first_arg = false;
				else
					stream << ", ";
				stream << args.name << "=" << args.name;
			}

			if (first_arg)
				stream << "**kwargs)\n\n";
			else
				stream << ", **kwargs)\n\n";
		}
	}

	void 
	GenerateContextsFile(std::ofstream& stream)
	{
		const auto& commands = GetModuleParsers();

		// current date/time based on current system
		time_t now = time(0);

		// convert now to string form
		char* dt = ctime(&now);

		for (const auto& parser : commands)
		{
			if (!parser.second.createContextManager)
				continue;

			stream << "\n@contextmanager\n";
			stream << "def " << parser.first.substr(4) << "(";

			bool first_arg = true;
			for (const auto& args : parser.second.required_elements)
			{
				if (first_arg)
					first_arg = false;
				else
					stream << ", ";
				stream << args.name << PythonDataTypeString(args.type);
			}

			for (const auto& args : parser.second.optional_elements)
			{
				if (first_arg)
					first_arg = false;
				else
					stream << ", ";
				stream << args.name << PythonDataTypeString(args.type) << " =" << args.default_value;
			}

			if (!parser.second.keyword_elements.empty())
			{
				if (first_arg)
					first_arg = false;
				else
					stream << ", ";

				stream << "*";
			}

			for (const auto& args : parser.second.keyword_elements)
				stream << ", " << args.name << ": " << PythonDataTypeActual(args.type) << " =" << args.default_value;

			if (first_arg)
				stream << "**kwargs) -> ";
			else
				stream << ", **kwargs) -> ";
			stream << PythonDataTypeActual(parser.second.returnType) << ":";

			stream << "\n\t\"\"\"\t " << parser.second.about.c_str();

			stream << "\n\n\tArgs:";
			for (const auto& args : parser.second.required_elements)
			{
				stream << "\n\t\t" << args.name << " (" << PythonDataTypeActual(args.type) << "): " << args.description;
			}

			for (const auto& args : parser.second.optional_elements)
			{
				stream << "\n\t\t" << args.name << " (" << PythonDataTypeActual(args.type) << ", optional): " << args.description;
			}

			for (const auto& args : parser.second.keyword_elements)
			{
				stream << "\n\t\t" << args.name << " (" << PythonDataTypeActual(args.type) << ", optional): " << args.description;
			}

			for (const auto& args : parser.second.deprecated_elements)
			{
				stream << "\n\t\t" << args.name << " (" << PythonDataTypeActual(args.type) << ", optional): (deprecated) " << args.description;
			}

			stream << "\n\tYields:";
			stream << "\n\t\t" << PythonDataTypeActual(parser.second.returnType);
			stream << "\n\t\"\"\"";

			stream << "\n\ttry:";

			for (const auto& args : parser.second.deprecated_elements)
			{
				if (args.arg_type == mvArgType::DEPRECATED_REMOVE_KEYWORD_ARG)
				{
					stream << "\n\n\t\tif '" << args.name << "' in kwargs.keys():";
					stream << "\n\t\t\twarnings.warn('" << args.name << " keyword removed', DeprecationWarning, 2)";
					stream << "\n\t\t\tkwargs.pop('" << args.name << "', None)";
				}

				else if (args.arg_type == mvArgType::DEPRECATED_RENAME_KEYWORD_ARG)
				{
					stream << "\n\n\t\tif '" << args.name << "' in kwargs.keys():";
					stream << "\n\t\t\twarnings.warn('" << args.name << " keyword renamed to " << args.new_name << "', DeprecationWarning, 2)";
					stream << "\n\t\t\t" << args.new_name << "=kwargs['" << args.name << "']";
				}
			}

			stream << "\n\t\twidget = internal_dpg." << parser.first << "(";

			first_arg = true;
			for (const auto& args : parser.second.required_elements)
			{
				if (first_arg)
					first_arg = false;
				else
					stream << ", ";
				stream << args.name;
			}

			for (const auto& args : parser.second.optional_elements)
			{
				if (first_arg)
					first_arg = false;
				else
					stream << ", ";
				stream << args.name;
			}

			for (const auto& args : parser.second.keyword_elements)
			{
				if (first_arg)
					first_arg = false;
				else
					stream << ", ";
				stream << args.name << "=" << args.name;
			}

			if (first_arg)
				stream << "**kwargs)\n";
			else
				stream << ", **kwargs)\n";

			stream << "\t\tinternal_dpg.push_container_stack(widget)\n";
			stream << "\t\tyield widget\n";
			stream << "\tfinally:\n";
			stream << "\t\tinternal_dpg.pop_container_stack()\n";

		}

	}

	void 
	GenerateCoreFileRTD(std::ofstream& stream)
	{
		const auto& commands = GetModuleParsers();

		// current date/time based on current system
		time_t now = time(0);

		// convert now to string form
		char* dt = ctime(&now);

		for (const auto& parser : commands)
		{
			if (parser.second.internal)
				continue;

			stream << "def " << parser.first << "(";

			bool first_arg = true;
			for (const auto& args : parser.second.required_elements)
			{
				if (first_arg)
					first_arg = false;
				else
					stream << ", ";
				stream << args.name;
			}

			for (const auto& args : parser.second.optional_elements)
			{
				if (first_arg)
					first_arg = false;
				else
					stream << ", ";
				stream << args.name << "=" << args.default_value;
			}

			if (parser.second.keyword_elements.empty())
				stream << "):";
			else
			{
				if(parser.second.required_elements.empty() && parser.second.optional_elements.empty())
					stream << "**kwargs):";
				else
					stream << ", **kwargs):";
			}

			stream << "\n\t\"\"\"\t " << parser.second.about.c_str();

			stream << "\n\n\tArgs:";
			for (const auto& args : parser.second.required_elements)
			{
				stream << "\n\t\t" << args.name << " (" << PythonDataTypeActual(args.type) << "): " << args.description;
			}

			for (const auto& args : parser.second.optional_elements)
			{
				stream << "\n\t\t" << args.name << " (" << PythonDataTypeActual(args.type) << ", optional): " << args.description;
			}

			for (const auto& args : parser.second.keyword_elements)
			{
				stream << "\n\t\t" << args.name << " (" << PythonDataTypeActual(args.type) << ", optional): " << args.description;
			}

			for (const auto& args : parser.second.deprecated_elements)
			{
				stream << "\n\t\t" << args.name << " (" << PythonDataTypeActual(args.type) << ", optional): (deprecated)" << args.description;
			}

			stream << "\n\tReturns:";
			stream << "\n\t\t" << PythonDataTypeActual(parser.second.returnType);
			stream << "\n\t\"\"\"";

			stream << "\n\n\treturn internal_dpg." << parser.first << "(";

			first_arg = true;
			for (const auto& args : parser.second.required_elements)
			{
				if (first_arg)
					first_arg = false;
				else
					stream << ", ";
				stream << args.name;
			}

			for (const auto& args : parser.second.optional_elements)
			{
				if (first_arg)
					first_arg = false;
				else
					stream << ", ";
				stream << args.name;
			}

			if (!parser.second.keyword_elements.empty())
			{
				if (parser.second.required_elements.empty() && parser.second.optional_elements.empty())
					stream << "**kwargs";
				else
					stream << ", **kwargs";
			}

			stream << ")\n\n";
		}
	}

	void 
	GenerateContextsFileRTD(std::ofstream& stream)
	{
		const auto& commands = GetModuleParsers();

		// current date/time based on current system
		time_t now = time(0);

		// convert now to string form
		char* dt = ctime(&now);

		for (const auto& parser : commands)
		{
			if (!parser.second.createContextManager)
				continue;

			stream << "\n@contextmanager\n";
			stream << "def " << parser.first.substr(4) << "(";

			bool first_arg = true;
			for (const auto& args : parser.second.required_elements)
			{
				if (first_arg)
					first_arg = false;
				else
					stream << ", ";
				stream << args.name;
			}

			for (const auto& args : parser.second.optional_elements)
			{
				if (first_arg)
					first_arg = false;
				else
					stream << ", ";
				stream << args.name << "=" << args.default_value;
			}

			if (parser.second.keyword_elements.empty())
				stream << "):";
			else
			{
				if (parser.second.required_elements.empty() && parser.second.optional_elements.empty())
					stream << "**kwargs):";
				else
					stream << ", **kwargs):";
			}

			stream << "\n\t\"\"\"\t " << parser.second.about.c_str();

			stream << "\n\n\tArgs:";
			for (const auto& args : parser.second.required_elements)
			{
				stream << "\n\t\t" << args.name << " (" << PythonDataTypeActual(args.type) << "): " << args.description;
			}

			for (const auto& args : parser.second.optional_elements)
			{
				stream << "\n\t\t" << args.name << " (" << PythonDataTypeActual(args.type) << ", optional): " << args.description;
			}

			for (const auto& args : parser.second.keyword_elements)
			{
				stream << "\n\t\t" << args.name << " (" << PythonDataTypeActual(args.type) << ", optional): " << args.description;
			}

			for (const auto& args : parser.second.deprecated_elements)
			{
				stream << "\n\t\t" << args.name << " (" << PythonDataTypeActual(args.type) << ", optional): (deprecated)" << args.description;
			}

			stream << "\n\tYields:";
			stream << "\n\t\t" << PythonDataTypeActual(parser.second.returnType);
			stream << "\n\t\"\"\"";

			stream << "\n\ttry:";
			stream << "\n\t\twidget = internal_dpg." << parser.first << "(";

			first_arg = true;
			for (const auto& args : parser.second.required_elements)
			{
				if (first_arg)
					first_arg = false;
				else
					stream << ", ";
				stream << args.name;
			}

			for (const auto& args : parser.second.optional_elements)
			{
				if (first_arg)
					first_arg = false;
				else
					stream << ", ";
				stream << args.name;
			}

			if (!parser.second.keyword_elements.empty())
			{
				if (parser.second.required_elements.empty() && parser.second.optional_elements.empty())
					stream << "**kwargs";
				else
					stream << ", **kwargs";
			}

			stream << ")\n";
			stream << "\t\tinternal_dpg.push_container_stack(widget)\n";
			stream << "\t\tyield widget\n";
			stream << "\tfinally:\n";
			stream << "\t\tinternal_dpg.pop_container_stack()\n";

		}

	}

	void 
	GenerateDearPyGuiFile(const std::string& directory)
	{
		std::ofstream stub;
		stub.open(directory + "/dearpygui.py");

		stub << "\n##########################################################\n";
		stub << "# Dear PyGui User Interface\n";
		stub << "#   ~ Version: " << MV_SANDBOX_VERSION << "\n";
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

		std::ifstream inputStream0(directory + "/_header.py");

		for (std::string line; std::getline(inputStream0, line);)
			stub << line << "\n";

		stub << "\n##########################################################\n";
		stub << "# Deprecated Commands\n";
		stub << "##########################################################\n";

		std::ifstream inputStream1(directory + "/_deprecated.py");

		for (std::string line; std::getline(inputStream1, line);)
			stub << line << "\n";

		stub << "\n##########################################################\n";
		stub << "# Container Context Managers\n";
		stub << "##########################################################\n\n";

		GenerateContextsFile(stub);

		stub << "\n##########################################################\n";
		stub << "# Core Wrappings\n";
		stub << "##########################################################\n\n";

		GenerateCoreFile(stub);

		stub << "\n##########################################################\n";
		stub << "# Constants #\n";
		stub << "##########################################################\n\n";

		auto& constants = GetModuleConstants();

		for (auto& item : constants)
			stub << item.first << "=internal_dpg." << item.first << "\n";

		stub.close();

		std::ofstream redirect;
		redirect.open(directory + "/_dearpygui.py");
		redirect << "from _dearpygui import *\n";
		redirect.close();
	}

	void 
	GenerateDearPyGuiFileRTD(const std::string& directory)
	{
		std::ofstream stub;
		stub.open(directory + "/_dearpygui_RTD.py");

		stub << "\n##########################################################\n";
		stub << "# Dear PyGui User Interface (MODIFIED FOR READTHEDOCS)\n";
		stub << "#   ~ Version: " << MV_SANDBOX_VERSION << "\n";
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

		std::ifstream inputStream0(directory + "/_header.py");

		for (std::string line; std::getline(inputStream0, line);)
			stub << line << "\n";

		stub << "\n##########################################################\n";
		stub << "# Deprecated Commands\n";
		stub << "##########################################################\n";

		std::ifstream inputStream1(directory + "/_deprecated.py");

		for (std::string line; std::getline(inputStream1, line);)
			stub << line << "\n";

		stub << "\n##########################################################\n";
		stub << "# Container Context Managers\n";
		stub << "##########################################################\n\n";

		GenerateContextsFileRTD(stub);

		stub << "\n##########################################################\n";
		stub << "# Core Wrappings\n";
		stub << "##########################################################\n\n";

		GenerateCoreFileRTD(stub);

		stub << "\n##########################################################\n";
		stub << "# Constants #\n";
		stub << "##########################################################\n\n";

		auto& constants = GetModuleConstants();

		for (auto& item : constants)
			stub << item.first << "=internal_dpg." << item.first << "\n";

		stub.close();

		std::ofstream redirect;
		redirect.open(directory + "/_dearpygui.py");
		redirect << "from _dearpygui import *\n";
		redirect.close();
	}

	void 
	AddCommonArgs(std::vector<mvPythonDataElement>& args, CommonParserArgs argsFlags)
	{

		args.push_back({ mvPyDataType::UUID, "id", mvArgType::DEPRECATED_RENAME_KEYWORD_ARG, "0", "", "tag" });
		args.push_back({ mvPyDataType::String, "label", mvArgType::KEYWORD_ARG, "None", "Overrides 'name' as label." });
		args.push_back({ mvPyDataType::Object, "user_data", mvArgType::KEYWORD_ARG, "None", "User data for callbacks" });
		args.push_back({ mvPyDataType::Bool, "use_internal_label", mvArgType::KEYWORD_ARG, "True", "Use generated internal label instead of user specified (appends ### uuid)." });

		if (argsFlags & MV_PARSER_ARG_ID)           args.push_back({ mvPyDataType::UUID, "tag", mvArgType::KEYWORD_ARG, "0", "Unique id used to programmatically refer to the item.If label is unused this will be the label." });
		if (argsFlags & MV_PARSER_ARG_WIDTH)        args.push_back({ mvPyDataType::Integer, "width", mvArgType::KEYWORD_ARG, "0", "Width of the item." });
		if (argsFlags & MV_PARSER_ARG_HEIGHT)       args.push_back({ mvPyDataType::Integer, "height", mvArgType::KEYWORD_ARG, "0", "Height of the item." });
		if (argsFlags & MV_PARSER_ARG_INDENT)       args.push_back({ mvPyDataType::Integer, "indent", mvArgType::KEYWORD_ARG, "-1", "Offsets the widget to the right the specified number multiplied by the indent style." });
		if (argsFlags & MV_PARSER_ARG_PARENT)       args.push_back({ mvPyDataType::UUID, "parent", mvArgType::KEYWORD_ARG, "0", "Parent to add this item to. (runtime adding)" });
		if (argsFlags & MV_PARSER_ARG_BEFORE)       args.push_back({ mvPyDataType::UUID, "before", mvArgType::KEYWORD_ARG, "0", "This item will be displayed before the specified item in the parent." });
		if (argsFlags & MV_PARSER_ARG_SOURCE)       args.push_back({ mvPyDataType::UUID, "source", mvArgType::KEYWORD_ARG, "0", "Overrides 'id' as value storage key." });
		if (argsFlags & MV_PARSER_ARG_PAYLOAD_TYPE) args.push_back({ mvPyDataType::String, "payload_type", mvArgType::KEYWORD_ARG, "'$$DPG_PAYLOAD'", "Sender string type must be the same as the target for the target to run the payload_callback." });
		if (argsFlags & MV_PARSER_ARG_CALLBACK)     args.push_back({ mvPyDataType::Callable, "callback", mvArgType::KEYWORD_ARG, "None", "Registers a callback." });
		if (argsFlags & MV_PARSER_ARG_DRAG_CALLBACK)args.push_back({ mvPyDataType::Callable, "drag_callback", mvArgType::KEYWORD_ARG, "None", "Registers a drag callback for drag and drop." });
		if (argsFlags & MV_PARSER_ARG_DROP_CALLBACK)args.push_back({ mvPyDataType::Callable, "drop_callback", mvArgType::KEYWORD_ARG, "None", "Registers a drop callback for drag and drop." });
		if (argsFlags & MV_PARSER_ARG_SHOW)         args.push_back({ mvPyDataType::Bool, "show", mvArgType::KEYWORD_ARG, "True", "Attempt to render widget." });
		if (argsFlags & MV_PARSER_ARG_ENABLED)      args.push_back({ mvPyDataType::Bool, "enabled", mvArgType::KEYWORD_ARG, "True", "Turns off functionality of widget and applies the disabled theme." });
		if (argsFlags & MV_PARSER_ARG_POS)		    args.push_back({ mvPyDataType::IntList, "pos", mvArgType::KEYWORD_ARG, "[]", "Places the item relative to window coordinates, [0,0] is top left." });
		if (argsFlags & MV_PARSER_ARG_FILTER)		args.push_back({ mvPyDataType::String, "filter_key", mvArgType::KEYWORD_ARG, "''", "Used by filter widget." });
		if (argsFlags & MV_PARSER_ARG_SEARCH_DELAY) args.push_back({ mvPyDataType::Bool, "delay_search", mvArgType::KEYWORD_ARG, "False", "Delays searching container for specified items until the end of the app. Possible optimization when a container has many children that are not accessed often." });

		if (argsFlags & MV_PARSER_ARG_TRACKED)
		{
			args.push_back({ mvPyDataType::Bool, "tracked", mvArgType::KEYWORD_ARG, "False", "Scroll tracking" });
			args.push_back({ mvPyDataType::Float, "track_offset", mvArgType::KEYWORD_ARG, "0.5", "0.0f:top, 0.5f:center, 1.0f:bottom" });
		}

	}
}
