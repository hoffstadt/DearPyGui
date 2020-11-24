#pragma once

#include "mvCore.h"
#include <unordered_map>
#include "mvMarvel.h"
#include "PythonUtilities/mvPythonExceptions.h"

namespace Marvel {

	class mvAppStyleManager
	{
	
	public:

		static int GetConstantId(std::string constName)
		{
			auto constants = GetModuleConstants();
			for (const auto& item : constants)
			{
				if (item.first == constName)
					return item.second;
			}
			//this error below is really only so we done mis type any constants while building this system
			std::string message = constName + "does not exist in module constants and was not added to the App Style Manager constants.";
			printf(message.c_str());
			return -1;

		}

		static void SetAppColor(int id, mvColor color);

		static std::unordered_map<int, mvColor*> s_colorDefines;

	};
}