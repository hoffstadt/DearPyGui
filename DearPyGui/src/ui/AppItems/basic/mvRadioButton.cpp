#include <utility>
#include "mvRadioButton.h"
#include "mvContext.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"
#include "AppItems/widget_handlers/mvItemHandlerRegistry.h"

namespace Marvel {

	mvRadioButton::mvRadioButton(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	void mvRadioButton::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvRadioButton*>(item);
		if (config.source != 0) _value = titem->_value;
		_disabled_value = titem->_disabled_value;
		_itemnames = titem->_itemnames;
		_horizontal = titem->_horizontal;
		_index = titem->_index;
		_disabledindex = titem->_disabledindex;
	}

	void mvRadioButton::setPyValue(PyObject* value)
	{
		*_value = ToString(value);
		updateIndex();
	}

	PyObject* mvRadioButton::getPyValue()
	{
		return ToPyString(*_value);
	}

	void mvRadioButton::setDataSource(mvUUID dataSource)
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
		_value = *static_cast<std::shared_ptr<std::string>*>(item->getValue());
	}

	void mvRadioButton::updateIndex()
	{
		_index = 0;
		_disabledindex = 0;

		int index = 0;
		for (const auto& name : _itemnames)
		{
			if (name == *_value)
			{
				_index = index;
				_disabledindex = index;
				break;
			}
			index++;
		}
	}

	void mvRadioButton::draw(ImDrawList* drawlist, float x, float y)
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

			ImGui::BeginGroup();

			ScopedID id(uuid);

			if(!config.enabled)
			{
				_disabled_value = *_value;
				_disabledindex = _index;
			}

			for (size_t i = 0; i < _itemnames.size(); i++)
			{
				if (_horizontal && i != 0)
					ImGui::SameLine();

				if (ImGui::RadioButton(_itemnames[i].c_str(), config.enabled ? &_index : &_disabledindex, (int)i))
				{
					*_value = _itemnames[_index];
					_disabled_value = _itemnames[_index];
					auto value = *_value;

					if(config.alias.empty())
						mvSubmitCallback([=]() {
							mvAddCallback(getCallback(false), uuid, ToPyString(value), config.user_data);
							});
					else
						mvSubmitCallback([=]() {
							mvAddCallback(getCallback(false), config.alias, ToPyString(value), config.user_data);
							});
				}

				state.edited = ImGui::IsItemEdited();
			}

			ImGui::EndGroup();

		}

		//-----------------------------------------------------------------------------
		// update state
		//-----------------------------------------------------------------------------
		state.lastFrameUpdate = GContext->frame;
		state.hovered = ImGui::IsItemHovered();
		state.active = ImGui::IsItemActive();
		state.focused = ImGui::IsItemFocused();
		state.leftclicked = ImGui::IsItemClicked();
		state.rightclicked = ImGui::IsItemClicked(1);
		state.middleclicked = ImGui::IsItemClicked(2);
		state.visible = ImGui::IsItemVisible();
		state.activated = ImGui::IsItemActivated();
		state.deactivated = ImGui::IsItemDeactivated();
		state.deactivatedAfterEdit = ImGui::IsItemDeactivatedAfterEdit();
		state.toggledOpen = ImGui::IsItemToggledOpen();
		state.rectMin = { ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y };
		state.rectMax = { ImGui::GetItemRectMax().x, ImGui::GetItemRectMax().y };
		state.rectSize = { ImGui::GetItemRectSize().x, ImGui::GetItemRectSize().y };
		state.contextRegionAvail = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };

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

	void mvRadioButton::handleSpecificPositionalArgs(PyObject* dict)
	{
		if (!VerifyPositionalArguments(GetParsers()[GetEntityCommand(type)], dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				_itemnames = ToStringVect(item);
				break;

			default:
				break;
			}
		}
	}

	void mvRadioButton::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		if (PyObject* item = PyDict_GetItemString(dict, "items"))
		{
			_itemnames = ToStringVect(item);
			updateIndex();
		}
		if (PyObject* item = PyDict_GetItemString(dict, "horizontal")) _horizontal = ToBool(item);
	}

	void mvRadioButton::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		PyDict_SetItemString(dict, "items", mvPyObject(ToPyList(_itemnames)));
		PyDict_SetItemString(dict, "horizontal", mvPyObject(ToPyBool(_horizontal)));
	}

}