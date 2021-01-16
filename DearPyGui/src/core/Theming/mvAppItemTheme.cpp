#include "mvAppItemTheme.h"
#include "mvAppItemStyleManager.h"
#include "mvApp.h"

namespace Marvel {

	mvAppItemTheme::mvAppItemTheme(int code)
		:
		m_itemCode(code)
	{
		mvEventBus::Subscribe(this, SID(std::string(std::to_string((int)code) + "_color").c_str()));
	};

	mvAppItemTheme::~mvAppItemTheme()
	{
		mvEventBus::UnSubscribe(this);
	}

	std::unordered_map<int, mvColor>& mvAppItemTheme::getColors()
	{ 
		return m_colors; 
	}


	bool mvAppItemTheme::onEvent(mvEvent& event) 
	{
		mvEventDispatcher dispatcher(event);
		dispatcher.dispatch(BIND_EVENT_METH(mvAppItemTheme::add_color), SID(std::string(std::to_string((int)m_itemCode) + "_color").c_str()));
		return event.handled;
	};

	bool mvAppItemTheme::add_color(mvEvent& event)
	{
		m_colors[GetEInt(event, "ID") % 100] = GetEColor(event, "COLOR");

		int id = GetEInt(event, "ID") % 100;
		mvColor color = GetEColor(event, "COLOR");
		std::string widget = GetEString(event, "WIDGET");

		if (widget.empty())
		{
			//TODO:we may want to have containers for each type in the item registry so we can just get all items of type blah
			//i think a few places in the code could benifit from this to do things like 

			//temp optimization below, if item is not window we wont look there
			if (m_itemCode == 1) {
				auto windows = mvApp::GetApp()->getItemRegistry().getWindows();
				for (auto& window : windows)
				{
					auto item = mvApp::GetApp()->getItemRegistry().getItem(window);

					if ((int)item->getType() == m_itemCode)
					{
						item->getIndividualTheme().getColors()[id] = color;
						item->getStyleManager().addColorStyle(id, color);
					}

				}
			}

			else {
				auto widgets = mvApp::GetApp()->getItemRegistry().getAllItems();
				for (auto& widget : widgets)
				{
					auto item = mvApp::GetApp()->getItemRegistry().getItem(widget);
					if ((int)item->getType() == m_itemCode)
					{
						item->getIndividualTheme().getColors()[id] = color;
						item->getStyleManager().addColorStyle(id, color);
					}
				}
			}
			return true;
		}


		mvRef<mvAppItem> item = mvApp::GetApp()->getItemRegistry().getItem(widget);

		if (item)
		{
			if (item->getType() == (mvAppItemType)m_itemCode)
			{
				item->getIndividualTheme().getColors()[id] = color;
				item->getStyleManager().clearColors();
				item->getStyleManager().addColorStyle(id, color);
				return true;
			}
		}

		return false;
	}

}