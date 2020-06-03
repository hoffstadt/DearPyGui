#include "mvTab.h"
#include <imgui.h>
#include "Core/mvApp.h"

namespace Marvel {

	void mvTabBar::draw()
	{
		mvApp::GetApp()->pushParent(this);
		ImGui::BeginTabBar(m_label.c_str());
	}

	PyObject* mvTabBar::getPyValue()
	{

		PyObject* pvalue = Py_BuildValue("s", m_value->c_str());

		return pvalue;

	}

	void mvEndTabBar::draw()
	{
		mvApp::GetApp()->popParent();
		ImGui::EndTabBar();
	}

	PyObject* mvEndTabBar::getPyValue()
	{

		PyObject* pvalue = Py_BuildValue("i", m_value);

		return pvalue;

	}

	void mvTab::draw()
	{
		// cast parent to mvTabBar
		mvTabBar* parent = static_cast<mvTabBar*>(m_parent);

		// check if this is first tab
		if (parent->getValue() == "")
		{

			// set mvTabBar value to the first tab name
			parent->setValue(m_name);
			m_value = true;

		}

		// create tab item and see if it is selected
		if (ImGui::BeginTabItem(m_label.c_str()))
		{
			mvApp::GetApp()->pushParent(this); // push parent onto the parent stack

			bool changed = false;

			// change tab bar value to this selected tab
			if (parent->getValue() != m_name)
				changed = true;
			else
				changed = false;

			parent->setValue(m_name);

			// set other tab's value false
			for (mvAppItem* child : parent->getChildren())
				static_cast<mvTab*>(child)->setValue(false);

			// set current tab value true
			m_value = true;

			showAll();

			// run call back if it exists
			if (changed)
				mvApp::GetApp()->triggerCallback(m_callback, m_name);

		}
	}

	PyObject* mvTab::getPyValue()
	{

		PyObject* pvalue = Py_BuildValue("i", m_value);

		return pvalue;

	}

	void mvEndTab::draw()
	{
		mvApp::GetApp()->popParent();
		ImGui::EndTabItem();
	}

	PyObject* mvEndTab::getPyValue()
	{

		PyObject* pvalue = Py_BuildValue("i", m_value);

		return pvalue;

	}
}