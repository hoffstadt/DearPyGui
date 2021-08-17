#include <utility>
#include "mvCombo.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"
#include "mvPyObject.h"

namespace Marvel {

	void mvCombo::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Adds a combo dropdown that allows a user to select a single option from a drop down window.", { "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_WIDTH |
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

		parser.addArg<mvPyDataType::StringList>("items", mvArgType::POSITIONAL_ARG, "()", "A tuple of items to be shown in the drop down window. Can consist of any combination of types.");

		parser.addArg<mvPyDataType::String>("default_value", mvArgType::KEYWORD_ARG, "''");
		parser.addArg<mvPyDataType::Bool>("popup_align_left", mvArgType::KEYWORD_ARG, "False", "Align the popup toward the left.");
		parser.addArg<mvPyDataType::Bool>("no_arrow_button", mvArgType::KEYWORD_ARG, "False", "Display the preview box without the square arrow button.");
		parser.addArg<mvPyDataType::Bool>("no_preview", mvArgType::KEYWORD_ARG, "False", "Display only the square arrow button.");

		parser.addArg<mvPyDataType::Long>("height_mode", mvArgType::KEYWORD_ARG, "1", "mvComboHeight_Small, _Regular, _Large, _Largest");


		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvCombo::mvCombo(mvUUID uuid)
		: 
		mvAppItem(uuid)
	{
	}

	PyObject* mvCombo::getPyValue()
	{
		return ToPyString(*_value);
	}

	void mvCombo::setPyValue(PyObject* value)
	{
		*_value = ToString(value);
	}

	void mvCombo::setDataSource(mvUUID dataSource)
	{
		if (dataSource == _source) return;
		_source = dataSource;

		mvAppItem* item = mvApp::GetApp()->getItemRegistry().getItem(dataSource);
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

	void mvCombo::draw(ImDrawList* drawlist, float x, float y)
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
			ImGui::SetCursorPos(_state.getItemPos());

		// update widget's position state
		_state.setPos({ ImGui::GetCursorPosX(), ImGui::GetCursorPosY() });

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

		// handle enabled theming
		if (_enabled)
		{
			// push class theme (if it exists)
			if (auto classTheme = getClassTheme())
				static_cast<mvTheme*>(classTheme.get())->draw(nullptr, 0.0f, 0.0f);

			// push item theme (if it exists)
			if (_theme)
				static_cast<mvTheme*>(_theme.get())->draw(nullptr, 0.0f, 0.0f);
		}

		// handled disabled theming
		else
		{
			// push class theme (if it exists)
			if (auto classTheme = getClassDisabledTheme())
				static_cast<mvTheme*>(classTheme.get())->draw(nullptr, 0.0f, 0.0f);

			// push item theme (if it exists)
			if (_disabledTheme)
				static_cast<mvTheme*>(_disabledTheme.get())->draw(nullptr, 0.0f, 0.0f);
		}

		//-----------------------------------------------------------------------------
		// draw
		//-----------------------------------------------------------------------------
		{
			ScopedID id(_uuid);

			static std::vector<std::string> disabled_items{};

			// The second parameter is the label previewed before opening the combo.
			if (ImGui::BeginCombo(_internalLabel.c_str(), _value->c_str(), _flags))
			{
				for (const auto& name : _enabled ? _items : disabled_items)
				{
					bool is_selected = (*_value == name);
					if (ImGui::Selectable((name).c_str(), is_selected))
					{
						if (_enabled) { *_value = name; }

						auto value = *_value;

						if(_alias.empty())
							mvApp::GetApp()->getCallbackRegistry().submitCallback([=]() {
								mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), _uuid, ToPyString(value), _user_data);
								});
						else
							mvApp::GetApp()->getCallbackRegistry().submitCallback([=]() {
								mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), _alias, ToPyString(value), _user_data);
									});


					}

					// Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}
		}

		//-----------------------------------------------------------------------------
		// update state
		//   * only update if applicable
		//-----------------------------------------------------------------------------
		_state._lastFrameUpdate = mvApp::s_frame;
		_state._hovered = ImGui::IsItemHovered();
		_state._active = ImGui::IsItemActive();
		_state._focused = ImGui::IsItemFocused();
		_state._leftclicked = ImGui::IsItemClicked();
		_state._rightclicked = ImGui::IsItemClicked(1);
		_state._middleclicked = ImGui::IsItemClicked(2);
		_state._visible = ImGui::IsItemVisible();
		_state._edited = ImGui::IsItemEdited();
		_state._activated = ImGui::IsItemActivated();
		_state._deactivated = ImGui::IsItemDeactivated();
		_state._rectMin = { ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y };
		_state._rectMax = { ImGui::GetItemRectMax().x, ImGui::GetItemRectMax().y };
		_state._rectSize = { ImGui::GetItemRectSize().x, ImGui::GetItemRectSize().y };
		_state._contextRegionAvail = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };

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

		// handle popping styles
		if (_enabled)
		{
			if (auto classTheme = getClassTheme())
				static_cast<mvTheme*>(classTheme.get())->customAction();

			if (_theme)
				static_cast<mvTheme*>(_theme.get())->customAction();
		}
		else
		{
			if (auto classTheme = getClassDisabledTheme())
				static_cast<mvTheme*>(classTheme.get())->customAction();

			if (_disabledTheme)
				static_cast<mvTheme*>(_disabledTheme.get())->customAction();
		}

		// handle widget's event handlers
		for (auto& item : _children[3])
		{
			if (!item->preDraw())
				continue;

			item->draw(nullptr, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
		}

		// handle drag & drop payloads
		for (auto& item : _children[4])
		{
			if (!item->preDraw())
				continue;

			item->draw(nullptr, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
		}

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
						mvApp::GetApp()->getCallbackRegistry().addCallback(getDropCallback(), _uuid, payloadActual->getDragData(), _user_data);
					else
						mvApp::GetApp()->getCallbackRegistry().addCallback(getDropCallback(), _alias, payloadActual->getDragData(), _user_data);
				}

				ImGui::EndDragDropTarget();
			}
		}
	}

	void mvCombo::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvCombo*>(item);
		if(_source != 0) _value = titem->_value;
		_disabled_value = titem->_disabled_value;
		_flags = titem->_flags;
		_items = titem->_items;
		_popup_align_left = titem->_popup_align_left;
		_no_preview = titem->_no_preview;
	}

	void mvCombo::handleSpecificPositionalArgs(PyObject* dict)
	{
		if (!mvApp::GetApp()->getParsers()[s_command].verifyPositionalArguments(dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				_items = ToStringVect(item);
				break;

			default:
				break;
			}
		}
	}

	void mvCombo::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "items")) _items = ToStringVect(item);

		if (PyObject* item = PyDict_GetItemString(dict, "height_mode"))
		{
			long height_mode = ToUUID(item);

			if (height_mode == (long)mvCombo::ComboHeightMode::mvComboHeight_Small)
				_flags = ImGuiComboFlags_HeightSmall;
			else if (height_mode == (long)mvCombo::ComboHeightMode::mvComboHeight_Regular)
				_flags = ImGuiComboFlags_HeightRegular;
			else if (height_mode == (long)mvCombo::ComboHeightMode::mvComboHeight_Large)
				_flags = ImGuiComboFlags_HeightLarge;
			else
				_flags = ImGuiComboFlags_HeightLargest;
		}

		// helpers for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		flagop("popup_align_left", ImGuiComboFlags_PopupAlignLeft, _flags);
		flagop("no_arrow_button", ImGuiComboFlags_NoArrowButton, _flags);
		flagop("no_preview", ImGuiComboFlags_NoPreview, _flags);

	}

	void mvCombo::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			mvPyObject py_value = ToPyBool(flags & flag);
			PyDict_SetItemString(dict, keyword, py_value);
		};
		checkbitset("popup_align_left", ImGuiComboFlags_PopupAlignLeft, _flags);
		checkbitset("no_arrow_button", ImGuiComboFlags_NoArrowButton, _flags);
		checkbitset("no_preview", ImGuiComboFlags_NoPreview, _flags);

		mvUUID mode;
		if (_flags & ImGuiComboFlags_HeightSmall)
			mode = (long)mvCombo::ComboHeightMode::mvComboHeight_Small;
		else if (_flags & ImGuiComboFlags_HeightRegular)
			mode = (long)mvCombo::ComboHeightMode::mvComboHeight_Regular;
		else if (_flags & ImGuiComboFlags_HeightLarge)
			mode = (long)mvCombo::ComboHeightMode::mvComboHeight_Large;
		else
			mode = (long)mvCombo::ComboHeightMode::mvComboHeight_Largest;

		mvPyObject py_height_mode = ToPyUUID(mode);
		mvPyObject py_items = ToPyList(_items);
		PyDict_SetItemString(dict, "height_mode", py_height_mode);
		PyDict_SetItemString(dict, "items", py_items);
	}

}