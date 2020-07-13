#pragma once

#include <string>
#include <vector>
#include <map>
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "Core/mvCore.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// Enums and Helper Functions
	//-----------------------------------------------------------------------------
	enum class mvPythonDataType
	{
		String, Integer, Float, Bool, StringList, FloatList, Optional,
		Object, IntList, KeywordOnly
	};

	//-----------------------------------------------------------------------------
	// mvPythonDataElement
	//-----------------------------------------------------------------------------
	struct mvPythonDataElement
	{
		const char*      name;
		mvPythonDataType type;
		std::string      description;

		mvPythonDataElement(mvPythonDataType type, const char* name = "", const std::string& description = "");
		const char getSymbol() const;
	};

	//-----------------------------------------------------------------------------
	// mvPythonParser
	//-----------------------------------------------------------------------------
	class mvPythonParser
	{

	public:

		mvPythonParser() = default;

		mvPythonParser(const std::initializer_list<mvPythonDataElement>& elements, 
			const std::string& about = "", const std::string& returnType = "None");

		bool               parse(PyObject* args, PyObject* kwargs, const char* message, ...);
		inline const char* getDocumentation() const { return m_documentation.c_str(); }

		void buildDocumentation();

	private:

		std::vector<mvPythonDataElement> m_elements;
		std::vector<char>                m_formatstring;
		std::vector<const char*>         m_keywords;
		bool                             m_optional = false; // check if optional has been found already
		bool                             m_keyword = false; // check if keyword has been found already
		std::string                      m_about;
		std::string                      m_return;
		std::string                      m_documentation;


	};
}