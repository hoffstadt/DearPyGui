#include "mvTreeNode.h"
#include "mvContext.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"
#include "AppItems/widget_handlers/mvItemHandlerRegistry.h"

namespace Marvel {
	
	mvTreeNode::mvTreeNode(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	void mvTreeNode::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvTreeNode*>(item);
		if (config.source != 0) _value = titem->_value;
		_disabled_value = titem->_disabled_value;
		_flags = titem->_flags;
		_selectable = titem->_selectable;
	}

	void mvTreeNode::draw(ImDrawList* drawlist, float x, float y)
	{

		//-----------------------------------------------------------------------------
		// pre draw
		//-----------------------------------------------------------------------------

		// show/hide
		if (!config.show)
			return;

		// focusing
		if (info.focusNextFrame)
		{
			ImGui::SetKeyboardFocusHere();
			info.focusNextFrame = false;
		}

		// cache old cursor position
		ImVec2 previousCursorPos = ImGui::GetCursorPos();

		// set cursor position if user set
		if (info.dirtyPos)
			ImGui::SetCursorPos(state.pos);

		// update widget's position state
		state.pos = { ImGui::GetCursorPosX(), ImGui::GetCursorPosY() };

		// set item width
		if (config.width != 0)
			ImGui::SetNextItemWidth((float)config.width);

		// set indent
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

		ImGui::BeginGroup();
		
		if (*_value && _selectable)
			_flags |= ImGuiTreeNodeFlags_Selected;
		else
			_flags &= ~ImGuiTreeNodeFlags_Selected;

		ImGui::SetNextItemOpen(*_value);

		*_value = ImGui::TreeNodeEx(info.internalLabel.c_str(), _flags);
		UpdateAppItemState(state);

		if (state.toggledOpen && !*_value)
		{
			state.toggledOpen = false;
		}

		if (!*_value)
		{
			ImGui::EndGroup();
			return;
		}

		for (auto& item : childslots[1])
			item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

		ImGui::TreePop();
		ImGui::EndGroup();
		}

		//-----------------------------------------------------------------------------
		// post draw
		//-----------------------------------------------------------------------------

		// set cursor position to cached position
		if (info.dirtyPos)
			ImGui::SetCursorPos(previousCursorPos);

		if (config.indent > 0.0f)
			ImGui::Unindent(config.indent);

		// pop font off stack
		if (font)
			ImGui::PopFont();

		// handle popping themes
		cleanup_local_theming(this);

		if (handlerRegistry)
			handlerRegistry->checkEvents(&state);

		// handle drag & drop if used
		apply_drag_drop(this);
	}

	PyObject* mvTreeNode::getPyValue()
	{
		return ToPyBool(*_value);
	}

	void mvTreeNode::setPyValue(PyObject* value)
	{
		*_value = ToBool(value);
	}

	void mvTreeNode::setDataSource(mvUUID dataSource)
	{
		if (dataSource == config.source) return;
		config.source = dataSource;

		mvAppItem* item = GetItem((*GContext->itemRegistry), dataSource);
		if (!item)
		{
			mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
				"Source item not found: " + std::to_string(dataSource), this);
			return;
		}
		if (GetEntityValueType(item->type) != GetEntityValueType(type))
		{
			mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
				"Values types do not match: " + std::to_string(dataSource), this);
			return;
		}
		_value = *static_cast<std::shared_ptr<bool>*>(item->getValue());
	}

	void mvTreeNode::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		if (PyObject* item = PyDict_GetItemString(dict, "selectable")) _selectable = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "default_open")) setPyValue(item);

		// helper for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		// flags
		flagop("default_open", ImGuiTreeNodeFlags_DefaultOpen, _flags);
		flagop("open_on_double_click", ImGuiTreeNodeFlags_OpenOnDoubleClick, _flags);
		flagop("open_on_arrow", ImGuiTreeNodeFlags_OpenOnArrow, _flags);
		flagop("leaf", ImGuiTreeNodeFlags_Leaf, _flags);
		flagop("bullet", ImGuiTreeNodeFlags_Bullet, _flags);
	}

	void mvTreeNode::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		PyDict_SetItemString(dict, "selectable", mvPyObject(ToPyBool(_selectable)));

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, mvPyObject(ToPyBool(flags & flag)));
		};

		// flags
		checkbitset("default_open", ImGuiTreeNodeFlags_DefaultOpen, _flags);
		checkbitset("open_on_double_click", ImGuiTreeNodeFlags_OpenOnDoubleClick, _flags);
		checkbitset("open_on_arrow", ImGuiTreeNodeFlags_OpenOnArrow, _flags);
		checkbitset("leaf", ImGuiTreeNodeFlags_Leaf, _flags);
		checkbitset("bullet", ImGuiTreeNodeFlags_Bullet, _flags);

	}

}