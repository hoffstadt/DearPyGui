#include <utility>
#include "mvButton.h"
#include "mvCore.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"
#include "mvPyObject.h"

namespace Marvel {

	void mvButton::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Adds a button.", { "Widgets" });
		mvAppItem::AddCommonArgs(parser,(CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_WIDTH |
			MV_PARSER_ARG_HEIGHT |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
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

		parser.addArg<mvPyDataType::Bool>("small", mvArgType::KEYWORD_ARG, "False", "Small button, useful for embedding in text.");
		parser.addArg<mvPyDataType::Bool>("arrow", mvArgType::KEYWORD_ARG, "False", "Arrow button, requires the direction keyword.");
		parser.addArg<mvPyDataType::Integer>("direction", mvArgType::KEYWORD_ARG, "0", "A cardinal direction for arrow.");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvButton::mvButton(mvUUID uuid)
		: 
		mvAppItem(uuid)
	{
		_state._applicableState = MV_STATE_HOVER | MV_STATE_ACTIVE | MV_STATE_FOCUSED | MV_STATE_CLICKED | MV_STATE_CONT_AVAIL |
			MV_STATE_VISIBLE | MV_STATE_ACTIVATED | MV_STATE_DEACTIVATED | MV_STATE_RECT_MIN | MV_STATE_RECT_MAX | MV_STATE_RECT_SIZE;
	}

	void mvButton::draw(ImDrawList* drawlist, float x, float y)
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
			// push imgui id to prevent name collisions
			ScopedID id(_uuid);

			bool activated = false;

			if (_small_button)
				activated = ImGui::SmallButton(_internalLabel.c_str());

			else if (_arrow)
				activated = ImGui::ArrowButton(_internalLabel.c_str(), _direction);

			else
				activated = ImGui::Button(_internalLabel.c_str(), ImVec2((float)_width, (float)_height));

			if (activated)
			{
				if (_alias.empty())
					mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), _uuid, nullptr, _user_data);
				else
					mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), _alias, nullptr, _user_data);
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
						mvApp::GetApp()->getCallbackRegistry().addCallback(getDropCallback(), _uuid, payloadActual->getDragData(), nullptr);
					else
						mvApp::GetApp()->getCallbackRegistry().addCallback(getDropCallback(), _alias, payloadActual->getDragData(), nullptr);
				}

				ImGui::EndDragDropTarget();
			}
		}
	}

	void mvButton::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvButton*>(item);
		_direction = titem->_direction;
		_small_button = titem->_small_button;
		_arrow = titem->_arrow;
	}

	void mvButton::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "small")) _small_button = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "arrow")) _arrow = ToBool(item);

		if (PyObject* item = PyDict_GetItemString(dict, "direction"))
		{
			_direction = ToInt(item);
		}

	}

	void mvButton::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		mvPyObject py_small = ToPyBool(_small_button);
		mvPyObject py_arrow = ToPyBool(_arrow);
		mvPyObject py_direction = ToPyInt(_direction);

		PyDict_SetItemString(dict, "small", py_small);
		PyDict_SetItemString(dict, "arrow", py_arrow);
		PyDict_SetItemString(dict, "direction", py_direction);
	}

}