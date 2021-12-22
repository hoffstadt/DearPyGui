#include "mvNode.h"
#include <imnodes.h>
#include "mvContext.h"
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

	mvNode::mvNode(mvUUID uuid)
		: mvAppItem(uuid)
	{
		_internalLabel = FindRenderedTextEnd(_internalLabel.c_str()) + std::to_string(_uuid);
		_specifiedLabel = _internalLabel;
        int64_t address = (int64_t)this;
        int64_t reduced_address = address % 2147483648;
        _id = (int)reduced_address;
	}

	void mvNode::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvNode*>(item);
		_draggable = titem->_draggable;
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

		// themes
		apply_local_theming(this);

		//-----------------------------------------------------------------------------
		// draw
		//-----------------------------------------------------------------------------
		{
			ScopedID id(_uuid);

			if (_dirtyPos)
			{
				imnodes::SetNodeGridSpacePos((int)_id, _state.pos);
				_dirtyPos = false;
			}

			imnodes::SetNodeDraggable((int)_id, _draggable);

			imnodes::BeginNode(_id);

			imnodes::BeginNodeTitleBar();
			ImGui::TextUnformatted(_specifiedLabel.c_str());
			imnodes::EndNodeTitleBar();

			_state.lastFrameUpdate = GContext->frame;
			_state.leftclicked = ImGui::IsItemClicked();
			_state.rightclicked = ImGui::IsItemClicked(1);
			_state.middleclicked = ImGui::IsItemClicked(2);
			_state.visible = ImGui::IsItemVisible();

			for (auto& item : _children[1])
			{
				// skip item if it's not shown
				if (!item->_show)
					continue;

				// set item width
				if (item->_width != 0)
					ImGui::SetNextItemWidth((float)item->_width);

				item->draw(drawlist, x, y);

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

		_state.pos = { pos.x , pos.y };

		// undo indents
		if (_indent > 0.0f)
			ImGui::Unindent(_indent);

		// pop font off stack
		if (_font)
			ImGui::PopFont();

		// handle popping themes
		cleanup_local_theming(this);

		if (_handlerRegistry)
			_handlerRegistry->customAction(&_state);

		// handle drag & drop payloads
		apply_drag_drop(this);
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