#include "mvText.h"
#include "mvContext.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "fonts/mvFont.h"
#include "themes/mvTheme.h"
#include "containers/mvDragPayload.h"
#include "AppItems/widget_handlers/mvItemHandlerRegistry.h"

namespace Marvel {

	mvText::mvText(mvUUID uuid)
		: 
		mvAppItem(uuid)
	{
		*_value = "Not Specified";
	}

	void mvText::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvText*>(item);
		if (config.source != 0) _value = titem->_value;
		_disabled_value = titem->_disabled_value;
		_color = titem->_color;
		_wrap = titem->_wrap;
		_bullet = titem->_bullet;
		_show_label = titem->_show_label;
	}

	void mvText::draw(ImDrawList* drawlist, float x, float y)
	{
		//-----------------------------------------------------------------------------
		// predraw
		//-----------------------------------------------------------------------------
		if (!config.show)
			return;

		if (info.focusNextFrame)
		{
			ImGui::SetKeyboardFocusHere();
			info.focusNextFrame = false;
		}

		info.previousCursorPos = ImGui::GetCursorPos();
		if (info.dirtyPos)
			ImGui::SetCursorPos(state.pos);

		state.pos = { ImGui::GetCursorPosX(), ImGui::GetCursorPosY() };

		// set item width
		if (config.width != 0)
			ImGui::SetNextItemWidth((float)config.width);

		if (config.indent > 0.0f)
			ImGui::Indent(config.indent);

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
			// this fixes the vertical text alignment issue according it DearImGui issue #2317
			ImGui::AlignTextToFramePadding();

			const ImGuiStyle& style = ImGui::GetStyle();
			const float w = ImGui::CalcItemWidth();
			const float textVertCenter = ImGui::GetCursorPosY();
			const float valueEndX = ImGui::GetCursorPosX() + w;

			if (_color.r >= 0.0f)
				ImGui::PushStyleColor(ImGuiCol_Text, _color.toVec4());

			if (_wrap == 0)
				ImGui::PushTextWrapPos((float)_wrap);
			else if (_wrap > 0)
				ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + (float)_wrap);

			if (_bullet)
				ImGui::Bullet();

			//ImGui::Text("%s", _value.c_str());
			ImGui::TextUnformatted(_value->c_str()); // this doesn't have a buffer size limit

			state.lastFrameUpdate = GContext->frame;
			state.visible = ImGui::IsItemVisible();
			state.hovered = ImGui::IsItemHovered();
			state.leftclicked = ImGui::IsItemClicked(0);
			state.rightclicked = ImGui::IsItemClicked(1);
			state.middleclicked = ImGui::IsItemClicked(2);
			state.contextRegionAvail = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };

			if (_wrap >= 0)
				ImGui::PopTextWrapPos();

			if (_color.r >= 0.0f)
				ImGui::PopStyleColor();

			if (_show_label)
			{
				ImGui::SameLine();
				ImGui::SetCursorPos({ valueEndX + style.ItemInnerSpacing.x, textVertCenter });
				ImGui::TextUnformatted(config.specifiedLabel.c_str());

				state.visible = ImGui::IsItemVisible();
				state.hovered = ImGui::IsItemHovered();
				state.leftclicked = ImGui::IsItemClicked(0);
				state.rightclicked = ImGui::IsItemClicked(1);
				state.middleclicked = ImGui::IsItemClicked(2);
			}
		}

		//-----------------------------------------------------------------------------
		// postdraw
		//-----------------------------------------------------------------------------
		if (info.dirtyPos)
			ImGui::SetCursorPos(info.previousCursorPos);

		if (config.indent > 0.0f)
			ImGui::Unindent(config.indent);

		if (font)
		{
			ImGui::PopFont();
		}

		// handle popping themes
		cleanup_local_theming(this);

		if (handlerRegistry)
			handlerRegistry->checkEvents(&state);

		// handle drag & drop payloads
		apply_drag_drop(this);
	}

	void mvText::handleSpecificPositionalArgs(PyObject* dict)
	{
		if (!VerifyPositionalArguments(GetParsers()[GetEntityCommand(type)], dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				*_value = ToString(item);
				break;

			default:
				break;
			}
		}
	}

	void mvText::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "color")) _color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "wrap")) _wrap = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "bullet")) _bullet = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "show_label")) _show_label = ToBool(item);

	}

	void mvText::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "color", mvPyObject(ToPyColor(_color)));
		PyDict_SetItemString(dict, "wrap", mvPyObject(ToPyInt(_wrap)));
		PyDict_SetItemString(dict, "bullet", mvPyObject(ToPyBool(_bullet)));
		PyDict_SetItemString(dict, "show_label", mvPyObject(ToPyBool(_show_label)));
	}

	PyObject* mvText::getPyValue()
	{
		return ToPyString(*_value);
	}

	void mvText::setPyValue(PyObject* value)
	{
		*_value = ToString(value);
	}

	void mvText::setDataSource(mvUUID dataSource)
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
}