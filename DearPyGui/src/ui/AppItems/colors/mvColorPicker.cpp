#include "mvColorPicker.h"
#include "mvContext.h"
#include <array>
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"
#include "AppItems/widget_handlers/mvItemHandlerRegistry.h"

namespace Marvel {

	mvColorPicker::mvColorPicker(mvUUID uuid)
		: 
		mvAppItem(uuid)
	{
	}

	void mvColorPicker::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvColorPicker*>(item);
		if (config.source != 0) _value = titem->_value;
		_flags = titem->_flags;
		_no_inputs = titem->_no_inputs;
		_no_label = titem->_no_label;
		_no_side_preview = titem->_no_side_preview;
		_alpha_bar = titem->_alpha_bar;
		_disabled_value[0] = titem->_disabled_value[0];
		_disabled_value[1] = titem->_disabled_value[1];
		_disabled_value[2] = titem->_disabled_value[2];
		_disabled_value[3] = titem->_disabled_value[3];
	}

	void mvColorPicker::draw(ImDrawList* drawlist, float x, float y)
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

			if(!config.enabled) std::copy(_value->data(), _value->data() + 4, _disabled_value);

			if (ImGui::ColorPicker4(info.internalLabel.c_str(), config.enabled ? _value->data() : &_disabled_value[0], _flags))
			{
				auto value = *_value;
				mvColor color = mvColor(value[0], value[1], value[2], value[3]);

				if(config.alias.empty())
					mvSubmitCallback([=]() {
						mvAddCallback(getCallback(false), uuid, ToPyColor(color), config.user_data);
						});
				else
					mvSubmitCallback([=]() {
						mvAddCallback(getCallback(false), config.alias, ToPyColor(color), config.user_data);
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

	void mvColorPicker::handleSpecificPositionalArgs(PyObject* dict)
	{
		if (!VerifyPositionalArguments(GetParsers()[GetEntityCommand(type)], dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				setPyValue(item);
				break;

			default:
				break;
			}
		}
	}

	PyObject* mvColorPicker::getPyValue()
	{
		// nasty hack
		int r = (int)(_value->data()[0] * 255.0f * 255.0f);
		int g = (int)(_value->data()[1] * 255.0f * 255.0f);
		int b = (int)(_value->data()[2] * 255.0f * 255.0f);
		int a = (int)(_value->data()[3] * 255.0f * 255.0f);

		auto color = mvColor(r, g, b, a);
		return ToPyColor(color);
	}

	void mvColorPicker::setPyValue(PyObject* value)
	{
		mvColor color = ToColor(value);
		std::array<float, 4> temp_array;
		temp_array[0] = color.r;
		temp_array[1] = color.g;
		temp_array[2] = color.b;
		temp_array[3] = color.a;
		if (_value)
			*_value = temp_array;
		else
			_value = std::make_shared<std::array<float, 4>>(temp_array);
	}

	void mvColorPicker::setDataSource(mvUUID dataSource)
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
		_value = *static_cast<std::shared_ptr<std::array<float, 4>>*>(item->getValue());
	}

	void mvColorPicker::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		// helpers for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};


		flagop("no_alpha", ImGuiColorEditFlags_NoAlpha, _flags);
		flagop("no_small_preview", ImGuiColorEditFlags_NoSmallPreview, _flags);
		flagop("no_inputs", ImGuiColorEditFlags_NoInputs, _flags);
		flagop("no_tooltip", ImGuiColorEditFlags_NoTooltip, _flags);
		flagop("no_label", ImGuiColorEditFlags_NoLabel, _flags);
		flagop("no_side_preview", ImGuiColorEditFlags_NoSidePreview, _flags);
		flagop("alpha_bar", ImGuiColorEditFlags_AlphaBar, _flags);
		flagop("display_rgb", ImGuiColorEditFlags_DisplayRGB, _flags);
		flagop("display_hsv", ImGuiColorEditFlags_DisplayHSV, _flags);
		flagop("display_hex", ImGuiColorEditFlags_DisplayHex, _flags);

		if (PyObject* item = PyDict_GetItemString(dict, "picker_mode"))
		{
			long mode = ToUUID(item);

			// reset target flags
			_flags &= ~ImGuiColorEditFlags_PickerHueBar;
			_flags &= ~ImGuiColorEditFlags_PickerHueWheel;

			switch (mode)
			{
			case ImGuiColorEditFlags_PickerHueWheel:
				_flags |= ImGuiColorEditFlags_PickerHueWheel;
				break;
			default:
				_flags |= ImGuiColorEditFlags_PickerHueBar;
				break;
			}
		}

		if (PyObject* item = PyDict_GetItemString(dict, "alpha_preview"))
		{
			long mode = ToUUID(item);

			// reset target flags
			_flags &= ~ImGuiColorEditFlags_AlphaPreview;
			_flags &= ~ImGuiColorEditFlags_AlphaPreviewHalf;

			switch (mode)
			{
			case ImGuiColorEditFlags_AlphaPreview:
				_flags |= ImGuiColorEditFlags_AlphaPreview;
				break;
			case ImGuiColorEditFlags_AlphaPreviewHalf:
				_flags |= ImGuiColorEditFlags_AlphaPreviewHalf;
				break;
			default:
				break;
			}
		}

		if (PyObject* item = PyDict_GetItemString(dict, "display_type"))
		{
			long mode = ToUUID(item);

			// reset target flags
			_flags &= ~ImGuiColorEditFlags_Uint8;
			_flags &= ~ImGuiColorEditFlags_Float;

			switch (mode)
			{
			case ImGuiColorEditFlags_Float:
				_flags |= ImGuiColorEditFlags_Float;
				break;
			default:
				_flags |= ImGuiColorEditFlags_Uint8;
				break;
			}
		}

		if (PyObject* item = PyDict_GetItemString(dict, "input_mode"))
		{
			long mode = ToUUID(item);

			// reset target flags
			_flags &= ~ImGuiColorEditFlags_InputRGB;
			_flags &= ~ImGuiColorEditFlags_InputHSV;

			switch (mode)
			{
			case ImGuiColorEditFlags_InputHSV:
				_flags |= ImGuiColorEditFlags_InputHSV;
				break;
			default:
				_flags |= ImGuiColorEditFlags_InputRGB;
				break;
			}
		}

	}

	void mvColorPicker::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, mvPyObject(ToPyBool(flags & flag)));
		};

		checkbitset("no_alpha", ImGuiColorEditFlags_NoAlpha, _flags);
		checkbitset("no_small_preview", ImGuiColorEditFlags_NoSmallPreview, _flags);
		checkbitset("no_inputs", ImGuiColorEditFlags_NoInputs, _flags);
		checkbitset("no_tooltip", ImGuiColorEditFlags_NoTooltip, _flags);
		checkbitset("no_label", ImGuiColorEditFlags_NoLabel, _flags);
		checkbitset("no_side_preview", ImGuiColorEditFlags_NoSidePreview, _flags);
		checkbitset("alpha_bar", ImGuiColorEditFlags_AlphaBar, _flags);
		checkbitset("display_rgb", ImGuiColorEditFlags_DisplayRGB, _flags);
		checkbitset("display_hsv", ImGuiColorEditFlags_DisplayHSV, _flags);
		checkbitset("display_hex", ImGuiColorEditFlags_DisplayHex, _flags);

		// input_mode
		if (_flags & ImGuiColorEditFlags_InputRGB)
			PyDict_SetItemString(dict, "input_mode", mvPyObject(ToPyLong(ImGuiColorEditFlags_InputRGB)));
		else if (_flags & ImGuiColorEditFlags_InputHSV)
			PyDict_SetItemString(dict, "input_mode", mvPyObject(ToPyLong(ImGuiColorEditFlags_InputHSV)));

		// alpha_preview
		if (_flags & ImGuiColorEditFlags_AlphaPreview)
			PyDict_SetItemString(dict, "alpha_preview", mvPyObject(ToPyLong(ImGuiColorEditFlags_AlphaPreview)));
		else if (_flags & ImGuiColorEditFlags_AlphaPreviewHalf)
			PyDict_SetItemString(dict, "alpha_preview", mvPyObject(ToPyLong(ImGuiColorEditFlags_AlphaPreviewHalf)));
		else
			PyDict_SetItemString(dict, "alpha_preview", mvPyObject(ToPyLong(0)));

		// display_type
		if (_flags & ImGuiColorEditFlags_Uint8)
			PyDict_SetItemString(dict, "display_type", mvPyObject(ToPyLong(ImGuiColorEditFlags_Uint8)));
		else if (_flags & ImGuiColorEditFlags_Float)
			PyDict_SetItemString(dict, "display_type", mvPyObject(ToPyLong(ImGuiColorEditFlags_Float)));

		// picker_mode
		if (_flags & ImGuiColorEditFlags_PickerHueWheel)
			PyDict_SetItemString(dict, "picker_mode", mvPyObject(ToPyLong(ImGuiColorEditFlags_PickerHueWheel)));
		else if (_flags & ImGuiColorEditFlags_PickerHueBar)
			PyDict_SetItemString(dict, "picker_mode", mvPyObject(ToPyLong(ImGuiColorEditFlags_PickerHueBar)));
	}

}