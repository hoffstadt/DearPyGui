#pragma once

#include <vector>
#include "mvContext.h"
#include "mvToolWindow.h"

class mvFontManager;

class mvToolManager
{

public:

	static void Draw();
	static void Reset();

	static void ShowTool(mvUUID name);

	static mvFontManager& GetFontManager();


private:

	static std::vector<std::shared_ptr<mvToolWindow>> s_tools;

};