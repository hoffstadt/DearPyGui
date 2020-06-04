#include "mvApp.h"
#include "mvLogger.h"
#include "AppItems/mvInputText.h"
#include "AppItems/mvTab.h"
#include "AppItems/mvMenu.h"
#include "AppItems/mvSpacing.h"
#include "AppItems/mvSameLine.h"
#include "AppItems/mvRadioButton.h"
#include "AppItems/mvButton.h"
#include "AppItems/mvChild.h"
#include "AppItems/mvGroup.h"
#include "AppItems/mvTooltip.h"
#include "AppItems/mvCollapsingHeader.h"
#include "AppItems/mvSeperator.h"
#include "AppItems/mvColorEdit4.h"
#include "AppItems/mvInputInt.h"
#include "AppItems/mvInputFloat.h"
#include "AppItems/mvCheckbox.h"

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
		ImGui::Begin("Blah", (bool*)0, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoSavedSettings| ImGuiWindowFlags_NoResize| ImGuiWindowFlags_NoCollapse| ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar);

		m_parents.push(nullptr);

		// main callback
		triggerCallback(m_callback, "Main Application");

		for (mvAppItem* item : m_items)
		{
			// skip item if it's not shown
			if (!item->isShown())
				continue;

			// check if item is owned & it's parent is visible
			if (!doesParentAllowRender(item))
				continue;

			// if parent isn't the most recent parent, skip
			if (item->getParent() && m_parents.top() && item->getType() != mvAppItemType::Tooltip)
				if (!(m_parents.top()->getName() == item->getParent()->getName()))
					continue;

			// set item width
			if(item->getWidth() != 0)
				ImGui::SetNextItemWidth((float)item->getWidth());

			item->draw();

			// Regular Tooltip (simple)
			if (item->getTip() != "" && ImGui::IsItemHovered())
				ImGui::SetTooltip(item->getTip().c_str());
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

			case mvAppItemType::Child:
				return static_cast<mvChild*>(item->getParent())->getValue();
				break;

			case mvAppItemType::Tooltip:
				return static_cast<mvTooltip*>(item->getParent())->getValue();
				break;

			case mvAppItemType::CollapsingHeader:
			{
				if (!static_cast<mvCollapsingHeader*>(item->getParent())->getValue())
				{
					item->hide();
					return false;
				}
				else
					item->show();
				break;
			}

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

	mvAppItem* mvApp::topParent()
	{
		return m_parents.top();
	}

	void mvApp::setItemCallback(const std::string& name, const std::string& callback)
	{
		auto item = getItem(name);
		if (item)
			item->setCallback(callback);
	}

	void mvApp::setItemTip(const std::string& name, const std::string& tip)
	{
		auto item = getItem(name);
		if (item)
			item->setTip(tip);
	}

	void mvApp::setItemWidth(const std::string& name, int width)
	{
		auto item = getItem(name);
		if (item)
			item->setWidth(width);
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

		PyErr_Clear();

		PyObject* pHandler = PyDict_GetItemString(m_pDict, name.c_str()); // borrowed reference

		PyObject* pArgs = PyTuple_New(1);
		PyTuple_SetItem(pArgs, 0, PyUnicode_FromString(sender.c_str()));

		PyObject* result = PyObject_CallObject(pHandler, pArgs);

		// check if error occurred
		PyErr_Print();


		Py_XDECREF(pArgs);
		Py_XDECREF(result);

	}

	void mvApp::setAppTheme(const std::string& theme)
	{
		if (theme == "dark")
			ImGui::StyleColorsDark();
		else if (theme == "classic")
			ImGui::StyleColorsClassic();
		else if (theme == "light")
			ImGui::StyleColorsLight();
		else
			ImGui::StyleColorsDark();
	}

	void mvApp::updateTheme()
	{
		ImGuiStyle* style = &ImGui::GetStyle();
		ImVec4* colors = style->Colors;

		auto themecolors = m_theme.getColors();

		for (unsigned i = 0; i < m_theme.getNumberOfItems(); i++)
		{
			auto color = themecolors[i];
			colors[i] = ImVec4(color[0], color[1], color[2], color[3]);
		}
			
	}

	void mvApp::changeThemeItem(const char* name, float r, float g, float b, float a)
	{
		m_theme.changeThemeItem(name, r, g, b, a);
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

	mvAppItem* mvApp::addInputInt(const std::string& parent, const std::string& name, int default_value)
	{
		mvAppItem* item = new mvInputInt(parent, name, default_value);
		m_items.push_back(item);
		return item;
	}

	mvAppItem* mvApp::addInputFloat(const std::string& parent, const std::string& name, float default_value)
	{
		mvAppItem* item = new mvInputFloat(parent, name, default_value);
		m_items.push_back(item);
		return item;
	}

	mvAppItem* mvApp::addButton(const std::string& parent, const std::string& name)
	{
		mvAppItem* item = new mvButton(parent, name);
		m_items.push_back(item);
		return item;
	}

	mvAppItem* mvApp::addColorEdit4(const std::string& parent, const std::string& name, float r, float g, float b, float a)
	{
		mvAppItem* item = new mvColorEdit4(parent, name, r, g, b, a);
		m_items.push_back(item);
		return item;
	}

	mvAppItem* mvApp::addRadioButtons(const std::string& parent, const std::string& name, const std::vector<std::string>& itemnames, int default_value)
	{
		mvAppItem* item = new mvRadioButton(parent, name, itemnames, default_value);
		m_items.push_back(item);
		return item;
	}

	mvAppItem* mvApp::addCheckbox(const std::string& parent, const std::string& name, bool default_value)
	{
		mvAppItem* item = new mvCheckbox(parent, name, default_value);
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
	// Groups
	//-----------------------------------------------------------------------------

	mvAppItem* mvApp::addGroup(const std::string& parent, const std::string& name)
	{
		mvAppItem* item = new mvGroup(parent, name);
		m_items.push_back(item);
		return item;
	}

	mvAppItem* mvApp::endGroup(const std::string& parent)
	{
		mvAppItem* item = new mvEndGroup(parent);
		m_items.push_back(item);
		return item;
	}

	//-----------------------------------------------------------------------------
	// Child Window
	//-----------------------------------------------------------------------------

	mvAppItem* mvApp::addChild(const std::string& parent, const std::string& name, int width, int height)
	{
		mvAppItem* item = new mvChild(parent, name, width, height);
		m_items.push_back(item);
		return item;
	}

	mvAppItem* mvApp::endChild(const std::string& parent)
	{
		mvAppItem* item = new mvEndChild(parent);
		m_items.push_back(item);
		return item;
	}

	//-----------------------------------------------------------------------------
	// Misc Items
	//-----------------------------------------------------------------------------

	mvAppItem* mvApp::addCollapsingHeader(const std::string& parent, const std::string& name)
	{
		mvAppItem* item = new mvCollapsingHeader(parent, name);
		m_items.push_back(item);
		return item;
	}

	mvAppItem* mvApp::addSpacing(const std::string& parent, int count)
	{
		mvAppItem* item = new mvSpacing(parent, count);
		m_items.push_back(item);
		return item;
	}

	mvAppItem* mvApp::addSameLine(const std::string& parent, float offsetx, float spacing)
	{
		mvAppItem* item = new mvSameLine(parent, offsetx, spacing);
		m_items.push_back(item);
		return item;
	}

	mvAppItem* mvApp::addTooltip(const std::string& parent, const std::string& name)
	{
		mvAppItem* item = new mvTooltip(parent, name);
		m_items.push_back(item);
		return item;
	}

	mvAppItem* mvApp::endTooltip(const std::string& parent)
	{
		mvAppItem* item = new mvEndTooltip(parent);
		m_items.push_back(item);
		return item;
	}

	mvAppItem* mvApp::addSeperator(const std::string& parent)
	{
		mvAppItem* item = new mvSeparator(parent);
		m_items.push_back(item);
		return item;
	}
}