#include "mvApp.h"
#include "AppItems/mvInputText.h"

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

		for (mvAppItem* item : m_items)
		{
			item->draw();
		}
	}

	mvAppItem* mvApp::addInputText(const std::string& parent, const std::string& name, const std::string& hint)
	{
		mvAppItem* item = new mvInputText({ name, name, "", true }, hint);

		m_items.push_back(item);

		return item;
	}

}