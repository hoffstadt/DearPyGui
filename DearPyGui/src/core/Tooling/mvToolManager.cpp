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

namespace Marvel {

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

	void mvToolManager::InsertConstants(std::vector<std::pair<std::string, long>>& constants)
	{
	}

	void mvToolManager::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		std::vector<mvPythonDataElement> args;

		args.push_back({ mvPyDataType::UUID, "tool" });

		mvPythonParserSetup setup;
		setup.about = "Shows a built in tool.";
		setup.category = { "Widgets" };
		setup.returnType = mvPyDataType::String;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers->insert({ "show_tool", parser });
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

	PyObject* mvToolManager::show_tool(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* toolraw;

		if (!Parse((GetParsers())["show_tool"], args, kwargs, __FUNCTION__,
			&toolraw))
			return GetPyNone();

		mvUUID tool = mvAppItem::GetIDFromPyObject(toolraw);

		mvToolManager::ShowTool(tool);

		return GetPyNone();
	}
}