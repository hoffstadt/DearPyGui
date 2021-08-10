#include "mvTab.h"
#include "mvTabBar.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvTab::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Adds a tab to a tab bar. Must be closed with thes end command.", { "Containers", "Widgets" }, true);
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_FILTER |
			MV_PARSER_ARG_DROP_CALLBACK |
			MV_PARSER_ARG_DRAG_CALLBACK |
			MV_PARSER_ARG_PAYLOAD_TYPE |
			MV_PARSER_ARG_SEARCH_DELAY |
			MV_PARSER_ARG_TRACKED |
			MV_PARSER_ARG_SHOW)
		);

		parser.addArg<mvPyDataType::Bool>("closable", mvArgType::KEYWORD_ARG, "False", "Creates a button on the tab that can hide the tab.");
		parser.addArg<mvPyDataType::Bool>("no_tooltip", mvArgType::KEYWORD_ARG, "False", "Disable tooltip for the given tab.");
		
		parser.addArg<mvPyDataType::Bool>("order_mode", mvArgType::KEYWORD_ARG, "0", "set using a constant: mvTabOrder_Reorderable: allows reordering, mvTabOrder_Fixed: fixed ordering, mvTabOrder_Leading: adds tab to front, mvTabOrder_Trailing: adds tab to back");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}
	mvTab::mvTab(mvUUID uuid)
		: 
		mvAppItem(uuid)
	{
	}

	void mvTab::addFlag(ImGuiTabItemFlags flag)
	{
		_flags |= flag;
	}

	void mvTab::removeFlag(ImGuiTabItemFlags flag)
	{
		_flags &= ~flag;
	}

	PyObject* mvTab::getPyValue()
	{
		return ToPyBool(*_value);
	}

	void mvTab::setPyValue(PyObject* value)
	{
		*_value = ToBool(value);
	}

	void mvTab::setDataSource(mvUUID dataSource)
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

	void mvTab::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id(_uuid);

		// cast parent to mvTabBar
		auto parent = (mvTabBar*)_parentPtr;

		// check if this is first tab
		if (parent->getSpecificValue() == 0)
		{
			// set mvTabBar value to the first tab name
			parent->setValue(_uuid);
			*_value = true;
		}

		// create tab item and see if it is selected
		if (ImGui::BeginTabItem(_internalLabel.c_str(), _closable ? &_show : nullptr, _flags))
		{

			// set other tab's value false
			for (auto& child : parent->getChildren(1))
			{
				if (child->getType() == mvAppItemType::mvTab)
					*((mvTab*)child.get())->_value = false;
			}

			// set current tab value true
			*_value = true;

			// run call back if it exists
			if (parent->getSpecificValue() != _uuid)
			{
				mvApp::GetApp()->getCallbackRegistry().submitCallback([=]() {
					mvApp::GetApp()->getCallbackRegistry().addCallback(parent->getCallback(), parent->getUUID(), ToPyUUID(_uuid), parent->getCallbackData());
					});
			}

			parent->setValue(_uuid);

			for (auto& item : _children[1])
			{
				if (!item->preDraw())
					continue;

				item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

				item->postDraw();
			}

			ImGui::EndTabItem();
		}

	}

	void mvTab::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		if (PyObject* item = PyDict_GetItemString(dict, "closable")) _closable = ToBool(item);


		if (PyObject* item = PyDict_GetItemString(dict, "order_mode"))
		{
			long order_mode = ToUUID(item);

			if (order_mode == (long)mvTab::TabOrdering::mvTabOrder_Fixed)
				_flags = ImGuiTabItemFlags_NoReorder;
			else if (order_mode == (long)mvTab::TabOrdering::mvTabOrder_Leading)
				_flags = ImGuiTabItemFlags_Leading;
			else if (order_mode == (long)mvTab::TabOrdering::mvTabOrder_Trailing)
				_flags = ImGuiTabItemFlags_Trailing;
			else
				_flags = ImGuiTabItemFlags_None;
		}

		if (PyObject* item = PyDict_GetItemString(dict, "no_tooltip"))
		{
			bool value = ToBool(item);
			if (value)
				_flags |= ImGuiTabItemFlags_NoTooltip;
			else
				_flags &= ~ImGuiTabItemFlags_NoTooltip;
		}

	}

	void mvTab::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		PyDict_SetItemString(dict, "closable", ToPyBool(_closable));

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
		};

		checkbitset("no_tooltip", ImGuiTabItemFlags_NoTooltip, _flags);

		if(_flags & ImGuiTabItemFlags_Leading)
			PyDict_SetItemString(dict, "order_mode", ToPyUUID((long)mvTab::TabOrdering::mvTabOrder_Leading));
		else if (_flags & ImGuiTabItemFlags_Trailing)
			PyDict_SetItemString(dict, "order_mode", ToPyUUID((long)mvTab::TabOrdering::mvTabOrder_Trailing));
		else if (_flags & ImGuiTabBarFlags_Reorderable)
			PyDict_SetItemString(dict, "order_mode", ToPyUUID((long)mvTab::TabOrdering::mvTabOrder_Reorderable));
		else
			PyDict_SetItemString(dict, "order_mode", ToPyUUID((long)mvTab::TabOrdering::mvTabOrder_Fixed));

	}

}