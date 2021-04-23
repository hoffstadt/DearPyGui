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
		mvPythonParser parser(mvPyDataType::String);
		mvAppItem::AddCommonArgs(parser);
		parser.removeArg("source");
		parser.removeArg("width");
		parser.removeArg("height");
		parser.removeArg("label");
		parser.removeArg("enabled");

		parser.addArg<mvPyDataType::Bool>("reorderable", mvArgType::KEYWORD_ARG, "False", "allows for moveable tabs");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvTabBar::mvTabBar(const std::string& name)
		:
		mvStringPtrBase(name)
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

	std::string& mvTabBar::getSpecificValue()
	{
		return m_uiValue;
	}

	void mvTabBar::setValue(const std::string& value)
	{
		m_uiValue = value;
	}

	void mvTabBar::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id;
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);
		ImGui::BeginGroup();

		if (ImGui::BeginTabBar(m_label.c_str(), m_flags))
		{
			//we do this so that the children dont get the theme
			scope.cleanup();
			for (auto& item : m_children[1])
			{
				// skip item if it's not shown
				if (!item->m_show)
					continue;

				// set item width
				if (item->m_width != 0)
					ImGui::SetNextItemWidth((float)item->m_width);

				if (*m_value == item->m_name && m_lastValue != *m_value)
					static_cast<mvTab*>(item.get())->addFlag(ImGuiTabItemFlags_SetSelected);

				if (item->m_focusNextFrame)
				{
					ImGui::SetKeyboardFocusHere();
					item->m_focusNextFrame = false;
				}

				if (item->m_dirtyPos)
				{
					ImGui::SetCursorPos(item->getState().getItemPos());
					item->m_dirtyPos = false;
				}
				item->getState().setPos({ ImGui::GetCursorPosX(), ImGui::GetCursorPosY() });

				item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

				if (*m_value == item->m_name)
					static_cast<mvTab*>(item.get())->removeFlag(ImGuiTabItemFlags_SetSelected);

				item->getState().update();
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