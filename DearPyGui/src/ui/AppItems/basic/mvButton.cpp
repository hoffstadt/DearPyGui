#include <utility>
#include "mvButton.h"
#include "mvCore.h"
#include "mvContext.h"
#include "mvItemRegistry.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"
#include "mvPyObject.h"
#include "AppItems/widget_handlers/mvItemHandlerRegistry.h"

mvButton::mvButton(mvUUID uuid)
	: 
	mvAppItem(uuid)
{
}

void mvButton::draw(ImDrawList* drawlist, float x, float y)
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
		// push imgui id to prevent name collisions
		ScopedID id(uuid);

		bool activated = false;

		if (configData.small_button)
			activated = ImGui::SmallButton(info.internalLabel.c_str());

		else if (configData.arrow)
			activated = ImGui::ArrowButton(info.internalLabel.c_str(), configData.direction);

		else
			activated = ImGui::Button(info.internalLabel.c_str(), ImVec2((float)config.width, (float)config.height));

		if (activated)
		{
			if (config.alias.empty())
				mvAddCallback(getCallback(false), uuid, nullptr, config.user_data);
			else
				mvAddCallback(getCallback(false), config.alias, nullptr, config.user_data);
		}
	}

	//-----------------------------------------------------------------------------
	// update state
	//-----------------------------------------------------------------------------
	UpdateAppItemState(state);

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

void mvButton::applySpecificTemplate(mvAppItem* item)
{
	auto titem = static_cast<mvButton*>(item);
	configData.direction = titem->configData.direction;
	configData.small_button = titem->configData.small_button;
	configData.arrow = titem->configData.arrow;
}

void mvButton::handleSpecificKeywordArgs(PyObject* dict)
{
	if (dict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(dict, "small")) configData.small_button = ToBool(item);
	if (PyObject* item = PyDict_GetItemString(dict, "arrow")) configData.arrow = ToBool(item);

	if (PyObject* item = PyDict_GetItemString(dict, "direction"))
	{
		configData.direction = ToInt(item);
	}

}

void mvButton::getSpecificConfiguration(PyObject* dict)
{
	if (dict == nullptr)
		return;

	mvPyObject py_small = ToPyBool(configData.small_button);
	mvPyObject py_arrow = ToPyBool(configData.arrow);
	mvPyObject py_direction = ToPyInt(configData.direction);

	PyDict_SetItemString(dict, "small", py_small);
	PyDict_SetItemString(dict, "arrow", py_arrow);
	PyDict_SetItemString(dict, "direction", py_direction);
}