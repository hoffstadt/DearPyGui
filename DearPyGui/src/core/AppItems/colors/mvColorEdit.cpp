#include "mvColorEdit.h"
#include "mvContext.h"
#include <array>
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"

namespace Marvel {

	void mvColorEdit::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		std::vector<mvPythonDataElement> args;

		AddCommonArgs(args,(CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_WIDTH |
			MV_PARSER_ARG_HEIGHT |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SOURCE |
			MV_PARSER_ARG_CALLBACK |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_ENABLED |
			MV_PARSER_ARG_FILTER |
			MV_PARSER_ARG_DROP_CALLBACK |
			MV_PARSER_ARG_DRAG_CALLBACK |
			MV_PARSER_ARG_PAYLOAD_TYPE |
			MV_PARSER_ARG_TRACKED |
			MV_PARSER_ARG_POS)
		);

		args.push_back({ mvPyDataType::IntList, "default_value", mvArgType::POSITIONAL_ARG, "(0, 0, 0, 255)"});
		args.push_back({ mvPyDataType::Bool, "no_alpha", mvArgType::KEYWORD_ARG, "False", "Removes the displayed slider that can change alpha channel." });
		args.push_back({ mvPyDataType::Bool, "no_picker", mvArgType::KEYWORD_ARG, "False", "Disable picker popup when color square is clicked." });
		args.push_back({ mvPyDataType::Bool, "no_options", mvArgType::KEYWORD_ARG, "False", "Disable toggling options menu when right-clicking on inputs/small preview." });
		args.push_back({ mvPyDataType::Bool, "no_small_preview", mvArgType::KEYWORD_ARG, "False", "Disable colored square preview next to the inputs. (e.g. to show only the inputs). This only displays if the side preview is not shown." });
		args.push_back({ mvPyDataType::Bool, "no_inputs", mvArgType::KEYWORD_ARG, "False", "Disable inputs sliders/text widgets. (e.g. to show only the small preview colored square)" });
		args.push_back({ mvPyDataType::Bool, "no_tooltip", mvArgType::KEYWORD_ARG, "False", "Disable tooltip when hovering the preview." });
		args.push_back({ mvPyDataType::Bool, "no_label", mvArgType::KEYWORD_ARG, "False", "Disable display of inline text label." });
		args.push_back({ mvPyDataType::Bool, "no_drag_drop", mvArgType::KEYWORD_ARG, "False", "Disable ability to drag and drop small preview (color square) to apply colors to other items." });
		args.push_back({ mvPyDataType::Bool, "alpha_bar", mvArgType::KEYWORD_ARG, "False", "Show vertical alpha bar/gradient in picker." });
		args.push_back({ mvPyDataType::Long, "alpha_preview", mvArgType::KEYWORD_ARG, "0", "mvColorEdit_AlphaPreviewNone, mvColorEdit_AlphaPreview, or mvColorEdit_AlphaPreviewHalf" });
		args.push_back({ mvPyDataType::Long, "display_mode", mvArgType::KEYWORD_ARG, "1048576", "mvColorEdit_rgb, mvColorEdit_hsv, or mvColorEdit_hex" });
		args.push_back({ mvPyDataType::Long, "display_type", mvArgType::KEYWORD_ARG, "8388608", "mvColorEdit_uint8 or mvColorEdit_float" });
		args.push_back({ mvPyDataType::Long, "input_mode", mvArgType::KEYWORD_ARG, "134217728", "mvColorEdit_input_rgb or mvColorEdit_input_hsv" });

