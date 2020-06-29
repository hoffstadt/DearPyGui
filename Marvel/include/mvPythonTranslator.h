#pragma once

#include <string>
#include <vector>
#include <map>
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "Core/mvCore.h"

//-----------------------------------------------------------------------------
// Helper Macros
//-----------------------------------------------------------------------------
#define MV_STANDARD_CALLBACK_INIT() const char* callback = "", *tip="", *popup=""; int width=0; int height=0; const char* after=""; const char* parent="";
#define MV_STANDARD_CALLBACK_PARSE &callback, &tip, &popup, &width, &height, &after, &parent
#define MV_STANDARD_CALLBACK_EVAL() item->setCallback(callback);\
item->setTip(tip);\
item->setWidth(width);\
item->setHeight(height);\
item->setPopup(popup);\
auto ma = mvApp::GetApp();\
if(!std::string(parent).empty() || !std::string(after).empty() && ma->isStarted())ma->addRuntimeItem(parent, after, item);\
else if(std::string(parent).empty() && std::string(after).empty() && ma->isStarted())mvAppLog::getLogger()->LogWarning(item->getName() + " not added. Must use after/parent if added during runtime.");\
if(ma->isStarted() && item->isContainer())ma->popParent();

namespace Marvel {

	//-----------------------------------------------------------------------------
	// Enums and Helper Functions
	//-----------------------------------------------------------------------------
	enum class mvPythonDataType
	{
		String, Integer, Float, Bool, StringList, FloatList, Optional,
		Object, IntList, KeywordOnly
	};

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
		case mvPythonDataType::Optional:   return "Optional Arguments\n____________________";
		case mvPythonDataType::KeywordOnly:return "Keyword Only Arguments\n____________________";
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
		std::string      description;

		mvPythonDataElement(mvPythonDataType type, const char* name = "", const std::string& description = "")
			: name(name), type(type), description(description)
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
			const std::string& about = "", const std::string& returnType = "None");

		bool                                    parse(PyObject* args, PyObject* kwargs, const char* message, ...);
		static std::vector<std::string>         getStringVec(PyObject* obj);
		static std::vector<float>               getFloatVec(PyObject* obj);
		static std::vector<int>                 getIntVec(PyObject* obj);
		static mvVec2                           getVec2(PyObject* obj);
		static mvColor                          getColor(PyObject* obj);
		static std::vector<mvVec2>              getVectVec2(PyObject* obj);
		static std::vector<std::pair<int, int>> getVectInt2(PyObject* obj);
		inline const char*                      getDocumentation() const { return m_documentation.c_str(); }

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