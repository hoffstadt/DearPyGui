# pragma once

#include "Core/PythonUtilities/mvPythonTranslator.h"
#include <map>
#include <string>

namespace Marvel{

	static std::map<std::string, mvPythonTranslator> BuildTranslations()
	{
		std::map<std::string, mvPythonTranslator> translators = {

			{"SetLogLevel", mvPythonTranslator({
				{mvPythonDataType::Integer, "level"}
			}, false, "Needs documentation")},

			{"Log", mvPythonTranslator({
				{mvPythonDataType::String, "message"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::String, "level"}
			}, false, "Needs documentation")},

			{"LogDebug", mvPythonTranslator({
				{mvPythonDataType::String, "message"}
			}, false, "Needs documentation")},

			{"LogInfo", mvPythonTranslator({
				{mvPythonDataType::String, "message"}
			}, false, "Needs documentation")},

			{"LogWarning", mvPythonTranslator({
				{mvPythonDataType::String, "message"}
			}, false, "Needs documentation")},

			{"LogError", mvPythonTranslator({
				{mvPythonDataType::String, "message"}
			}, false, "Needs documentation")}

		};

		return translators;
	}

}