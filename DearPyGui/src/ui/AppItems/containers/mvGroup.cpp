#include "mvGroup.h"
#include "mvContext.h"
#include "mvItemRegistry.h"
#include "AppItems/mvFontItems.h"
#include "AppItems/mvThemes.h"
#include "AppItems/containers/mvDragPayload.h"
#include "AppItems/mvItemHandlers.h"

void mvGroup::applySpecificTemplate(mvAppItem* item)
{
	auto titem = static_cast<mvGroup*>(item);
	_horizontal = titem->_horizontal;
	_hspacing = titem->_hspacing;
}

void mvGroup::draw(ImDrawList* drawlist, float x, float y)
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

	if (config.width != 0)
		ImGui::PushItemWidth((float)config.width);

	ImGui::BeginGroup();

	for (auto& item : childslots[1])
	{
		if (config.width != 0)
			item->config.width = config.width;

		if (config.height != 0)
			item->config.height = config.height;

		item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

		if (_horizontal)
			ImGui::SameLine((1 +item->info.location)*_xoffset, _hspacing);

		if (item->config.tracked)
		{
			ImGui::SetScrollHereX(item->config.trackOffset);
			ImGui::SetScrollHereY(item->config.trackOffset);
		}
	}

	if (config.width != 0)
		ImGui::PopItemWidth();

	ImGui::EndGroup();
	UpdateAppItemState(state);

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

void mvGroup::handleSpecificKeywordArgs(PyObject* dict)
{
	if (dict == nullptr)
		return;
		 
	if (PyObject* item = PyDict_GetItemString(dict, "horizontal")) _horizontal = ToBool(item);
	if (PyObject* item = PyDict_GetItemString(dict, "horizontal_spacing")) _hspacing = ToFloat(item);
	if (PyObject* item = PyDict_GetItemString(dict, "xoffset")) _xoffset = ToFloat(item);
}

void mvGroup::getSpecificConfiguration(PyObject* dict)
{
	if (dict == nullptr)
		return;
		 
	PyDict_SetItemString(dict, "horizontal", mvPyObject(ToPyBool(_horizontal)));
	PyDict_SetItemString(dict, "horizontal_spacing", mvPyObject(ToPyFloat(_hspacing)));
	PyDict_SetItemString(dict, "xoffset", mvPyObject(ToPyFloat(_xoffset)));
}