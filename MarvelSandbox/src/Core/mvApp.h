#pragma once

#include <vector>
#include <stack>
#include <string>
#include <functional>
#include "Core/AppItems/mvAppItem.h"


namespace Marvel {

	class mvApp final
	{

	public:

		static mvApp* GetApp();

		//-----------------------------------------------------------------------------
		// Basic AppItems
		//-----------------------------------------------------------------------------

		mvAppItem* addInputText(const std::string& parent, const std::string& name, const std::string& hint = "");

		//-----------------------------------------------------------------------------
		// Tabs
		//-----------------------------------------------------------------------------

		mvAppItem* addTabBar(const std::string& parent, const std::string& name);
		mvAppItem* addTab   (const std::string& parent, const std::string& name);
		mvAppItem* endTab   (const std::string& parent);
		mvAppItem* endTabBar(const std::string& parent);

		void       render         ();
		void       setItemCallback(const std::string& name, const std::string& callback);
		void       setModuleDict  (PyObject* dict) { m_pDict = dict; }
		PyObject*  getModuleDict  () { return m_pDict; }
		mvAppItem* getItem        (const std::string& name);
		void       triggerCallback(const std::string& name, const std::string& sender);
		void       pushParent     (mvAppItem* item);
		mvAppItem* popParent      ();
		bool       doesParentAllowRender(mvAppItem* item);

	private:

		mvApp() = default;

		mvApp(const mvApp& other) = delete;
		mvApp(mvApp&& other) = delete;
		mvApp operator=(const mvApp& other) = delete;
		mvApp operator=(mvApp&& other) = delete;

	private:

		static mvApp*           s_instance;
		std::vector<mvAppItem*> m_items;
		std::stack<mvAppItem*>  m_parents;
		PyObject*               m_pDict;

	};

}