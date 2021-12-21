#pragma once

#include <vector>
#include "mvContext.h"
#include "cpp.hint"
#include "mvToolWindow.h"

namespace Marvel{

	class mvFontManager;

	class mvToolManager
	{

	public:

		static void Draw();
		static void Reset();

		static void ShowTool(mvUUID name);

		static mvFontManager& GetFontManager();


	private:

		static std::vector<mvRef<mvToolWindow>> s_tools;

	};

}