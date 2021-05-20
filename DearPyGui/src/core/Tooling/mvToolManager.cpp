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

		mvPythonParser parser(mvPyDataType::String);

		parser.addArg<mvPyDataType::String>("tool");

		parser.finalize();

		parsers->insert({ "show_tool", parser });
	}

	void mvToolManager::Draw()
	{
		MV_PROFILE_SCOPE("Tool rendering")

		for (auto& tool : s_tools)
			tool->draw();
	}

	void mvToolManager::ShowTool(const std::string& name)
	{
		for (auto& tool : s_tools)
		{
			if (std::string(tool->getName()) == name)
			{
				tool->m_show = true;
				return;
			}
		}
	}

	PyObject* mvToolManager::show_tool(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* tool;

		if (!(mvApp::GetApp()->getParsers())["show_tool"].parse(args, kwargs, __FUNCTION__,
			&tool))
			return GetPyNone();

		mvToolManager::ShowTool(tool);

		return GetPyNone();
	}
}