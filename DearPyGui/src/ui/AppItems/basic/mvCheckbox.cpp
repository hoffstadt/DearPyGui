#include <utility>
#include "mvCheckbox.h"
#include "mvContext.h"
#include "mvItemRegistry.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"
#include "mvPythonExceptions.h"
#include "AppItems/widget_handlers/mvItemHandlerRegistry.h"

mvCheckbox::mvCheckbox(mvUUID uuid)
	: 
	mvAppItem(uuid)
{
}

void mvCheckbox::applySpecificTemplate(mvAppItem* item)
{
	auto titem = static_cast<mvCheckbox*>(item);
	if (config.source != 0) value = titem->value;
	disabled_value = titem->disabled_value;
}

void mvCheckbox::draw(ImDrawList* drawlist, float x, float y)
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

	apply_local_theming(this);

	//-----------------------------------------------------------------------------
	// draw
	//-----------------------------------------------------------------------------
	{
		// push imgui id to prevent name collisions
		ScopedID id(uuid);

		if(!config.enabled) disabled_value = *value;

		if (ImGui::Checkbox(info.internalLabel.c_str(), config.enabled ? value.get() : &disabled_value))
		{
			bool value = *this->value;

			if(config.alias.empty())
				mvSubmitCallback([=]() {
					mvAddCallback(getCallback(false), uuid, ToPyBool(value), config.user_data);
					});
			else
				mvSubmitCallback([=]() {
				mvAddCallback(getCallback(false), config.alias, ToPyBool(value), config.user_data);
					});
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

	cleanup_local_theming(this);

	if (handlerRegistry)
		handlerRegistry->checkEvents(&state);

	// handle drag & drop if used
	apply_drag_drop(this);
}

PyObject* mvCheckbox::getPyValue()
{
	return ToPyBool(*value);
}

void mvCheckbox::setPyValue(PyObject* value)
{
	*this->value = ToBool(value);
}

void mvCheckbox::setDataSource(mvUUID dataSource)
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
	value = *static_cast<std::shared_ptr<bool>*>(item->getValue());
}