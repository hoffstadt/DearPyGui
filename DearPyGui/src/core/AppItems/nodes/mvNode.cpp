#include "mvNode.h"
#include <imnodes.h>
#include "mvApp.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"

namespace Marvel {

	static std::string FindRenderedTextEnd(const char* text, const char* text_end = nullptr)
	{
		int size = 0;

		const char* text_display_end = text;
		if (!text_end)
			text_end = (const char*)-1;

		while (text_display_end < text_end && *text_display_end != '\0' && (text_display_end[0] != '#' || text_display_end[1] != '#'))
		{
			text_display_end++;
			size++;
		}

		char* cvalue = new char[size + 1];
		for (int i = 0; i < size; i++)
			cvalue[i] = text[i];

		cvalue[size] = '\0';

		auto result = std::string(cvalue);
		delete[] cvalue;

		return result;
	}

	void mvNode::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Adds a node to a node editor.", { "Node Editor", "Widgets", "Containers"}, true);
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_FILTER |
			MV_PARSER_ARG_POS |
			MV_PARSER_ARG_DROP_CALLBACK |
			MV_PARSER_ARG_DRAG_CALLBACK |
			MV_PARSER_ARG_PAYLOAD_TYPE |
			MV_PARSER_ARG_SEARCH_DELAY |
			MV_PARSER_ARG_TRACKED |
			MV_PARSER_ARG_SHOW)
		);

		parser.addArg<mvPyDataType::Bool>("draggable", mvArgType::KEYWORD_ARG, "True", "Allow node to be draggable.");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvNode::mvNode(mvUUID uuid)
		: mvAppItem(uuid)
	{
		m_label = FindRenderedTextEnd(m_label.c_str());
		m_specificedlabel = m_label;
        int64_t address = (int64_t)this;
        int64_t reduced_address = address % 2147483648;
        m_id = (int)reduced_address;
	}

	bool mvNode::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvStagingContainer) return true;
		if (type == mvAppItemType::mvNodeEditor) return true;

		mvThrowPythonError(mvErrorCode::mvIncompatibleParent, s_command,
			"Incompatible parent. Acceptable parents include: node editor, staging container", this);
		MV_ITEM_REGISTRY_ERROR("Node parent must be node editor.");
		assert(false);
		return false;
	}

	bool mvNode::canChildBeAdded(mvAppItemType type)
	{
		if (type == mvAppItemType::mvNodeAttribute) return true;

		if (type == mvAppItemType::mvActiveHandler) return true;
		if (type == mvAppItemType::mvClickedHandler) return true;
		if (type == mvAppItemType::mvHoverHandler) return true;
		if (type == mvAppItemType::mvVisibleHandler) return true;

		mvThrowPythonError(mvErrorCode::mvIncompatibleChild, s_command,
			"Incompatible child. Acceptable children include: mvNodeAttribute", this);

		MV_ITEM_REGISTRY_ERROR("Node children must be node attributes only.");
		assert(false);

		return false;
	}

	void mvNode::draw(ImDrawList* drawlist, float x, float y)
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

		// indent (for children
		if (m_indent > 0.0f)
			ImGui::Indent(m_indent);

		// push font if a font object is attached
		if (m_font)
		{
			ImFont* fontptr = static_cast<mvFont*>(m_font.get())->getFontPtr();
			ImGui::PushFont(fontptr);
		}

		// handle class theming
		if (auto classTheme = getClassTheme())
			static_cast<mvTheme*>(classTheme.get())->draw(nullptr, 0.0f, 0.0f);

		// handle item theming
		if (m_theme)
			static_cast<mvTheme*>(m_theme.get())->draw(nullptr, 0.0f, 0.0f);

		//-----------------------------------------------------------------------------
		// draw
		//-----------------------------------------------------------------------------
		{
			ScopedID id(m_uuid);

			if (m_dirtyPos)
			{
				imnodes::SetNodeGridSpacePos((int)m_id, m_state.getItemPos());
				m_dirtyPos = false;
			}

			imnodes::SetNodeDraggable((int)m_id, m_draggable);

			imnodes::BeginNode(m_id);

			imnodes::BeginNodeTitleBar();
			ImGui::TextUnformatted(m_specificedlabel.c_str());
			imnodes::EndNodeTitleBar();

			for (auto& item : m_children[1])
			{
				// skip item if it's not shown
				if (!item->m_show)
					continue;

				// set item width
				if (item->m_width != 0)
					ImGui::SetNextItemWidth((float)item->m_width);

				item->draw(drawlist, x, y);

				auto& state = item->getState();
				state.setActive(imnodes::IsAttributeActive());

			}

			imnodes::EndNode();
		}

		//-----------------------------------------------------------------------------
		// post draw
		//-----------------------------------------------------------------------------

		//-----------------------------------------------------------------------------
		// update state
		//   * only update if applicable
		//-----------------------------------------------------------------------------
		ImVec2 pos = imnodes::GetNodeGridSpacePos((int)m_id);
		m_state.m_hovered = ImGui::IsItemHovered();
		m_state.m_clicked = ImGui::IsItemClicked();
		m_state.m_visible = ImGui::IsItemVisible();
		m_state.m_active = imnodes::IsAnyAttributeActive();

		m_state.setPos({ pos.x , pos.y });

		// undo indents
		if (m_indent > 0.0f)
			ImGui::Unindent(m_indent);

		// pop font off stack
		if (m_font)
			ImGui::PopFont();

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

		// drag drop
		for (auto& item : m_children[4])
		{
			if (!item->preDraw())
				continue;

			item->draw(nullptr, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
		}

		if (m_dropCallback)
		{
			ScopedID id(m_uuid);
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(m_payloadType.c_str()))
				{
					auto payloadActual = static_cast<const mvDragPayload*>(payload->Data);
					mvApp::GetApp()->getCallbackRegistry().addCallback(getDropCallback(), m_uuid, payloadActual->getDragData(), nullptr);
				}

				ImGui::EndDragDropTarget();
			}
		}
	}

	void mvNode::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "draggable")) m_draggable = ToBool(item);

	}

	void mvNode::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		PyDict_SetItemString(dict, "draggable", ToPyBool(m_draggable));	
	}

}