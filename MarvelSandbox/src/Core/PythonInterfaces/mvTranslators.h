# pragma once

#include "Core/mvPythonTranslator.h"
#include <map>
#include <string>

namespace Marvel{

	static std::map<std::string, mvPythonTranslator> BuildWidgetTranslations()
	{

		std::map<std::string, mvPythonTranslator> translators = {

			{"addListbox", mvPythonTranslator({
				mvPythonDataElement(mvPythonDataType::String, "name", false, "Name of the listbox"),
				mvPythonDataElement(mvPythonDataType::StringList, "items"),
				mvPythonDataElement(mvPythonDataType::Optional, ""),
				mvPythonDataElement(mvPythonDataType::Integer, "default_value"),
				mvPythonDataElement(mvPythonDataType::Integer, "height"),
			}, true, "A Listbox is a listbox")}

		};

		return translators;

	}

}