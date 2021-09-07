#include "mvGroup.h"
#include "mvInput.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"
namespace Marvel {

	void mvGroup::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, 
			"Creates a group that other widgets can belong to. The group allows item commands to be issued for all of its members. Must be closed with the end command.", 
			{ "Containers", "Widgets" }, true);
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_WIDTH |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_FILTER |
			MV_PARSER_ARG_DROP_CALLBACK |
			MV_PARSER_ARG_DRAG_CALLBACK |
			MV_PARSER_ARG_PAYLOAD_TYPE |
			MV_PARSER_ARG_TRACKED |
			MV_PARSER_ARG_SEARCH_DELAY |
			MV_PARSER_ARG_POS)
		);

		parser.addArg<mvPyDataType::Bool>("horizontal", mvArgType::KEYWORD_ARG, "False", "Forces child widgets to be added in a horizontal layout.");
		parser.addArg<mvPyDataType::Float>("horizontal_spacing", mvArgType::KEYWORD_ARG, "-1", "Spacing for the horizontal layout.");
		parser.addArg<mvPyDataType::Float>("xoffset", mvArgType::KEYWORD_ARG, "0.0", "Offset from containing window x item location within group.");


		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvGroup::mvGroup(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	void mvGroup::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvGroup*>(item);
		_horizontal = titem->_horizontal;
		_hspacing = titem->_hspacing;
	}

	void mvGroup::draw(ImDrawList* drawlist, float x, float y)
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

		// themes
		if (auto classTheme = getClassThemeComponent())
			static_cast<mvThemeComponent*>(classTheme.get())->draw(nullptr, 0.0f, 0.0f);

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

		if (_width != 0)
			ImGui::PushItemWidth((float)_width);

		ImGui::BeginGroup();

		for (auto& item : _children[1])
		{
			if (_width != 0)
				item->setWidth(_width);

			item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

			if (_horizontal)
				ImGui::SameLine((1 +item->getLocation())*_xoffset, _hspacing);

			if (item->isTracked())
			{
				ImGui::SetScrollHereX(item->getTrackOffset());
				ImGui::SetScrollHereY(item->getTrackOffset());
			}
		}

		if (_width != 0)
			ImGui::PopItemWidth();

		ImGui::EndGroup();
		_state.update();

		}

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
		if (auto classTheme = getClassThemeComponent())
			static_cast<mvThemeComponent*>(classTheme.get())->customAction();

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
						mvApp::GetApp()->getCallbackRegistry().addCallback(getDropCallback(), _uuid, payloadActual->getDragData(), nullptr);
					else
						mvApp::GetApp()->getCallbackRegistry().addCallback(getDropCallback(), _alias, payloadActual->getDragData(), nullptr);
				}

				ImGui::EndDragDropTarget();
			}
		}
	}

	void mvGroup::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		if (PyObject* item = PyDict_GetItemString(dict, "horizontal")) _horizontal = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "horizontal_spacing")) _hspacing = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "xoffset")) _xoffset = ToFloat(item);
	}

	void mvGroup::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		PyDict_SetItemString(dict, "horizontal", mvPyObject(ToPyBool(_horizontal)));
		PyDict_SetItemString(dict, "horizontal_spacing", mvPyObject(ToPyFloat(_hspacing)));
		PyDict_SetItemString(dict, "xoffset", mvPyObject(ToPyFloat(_xoffset)));
	}

}