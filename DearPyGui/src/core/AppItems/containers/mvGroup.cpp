#include "mvGroup.h"
#include "mvInput.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
namespace Marvel {

	void mvGroup::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, 
			"Creates a group that other widgets can belong to. The group allows item commands to be issued for all of its members. Must be closed with the end command.", 
			{ "Containers", "Widgets" }, true);
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_WIDTH |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_FILTER |
			MV_PARSER_ARG_DROP_CALLBACK |
			MV_PARSER_ARG_DRAG_CALLBACK |
			MV_PARSER_ARG_PAYLOAD_TYPE |
			MV_PARSER_ARG_TRACKED |
			MV_PARSER_ARG_SEARCH_DELAY |
			MV_PARSER_ARG_POS)
		);

		parser.addArg<mvPyDataType::Bool>("horizontal", mvArgType::KEYWORD_ARG, "False", "Forces child widgets to be added in a horizontal layout.");
		parser.addArg<mvPyDataType::Float>("horizontal_spacing", mvArgType::KEYWORD_ARG, "-1", "Spacing for the horizontal layout.");


		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvGroup::mvGroup(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	void mvGroup::draw(ImDrawList* drawlist, float x, float y)
	{

		if (_width != 0)
			ImGui::PushItemWidth((float)_width);

		ImGui::BeginGroup();

		for (auto& item : _children[1])
		{
			if (_width != 0)
				item->setWidth(_width);

			if (!item->preDraw())
				continue;

			item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

			item->postDraw();

			if (_horizontal)
				ImGui::SameLine(0.0, _hspacing);
		}

		if (_width != 0)
			ImGui::PopItemWidth();

		ImGui::EndGroup();
	}

	void mvGroup::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		if (PyObject* item = PyDict_GetItemString(dict, "horizontal")) _horizontal = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "horizontal_spacing")) _hspacing = ToFloat(item);
	}

	void mvGroup::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		PyDict_SetItemString(dict, "horizontal", mvPyObject(ToPyBool(_horizontal)));
		PyDict_SetItemString(dict, "horizontal_spacing", mvPyObject(ToPyFloat(_hspacing)));
	}

}