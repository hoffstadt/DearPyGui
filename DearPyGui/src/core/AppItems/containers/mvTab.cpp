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
		
		parser.addArg<mvPyDataType::Bool>("order_mode", mvArgType::KEYWORD_ARG, "0");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}
	mvTab::mvTab(mvUUID uuid)
		: 
		mvBoolPtrBase(uuid)
	{
	}

	void mvTab::addFlag(ImGuiTabItemFlags flag)
	{
		m_flags |= flag;
	}

	void mvTab::removeFlag(ImGuiTabItemFlags flag)
	{
		m_flags &= ~flag;
	}

	bool mvTab::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvTabBar) return true;
		if (type == mvAppItemType::mvStagingContainer) return true;

		mvThrowPythonError(mvErrorCode::mvIncompatibleParent, s_command,
			"Incompatible parent. Acceptable parents include: tab bar, staging container.", this);

		assert(false);
		return false;
	}

	void mvTab::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id(m_uuid);

		// cast parent to mvTabBar
		auto parent = (mvTabBar*)m_parentPtr;

		// check if this is first tab
		if (parent->getSpecificValue() == 0)
		{
			// set mvTabBar value to the first tab name
			parent->setValue(m_uuid);
			*m_value = true;
		}

		// create tab item and see if it is selected
		if (ImGui::BeginTabItem(m_label.c_str(), m_closable ? &m_show : nullptr, m_flags))
		{

			// set other tab's value false
			for (auto& child : parent->m_children[1])
			{
				if (child->getType() == mvAppItemType::mvTab)
					*((mvTab*)child.get())->m_value = false;
			}

			// set current tab value true
			*m_value = true;

			// run call back if it exists
			if (parent->getSpecificValue() != m_uuid)
				mvApp::GetApp()->getCallbackRegistry().addCallback(parent->getCallback(), m_uuid, nullptr, parent->getCallbackData());

			parent->setValue(m_uuid);

			for (auto& item : m_children[1])
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
		 
		if (PyObject* item = PyDict_GetItemString(dict, "closable")) m_closable = ToBool(item);


		if (PyObject* item = PyDict_GetItemString(dict, "order_mode"))
		{
			long order_mode = ToUUID(item);

			if (order_mode == (long)mvTab::TabOrdering::mvTabOrder_Fixed)
				m_flags = ImGuiTabItemFlags_NoReorder;
			else if (order_mode == (long)mvTab::TabOrdering::mvTabOrder_Leading)
				m_flags = ImGuiTabItemFlags_Leading;
			else if (order_mode == (long)mvTab::TabOrdering::mvTabOrder_Trailing)
				m_flags = ImGuiTabItemFlags_Trailing;
			else
				m_flags = ImGuiTabItemFlags_None;
		}

		if (PyObject* item = PyDict_GetItemString(dict, "no_tooltip"))
		{
			bool value = ToBool(item);
			if (value)
				m_flags |= ImGuiTabItemFlags_NoTooltip;
			else
				m_flags &= ~ImGuiTabItemFlags_NoTooltip;
		}

	}

	void mvTab::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		PyDict_SetItemString(dict, "closable", ToPyBool(m_closable));

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
		};

		checkbitset("no_tooltip", ImGuiTabItemFlags_NoTooltip, m_flags);

		if(m_flags & ImGuiTabItemFlags_Leading)
			PyDict_SetItemString(dict, "order_mode", ToPyUUID((long)mvTab::TabOrdering::mvTabOrder_Leading));
		else if (m_flags & ImGuiTabItemFlags_Trailing)
			PyDict_SetItemString(dict, "order_mode", ToPyUUID((long)mvTab::TabOrdering::mvTabOrder_Trailing));
		else if (m_flags & ImGuiTabBarFlags_Reorderable)
			PyDict_SetItemString(dict, "order_mode", ToPyUUID((long)mvTab::TabOrdering::mvTabOrder_Reorderable));
		else
			PyDict_SetItemString(dict, "order_mode", ToPyUUID((long)mvTab::TabOrdering::mvTabOrder_Fixed));

	}

}