#include <utility>
#include "mvSelectable.h"
#include "mvContext.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"
#include "AppItems/widget_handlers/mvItemHandlerRegistry.h"

namespace Marvel {

	mvSelectable::mvSelectable(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	void mvSelectable::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvSelectable*>(item);
		if (config.source != 0) _value = titem->_value;
		_disabled_value = titem->_disabled_value;
		_flags = titem->_flags;
	}

	PyObject* mvSelectable::getPyValue()
	{
		return ToPyBool(*_value);
	}

	void mvSelectable::setPyValue(PyObject* value)
	{
		*_value = ToBool(value);
	}

	void mvSelectable::setDataSource(mvUUID dataSource)
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

	void mvSelectable::draw(ImDrawList* drawlist, float x, float y)
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

			if (ImGui::Selectable(info.internalLabel.c_str(), _value.get(), _flags, ImVec2((float)config.width, (float)config.height)))
			{
				auto value = *_value;

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

		// handle popping themes
		cleanup_local_theming(this);

		if (handlerRegistry)
			handlerRegistry->checkEvents(&state);

		// handle drag & drop if used
		apply_drag_drop(this);
	}

	void mvSelectable::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 

		// helper for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags, bool flip)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		// window flags
		flagop("span_columns", ImGuiSelectableFlags_SpanAllColumns, _flags, false);

		if (info.enabledLastFrame)
		{
			info.enabledLastFrame = false;
			_flags &= ~ImGuiSelectableFlags_Disabled;
		}

		if (info.disabledLastFrame)
		{
			info.disabledLastFrame = false;
			_flags |= ImGuiSelectableFlags_Disabled;
		}

	}

	void mvSelectable::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags, bool flip)
		{
			PyDict_SetItemString(dict, keyword, mvPyObject(ToPyBool(flags & flag)));
		};

		// window flags
		checkbitset("span_columns", ImGuiSelectableFlags_SpanAllColumns, _flags, false);
	}

}