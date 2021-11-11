#include "mvText.h"
#include "mvContext.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "fonts/mvFont.h"
#include "themes/mvTheme.h"
#include "containers/mvDragPayload.h"

namespace Marvel {

	void mvText::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		std::vector<mvPythonDataElement> args;

		AddCommonArgs(args,(CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SOURCE |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_FILTER |
			MV_PARSER_ARG_DROP_CALLBACK |
			MV_PARSER_ARG_DRAG_CALLBACK |
			MV_PARSER_ARG_PAYLOAD_TYPE |
			MV_PARSER_ARG_TRACKED |
			MV_PARSER_ARG_POS)
		);

		args.push_back({ mvPyDataType::String, "default_value", mvArgType::POSITIONAL_ARG, "''" });
		args.push_back({ mvPyDataType::Integer, "wrap", mvArgType::KEYWORD_ARG, "-1", "Number of pixels from the start of the item until wrapping starts." });
		args.push_back({ mvPyDataType::Bool, "bullet", mvArgType::KEYWORD_ARG, "False", "Places a bullet to the left of the text." });
		args.push_back({ mvPyDataType::IntList, "color", mvArgType::KEYWORD_ARG, "(-255, 0, 0, 255)", "Color of the text (rgba)." });
		args.push_back({ mvPyDataType::Bool, "show_label", mvArgType::KEYWORD_ARG, "False", "Displays the label to the right of the text." });

		mvPythonParserSetup setup;
		setup.about = "Adds text. Text can have an optional label that will display to the right of the text.";
		setup.category = { "Widgets" };
		setup.returnType = mvPyDataType::UUID;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers->insert({ s_command, parser });
	}

	mvText::mvText(mvUUID uuid)
		: 
		mvAppItem(uuid)
	{
		*_value = "Not Specified";
	}

	void mvText::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvText*>(item);
		if (_source != 0) _value = titem->_value;
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
		if (!_show)
			return;

		if (_focusNextFrame)
		{
			ImGui::SetKeyboardFocusHere();
			_focusNextFrame = false;
		}

		_previousCursorPos = ImGui::GetCursorPos();
		if (_dirtyPos)
			ImGui::SetCursorPos(_state.pos);

		_state.pos = { ImGui::GetCursorPosX(), ImGui::GetCursorPosY() };

		// set item width
		if (_width != 0)
			ImGui::SetNextItemWidth((float)_width);

		if (_indent > 0.0f)
			ImGui::Indent(_indent);

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
			// this fixes the vertical text alignment issue according it DearImGui issue #2317
			ImGui::AlignTextToFramePadding();

			const ImGuiStyle& style = ImGui::GetStyle();
			const float w = ImGui::CalcItemWidth();
			const float textVertCenter = ImGui::GetCursorPosY();
			const float valueEndX = ImGui::GetCursorPosX() + w;

			if (_color.r >= 0.0f)
				ImGui::PushStyleColor(ImGuiCol_Text, _color.toVec4());

			if (_wrap >= 0)
				ImGui::PushTextWrapPos((float)_wrap);

			if (_bullet)
				ImGui::Bullet();

			//ImGui::Text("%s", _value.c_str());
			ImGui::TextUnformatted(_value->c_str()); // this doesn't have a buffer size limit

			_state.lastFrameUpdate = GContext->frame;
			_state.visible = ImGui::IsItemVisible();
			_state.hovered = ImGui::IsItemHovered();
			_state.leftclicked = ImGui::IsItemClicked(0);
			_state.rightclicked = ImGui::IsItemClicked(1);
			 _state.middleclicked = ImGui::IsItemClicked(2);
			 _state.contextRegionAvail = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };

			if (_wrap >= 0)
				ImGui::PopTextWrapPos();

			if (_color.r >= 0.0f)
				ImGui::PopStyleColor();

			if (_show_label)
			{
				ImGui::SameLine();
				ImGui::SetCursorPos({ valueEndX + style.ItemInnerSpacing.x, textVertCenter });
				ImGui::TextUnformatted(_specifiedLabel.c_str());

				_state.visible = ImGui::IsItemVisible();
				_state.hovered = ImGui::IsItemHovered();
				_state.leftclicked = ImGui::IsItemClicked(0);
				_state.rightclicked = ImGui::IsItemClicked(1);
				_state.middleclicked = ImGui::IsItemClicked(2);
			}
		}

		//-----------------------------------------------------------------------------
		// postdraw
		//-----------------------------------------------------------------------------
		if (_dirtyPos)
			ImGui::SetCursorPos(_previousCursorPos);

		if (_indent > 0.0f)
			ImGui::Unindent(_indent);

		if (_font)
		{
			ImGui::PopFont();
		}

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

	void mvText::handleSpecificPositionalArgs(PyObject* dict)
	{
		if (!VerifyPositionalArguments(GetParsers()[s_command], dict))
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
		if (dataSource == _source) return;
		_source = dataSource;

		mvAppItem* item = GetItem((*GContext->itemRegistry), dataSource);
		if (!item)
		{
			mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
				"Source item not found: " + std::to_string(dataSource), this);
			return;
		}
		if (item->getValueType() != getValueType())
		{
			mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
				"Values types do not match: " + std::to_string(dataSource), this);
			return;
		}
		_value = *static_cast<std::shared_ptr<std::string>*>(item->getValue());
	}
}