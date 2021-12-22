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
#include "AppItems/widget_handlers/mvItemHandlerRegistry.h"

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
		info.internalLabel = FindRenderedTextEnd(info.internalLabel.c_str()) + std::to_string(uuid);
		config.specifiedLabel = info.internalLabel;
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
		if (!config.show)
			return;

		// set item width
		if (config.width != 0)
			ImGui::SetNextItemWidth((float)config.width);

		// indent (for children
		if (config.indent > 0.0f)
			ImGui::Indent(config.indent);

		// push font if a font object is attached
		if (font)
		{
			ImFont* fontptr = static_cast<mvFont*>(font.get())->getFontPtr();
			ImGui::PushFont(fontptr);
		}

		// themes
		apply_local_theming(this);

		//-----------------------------------------------------------------------------
		// draw
		//-----------------------------------------------------------------------------
		{
			ScopedID id(uuid);

			if (info.dirtyPos)
			{
				imnodes::SetNodeGridSpacePos((int)_id, state.pos);
				info.dirtyPos = false;
			}

			imnodes::SetNodeDraggable((int)_id, _draggable);

			imnodes::BeginNode(_id);

			imnodes::BeginNodeTitleBar();
			ImGui::TextUnformatted(config.specifiedLabel.c_str());
			imnodes::EndNodeTitleBar();

			state.lastFrameUpdate = GContext->frame;
			state.leftclicked = ImGui::IsItemClicked();
			state.rightclicked = ImGui::IsItemClicked(1);
			state.middleclicked = ImGui::IsItemClicked(2);
			state.visible = ImGui::IsItemVisible();

			for (auto& item : childslots[1])
			{
				// skip item if it's not shown
				if (!item->config.show)
					continue;

				// set item width
				if (item->config.width != 0)
					ImGui::SetNextItemWidth((float)item->config.width);

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

		state.pos = { pos.x , pos.y };

		// undo indents
		if (config.indent > 0.0f)
			ImGui::Unindent(config.indent);

		// pop font off stack
		if (font)
			ImGui::PopFont();

		// handle popping themes
		cleanup_local_theming(this);

		if (handlerRegistry)
			handlerRegistry->checkEvents(&state);

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