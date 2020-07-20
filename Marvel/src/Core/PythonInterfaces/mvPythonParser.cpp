#include "mvPythonParser.h"
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

	const char* PythonDataTypeString(mvPythonDataType type)
	{
		switch (type)
		{
		case mvPythonDataType::String:     return " : str";
		case mvPythonDataType::Integer:    return " : int";
		case mvPythonDataType::Float:      return " : float";
		case mvPythonDataType::Bool:       return " : bool";
		case mvPythonDataType::StringList: return " : List[str]";
		case mvPythonDataType::FloatList:  return " : List[float]";
		case mvPythonDataType::IntList:    return " : List[int]";
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
		const std::string& about, const std::string& returnType, const std::string& category)
		: m_elements(elements), m_about(about), m_return(returnType), m_category(category)
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