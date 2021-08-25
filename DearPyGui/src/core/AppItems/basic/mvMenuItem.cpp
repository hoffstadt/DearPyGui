#include "mvMenuItem.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"

namespace Marvel {

	void mvMenuItem::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Adds a menu item to an existing menu. Menu items act similar to selectables.", { "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_CALLBACK |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_FILTER |
			MV_PARSER_ARG_DROP_CALLBACK |
			MV_PARSER_ARG_DRAG_CALLBACK |
			MV_PARSER_ARG_PAYLOAD_TYPE |
			MV_PARSER_ARG_TRACKED |
			MV_PARSER_ARG_ENABLED)
		);

		parser.addArg<mvPyDataType::Bool>("default_value", mvArgType::KEYWORD_ARG, "False");

		parser.addArg<mvPyDataType::String>("shortcut", mvArgType::KEYWORD_ARG, "''", "Displays text on the menu item. Typically used to show a shortcut key command.");

		parser.addArg<mvPyDataType::Bool>("check", mvArgType::KEYWORD_ARG, "False", "Displays a checkmark on the menu item when it is selected.");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvMenuItem::mvMenuItem(mvUUID uuid)
		: mvAppItem(uuid) 
	{
	}

	void mvMenuItem::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvMenuItem*>(item);
		if (_source != 0) _value = titem->_value;
		_disabled_value = titem->_disabled_value;
		_shortcut = titem->_shortcut;
		_check = titem->_check;
	}

	void mvMenuItem::draw(ImDrawList* drawlist, float x, float y)
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

			// This is ugly and goes against our style system but its the only widget that ImGui chooses to push the disable color for us
			// so we have to map our text disable color to the system text disable color, or we can create a new constant which goes agains our 
			// constants. 
			ImGui::PushStyleColor(ImGuiCol_TextDisabled, ImGui::GetStyleColorVec4(ImGuiCol_Text));

			// create menu item and see if its selected
			if (ImGui::MenuItem(_internalLabel.c_str(), _shortcut.c_str(), _check ? _value.get() : nullptr, _enabled))
			{
				if(_alias.empty())
					mvApp::GetApp()->getCallbackRegistry().addCallback(_callback, _uuid, nullptr, _user_data);
				else
					mvApp::GetApp()->getCallbackRegistry().addCallback(_callback, _alias, nullptr, _user_data);
			}

			ImGui::PopStyleColor();

		}

		//-----------------------------------------------------------------------------
		// update state
		//-----------------------------------------------------------------------------
		_state.update();

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

		if (_handlerRegistry)
			_handlerRegistry->customAction(&_state);

		// handle drag & drop payloads
		for (auto& item : _children[3])
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
						mvApp::GetApp()->getCallbackRegistry().addCallback(getDropCallback(), _uuid, payloadActual->getDragData(), nullptr);
					else
						mvApp::GetApp()->getCallbackRegistry().addCallback(getDropCallback(), _alias, payloadActual->getDragData(), nullptr);
				}

				ImGui::EndDragDropTarget();
			}
		}
	}

	void mvMenuItem::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "shortcut")) _shortcut = ToString(item);
		if (PyObject* item = PyDict_GetItemString(dict, "check")) _check = ToBool(item);

}

	void mvMenuItem::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "shortcut", mvPyObject(ToPyString(_shortcut)));
		PyDict_SetItemString(dict, "check", mvPyObject(ToPyBool(_check)));
	}

	PyObject* mvMenuItem::getPyValue()
	{
		return ToPyBool(*_value);
	}

	void mvMenuItem::setPyValue(PyObject* value)
	{
		*_value = ToBool(value);
	}

	void mvMenuItem::setDataSource(mvUUID dataSource)
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
		_value = *static_cast<std::shared_ptr<bool>*>(item->getValue());
	}
}
