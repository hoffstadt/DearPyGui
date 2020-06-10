#pragma once

#include "Core/AppItems/mvTypeBases.h"
#include "Core/mvApp.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvMenuBar
//     * mvEndMenuBar
//     * mvMenu
//     * mvEndMenu
//     * mvMenuItem
//
//-----------------------------------------------------------------------------

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvMenuBar
	//-----------------------------------------------------------------------------
	class mvMenuBar : public mvBoolItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::MenuBar)

		mvMenuBar(const std::string& name)
			: mvBoolItemBase("", name, true)
		{}

		virtual void draw() override
		{
			mvApp::GetApp()->pushParent(this);
			ImGui::BeginMenuBar();
		}

	};

	//-----------------------------------------------------------------------------
	// mvEndMenuBar
	//-----------------------------------------------------------------------------
	class mvEndMenuBar : public mvNoneItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::EndMenuBar)

		mvEndMenuBar(const std::string& parent)
			: mvNoneItemBase(parent, "temporary")
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
	class mvMenu : public mvBoolItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Menu)

		mvMenu(const std::string& parent, const std::string& name)
			: mvBoolItemBase(parent, name, false)
		{
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

				// Context Menu
				if (getPopup() != "")
					ImGui::OpenPopup(getPopup().c_str());

			}
		}

	};

	//-----------------------------------------------------------------------------
	// mvEndMenu
	//-----------------------------------------------------------------------------
	class mvEndMenu : public mvBoolItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::EndMenu)

		mvEndMenu(const std::string& parent)
			: mvBoolItemBase(parent, "temporary", false)
		{
			static int i = 0;
			i++;

			m_name = "endTab" + std::to_string(i);

		}

		virtual void draw() override
		{
			mvApp::GetApp()->popParent();
			ImGui::EndMenu();
		}

	};

	//-----------------------------------------------------------------------------
	// mvMenuItem
	//-----------------------------------------------------------------------------
	class mvMenuItem : public mvBoolItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::MenuItem)

		mvMenuItem(const std::string& parent, const std::string& name)
			: mvBoolItemBase(parent, name, false){}

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

				// Context Menu
				if (getPopup() != "")
					ImGui::OpenPopup(getPopup().c_str());

			}
		}

	};

}