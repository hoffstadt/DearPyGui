#include "mvColorMapRegistry.h"
#include "mvPythonExceptions.h"
#include "mvLog.h"

namespace Marvel {

	void mvColorMapRegistry::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Adds a colormap registry.", { "Events", "Widgets" }, true);
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID)
		);

		parser.addArg<mvPyDataType::Bool>("show", mvArgType::KEYWORD_ARG, "False", "Attempt to render widget.");
		parser.finalize();

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
		if (ImGui::Begin(_label.c_str(), &_show))
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

	void mvColorMapRegistry::alternativeCustomAction()
	{
		for (auto& item : _children[1])
			item->alternativeCustomAction();

		_triggerAlternativeAction = false;
	}

	void mvColorMapRegistry::onChildAdd(mvRef<mvAppItem> item)
	{
		_triggerAlternativeAction = true;
	}
}