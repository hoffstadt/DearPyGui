#pragma once

#include <string>
#include <vector>
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "mvCore.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// Enums and Helper Functions
	//-----------------------------------------------------------------------------
	enum class mvPythonDataType
	{
		String, Integer, Float, Bool, StringList, FloatList, Optional,
		Object
	};

	static const char PythonDataTypeSymbol(mvPythonDataType type)
	{
		switch (type)
		{
		case mvPythonDataType::String:     return 's';
		case mvPythonDataType::Integer:    return 'i';
		case mvPythonDataType::Float:      return 'f';
		case mvPythonDataType::Bool:       return 'i';
		case mvPythonDataType::StringList: return 'O';
		case mvPythonDataType::FloatList:  return 'O';
		case mvPythonDataType::Optional:  return '|';
		case mvPythonDataType::Object:  return 'O';
		default: return 'O';
		}
	}

	//-----------------------------------------------------------------------------
	// mvPythonDataElement
	//-----------------------------------------------------------------------------
	struct mvPythonDataElement
	{
		const char* name;
		mvPythonDataType type;
		bool keywordOnly;

		mvPythonDataElement(mvPythonDataType type, const char* name, bool keywordOnly = false)
			: name(name), type(type), keywordOnly(keywordOnly)
		{}

		const char getSymbol() const
		{
			return PythonDataTypeSymbol(type);
		}
	};

	//-----------------------------------------------------------------------------
	// mvPythonTranslator
	//-----------------------------------------------------------------------------
	class mvPythonTranslator
	{

	public:

		mvPythonTranslator() = default;

		mvPythonTranslator(PyObject* args, PyObject* kwargs, const std::initializer_list<mvPythonDataElement>& elements)
			: m_args(args), m_kwargs(kwargs), m_elements(elements)
		{
			for (const auto& element : m_elements)
			{
				if(element.type != mvPythonDataType::Optional)
					m_keywords.push_back(element.name);

				if (element.keywordOnly)
					m_formatstring.push_back('$');
				m_formatstring.push_back(element.getSymbol());
			}
			m_formatstring.push_back(0);
			m_keywords.push_back(NULL);
		}

		bool parse(const char* message, ...);

		std::vector<std::string> getStringVec(PyObject* obj);
		std::vector<float>       getFloatVec(PyObject* obj);
		mvVec2                   getVec2(PyObject* obj);
		mvColor                  getColor(PyObject* obj);
		std::vector<mvVec2>      getVectVec2(PyObject* obj);

	private:

		std::vector<mvPythonDataElement> m_elements;
		std::vector<char>                m_formatstring;
		std::vector<const char*>         m_keywords;
		PyObject*                        m_args;
		PyObject*                        m_kwargs;


	};
}