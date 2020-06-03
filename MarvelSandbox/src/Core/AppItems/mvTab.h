#pragma once

#include "Core/mvAppItem.h"
#include "Core/mvApp.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvTabBar
	//-----------------------------------------------------------------------------
	class mvTabBar : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::TabBar)

		mvTabBar(const std::string& parent, const std::string& name)
			: mvAppItem(parent, name)
		{
			m_value = new std::string();
		}

		virtual PyObject* getPyValue() override
		{
			PyObject* pvalue = Py_BuildValue("s", m_value->c_str());

			return pvalue;
		}

		virtual void draw() override
		{
			mvApp::GetApp()->pushParent(this);
			ImGui::BeginTabBar(m_label.c_str());
		}

		inline void setValue(const std::string& value) { *m_value = value; }
		inline const std::string& getValue() const { return *m_value; }

	private:

		std::string* m_value;

	};

	//-----------------------------------------------------------------------------
	// mvEndTabBar
	//-----------------------------------------------------------------------------
	class mvEndTabBar : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::EndTabBar)

		mvEndTabBar(const std::string& parent)
			: mvAppItem(parent, "temporary"), m_value(false)
		{
			static int i = 0;
			i++;

			m_name = "endTabBar" + std::to_string(i);
			m_show = true;

		}

		virtual PyObject* getPyValue() override
		{
			PyObject* pvalue = Py_BuildValue("i", m_value);

			return pvalue;
		}

		virtual void draw() override
		{
			mvApp::GetApp()->popParent();
			ImGui::EndTabBar();
		}

		inline bool getValue() const { return m_value; }

	private:

		bool m_value;

	};

	//-----------------------------------------------------------------------------
	// mvTab
	//-----------------------------------------------------------------------------
	class mvTab : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::TabItem)

		mvTab(const std::string& parent, const std::string& name)
			: mvAppItem(parent, name), m_value(false)
		{
		}

		virtual PyObject* getPyValue() override
		{
			PyObject* pvalue = Py_BuildValue("i", m_value);

			return pvalue;
		}

		virtual void draw() override
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

		inline void setValue(bool value) { m_value = value; }
		inline bool getValue() const { return m_value; }

	private:

		bool m_value;

	};

	//-----------------------------------------------------------------------------
	// mvEndTab
	//-----------------------------------------------------------------------------
	class mvEndTab : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::EndTabItem)

		mvEndTab(const std::string& parent)
			: mvAppItem(parent, "temporary"), m_value(false)
		{
			static int i = 0;
			i++;

			m_name = "endTab" + std::to_string(i);

		}

		virtual PyObject* getPyValue() override
		{
			PyObject* pvalue = Py_BuildValue("i", m_value);

			return pvalue;
		}

		virtual void draw() override
		{
			mvApp::GetApp()->popParent();
			ImGui::EndTabItem();
		}

		inline bool getValue() const { return m_value; }

	private:

		bool m_value;

	};

}