#include "mvPythonParser.h"
#include "mvMarvel.h"
#include "mvApp.h"
#include "mvPythonTranslator.h"
#include "mvAppLog.h"
#include <fstream>
#include <utility>
#include <frameobject.h>
#include <ctime>

namespace Marvel {

	static char PythonDataTypeSymbol(mvPythonDataType type)
	{
		switch (type)
		{
		case mvPythonDataType::String:     return 's';
		case mvPythonDataType::Integer:    return 'i';
		case mvPythonDataType::Float:      return 'f';
		case mvPythonDataType::Double:     return 'd';
		case mvPythonDataType::Bool:       return 'p';
		case mvPythonDataType::Optional:   return '|';
		case mvPythonDataType::KeywordOnly:return '$';
		default:                           return 'O';
		}
	}

	static const char* PythonDataTypeString(mvPythonDataType type)
	{
		switch (type)
		{
		case mvPythonDataType::String:        return " : str";
		case mvPythonDataType::Integer:       return " : int";
		case mvPythonDataType::Float:         return " : float";
		case mvPythonDataType::Double:        return " : float";
		case mvPythonDataType::Bool:          return " : bool";
		case mvPythonDataType::StringList:    return " : List[str]";
		case mvPythonDataType::FloatList:     return " : List[float]";
		case mvPythonDataType::IntList:       return " : List[int]";
		case mvPythonDataType::Optional:      return "Optional Arguments\n____________________";
		case mvPythonDataType::KeywordOnly:   return "Keyword Only Arguments\n____________________";
		case mvPythonDataType::Callable:      return " : Callable";
		case mvPythonDataType::Dict:          return " : dict";
		case mvPythonDataType::ListFloatList: return " : List[List[float]]";
		case mvPythonDataType::ListStrList:   return " : List[List[str]]";
		case mvPythonDataType::Kwargs:        return "";
		case mvPythonDataType::Object:        return " : Any";
		default:                              return " : unknown";
		}
	}

	const char* PythonDataTypeActual(mvPythonDataType type)
	{
		switch (type)
		{
		case mvPythonDataType::String:        return "str";
		case mvPythonDataType::Integer:       return "int";
		case mvPythonDataType::Float:         return "float";
		case mvPythonDataType::Double:        return "float";
		case mvPythonDataType::Bool:          return "bool";
		case mvPythonDataType::StringList:    return "List[str]";
		case mvPythonDataType::FloatList:     return "List[float]";
		case mvPythonDataType::IntList:       return "List[int]";
		case mvPythonDataType::Callable:      return "Callable";
		case mvPythonDataType::Dict:          return "dict";
		case mvPythonDataType::ListFloatList: return "List[List[float]]";
		case mvPythonDataType::ListStrList:   return "List[List[str]]";
		case mvPythonDataType::Kwargs:        return "";
		case mvPythonDataType::Object:        return "Any";
		default:                              return "";
		}
	}

	char mvPythonDataElement::getSymbol() const
	{ 
		return PythonDataTypeSymbol(type); 
	}

	mvPythonParser::mvPythonParser(const std::initializer_list<mvPythonDataElement>& elements, 
		std::string  about, std::string  returnType, std::string  category)
		: m_elements(elements), m_about(std::move(about)), m_return(std::move(returnType)), m_category(std::move(category))
	{

		for (const auto& element : m_elements)
		{
			// ignore name types for optional and keyword only
			if (element.type != mvPythonDataType::Optional && element.type != mvPythonDataType::KeywordOnly)
				m_keywords.push_back(element.name.c_str());

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
			check = false;
		}

		va_end(arguments);

		if (!check)
		{
			mvAppLog::Show();
			ThrowPythonException("Error parsing Dear PyGui command: " + std::string(message));
		}

		return check;
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

			if(element.type != mvPythonDataType::KeywordOnly && element.type != mvPythonDataType::Optional && (opfound || keyfound))
				documentation = documentation + "* " + element.name + PythonDataTypeString(element.type) + " = " + element.default_value +
					"\n\t" + element.description + "\n\n";
			else
				documentation = documentation + "* " + element.name + PythonDataTypeString(element.type) + 
				"\n\t" + element.description + "\n\n";
		}

		m_documentation = std::move(documentation);
	}

	void GenerateStubFile(const std::string& file)
	{
		auto commands = BuildDearPyGuiInterface();

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
		stub << "# ~ Dear PyGui Version: " << mvApp::GetVersion() <<"\n";

		for (const auto& parser : *commands)
		{
			stub << "def " << parser.first << "(";

			auto elements = parser.second.getElements();

			bool adddefault = false;

			for (size_t i = 0; i < elements.size(); i++)
			{
				if (elements[i].type == mvPythonDataType::KeywordOnly || elements[i].type == mvPythonDataType::Optional)
				{
					adddefault = true;
					if (elements[i].type == mvPythonDataType::KeywordOnly)
						stub << "*, ";
					continue;
				}

				if (i != elements.size() - 1)
				{
					if(adddefault)
						stub << elements[i].name << ": " << PythonDataTypeActual(elements[i].type) << " = " << elements[i].default_value << ", ";
					
					else
						stub << elements[i].name << ": " << PythonDataTypeActual(elements[i].type) << ", ";
				}
				else
				{
					if (elements[i].type == mvPythonDataType::Kwargs)
						stub << elements[i].name << ") -> " <<
						parser.second.getReturnType() << ":\n\t\"\"\"" << parser.second.getAbout() << "\"\"\"\n\t...\n\n";

					else if (adddefault)
						stub << elements[i].name << ": " << PythonDataTypeActual(elements[i].type) << " = " << elements[i].default_value << ") -> " <<
						parser.second.getReturnType() << ":\n\t\"\"\"" << parser.second.getAbout() << "\"\"\"\n\t...\n\n";
					else
						stub << elements[i].name << ": " << PythonDataTypeActual(elements[i].type) <<
						") -> " << parser.second.getReturnType() << ":\n\t\"\"\"" << parser.second.getAbout() << "\"\"\"\n\t...\n\n";
				}
			}

			if (elements.empty())
				stub << ") -> " << parser.second.getReturnType() << ":\n\t\"\"\"" << parser.second.getAbout() << "\"\"\"\n\t...\n\n";



		}

		stub.close();
	}
}
