#include "mvMenu.h"
#include "mvContext.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/mvFontItems.h"
#include "AppItems/mvThemes.h"
#include "AppItems/containers/mvDragPayload.h"
#include "AppItems/mvItemHandlers.h"

void mvMenu::draw(ImDrawList* drawlist, float x, float y)
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

		// create menu and see if its selected
		if (ImGui::BeginMenu(info.internalLabel.c_str(), config.enabled))
		{
			state.lastFrameUpdate = GContext->frame;
			state.active = ImGui::IsItemActive();
			state.activated = ImGui::IsItemActivated();
			state.deactivated = ImGui::IsItemDeactivated();
			state.focused = ImGui::IsWindowFocused();
			state.hovered = ImGui::IsWindowHovered();
			state.rectSize = { ImGui::GetWindowWidth(), ImGui::GetWindowHeight() };
			state.contextRegionAvail = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };

			// set other menus's value false on same level
			for (auto& sibling : info.parentPtr->childslots[1])
			{
				// ensure sibling
				if (sibling->type == mvAppItemType::mvMenu)
					*((mvMenu*)sibling.get())->_value = false;
			}

			// set current menu value true
			*_value = true;

			for (auto& item : childslots[1])
				item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

			if (ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows))
			{

				// update mouse
				ImVec2 mousePos = ImGui::GetMousePos();
				float x = mousePos.x - ImGui::GetWindowPos().x;
				float y = mousePos.y - ImGui::GetWindowPos().y;
				GContext->input.mousePos.x = (int)x;
				GContext->input.mousePos.y = (int)y;


				if (GContext->itemRegistry->activeWindow != uuid)
					GContext->itemRegistry->activeWindow = uuid;


			}

			ImGui::EndMenu();
		}
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

void mvMenu::handleSpecificKeywordArgs(PyObject* dict)
{
	if (dict == nullptr)
		return;
		 
	if (PyObject* item = PyDict_GetItemString(dict, "enabled")) config.enabled = ToBool(item);

}

void mvMenu::getSpecificConfiguration(PyObject* dict)
{
	if (dict == nullptr)
		return;
		 
	PyDict_SetItemString(dict, "enabled", mvPyObject(ToPyBool(config.enabled)));
}

PyObject* mvMenu::getPyValue()
{
	return ToPyBool(*_value);
}

void mvMenu::setPyValue(PyObject* value)
{
	*_value = ToBool(value);
}

void mvMenu::setDataSource(mvUUID dataSource)
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
	if (DearPyGui::GetEntityValueType(item->type) != DearPyGui::GetEntityValueType(type))
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
			"Values types do not match: " + std::to_string(dataSource), this);
		return;
	}
	_value = *static_cast<std::shared_ptr<bool>*>(item->getValue());
}