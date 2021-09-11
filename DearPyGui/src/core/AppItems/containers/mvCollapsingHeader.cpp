#include "mvCollapsingHeader.h"
#include "mvInput.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"

namespace Marvel {

	void mvCollapsingHeader::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Adds a collapsing header to add items to. Must be closed with the end command.", { "Containers", "Widgets" }, true);
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
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

		parser.addArg<mvPyDataType::Bool>("closable", mvArgType::KEYWORD_ARG, "False", "Adds the ability to hide this widget by pressing the (x) in the top right of widget.");
		parser.addArg<mvPyDataType::Bool>("default_open", mvArgType::KEYWORD_ARG, "False", "Sets the collapseable header open by default.");
		parser.addArg<mvPyDataType::Bool>("open_on_double_click", mvArgType::KEYWORD_ARG, "False", "Need double-click to open node.");
		parser.addArg<mvPyDataType::Bool>("open_on_arrow", mvArgType::KEYWORD_ARG, "False", "Only open when clicking on the arrow part.");
		parser.addArg<mvPyDataType::Bool>("leaf", mvArgType::KEYWORD_ARG, "False", "No collapsing, no arrow (use as a convenience for leaf nodes).");
		parser.addArg<mvPyDataType::Bool>("bullet", mvArgType::KEYWORD_ARG, "False", "Display a bullet instead of arrow.");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvCollapsingHeader::mvCollapsingHeader(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	void mvCollapsingHeader::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvCollapsingHeader*>(item);
		if (_source != 0) _value = titem->_value;
		_disabled_value = titem->_disabled_value;
		_flags = titem->_flags;
		_closable = titem->_closable;
	}

	void mvCollapsingHeader::draw(ImDrawList* drawlist, float x, float y)
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
			ScopedID id(_uuid);

			bool* toggle = nullptr;
			if (_closable)
				toggle = &_show;
			*_value = ImGui::CollapsingHeader(_internalLabel.c_str(), toggle, _flags);
			_state.update();

			if (*_value)
			{
				for (auto& item : _children[1])
					item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
			}
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

	void mvCollapsingHeader::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		if (PyObject* item = PyDict_GetItemString(dict, "closable")) _closable = ToBool(item);

		// helper for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		// flags
		flagop("default_open", ImGuiTreeNodeFlags_DefaultOpen, _flags);
		flagop("open_on_double_click", ImGuiTreeNodeFlags_OpenOnDoubleClick, _flags);
		flagop("open_on_arrow", ImGuiTreeNodeFlags_OpenOnArrow, _flags);
		flagop("leaf", ImGuiTreeNodeFlags_Leaf, _flags);
		flagop("bullet", ImGuiTreeNodeFlags_Bullet, _flags);


	}

	void mvCollapsingHeader::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		PyDict_SetItemString(dict, "closable", mvPyObject(ToPyBool(_closable)));

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, mvPyObject(ToPyBool(flags & flag)));
		};

		// flags
		checkbitset("default_open", ImGuiTreeNodeFlags_DefaultOpen, _flags);
		checkbitset("open_on_double_click", ImGuiTreeNodeFlags_OpenOnDoubleClick, _flags);
		checkbitset("open_on_arrow", ImGuiTreeNodeFlags_OpenOnArrow, _flags);
		checkbitset("leaf", ImGuiTreeNodeFlags_Leaf, _flags);
		checkbitset("bullet", ImGuiTreeNodeFlags_Bullet, _flags);

	}

	PyObject* mvCollapsingHeader::getPyValue()
	{
		return ToPyBool(*_value);
	}

	void mvCollapsingHeader::setPyValue(PyObject* value)
	{
		*_value = ToBool(value);
	}

	void mvCollapsingHeader::setDataSource(mvUUID dataSource)
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