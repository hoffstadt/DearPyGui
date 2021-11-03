#include "mvColorMapRegistry.h"
#include "mvPythonExceptions.h"
#include "mvLog.h"

namespace Marvel {

	void mvColorMapRegistry::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		std::vector<mvPythonDataElement> args;

		AddCommonArgs(args,(CommonParserArgs)(
			MV_PARSER_ARG_ID)
		);

		args.push_back({ mvPyDataType::Bool, "show", mvArgType::KEYWORD_ARG, "False", "Attempt to render widget." });

		mvPythonParserSetup setup;
		setup.about = "Adds a colormap registry.";
		setup.category = { "Containers", "Widgets", "Colors" };
		setup.returnType = mvPyDataType::UUID;
		setup.createContextManager = true;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers->insert({ s_command, parser });
	}

	mvColorMapRegistry::mvColorMapRegistry(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
		_show = false;
	}

	void mvColorMapRegistry::draw(ImDrawList* drawlist, float x, float y)
	{

		if (!_show)
			return;

		ImGui::PushID(this);

		ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_FirstUseEver);
		if (ImGui::Begin(_internalLabel.c_str(), &_show))
		{
			ImGui::Text("Builtin:");
			for (int i = 0; i < 16; i++)
				ImPlot::ColormapButton(ImPlot::GetColormapName(i), ImVec2(-1.0f, 0.0f), i);

			ImGui::Text("User:");

			for (auto& item : _children[1])
				item->draw(drawlist, 0.0f, 0.0f);
			
		}

		ImGui::End();

		ImGui::PopID();

	}
}