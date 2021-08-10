#include "mvNodeAttribute.h"
#include <imnodes.h>
#include "mvApp.h"
#include "mvCore.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvNodeEditor.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvNodeAttribute::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Adds a node attribute.", { "Node Editor", "Containers", "Widgets" }, true);
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_FILTER |
			MV_PARSER_ARG_DROP_CALLBACK |
			MV_PARSER_ARG_DRAG_CALLBACK |
			MV_PARSER_ARG_PAYLOAD_TYPE |
			MV_PARSER_ARG_TRACKED |
			MV_PARSER_ARG_SHOW)
		);

		parser.addArg<mvPyDataType::Long>("attribute_type", mvArgType::KEYWORD_ARG, "0", "mvNode_Attr_Input, mvNode_Attr_Output, or mvNode_Attr_Static.");
		parser.addArg<mvPyDataType::Integer>("shape", mvArgType::KEYWORD_ARG, "1", "Pin shape.");
		parser.addArg<mvPyDataType::String>("category", mvArgType::KEYWORD_ARG, "'general'", "Category");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvNodeAttribute::mvNodeAttribute(mvUUID uuid)
		: mvAppItem(uuid)
	{
		int64_t address = (int64_t)this;
		int64_t reduced_address = address % 2147483648;
		_id = (int)reduced_address;
	}

	void mvNodeAttribute::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id(_uuid);

		if (_attrType == mvNodeAttribute::AttributeType::mvAttr_Static)
			imnodes::BeginStaticAttribute((int)_id);
		else if(_attrType == mvNodeAttribute::AttributeType::mvAttr_Output)
			imnodes::BeginOutputAttribute((int)_id, _shape);
		else
			imnodes::BeginInputAttribute((int)_id, _shape);

		for (auto& item : _children[1])
		{
			// skip item if it's not shown
			if (!item->isShown())
				continue;

			// set item width
			if (item->getWidth() != 0)
				ImGui::SetNextItemWidth((float)item->getWidth());

			if (item->shouldFocusNextFrame())
			{
				ImGui::SetKeyboardFocusHere();
				item->unfocus();
			}

			auto oldCursorPos = ImGui::GetCursorPos();
			if (item->isPosDirty())
				ImGui::SetCursorPos(item->getState().getItemPos());

			item->getState().setPos({ ImGui::GetCursorPosX(), ImGui::GetCursorPosY() });

			item->draw(drawlist, x, y);

			if (item->isPosDirty())
				ImGui::SetCursorPos(oldCursorPos);

			item->getState().update();
		}

		if (_attrType == mvNodeAttribute::AttributeType::mvAttr_Static)
			imnodes::EndStaticAttribute();
		else if (_attrType == mvNodeAttribute::AttributeType::mvAttr_Output)
			imnodes::EndOutputAttribute();
		else
			imnodes::EndInputAttribute();
	}

	void mvNodeAttribute::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "category")) _category = ToString(item);
		if (PyObject* item = PyDict_GetItemString(dict, "attribute_type")) _attrType = (mvNodeAttribute::AttributeType)ToUUID(item);
		if (PyObject* item = PyDict_GetItemString(dict, "shape"))
		{
			_shape = (imnodes::PinShape)ToInt(item);
		}
	}

	void mvNodeAttribute::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "attribute_type", ToPyUUID((long)_attrType));
		PyDict_SetItemString(dict, "shape", ToPyInt((int)_shape));
		PyDict_SetItemString(dict, "category", ToPyString(_category));
	}

}