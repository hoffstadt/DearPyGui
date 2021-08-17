#include "mvColorMapButton.h"
#include "mvApp.h"
#include <array>
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvColorMapButton::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		mvPythonParser parser(mvPyDataType::UUID, "Adds a color button.", { "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_WIDTH |
			MV_PARSER_ARG_HEIGHT |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_CALLBACK |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_ENABLED |
			MV_PARSER_ARG_FILTER |
			MV_PARSER_ARG_DROP_CALLBACK |
			MV_PARSER_ARG_DRAG_CALLBACK |
			MV_PARSER_ARG_PAYLOAD_TYPE |
			MV_PARSER_ARG_TRACKED |
			MV_PARSER_ARG_POS)
		);

		parser.addArg<mvPyDataType::IntList>("default_value", mvArgType::POSITIONAL_ARG, "(0, 0, 0, 255)");

		parser.addArg<mvPyDataType::Bool>("no_alpha", mvArgType::KEYWORD_ARG, "False", "Ignore Alpha component.");
		parser.addArg<mvPyDataType::Bool>("no_border", mvArgType::KEYWORD_ARG, "False", "Disable border around the image.");
		parser.addArg<mvPyDataType::Bool>("no_drag_drop", mvArgType::KEYWORD_ARG, "False", "Disable display of inline text label.");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvColorMapButton::mvColorMapButton(mvUUID uuid)
		: 
		mvAppItem(uuid)
	{
	}

	void mvColorMapButton::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvColorMapButton*>(item);
		_colormap = titem->_colormap;
	}

	void mvColorMapButton::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id(_uuid);

		if (ImPlot::ColormapButton(_internalLabel.c_str(), ImVec2((float)_width, (float)_height), _colormap))
		{
			if(_alias.empty())
				mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), _uuid, nullptr, _user_data);
			else	
				mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), _alias, nullptr, _user_data);
		}

	}

}