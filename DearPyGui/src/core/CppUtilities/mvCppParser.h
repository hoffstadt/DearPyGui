#pragma once
#include "mvCore.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// Enums and Helper Functions
	//-----------------------------------------------------------------------------
	enum class mvPythonDataType
	{
		None, String, Integer, Float, Bool, StringList, FloatList, Optional,
		Object, IntList, KeywordOnly, Double, Callable, Dict, ListFloatList,
		ListStrList, Kwargs
	};

	//-----------------------------------------------------------------------------
	// mvPythonDataElement
	//-----------------------------------------------------------------------------
	struct mvPythonDataElement
	{
		mvPythonDataType type = mvPythonDataType::None;
		std::string      name = "";
		std::string      description;
		std::string      default_value = "...";

		[[nodiscard]] char getSymbol() const { return 'a'; }
	};

	class mvPythonParser
	{
	public:


		mvPythonParser() = default;

		mvPythonParser(const std::initializer_list<mvPythonDataElement>& elements,
			std::string about = "", std::string returnType = "None",
			std::string category = "App") {}

		bool parse(PyObject* args, PyObject* kwargs, const char* message, ...) { return true; }

		const char* getDocumentation() const { return ""; }
		const char* getCategory() const { return ""; }

	};

}