		mvPythonParserSetup setup;
		setup.about = "Adds an RGBA color editor. Left clicking the small color preview will provide a color picker. Click and draging the small color preview will copy the color to be applied on any other color widget.";
		setup.category = { "Widgets", "Colors" };
		setup.returnType = mvPyDataType::UUID;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers->insert({ s_command, parser });
	
	}

	mvColorEdit::mvColorEdit(mvUUID uuid)
		: 
		mvAppItem(uuid)
	{
	}

	void mvColorEdit::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvColorEdit*>(item);
		if (_source != 0) _value = titem->_value;
		_flags = titem->_flags;
		_no_picker = titem->_no_picker;
		_no_options = titem->_no_options;
		_no_inputs = titem->_no_inputs;
		_no_label = titem->_no_label;
		_alpha_bar = titem->_alpha_bar;
		_disabled_value[0] = titem->_disabled_value[0];
		_disabled_value[1] = titem->_disabled_value[1];
		_disabled_value[2] = titem->_disabled_value[2];
		_disabled_value[3] = titem->_disabled_value[3];
	}

	PyObject* mvColorEdit::getPyValue()
	{
		// nasty hack
		int r = (int)(_value->data()[0] * 255.0f * 255.0f);
		int g = (int)(_value->data()[1] * 255.0f * 255.0f);
		int b = (int)(_value->data()[2] * 255.0f * 255.0f);
		int a = (int)(_value->data()[3] * 255.0f * 255.0f);

		auto color = mvColor(r, g, b, a);
		return ToPyColor(color);
	}

	void mvColorEdit::setPyValue(PyObject* value)
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

	void mvColorEdit::setDataSource(mvUUID dataSource)
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
		if (GetEntityValueType(item->getType()) != GetEntityValueType(getType()))
		{
			mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
				"Values types do not match: " + std::to_string(dataSource), this);
			return;
		}
		_value = *static_cast<std::shared_ptr<std::array<float, 4>>*>(item->getValue());
	}

	void mvColorEdit::draw(ImDrawList* drawlist, float x, float y)
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

		// themes
		if (_enabled)
		{
			if (auto classTheme = getClassThemeComponent())
				static_cast<mvThemeComponent*>(classTheme.get())->draw(nullptr, 0.0f, 0.0f);
		}
		else
		{
			if (auto classTheme = getClassDisabledThemeComponent())
				static_cast<mvThemeComponent*>(classTheme.get())->draw(nullptr, 0.0f, 0.0f);
		}

		if (_theme)
		{
			static_cast<mvTheme*>(_theme.get())->setSpecificEnabled(_enabled);
			static_cast<mvTheme*>(_theme.get())->setSpecificType((int)getType());
			static_cast<mvTheme*>(_theme.get())->draw(nullptr, 0.0f, 0.0f);
		}

		//-----------------------------------------------------------------------------
		// draw
		//-----------------------------------------------------------------------------
		{
			ScopedID id(_uuid);

			if (!_enabled) std::copy(_value->data(), _value->data() + 4, _disabled_value);

			if (ImGui::ColorEdit4(_internalLabel.c_str(), _enabled ? _value->data() : &_disabled_value[0], _flags))
			{
				auto value = *_value;
				mvColor color = mvColor(value[0], value[1], value[2], value[3]);

				if(_alias.empty())
					mvSubmitCallback([=]() {
						mvAddCallback(getCallback(false), _uuid, ToPyColor(color), _user_data);
						});
				else
					mvSubmitCallback([=]() {
						mvAddCallback(getCallback(false), _alias, ToPyColor(color), _user_data);
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

		// handle popping themes
		if (_enabled)
		{
			if (auto classTheme = getClassThemeComponent())
				static_cast<mvThemeComponent*>(classTheme.get())->customAction();
		}
		else
		{
			if (auto classTheme = getClassDisabledThemeComponent())
				static_cast<mvThemeComponent*>(classTheme.get())->customAction();
		}

		if (_theme)
		{
			static_cast<mvTheme*>(_theme.get())->setSpecificEnabled(_enabled);
			static_cast<mvTheme*>(_theme.get())->setSpecificType((int)getType());
			static_cast<mvTheme*>(_theme.get())->customAction();
		}

		if (_handlerRegistry)
			_handlerRegistry->customAction(&_state);

		// handle drag & drop payloads
		for (auto& item : _children[3])
			item->draw(nullptr, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

		// handle drag & drop if used
		if (_dropCallback)
		{
			ScopedID id(_uuid);
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(_payloadType.c_str()))
				{
					auto payloadActual = static_cast<const mvDragPayload*>(payload->Data);
					if (_alias.empty())
						mvAddCallback(_dropCallback,_uuid, payloadActual->getDragData(), nullptr);
					else
						mvAddCallback(_dropCallback,_alias, payloadActual->getDragData(), nullptr);
				}

				ImGui::EndDragDropTarget();
			}
		}
	}

	void mvColorEdit::handleSpecificPositionalArgs(PyObject* dict)
	{
		if (!VerifyPositionalArguments(GetParsers()[s_command], dict))
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

	void mvColorEdit::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		// helpers for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		flagop("no_alpha", ImGuiColorEditFlags_NoAlpha, _flags);
		flagop("no_picker", ImGuiColorEditFlags_NoPicker, _flags);
		flagop("no_options", ImGuiColorEditFlags_NoOptions, _flags);
		flagop("no_small_preview", ImGuiColorEditFlags_NoSmallPreview, _flags);
		flagop("no_inputs", ImGuiColorEditFlags_NoInputs, _flags);
		flagop("no_tooltip", ImGuiColorEditFlags_NoTooltip, _flags);
		flagop("no_label", ImGuiColorEditFlags_NoLabel, _flags);
		flagop("no_side_preview", ImGuiColorEditFlags_NoSidePreview, _flags);
		flagop("no_drag_drop", ImGuiColorEditFlags_NoDragDrop, _flags);
		flagop("alpha_bar", ImGuiColorEditFlags_AlphaBar, _flags);

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

		if (PyObject* item = PyDict_GetItemString(dict, "display_mode"))
		{
			long mode = ToUUID(item);

			// reset target flags
			_flags &= ~ImGuiColorEditFlags_DisplayRGB;
			_flags &= ~ImGuiColorEditFlags_DisplayHSV;
			_flags &= ~ImGuiColorEditFlags_DisplayHex;

			switch (mode)
			{
			case ImGuiColorEditFlags_DisplayHex:
				_flags |= ImGuiColorEditFlags_DisplayHex;
				break;
			case ImGuiColorEditFlags_DisplayHSV:
				_flags |= ImGuiColorEditFlags_DisplayHSV;
				break;
			default:
				_flags |= ImGuiColorEditFlags_DisplayRGB;
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
			long mode = (long)ToUUID(item);

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

	void mvColorEdit::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, mvPyObject(ToPyBool(flags & flag)));
		};

		checkbitset("no_alpha", ImGuiColorEditFlags_NoAlpha, _flags);
		checkbitset("no_picker", ImGuiColorEditFlags_NoPicker, _flags);
		checkbitset("no_options", ImGuiColorEditFlags_NoOptions, _flags);
		checkbitset("no_small_preview", ImGuiColorEditFlags_NoSmallPreview, _flags);
		checkbitset("no_inputs", ImGuiColorEditFlags_NoInputs, _flags);
		checkbitset("no_tooltip", ImGuiColorEditFlags_NoTooltip, _flags);
		checkbitset("no_label", ImGuiColorEditFlags_NoLabel, _flags);
		checkbitset("no_drag_drop", ImGuiColorEditFlags_NoDragDrop, _flags);
		checkbitset("alpha_bar", ImGuiColorEditFlags_AlphaBar, _flags);

		// input_mode
		if (_flags & ImGuiColorEditFlags_InputRGB)
			PyDict_SetItemString(dict, "input_mode", mvPyObject(ToPyLong(ImGuiColorEditFlags_InputRGB)));
		else if (_flags & ImGuiColorEditFlags_InputHSV)
			PyDict_SetItemString(dict, "input_mode", mvPyObject(ToPyLong(ImGuiColorEditFlags_InputHSV)));

		// alpha_preview
		if(_flags & ImGuiColorEditFlags_AlphaPreview)
			PyDict_SetItemString(dict, "alpha_preview", mvPyObject(ToPyLong(ImGuiColorEditFlags_AlphaPreview)));
		else if (_flags & ImGuiColorEditFlags_AlphaPreviewHalf)
			PyDict_SetItemString(dict, "alpha_preview", mvPyObject(ToPyLong(ImGuiColorEditFlags_AlphaPreviewHalf)));
		else
			PyDict_SetItemString(dict, "alpha_preview", mvPyObject(ToPyLong(0l)));

		// display_mode
		if (_flags & ImGuiColorEditFlags_DisplayHSV)
			PyDict_SetItemString(dict, "display_mode", mvPyObject(ToPyLong(ImGuiColorEditFlags_DisplayHSV)));
		else if (_flags & ImGuiColorEditFlags_DisplayHex)
			PyDict_SetItemString(dict, "display_mode", mvPyObject(ToPyLong(ImGuiColorEditFlags_DisplayHex)));
		else
			PyDict_SetItemString(dict, "display_mode", mvPyObject(ToPyLong(ImGuiColorEditFlags_DisplayRGB)));

		// display_type
		if (_flags & ImGuiColorEditFlags_Uint8)
			PyDict_SetItemString(dict, "display_type", mvPyObject(ToPyLong(ImGuiColorEditFlags_Uint8)));
		else if (_flags & ImGuiColorEditFlags_Float)
			PyDict_SetItemString(dict, "display_type", mvPyObject(ToPyLong(ImGuiColorEditFlags_Float)));

	}

}
