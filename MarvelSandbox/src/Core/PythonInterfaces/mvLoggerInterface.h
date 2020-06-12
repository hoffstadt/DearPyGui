# pragma once

#include "Core/mvPythonTranslator.h"
#include <map>
#include <string>

namespace Marvel{

	static std::map<std::string, mvPythonTranslator> BuildTranslations()
	{
		std::map<std::string, mvPythonTranslator> translators = {

			{"SetLogLevel", mvPythonTranslator({
				{mvPythonDataType::Integer, "button"}
			}, false, "Needs documentation")},

			{"Log", mvPythonTranslator({
				{mvPythonDataType::Integer, "message"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::String, "level"}
			}, false, "Needs documentation")},

			{"LogDebug", mvPythonTranslator({
				{mvPythonDataType::Integer, "message"}
			}, false, "Needs documentation")},

			{"LogInfo", mvPythonTranslator({
				{mvPythonDataType::Integer, "message"}
			}, false, "Needs documentation")},

			{"LogWarning", mvPythonTranslator({
				{mvPythonDataType::Integer, "message"}
			}, false, "Needs documentation")},

			{"LogError", mvPythonTranslator({
				{mvPythonDataType::Integer, "message"}
			}, false, "Needs documentation")}

		};

		return translators;
	}

}