#include <utility>
#include "mvRadioButton.h"
#include "mvContext.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"

namespace Marvel {

	void mvRadioButton::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		std::vector<mvPythonDataElement> args;

		AddCommonArgs(args,(CommonParserArgs)(
			MV_PARSER_ARG_ID |
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

		args.push_back({ mvPyDataType::StringList, "items", mvArgType::POSITIONAL_ARG, "()", "A tuple of items to be shown as radio options. Can consist of any combination of types. All types will be shown as strings." });
		args.push_back({ mvPyDataType::String, "default_value", mvArgType::KEYWORD_ARG, "''", "Default selected radio option. Set by using the string value of the item."});
		args.push_back({ mvPyDataType::Bool, "horizontal", mvArgType::KEYWORD_ARG, "False", "Displays the radio options horizontally." });

		mvPythonParserSetup setup;
		setup.about = "Adds a set of radio buttons. If items keyword is empty, nothing will be shown.";
		setup.category = { "Widgets" };
		setup.returnType = mvPyDataType::UUID;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers->insert({ s_command, parser });
	}

	mvRadioButton::mvRadioButton(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	void mvRadioButton::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvRadioButton*>(item);
		if (_source != 0) _value = titem->_value;
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

			ImGui::BeginGroup();

			ScopedID id(_uuid);

			if (!_enabled)
			{
				_disabled_value = *_value;
				_disabledindex = _index;
			}

			for (size_t i = 0; i < _itemnames.size(); i++)
			{
				if (_horizontal && i != 0)
					ImGui::SameLine();

				if (ImGui::RadioButton(_itemnames[i].c_str(), _enabled ? &_index : &_disabledindex, (int)i))
				{
					*_value = _itemnames[_index];
					_disabled_value = _itemnames[_index];
					auto value = *_value;

					if(_alias.empty())
						mvSubmitCallback([=]() {
							mvAddCallback(getCallback(false), _uuid, ToPyString(value), _user_data);
							});
					else
						mvSubmitCallback([=]() {
							mvAddCallback(getCallback(false), _alias, ToPyString(value), _user_data);
							});
				}

				if (ImGui::IsItemEdited())_state.edited = true;
			}

			ImGui::EndGroup();

		}

		//-----------------------------------------------------------------------------
		// update state
		//-----------------------------------------------------------------------------
		_state.lastFrameUpdate = GContext->frame;
		_state.hovered = ImGui::IsItemHovered();
		_state.active = ImGui::IsItemActive();
		_state.focused = ImGui::IsItemFocused();
		_state.leftclicked = ImGui::IsItemClicked();
		_state.rightclicked = ImGui::IsItemClicked(1);
		_state.middleclicked = ImGui::IsItemClicked(2);
		_state.visible = ImGui::IsItemVisible();
		_state.activated = ImGui::IsItemActivated();
		_state.deactivated = ImGui::IsItemDeactivated();
		_state.deactivatedAfterEdit = ImGui::IsItemDeactivatedAfterEdit();
		_state.toggledOpen = ImGui::IsItemToggledOpen();
		_state.rectMin = { ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y };
		_state.rectMax = { ImGui::GetItemRectMax().x, ImGui::GetItemRectMax().y };
		_state.rectSize = { ImGui::GetItemRectSize().x, ImGui::GetItemRectSize().y };
		_state.contextRegionAvail = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };

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

	void mvRadioButton::handleSpecificPositionalArgs(PyObject* dict)
	{
		if (!VerifyPositionalArguments(GetParsers()[s_command], dict))
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