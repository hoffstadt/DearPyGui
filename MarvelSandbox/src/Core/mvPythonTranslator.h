#pragma once

#include <string>
#include <vector>
#include <map>
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "mvCore.h"

//-----------------------------------------------------------------------------
// Helper Macros
//-----------------------------------------------------------------------------
#define MV_STANDARD_CALLBACK_INIT() const char* callback = "", *tip="", *popup=""; int width=0;
#define MV_STANDARD_CALLBACK_PARSE &callback, &tip, &popup, &width
#define MV_STANDARD_CALLBACK_EVAL() mvApp::GetApp()->setItemCallback(name, callback);\
mvApp::GetApp()->setItemTip(name, tip);\
mvApp::GetApp()->setItemWidth(name, width);\
mvApp::GetApp()->setItemPopup(name, popup);

namespace Marvel {

	//-----------------------------------------------------------------------------
	// Enums and Helper Functions
	//-----------------------------------------------------------------------------
	enum class mvPythonDataType
	{
		String, Integer, Float, Bool, StringList, FloatList, Optional,
		Object, IntList
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
		case mvPythonDataType::IntList:    return 'O';
		case mvPythonDataType::Optional:   return '|';
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
		case mvPythonDataType::Optional:    return "Optional Arguments\n____________________";
		case mvPythonDataType::Object:     return " : object";
		default:                           return " : unknown";
		}
	}

	//-----------------------------------------------------------------------------
	// mvPythonDataElement
	//-----------------------------------------------------------------------------
	struct mvPythonDataElement
	{
		const char*      name;
		mvPythonDataType type;
		bool             keywordOnly;
		std::string      description;

		mvPythonDataElement(mvPythonDataType type, const char* name, bool keywordOnly = false, const std::string& description = "")
			: name(name), type(type), keywordOnly(keywordOnly), description(description)
		{}

		const char getSymbol() const { return PythonDataTypeSymbol(type);}
	};

	//-----------------------------------------------------------------------------
	// mvPythonTranslator
	//-----------------------------------------------------------------------------
	class mvPythonTranslator
	{

	public:

		mvPythonTranslator() = default;

		mvPythonTranslator(const std::initializer_list<mvPythonDataElement>& elements, bool standardKeywords = false, 
			const std::string& about = "");

		bool                             parse(PyObject* args, PyObject* kwargs, const char* message, ...);
		std::vector<std::string>         getStringVec(PyObject* obj);
		std::vector<float>               getFloatVec(PyObject* obj);
		mvVec2                           getVec2(PyObject* obj);
		mvColor                          getColor(PyObject* obj);
		std::vector<mvVec2>              getVectVec2(PyObject* obj);
		std::vector<mvVec2>              getVectVec2L(PyObject* obj); // TODO combine this and the previous one
		std::vector<std::pair<int, int>> getVectInt2(PyObject* obj);
		inline const std::string&        getDocumentation() const { return m_documentation; }

		void buildDocumentation();

	private:

		std::vector<mvPythonDataElement> m_elements;
		std::vector<char>                m_formatstring;
		std::vector<const char*>         m_keywords;
		bool                             m_optional = false; // check if optional has been found already
		std::string                      m_about;
		std::string                      m_documentation;


	};
}