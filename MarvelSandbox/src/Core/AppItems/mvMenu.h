#pragma once

#include "Core/mvAppItem.h"
#include "Core/mvApp.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvMenuBar
	//-----------------------------------------------------------------------------
	class mvMenuBar : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::MenuBar)

		mvMenuBar(const std::string& name)
			: mvAppItem("", name)
		{
			m_value = true;
		}

		virtual PyObject* getPyValue() override
		{
			PyObject* pvalue = Py_BuildValue("i", m_value);
			return pvalue;
		}

		virtual void draw() override
		{
			mvApp::GetApp()->pushParent(this);
			ImGui::BeginMenuBar();
		}

		inline void setValue(bool value) { m_value = value; }
		inline bool getValue() const { return m_value; }

	private:

		bool m_value;

	};

	//-----------------------------------------------------------------------------
	// mvEndMenuBar
	//-----------------------------------------------------------------------------
	class mvEndMenuBar : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::EndMenuBar)
		MV_NORETURN_VALUE()

		mvEndMenuBar(const std::string& parent)
			: mvAppItem(parent, "temporary")
		{
			static int i = 0;
			i++;

			m_name = "endMenuBar" + std::to_string(i);

		}

		virtual void draw() override
		{
			mvApp::GetApp()->popParent();
			ImGui::EndMenuBar();
		}


	};

	//-----------------------------------------------------------------------------
	// mvMenu
	//-----------------------------------------------------------------------------
	class mvMenu : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Menu)

		mvMenu(const std::string& parent, const std::string& name)
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

		inline void setValue(bool value) { m_value = value; }
		inline bool getValue() const { return m_value; }

	private:

		bool m_value;

	};

	//-----------------------------------------------------------------------------
	// mvEndMenu
	//-----------------------------------------------------------------------------
	class mvEndMenu : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::EndMenu)

		mvEndMenu(const std::string& parent)
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
			ImGui::EndMenu();
		}

		inline bool getValue() const { return m_value; }

	private:

		bool m_value;

	};

	//-----------------------------------------------------------------------------
	// mvMenuItem
	//-----------------------------------------------------------------------------
	class mvMenuItem : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::MenuItem)

		mvMenuItem(const std::string& parent, const std::string& name)
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

		inline void setValue(bool value) { m_value = value; }
		inline bool getValue() const { return m_value; }

	private:

		bool m_value;

	};

}