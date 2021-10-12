#include "mvFontRegistry.h"
#include "mvFont.h"
#include "mvPythonExceptions.h"
#include "mvLog.h"
#include "mvToolManager.h"
#include "mvFontManager.h"

namespace Marvel {

	void mvFontRegistry::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		std::vector<mvPythonDataElement> args;
		AddCommonArgs(args,(CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_SHOW)
		);

		mvPythonParserSetup setup;
		setup.about = "Adds a font registry.";
		setup.category = { "Fonts", "Containers", "Registries"};
		setup.returnType = mvPyDataType::UUID;
		setup.createContextManager = true;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers->insert({ s_command, parser });
	}

	mvFontRegistry::mvFontRegistry(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
		_show = true;
	}

	void mvFontRegistry::resetFont()
	{
		for (auto& item : _children[1])
		{
			static_cast<mvFont*>(item.get())->_default = false;
		}

		mvToolManager::GetFontManager()._resetDefault = true;
	}

	void mvFontRegistry::draw(ImDrawList* drawlist, float x, float y)
	{
		//ImGuiIO& io = ImGui::GetIO();
		//io.Fonts->Clear();
		//io.FontDefault = io.Fonts->AddFontDefault();

		for (auto& item : _children[1])
		{
			item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
		}
		_show = false;
	}

	void mvFontRegistry::customAction(void* data)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->Clear();
		io.FontDefault = io.Fonts->AddFontDefault();

		for (auto& item : _children[1])
		{
			item->customAction(data);
		}
	}

}