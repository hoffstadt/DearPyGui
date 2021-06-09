#include "mvTabBar.h"
#include "mvApp.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvTab.h"
#include "mvFontScope.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvTabBar::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		mvPythonParser parser(mvPyDataType::UUID, "Undocumented function", { "Containers", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_CALLBACK |
			MV_PARSER_ARG_USER_DATA |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_FILTER |
			MV_PARSER_ARG_DROP_CALLBACK |
			MV_PARSER_ARG_DRAG_CALLBACK |
			MV_PARSER_ARG_PAYLOAD_TYPE |
			MV_PARSER_ARG_SEARCH_DELAY |
			MV_PARSER_ARG_TRACKED |
			MV_PARSER_ARG_POS)
		);

		parser.addArg<mvPyDataType::Bool>("reorderable", mvArgType::KEYWORD_ARG, "False", "allows for moveable tabs");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvTabBar::mvTabBar(mvUUID uuid)
		:
		mvUUIDPtrBase(uuid)
	{
	}


	bool mvTabBar::canChildBeAdded(mvAppItemType type)
	{
		if (type == mvAppItemType::mvTab)return true;
		if (type == mvAppItemType::mvTabButton)return true;

		mvThrowPythonError(1000, "TabBar children must be tabs or tab buttons.");
		MV_ITEM_REGISTRY_ERROR("TabBar children must be tabs or tab buttons.");
		assert(false);

		return false;
	}

	mvUUID mvTabBar::getSpecificValue()
	{
		return m_uiValue;
	}

	void mvTabBar::setValue(mvUUID value)
	{
		m_uiValue = value;
	}

	void mvTabBar::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id(m_uuid);
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);
		ImGui::BeginGroup();

		if (ImGui::BeginTabBar(m_label.c_str(), m_flags))
		{
			//we do this so that the children dont get the theme
			scope.cleanup();
			for (auto& item : m_children[1])
			{

				if (!item->preDraw())
					continue;

				if (*m_value == item->m_uuid && m_lastValue != *m_value)
					static_cast<mvTab*>(item.get())->addFlag(ImGuiTabItemFlags_SetSelected);

				item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

				item->postDraw();

				if (*m_value == item->m_uuid)
					static_cast<mvTab*>(item.get())->removeFlag(ImGuiTabItemFlags_SetSelected);
			}

			ImGui::EndTabBar();
		}

		ImGui::EndGroup();

		*m_value = m_uiValue;
		m_lastValue = *m_value;
	}

	void mvTabBar::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 

		// helper for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		// window flags
		flagop("reorderable", ImGuiTabBarFlags_Reorderable, m_flags);

	}

	void mvTabBar::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
		};

		// window flags
		checkbitset("reorderable", ImGuiTabBarFlags_Reorderable, m_flags);
	}

}