#include "mvApp.h"
#include "AppItems/mvInputText.h"
#include "AppItems/mvTab.h"
#include "AppItems/mvMenu.h"
#include "AppItems/mvSpacing.h"
#include <imgui.h>

namespace Marvel {

	mvApp* mvApp::s_instance = nullptr;

	mvApp* mvApp::GetApp()
	{
		if (s_instance)
			return s_instance;

		s_instance = new mvApp();
		return s_instance;
	}

	void mvApp::render()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowRounding = 0.0f;

		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
		ImGui::SetNextWindowSize(ImVec2(m_width, m_height));
		ImGui::Begin("Blah", (bool*)0, ImGuiWindowFlags_NoSavedSettings| ImGuiWindowFlags_NoResize| ImGuiWindowFlags_NoCollapse| ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar);

		m_parents.push(nullptr);

		for (mvAppItem* item : m_items)
		{
			if (!doesParentAllowRender(item))
				continue;

			// if parent isn't the most recent parent, skip
			if (item->getParent() && m_parents.top())
				if (!(m_parents.top()->getName() == item->getParent()->getName()))
					continue;

			item->draw();
		}

		ImGui::End();
	}

	bool mvApp::doesParentAllowRender(mvAppItem* item)
	{
		if (item->getParent())
		{
			switch (item->getParent()->getType())
			{
			case mvAppItemType::TabItem:
				return static_cast<mvTab*>(item->getParent())->getValue();
				break;

			case mvAppItemType::Menu:
				return static_cast<mvMenu*>(item->getParent())->getValue();
				break;

			default:
				return item->getParent()->isShown();
			}
		}

		// doesn't have a parent
		return true;
	}

	void mvApp::pushParent(mvAppItem* item)
	{
		m_parents.push(item);
	}

	mvAppItem* mvApp::popParent()
	{
		mvAppItem* item = m_parents.top();
		m_parents.pop();
		return item;
	}

	void mvApp::setItemCallback(const std::string& name, const std::string& callback)
	{
		auto item = getItem(name);
		if (item)
			item->setCallback(callback);
	}

	mvAppItem* mvApp::getItem(const std::string& name)
	{
		for (mvAppItem* item : m_items)
		{
			if (item->getName() == name)
				return item;
		}

		return nullptr;
	}

	void mvApp::triggerCallback(const std::string& name, const std::string& sender)
	{
		if (name == "")
			return;

		PyObject* pHandler = PyDict_GetItemString(m_pDict, name.c_str()); // borrowed reference

		PyObject* pArgs = PyTuple_New(1);
		PyTuple_SetItem(pArgs, 0, PyUnicode_FromString(sender.c_str()));

		PyObject* result = PyObject_CallObject(pHandler, pArgs);

		Py_XDECREF(pArgs);
		Py_XDECREF(result);

	}

	//-----------------------------------------------------------------------------
	// Basic AppItems
	//-----------------------------------------------------------------------------
	mvAppItem* mvApp::addInputText(const std::string& parent, const std::string& name, const std::string& hint)
	{
		mvAppItem* item = new mvInputText(parent, name, hint);
		m_items.push_back(item);
		return item;
	}

	//-----------------------------------------------------------------------------
	// Tabs
	//-----------------------------------------------------------------------------
	mvAppItem* mvApp::addTabBar(const std::string& parent, const std::string& name)
	{
		mvAppItem* item = new mvTabBar(parent, name);
		m_items.push_back(item);
		return item;
	}

	mvAppItem* mvApp::addTab(const std::string& parent, const std::string& name)
	{
		mvAppItem* item = new mvTab(parent, name);
		m_items.push_back(item);
		return item;
	}

	mvAppItem* mvApp::endTab(const std::string& parent)
	{
		mvAppItem* item = new mvEndTab(parent);
		m_items.push_back(item);
		return item;
	}

	mvAppItem* mvApp::endTabBar(const std::string& parent)
	{
		mvAppItem* item = new mvEndTabBar(parent);
		m_items.push_back(item);
		return item;
	}

	//-----------------------------------------------------------------------------
	// Adding Menus
	//-----------------------------------------------------------------------------

	mvAppItem* mvApp::addMenuBar(const std::string& name)
	{
		mvAppItem* item = new mvMenuBar(name);
		m_items.push_back(item);
		return item;
	}

	mvAppItem* mvApp::addMenu(const std::string& parent, const std::string& name)
	{
		mvAppItem* item = new mvMenu(parent, name);
		m_items.push_back(item);
		return item;
	}

	mvAppItem* mvApp::addMenuItem(const std::string& parent, const std::string& name)
	{
		mvAppItem* item = new mvMenuItem(parent, name);
		m_items.push_back(item);
		return item;
	}

	mvAppItem* mvApp::endMenu(const std::string& parent)
	{
		mvAppItem* item = new mvEndMenu(parent);
		m_items.push_back(item);
		return item;
	}

	mvAppItem* mvApp::endMenuBar(const std::string& parent)
	{
		mvAppItem* item = new mvEndMenuBar(parent);
		m_items.push_back(item);
		return item;
	}

	//-----------------------------------------------------------------------------
	// Misc Items
	//-----------------------------------------------------------------------------

	mvAppItem* mvApp::addSpacing(const std::string& parent, int count)
	{
		mvAppItem* item = new mvSpacing(parent, count);
		m_items.push_back(item);
		return item;
	}
}