#include "mvNode.h"
#include <imnodes.h>
#include "mvApp.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"
#include "mvPyObject.h"

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
		_internalLabel = FindRenderedTextEnd(_internalLabel.c_str()) + std::to_string(_uuid);
		_specificedlabel = _internalLabel;
        int64_t address = (int64_t)this;
        int64_t reduced_address = address % 2147483648;
        _id = (int)reduced_address;
	}

	void mvNode::draw(ImDrawList* drawlist, float x, float y)
	{

		//-----------------------------------------------------------------------------
		// pre draw
		//-----------------------------------------------------------------------------

		// show/hide
		if (!_show)
			return;

		// set item width
		if (_width != 0)
			ImGui::SetNextItemWidth((float)_width);

		// indent (for children
		if (_indent > 0.0f)
			ImGui::Indent(_indent);

		// push font if a font object is attached
		if (_font)
		{
			ImFont* fontptr = static_cast<mvFont*>(_font.get())->getFontPtr();
			ImGui::PushFont(fontptr);
		}

		// handle class theming
		if (auto classTheme = getClassTheme())
			static_cast<mvTheme*>(classTheme.get())->draw(nullptr, 0.0f, 0.0f);

		// handle item theming
		if (_theme)
			static_cast<mvTheme*>(_theme.get())->draw(nullptr, 0.0f, 0.0f);

		//-----------------------------------------------------------------------------
		// draw
		//-----------------------------------------------------------------------------
		{
			ScopedID id(_uuid);

			if (_dirtyPos)
			{
				imnodes::SetNodeGridSpacePos((int)_id, _state.getItemPos());
				_dirtyPos = false;
			}

			imnodes::SetNodeDraggable((int)_id, _draggable);

			imnodes::BeginNode(_id);

			imnodes::BeginNodeTitleBar();
			ImGui::TextUnformatted(_specificedlabel.c_str());
			imnodes::EndNodeTitleBar();

			for (auto& item : _children[1])
			{
				// skip item if it's not shown
				if (!item->isShown())
					continue;

				// set item width
				if (item->getWidth() != 0)
					ImGui::SetNextItemWidth((float)item->getWidth());

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
		ImVec2 pos = imnodes::GetNodeGridSpacePos((int)_id);
		_state._lastFrameUpdate = mvApp::s_frame;
		_state._hovered = ImGui::IsItemHovered();
		_state._leftclicked = ImGui::IsItemClicked();
		_state._rightclicked = ImGui::IsItemClicked(1);
		_state._middleclicked = ImGui::IsItemClicked(2);
		_state._visible = ImGui::IsItemVisible();
		_state._active = imnodes::IsAnyAttributeActive();

		_state.setPos({ pos.x , pos.y });

		// undo indents
		if (_indent > 0.0f)
			ImGui::Unindent(_indent);

		// pop font off stack
		if (_font)
			ImGui::PopFont();

		// pop class themes
		if (auto classTheme = getClassTheme())
			static_cast<mvTheme*>(classTheme.get())->customAction();

		// pop item themes
		if (_theme)
			static_cast<mvTheme*>(_theme.get())->customAction();

		// event handlers
		for (auto& item : _children[3])
		{
			if (!item->preDraw())
				continue;

			item->draw(nullptr, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
		}

		// drag drop
		for (auto& item : _children[4])
		{
			if (!item->preDraw())
				continue;

			item->draw(nullptr, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
		}

		if (_dropCallback)
		{
			ScopedID id(_uuid);
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(_payloadType.c_str()))
				{
					auto payloadActual = static_cast<const mvDragPayload*>(payload->Data);
					mvApp::GetApp()->getCallbackRegistry().addCallback(getDropCallback(), _uuid, payloadActual->getDragData(), nullptr);
				}

				ImGui::EndDragDropTarget();
			}
		}
	}

	void mvNode::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "draggable")) _draggable = ToBool(item);

	}

	void mvNode::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		PyDict_SetItemString(dict, "draggable", mvPyObject(ToPyBool(_draggable)));	
	}

}