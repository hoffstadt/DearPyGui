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

		parser.addArg<mvPyDataType::Long>("attribute_type", mvArgType::KEYWORD_ARG, "0", "mvNode_Attr_Input, mvNode_Attr_Output, or mvNode_Attr_Static");
		parser.addArg<mvPyDataType::Integer>("shape", mvArgType::KEYWORD_ARG, "0", "Pin shape");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvNodeAttribute::mvNodeAttribute(mvUUID uuid)
		: mvAppItem(uuid)
	{
		int64_t address = (int64_t)this;
		int64_t reduced_address = address % 2147483648;
		m_id = (int)reduced_address;
	}

	bool mvNodeAttribute::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvStagingContainer) return true;
		if(type == mvAppItemType::mvNode) return true;
		
		mvThrowPythonError(mvErrorCode::mvIncompatibleParent, s_command,
			"Incompatible parent. Acceptable parents include: node, staging container", this);

		MV_ITEM_REGISTRY_ERROR("Node attribute parent must be node.");
		assert(false);
		return false;
	}

	void mvNodeAttribute::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id(m_uuid);

		if (m_attrType == mvNodeAttribute::AttributeType::mvAttr_Static)
			imnodes::BeginStaticAttribute((int)m_id);
		else if(m_attrType == mvNodeAttribute::AttributeType::mvAttr_Output)
			imnodes::BeginOutputAttribute((int)m_id, m_shape);
		else
			imnodes::BeginInputAttribute((int)m_id, m_shape);

		for (auto& item : m_children[1])
		{
			// skip item if it's not shown
			if (!item->m_show)
				continue;

			// set item width
			if (item->m_width != 0)
				ImGui::SetNextItemWidth((float)item->m_width);

			if (item->m_focusNextFrame)
			{
				ImGui::SetKeyboardFocusHere();
				item->m_focusNextFrame = false;
			}

			auto oldCursorPos = ImGui::GetCursorPos();
			if (item->m_dirtyPos)
				ImGui::SetCursorPos(item->getState().getItemPos());

			item->getState().setPos({ ImGui::GetCursorPosX(), ImGui::GetCursorPosY() });

			item->draw(drawlist, x, y);

			if (item->m_dirtyPos)
				ImGui::SetCursorPos(oldCursorPos);

			item->getState().update();
		}

		if (m_attrType == mvNodeAttribute::AttributeType::mvAttr_Static)
			imnodes::EndStaticAttribute();
		else if (m_attrType == mvNodeAttribute::AttributeType::mvAttr_Output)
			imnodes::EndOutputAttribute();
		else
			imnodes::EndInputAttribute();
	}

	void mvNodeAttribute::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "attribute_type")) m_attrType = (mvNodeAttribute::AttributeType)ToUUID(item);
		if (PyObject* item = PyDict_GetItemString(dict, "shape"))
		{
			m_shape = (imnodes::PinShape)ToInt(item);
		}
	}

	void mvNodeAttribute::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "attribute_type", ToPyUUID((long)m_attrType));
		PyDict_SetItemString(dict, "shape", ToPyInt((int)m_shape));
	}

}