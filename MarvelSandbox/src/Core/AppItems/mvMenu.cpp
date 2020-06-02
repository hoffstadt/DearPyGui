#include "mvMenu.h"
#include <imgui.h>
#include "Core/mvApp.h"

namespace Marvel {

	void mvMenuBar::draw()
	{
		mvApp::GetApp()->pushParent(this);
		ImGui::BeginMenuBar();
	}

	PyObject* mvMenuBar::getPyValue()
	{

		PyObject* pvalue = Py_BuildValue("i", m_value);

		return pvalue;

	}

	void mvEndMenuBar::draw()
	{
		mvApp::GetApp()->popParent();
		ImGui::EndMenuBar();
	}

	void mvMenu::draw()
	{

		// create menu and see if its selected
		if (ImGui::BeginMenu(m_label.c_str()))
		{
			mvApp::GetApp()->pushParent(this);

			// set other menus's value false on same level
			for (mvAppItem* child : m_parent->getChildren())
				static_cast<mvMenu*>(child)->setValue(false);

			// set current menu value true
			m_value = true;

		}

	}

	PyObject* mvMenu::getPyValue()
	{

		PyObject* pvalue = Py_BuildValue("i", m_value);

		return pvalue;

	}

	void mvEndMenu::draw()
	{
		mvApp::GetApp()->popParent();
		ImGui::EndMenu();
	}

	PyObject* mvEndMenu::getPyValue()
	{

		PyObject* pvalue = Py_BuildValue("i", m_value);

		return pvalue;

	}

	void mvMenuItem::draw()
	{
		// create menuitem and see if its selected
		if (ImGui::MenuItem(m_label.c_str(), NULL))
		{

			// set other menusitems's value false on same level
			for (mvAppItem* child : m_parent->getChildren())
				static_cast<mvMenuItem*>(child)->setValue(false);

			m_value = true;


			mvApp::GetApp()->triggerCallback(m_callback, m_name);

		}
	}

	PyObject* mvMenuItem::getPyValue()
	{

		PyObject* pvalue = Py_BuildValue("i", m_value);

		return pvalue;

	}
}