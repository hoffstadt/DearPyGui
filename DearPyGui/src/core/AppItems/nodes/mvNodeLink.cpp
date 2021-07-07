#include "mvNodeLink.h"
#include <imnodes.h>
#include "mvApp.h"
#include "mvLog.h"
#include "mvNodeAttribute.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"

namespace Marvel {

	void mvNodeLink::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Adds a node link between nodes.", { "Node Editor", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_SHOW)
		);

		parser.addArg<mvPyDataType::UUID>("node_1");
		parser.addArg<mvPyDataType::UUID>("node_2");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvNodeLink::mvNodeLink(mvUUID uuid)
		: mvAppItem(uuid)
	{
		int64_t address = (int64_t)this;
		int64_t reduced_address = address % 2147483648;
		m_id = (int)reduced_address;
	}

	bool mvNodeLink::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvStagingContainer) return true;
		if (type == mvAppItemType::mvNodeEditor) return true;

		mvThrowPythonError(mvErrorCode::mvIncompatibleParent, s_command,
			"Incompatible parent. Acceptable parents include: node editor, staging container", this);

		MV_ITEM_REGISTRY_ERROR("Node link parent must be node editor.");
		assert(false);
		return false;
	}

	void mvNodeLink::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!mvApp::GetApp()->getParsers()[s_command].verifyRequiredArguments(dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
			{
				mvUUID node1 = ToUUID(item);
				mvAppItem* node = mvApp::GetApp()->getItemRegistry().getItem(node1);
				if (node->getType() == mvAppItemType::mvNodeAttribute)
				{
					m_id1 = static_cast<const mvNodeAttribute*>(node)->getId();
				}
				else
				{
					mvThrowPythonError(mvErrorCode::mvIncompatibleType, s_command,
						"Incompatible type. Expected types include: mvNode", node);
					MV_ITEM_REGISTRY_ERROR("Nodes must be nodes. duh");
					assert(false);
					return;
				}
				break;
			}

			case 1:
			{
				mvUUID node2 = ToUUID(item);
				mvAppItem* node = mvApp::GetApp()->getItemRegistry().getItem(node2);
				if (node->getType() == mvAppItemType::mvNodeAttribute)
				{
					m_id2 = static_cast<const mvNodeAttribute*>(node)->getId();
				}
				else
				{
					mvThrowPythonError(mvErrorCode::mvIncompatibleType, s_command,
						"Incompatible type. Expected types include: mvNode", node);
					MV_ITEM_REGISTRY_ERROR("Nodes must be nodes. duh");
					assert(false);
					return;
				}
				break;
			}

			default:
				break;
			}
		}
	}

	void mvNodeLink::customAction()
	{
		//-----------------------------------------------------------------------------
		// update state
		//   * only update if applicable
		//-----------------------------------------------------------------------------
		//m_state.m_hovered = imnodes::IsLinkHovered(&m_id);
		m_state.m_visible = ImGui::IsItemVisible();
		//m_state.m_active = imnodes::IsLinkStarted(&m_id);
		//m_state.m_deactivated = imnodes::IsLinkDropped(&m_id);

		// pop class themes
		if (auto classTheme = getClassTheme())
			static_cast<mvTheme*>(classTheme.get())->customAction();

		// pop item themes
		if (m_theme)
			static_cast<mvTheme*>(m_theme.get())->customAction();

		// event handlers
		for (auto& item : m_children[3])
		{
			if (!item->preDraw())
				continue;

			item->draw(nullptr, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
		}
	}

	void mvNodeLink::draw(ImDrawList* drawlist, float x, float y)
	{

		//-----------------------------------------------------------------------------
		// pre draw
		//-----------------------------------------------------------------------------

		// show/hide
		if (!m_show)
			return;

		// set item width
		if (m_width != 0)
			ImGui::SetNextItemWidth((float)m_width);

		// handle class theming
		if (auto classTheme = getClassTheme())
			static_cast<mvTheme*>(classTheme.get())->draw(nullptr, 0.0f, 0.0f);

		// handle item theming
		if (m_theme)
			static_cast<mvTheme*>(m_theme.get())->draw(nullptr, 0.0f, 0.0f);

		//-----------------------------------------------------------------------------
		// draw
		//-----------------------------------------------------------------------------
		ScopedID id(m_uuid);

		imnodes::Link(m_id, m_id1, m_id2);

		//-----------------------------------------------------------------------------
		// post draw
		//-----------------------------------------------------------------------------

		// check custom action

	}

}