# pragma once

#include "Core/mvPythonTranslator.h"
#include <map>
#include <string>

namespace Marvel{

	static std::map<std::string, mvPythonTranslator> BuildTranslations()
	{
		std::map<std::string, mvPythonTranslator> translators = {

			{"isMouseButtonPressed", mvPythonTranslator({
				{mvPythonDataType::Integer, "button"}
			}, false, "Needs documentation")},

			{"isKeyPressed", mvPythonTranslator({
				{mvPythonDataType::Integer, "key"}
			}, false, "Needs documentation")}

		};

		return translators;
	}

}