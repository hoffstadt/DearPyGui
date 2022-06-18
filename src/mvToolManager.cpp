#include "mvToolManager.h"
#include "mvPythonTranslator.h"
#include "mvAboutWindow.h"
#include "mvDocWindow.h"
#include "mvMetricsWindow.h"
#include "mvStyleWindow.h"
#include "mvDebugWindow.h"
#include "mvFontManager.h"
#include "mvLayoutWindow.h"
#include "mvProfiler.h"
#include "mvItemRegistry.h"

std::vector<mvRef<mvToolWindow>> mvToolManager::s_tools = {
	std::make_shared<mvFontManager>(),
	std::make_shared<mvAboutWindow>(),
	std::make_shared<mvDocWindow>(),
	std::make_shared<mvMetricsWindow>(),
	std::make_shared<mvStyleWindow>(),
	std::make_shared<mvDebugWindow>(),
	std::make_shared<mvLayoutWindow>(),
};

mvFontManager& mvToolManager::GetFontManager()
{
	return *static_cast<mvFontManager*>(s_tools[0].get());
}

void mvToolManager::Reset()
{
	s_tools.clear();

	s_tools = {
		std::make_shared<mvFontManager>(),
		std::make_shared<mvAboutWindow>(),
		std::make_shared<mvDocWindow>(),
		std::make_shared<mvMetricsWindow>(),
		std::make_shared<mvStyleWindow>(),
		std::make_shared<mvDebugWindow>(),
		std::make_shared<mvLayoutWindow>(),
	};
}

void mvToolManager::Draw()
{
	MV_PROFILE_SCOPE("Tool rendering")

	for (auto& tool : s_tools)
		tool->draw();
}

void mvToolManager::ShowTool(mvUUID name)
{
	for (auto& tool : s_tools)
	{
		if (tool->getUUID() == name)
		{
			tool->m_show = true;
			return;
		}
	}
}