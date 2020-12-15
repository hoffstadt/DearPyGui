#include "mvAppItemTheme.h"
#include "mvApp.h"

namespace Marvel {

	mvAppItemTheme::mvAppItemTheme(int code)
		:
		m_itemCode(code)
	{
		mvEventBus::Subscribe(this, SID(std::string(std::to_string(code / 100) + "_color").c_str()));
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
		dispatcher.dispatch(BIND_EVENT_METH(mvAppItemTheme::add_color), 0, SID(std::string(std::to_string(m_itemCode / 100) + "_color").c_str()));
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
			m_colors[GetEInt(event, "ID") % 100] = GetEColor(event, "COLOR");
			return true;
		}


		mvRef<mvAppItem> item = mvApp::GetApp()->getItemRegistry().getItem(widget);

		if (item)
		{
			if (item->getType() == (mvAppItemType)m_itemCode)
			{
				item->getIndividualTheme().getColors()[id] = color;
				return true;
			}
		}

		return false;
	}

}