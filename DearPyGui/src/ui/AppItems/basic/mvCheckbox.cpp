#include <utility>
#include "mvCheckbox.h"
#include "mvContext.h"
#include "mvItemRegistry.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	mvCheckbox::mvCheckbox(mvUUID uuid)
		: 
		mvAppItem(uuid)
	{
	}

	void mvCheckbox::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvCheckbox*>(item);
		if(_source != 0) _value = titem->_value;
		_disabled_value = titem->_disabled_value;
	}

	void mvCheckbox::draw(ImDrawList* drawlist, float x, float y)
	{

		//-----------------------------------------------------------------------------
		// pre draw
		//-----------------------------------------------------------------------------

		// show/hide
		if (!_show)
			return;

		// focusing
		if (_focusNextFrame)
		{
			ImGui::SetKeyboardFocusHere();
			_focusNextFrame = false;
		}

		// cache old cursor position
		ImVec2 previousCursorPos = ImGui::GetCursorPos();

		// set cursor position if user set
		if (_dirtyPos)
			ImGui::SetCursorPos(_state.pos);

		// update widget's position state
		_state.pos = { ImGui::GetCursorPosX(), ImGui::GetCursorPosY() };

		// set item width
		if (_width != 0)
			ImGui::SetNextItemWidth((float)_width);

		// set indent
		if (_indent > 0.0f)
			ImGui::Indent(_indent);

		// push font if a font object is attached
		if (_font)
		{
			ImFont* fontptr = static_cast<mvFont*>(_font.get())->getFontPtr();
			ImGui::PushFont(fontptr);
		}

		apply_local_theming(this);

		//-----------------------------------------------------------------------------
		// draw
		//-----------------------------------------------------------------------------
		{
			// push imgui id to prevent name collisions
			ScopedID id(_uuid);

			if (!_enabled) _disabled_value = *_value;

			if (ImGui::Checkbox(_internalLabel.c_str(), _enabled ? _value.get() : &_disabled_value))
			{
				bool value = *_value;

				if(_alias.empty())
					mvSubmitCallback([=]() {
						mvAddCallback(getCallback(false), _uuid, ToPyBool(value), _user_data);
						});
				else
					mvSubmitCallback([=]() {
					mvAddCallback(getCallback(false), _alias, ToPyBool(value), _user_data);
						});
			}
		}

		//-----------------------------------------------------------------------------
		// update state
		//-----------------------------------------------------------------------------
		UpdateAppItemState(_state);

		//-----------------------------------------------------------------------------
		// post draw
		//-----------------------------------------------------------------------------

		// set cursor position to cached position
		if (_dirtyPos)
			ImGui::SetCursorPos(previousCursorPos);

		if (_indent > 0.0f)
			ImGui::Unindent(_indent);

		// pop font off stack
		if (_font)
			ImGui::PopFont();

		cleanup_local_theming(this);

		if (_handlerRegistry)
			_handlerRegistry->customAction(&_state);

		// handle drag & drop if used
		apply_drag_drop(this);
	}

	PyObject* mvCheckbox::getPyValue()
	{
		return ToPyBool(*_value);
	}

	void mvCheckbox::setPyValue(PyObject* value)
	{
		*_value = ToBool(value);
	}

	void mvCheckbox::setDataSource(mvUUID dataSource)
	{
		if (dataSource == _source) return;
		_source = dataSource;

		mvAppItem* item = GetItem((*GContext->itemRegistry), dataSource);
		if (!item)
		{
			mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
				"Source item not found: " + std::to_string(dataSource), this);
			return;
		}
		if (GetEntityValueType(item->_type) != GetEntityValueType(_type))
		{
			mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
				"Values types do not match: " + std::to_string(dataSource), this);
			return;
		}
		_value = *static_cast<std::shared_ptr<bool>*>(item->getValue());
	}

}